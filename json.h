#ifndef JSON_DEF
#define JSON_DEF

#include <setjmp.h>

#define JSON_ERROR_NON_TERMINATED_STRING 	-1
#define JSON_ERROR_INVALID_SYNTAX 			-2
#define JSON_ERROR_STACK_MEMORY 			-3

typedef struct JSON_Value JSON_Value;

enum {
	JSON_NOT_SET = 0,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL,
	JSON_ARRAY,
	JSON_OBJECT,
	JSON_NUMBER,
	JSON_STRING,
};

struct JSON_Value {
	char 				*string;
	// float 				number;
	JSON_Value 			*children;
	unsigned char 		type;
	char 				*key;
	JSON_Value 			*next;
};

typedef struct {

	JSON_Value 		**top;

	char 			*end;
	int 			size;

	char 			*on;

	jmp_buf 		env;

	char 			*key;

	void 			*stack;
	void 			*stackEnd;

	int 			align;

} JSON_Parser;


void 	JSON_Dump(JSON_Value *value, int tabs);
int JSON_Parse(JSON_Value **top, char *memory, int size, void *stack, void *stackEnd, int align);
char 	*JSON_Error(int err);

#endif