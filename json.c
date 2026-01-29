#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "json.h"

#define ALIGN_UP(T, offset, align) (T)((uintptr_t)offset)

static void *ParserStackAlloc(JSON_Parser *parser, int size){

	if(parser->stack + ALIGN_UP(int, size, parser->align) > parser->stackEnd)
		longjmp(parser->env, JSON_ERROR_STACK_MEMORY);

	void *mem = parser->stack;

	parser->stack += ALIGN_UP(int, size, parser->align);

	return mem;
}

static JSON_Value *AddChild(JSON_Parser *parser, JSON_Value **list, int type){

	if(*list && (*list)->type == JSON_NOT_SET){

		(*list)->type = type;

		return *list;
	}

	JSON_Value *next = *list;

	JSON_Value *curr = ParserStackAlloc(parser, sizeof(JSON_Value));

	memset(curr, 0, sizeof(JSON_Value));

	curr->next = next;
	curr->type = type;

	*list = curr;

	return curr;
}

static int ExclusiveSkip(JSON_Parser *parser, char *characters){

	for(; parser->on < parser->end; ++parser->on)
		if(strchr(characters, *parser->on))
			return 1;

	return 0;
}

static int Skip(JSON_Parser *parser, char *characters){

	for(; parser->on < parser->end; ++parser->on)
		if(strchr(characters, *parser->on) == NULL)
			return 1;

	return 0;
}

static int Escape(char *str, int len){

	char *start = str;
	char *from = str;

	while(*from && from < start + len){

		if(*from == '\\' && *(from+1) != '\\'){
			++from;
			continue;
		}

		*str = *from;

		++str;
		++from;
	}

	return str - start;
}

static char *CopyStr(JSON_Parser *parser, char *start, char *end){

	if(!start) return NULL;

	int len = end - start;

	if(len <= 0) return NULL;

	len = Escape(start, end - start);

	char *key = (char *)ParserStackAlloc(parser, len+1);

	memcpy(key, start, len);

	key[len] = 0;

	return key;
}

static void RecursiveParse(JSON_Parser *parser, JSON_Value *into){

	while(parser->on < parser->end){

		if(!ExclusiveSkip(parser, "{[\"]}0123456789-+tfn")) break;

		if(*parser->on == '{' || *parser->on == '['){

			++parser->on;

			RecursiveParse(parser, AddChild(parser, &into->children, JSON_ARRAY));
			
			continue;
		}

		if(*parser->on == '}' || *parser->on == ']'){
			
			++parser->on;

			return;
		}

		if(*parser->on == '"'){

			++parser->on;

			char *start = parser->on;

			while(1){

				if(!ExclusiveSkip(parser, "\""))
					break;

				if(*(parser->on - 1) != '\\')
					break;
			
				++parser->on;
			}

			if(*parser->on != '"')
				break;

			char *str = CopyStr(parser, start, parser->on);

			++parser->on;

			if(!str) continue;

			Skip(parser, " \r\n\t");
	
			if(*parser->on == ':')
				AddChild(parser, &into->children, JSON_NOT_SET)->key = str;
			else
				AddChild(parser, &into->children, JSON_STRING)->string = str;

			continue;
		}

		if((*parser->on >= '0' && *parser->on <= '9') || *parser->on == '+' || *parser->on == '-'){

			char *start = parser->on;

			Skip(parser, "0123456789-+");

			char *str = CopyStr(parser, start, parser->on);

			if(str)
				AddChild(parser, &into->children, JSON_STRING)->string = str;

			continue;
		}

		if(*parser->on == 't' || *parser->on == 'f' || *parser->on == 'n'){

			if(*parser->on == 'n')
				AddChild(parser, &into->children, JSON_NULL);
			else if(*parser->on == 't')
				AddChild(parser, &into->children, JSON_TRUE);
			else if(*parser->on == 'f')
				AddChild(parser, &into->children, JSON_FALSE);

			if(!ExclusiveSkip(parser, "}],\r\n\t ")) break;

			continue;
		}
	}
}

static void Parse(JSON_Parser *parser){

	*parser->top = ParserStackAlloc(parser, sizeof(JSON_Value));

	memset(*parser->top, 0, sizeof(JSON_Value));

	(*parser->top)->type = JSON_ARRAY;

	if(!ExclusiveSkip(parser, "{")) return;

	++parser->on;

	RecursiveParse(parser, *parser->top);

}

static void PaddedPrint(int tabs, char *format, ...){

	va_list args;
	va_start(args, format);

	int k;
	for(k = 0; k < tabs; k++) printf("    ");

	vprintf(format, args);

	va_end(args);
}

void JSON_Dump(JSON_Value *val, int tabs){

	if(val->next)
		JSON_Dump(val->next, tabs);

	if(val->type != JSON_ARRAY){

		if(val->key)
			PaddedPrint(tabs, "%s: ", val->key);
		else
			PaddedPrint(tabs, "(null): ");

		if(val->type == JSON_TRUE)
			printf("true\n");
		else if(val->type == JSON_FALSE)
			printf("false\n");
		else if(val->type == JSON_NULL)
			printf("null\n");
		else if(val->type == JSON_STRING)
			printf("%s\n", val->string);
		else
			printf("\n");
		// else if(val->type == JSON_NUMBER)
		// 	printf("%f\n", val->number);

	} else {

		if(val->key)
			PaddedPrint(tabs, "%s {\n", val->key);
		else
			PaddedPrint(tabs, "{\n");

		if(val->children)
			JSON_Dump(val->children, tabs+1);

		PaddedPrint(tabs, "}\n");
	}
}

char *JSON_Error(int err){

	switch(err){
		case JSON_ERROR_NON_TERMINATED_STRING:
			return "Non terminated string.\n";
			break;
		case JSON_ERROR_INVALID_SYNTAX:
			return "Invalid syntax.\n";
			break;
		case JSON_ERROR_STACK_MEMORY:
			return "Ran out of stack memory.\n";
			break;
	}

	return "";
}

int JSON_Parse(JSON_Value **top, char *memory, int size, void *stack, void *stackEnd, int align){

	stack = ALIGN_UP(void *, stack, align);
	stackEnd = ALIGN_UP(void *, stackEnd - align, align);

	JSON_Parser parser = {
		.top = top,
		.on = memory,
		.end = memory+size,
		.key = NULL,
		.size = size,
		.stack = stack,
		.stackEnd = stackEnd,
		.align = align,
	};

	int err = setjmp(parser.env);

	if(err < 0) return err;

	Parse(&parser);

	return parser.stack - stack;
}