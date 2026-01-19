#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "thoth.h"

void Thoth_Config_Read(Thoth_Config *cfg){
	memset(cfg, 0, sizeof(Thoth_Config));
	struct{
	    int r;
	    int g;
	    int b;
	} defaultColors[] = { 
#ifdef SDL_COMPILE
		{ 0x01, 0xb6, 0xed },
		{ 0xfa, 0x29, 0x34 },
		{ 0xff, 0xf3, 0x0a },
		{ 0x04, 0x43, 0xff },
		{ 0x98, 0xe1, 0x23 },
		{ 0xf8, 0x08, 0xf8 },
		{ 0xf9, 0xf9, 0xf9 },
		{ 0x12, 0x12, 0x12 },
		{ 0xfa, 0xfa, 0xfa },
		{ 0x12, 0x12, 0x12 },
#else
		{ 0x8e, 0xc0 ,0x7c},
		{ 0xfb, 0x49 ,0x34},
		{ 0xfa, 0xbd ,0x2f},
		{ 0x83, 0xa5 ,0x98},
		{ 0xb8, 0xbb ,0x26},
		{ 0xd3, 0x86 ,0x9b},
		{ 0xeb, 0xdb ,0xb2},
		{ 0x28, 0x28 ,0x28},
		{ 0x92, 0x83 ,0x74},
		{ 0x28, 0x28 ,0x28 },
#endif
	};
	#ifdef LINUX_COMPILE
	strcpy(cfg->makecmd, "make");
	#endif
	#ifdef WINDOWS_COMPILE
	strcpy(cfg->makecmd, "mingw32-make");
	#endif

	cfg->keybinds[THOTH_MoveLinesText_UP] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|THOTH_ARROW_UP;
	cfg->keybinds[THOTH_MoveLinesText_DOWN] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|THOTH_ARROW_DOWN;
	cfg->keybinds[THOTH_OpenFileBrowser] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'o';
	cfg->keybinds[THOTH_OpenFileZim] = THOTH_CTRL_KEY|'o';
	cfg->keybinds[THOTH_Help] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'m';
	cfg->keybinds[THOTH_NewFile] = THOTH_CTRL_KEY|'n';
	cfg->keybinds[THOTH_CloseFile] = THOTH_CTRL_KEY|'w';
	cfg->keybinds[THOTH_SwitchFile] = THOTH_CTRL_KEY|'p';
	cfg->keybinds[THOTH_SaveAsFile] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'s';
	cfg->keybinds[THOTH_SaveFile] = THOTH_CTRL_KEY|'s';
	cfg->keybinds[THOTH_ToggleComment] = THOTH_CTRL_KEY|'/';
	cfg->keybinds[THOTH_ToggleCommentMulti] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'/';
	cfg->keybinds[THOTH_MoveBrackets] = THOTH_CTRL_KEY|'m';
	cfg->keybinds[THOTH_SelectBrackets] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'j';
	cfg->keybinds[THOTH_GotoLine] = THOTH_CTRL_KEY|'g';
	cfg->keybinds[THOTH_FindTextInsensitive] = THOTH_CTRL_KEY|'f';
	cfg->keybinds[THOTH_FindTextZim] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'f';
	cfg->keybinds[THOTH_EventCtrlEnter] = THOTH_ENTER_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_SelectNextWord] = 	'd'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_AddCursorCommand_UP] = THOTH_ARROW_UP|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_AddCursorCommand_DOWN] = THOTH_ARROW_DOWN|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionLines] = 	'l'|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_DeleteLine] = 'k'|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByChars_BACK] = 	'h'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByChars_FORWARD] = 	'l'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveLines_UP] = 	'j'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveLines_DOWN] = 	'k'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByWords_BACK] = 'h'|THOTH_ALT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByWords_FORWARD] = 'l'|THOTH_ALT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_IndentLine_FORWARD] = ']'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_IndentLine_BACK] = '['|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionWords_BACK] = THOTH_ARROW_LEFT|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionWords_FORWARD] = THOTH_ARROW_RIGHT|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ScrollScreen_UP] = THOTH_ARROW_UP|THOTH_SHIFT_KEY;
	cfg->keybinds[THOTH_ScrollScreen_DOWN] = THOTH_ARROW_DOWN|THOTH_SHIFT_KEY;
	cfg->keybinds[THOTH_SelectAll] = THOTH_CTRL_KEY|'a';
	cfg->keybinds[THOTH_Undo] = 	'z'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Redo] = 	'y'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Cut] = 	'x'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Copy] = 	'c'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Paste] ='v'|THOTH_CTRL_KEY;

	memcpy(&cfg->colorPairs[THOTH_COLOR_SIDE_NUMBERS], (int[]){ THOTH_COLOR_WHITE, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_NORMAL], (int[]){ THOTH_COLOR_WHITE, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_KEYWORD], (int[]){ THOTH_COLOR_CYAN, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_COMMENT], (int[]){ THOTH_COLOR_GREY, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_TOKEN], (int[]){ THOTH_COLOR_GREEN, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_NUM], (int[]){ THOTH_COLOR_RED, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_FUNCTION], (int[]){ THOTH_COLOR_YELLOW, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_STRING], (int[]){ THOTH_COLOR_MAGENTA, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_SELECTED], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_GREEN }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_SELECTED_DIRECTORY], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_CYAN }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_UNSELECTED_DIRECTORY], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_AUTO_COMPLETE], (int[]){ THOTH_COLOR_BLACK, THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LOG_UNSELECTED], (int[]){ THOTH_COLOR_BLACK, THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_CURSOR], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_MAGENTA }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_FIND], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LINE_NUM], (int[]){ THOTH_COLOR_WHITE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LINENUM_CURSOR], (int[]){ THOTH_COLOR_GREEN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_BLACK], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_WHITE], (int[]){ THOTH_COLOR_WHITE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_CYAN], (int[]){ THOTH_COLOR_CYAN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_RED], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_YELLOW], (int[]){ THOTH_COLOR_YELLOW ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_BLUE], (int[]){ THOTH_COLOR_BLUE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_GREEN], (int[]){ THOTH_COLOR_GREEN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_MAGENTA], (int[]){ THOTH_COLOR_MAGENTA ,THOTH_COLOR_BLACK }, sizeof(int)*2);

	cfg->tabs = DEFAULT_TAB_WIDTH;

	FILE *fp = fopen(THOTH_CONFIG_FILE,"rb");

	if(fp){

		void ReadCommand(unsigned int *keybinding){

			*keybinding = 0;
			if(fgetc(fp) != '(') return;
			if(fgetc(fp) != ' ') return;

			while(!feof(fp)){

				char bind[32];
				fscanf(fp, "%s ", bind);
				if(bind[0] == ')') return;


				if(strlen(bind) == 1) *keybinding |= bind[0];

				else {				
					if(strcmp(bind, "CTRL") == 0)
						*keybinding |= THOTH_CTRL_KEY;
					else if(strcmp(bind, "ALT") == 0)
						*keybinding |= THOTH_ALT_KEY;
					else if(strcmp(bind, "SHIFT") == 0)
						*keybinding |= THOTH_SHIFT_KEY;
					else if(strcmp(bind, "ENTER") == 0)
						*keybinding |= THOTH_ENTER_KEY;
					else if(strcmp(bind, "ARROW_DOWN") == 0)
						*keybinding |= THOTH_ARROW_DOWN;
					else if(strcmp(bind, "ARROW_UP") == 0)
						*keybinding |= THOTH_ARROW_UP;
					else if(strcmp(bind, "ARROW_RIGHT") == 0)
						*keybinding |= THOTH_ARROW_RIGHT;
					else if(strcmp(bind, "ARROW_LEFT") == 0)
						*keybinding |= THOTH_ARROW_LEFT;
				}
			}			
		}

	    while(!feof(fp)){
			char lineType[100];

			fscanf(fp, "%s : ", lineType);
			
			if(lineType[0] == '#'){
			    while(!feof(fp) && fgetc(fp) != '\n'){}
			    continue;
			}

			if(strcmp(lineType, "MakeCMD") == 0)
			    fscanf(fp, "%s", cfg->makecmd);
			else if(strcmp(lineType, "COLOR_CYAN") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_CYAN].r, &defaultColors[THOTH_COLOR_CYAN].g, &defaultColors[THOTH_COLOR_CYAN].b);
			else if(strcmp(lineType, "COLOR_RED") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_RED].r, &defaultColors[THOTH_COLOR_RED].g, &defaultColors[THOTH_COLOR_RED].b);
			else if(strcmp(lineType, "COLOR_YELLOW") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_YELLOW].r, &defaultColors[THOTH_COLOR_YELLOW].g, &defaultColors[THOTH_COLOR_YELLOW].b);
			else if(strcmp(lineType, "COLOR_BLUE") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_BLUE].r, &defaultColors[THOTH_COLOR_BLUE].g, &defaultColors[THOTH_COLOR_BLUE].b);
			else if(strcmp(lineType, "COLOR_GREEN") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_GREEN].r, &defaultColors[THOTH_COLOR_GREEN].g, &defaultColors[THOTH_COLOR_GREEN].b);
			else if(strcmp(lineType, "COLOR_MAGENTA") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_MAGENTA].r, &defaultColors[THOTH_COLOR_MAGENTA].g, &defaultColors[THOTH_COLOR_MAGENTA].b);
			else if(strcmp(lineType, "COLOR_WHITE") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_WHITE].r, &defaultColors[THOTH_COLOR_WHITE].g, &defaultColors[THOTH_COLOR_WHITE].b);
			else if(strcmp(lineType, "COLOR_BLACK") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_BLACK].r, &defaultColors[THOTH_COLOR_BLACK].g, &defaultColors[THOTH_COLOR_BLACK].b);
			else if(strcmp(lineType, "COLOR_GREY") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_GREY].r, &defaultColors[THOTH_COLOR_GREY].g, &defaultColors[THOTH_COLOR_GREY].b);
			else if(strcmp(lineType, "COLOR_BG") == 0)
			    fscanf(fp, "%x %x %x", &defaultColors[THOTH_COLOR_BG].r, &defaultColors[THOTH_COLOR_BG].g, &defaultColors[THOTH_COLOR_BG].b);
			else if(strcmp(lineType, "MoveLinesText_UP") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveLinesText_UP]);
			else if(strcmp(lineType, "MoveLinesText_DOWN") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveLinesText_DOWN]);
			else if(strcmp(lineType, "OpenFileBrowser") == 0)
				ReadCommand(&cfg->keybinds[THOTH_OpenFileBrowser]);
			else if(strcmp(lineType, "NewFile") == 0)
				ReadCommand(&cfg->keybinds[THOTH_NewFile]);
			else if(strcmp(lineType, "CloseFile") == 0)
				ReadCommand(&cfg->keybinds[THOTH_CloseFile]);
			else if(strcmp(lineType, "SwitchFile") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SwitchFile]);
			else if(strcmp(lineType, "SaveAsFile") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SaveAsFile]);
			else if(strcmp(lineType, "SaveFile") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SaveFile]);
			else if(strcmp(lineType, "ToggleComment") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ToggleComment]);
			else if(strcmp(lineType, "ToggleCommentMulti") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ToggleCommentMulti]);
			else if(strcmp(lineType, "MoveBrackets") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveBrackets]);
			else if(strcmp(lineType, "SelectBrackets") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SelectBrackets]);
			else if(strcmp(lineType, "GotoLine") == 0)
				ReadCommand(&cfg->keybinds[THOTH_GotoLine]);
			else if(strcmp(lineType, "FindTextInsensitive") == 0)
				ReadCommand(&cfg->keybinds[THOTH_FindTextInsensitive]);
			else if(strcmp(lineType, "FindTextZim") == 0)
				ReadCommand(&cfg->keybinds[THOTH_FindTextZim]);
			else if(strcmp(lineType, "EventCtrlEnter") == 0)
				ReadCommand(&cfg->keybinds[THOTH_EventCtrlEnter]);
			else if(strcmp(lineType, "SelectNextWord") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SelectNextWord]);
			else if(strcmp(lineType, "AddCursorCommand_UP") == 0)
				ReadCommand(&cfg->keybinds[THOTH_AddCursorCommand_UP]);
			else if(strcmp(lineType, "AddCursorCommand_DOWN") == 0)
				ReadCommand(&cfg->keybinds[THOTH_AddCursorCommand_DOWN]);
			else if(strcmp(lineType, "ExpandSelectionLines") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ExpandSelectionLines]);
			else if(strcmp(lineType, "DeleteLine") == 0)
				ReadCommand(&cfg->keybinds[THOTH_DeleteLine]);
			else if(strcmp(lineType, "MoveByChars_BACKWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveByChars_BACK]);
			else if(strcmp(lineType, "MoveByChars_FORWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveByChars_FORWARD]);
			else if(strcmp(lineType, "MoveLines_UP") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveLines_UP]);
			else if(strcmp(lineType, "MoveLines_DOWN") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveLines_DOWN]);
			else if(strcmp(lineType, "MoveByWords_BACKWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveByWords_BACK]);
			else if(strcmp(lineType, "MoveByWords_FORWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_MoveByWords_FORWARD]);
			else if(strcmp(lineType, "IndentLine_FORWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_IndentLine_FORWARD]);
			else if(strcmp(lineType, "IndentLine_BACKWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_IndentLine_BACK]);
			else if(strcmp(lineType, "ExpandSelectionWords_BACKWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ExpandSelectionWords_BACK]);
			else if(strcmp(lineType, "ExpandSelectionWords_FORWARD") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ExpandSelectionWords_FORWARD]);
			else if(strcmp(lineType, "ScrollScreen_UP") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ScrollScreen_UP]);
			else if(strcmp(lineType, "ScrollScreen_DOWN") == 0)
				ReadCommand(&cfg->keybinds[THOTH_ScrollScreen_DOWN]);
			else if(strcmp(lineType, "SelectAll") == 0)
				ReadCommand(&cfg->keybinds[THOTH_SelectAll]);
			else if(strcmp(lineType, "Undo") == 0)
				ReadCommand(&cfg->keybinds[THOTH_Undo]);
			else if(strcmp(lineType, "Redo") == 0)
				ReadCommand(&cfg->keybinds[THOTH_Redo]);
			else if(strcmp(lineType, "Cut") == 0)
				ReadCommand(&cfg->keybinds[THOTH_Cut]);
			else if(strcmp(lineType, "Copy") == 0)
				ReadCommand(&cfg->keybinds[THOTH_Copy]);
			else if(strcmp(lineType, "Paste") == 0)
				ReadCommand(&cfg->keybinds[THOTH_Paste]);


			while(fgetc(fp) != '\n' && !feof(fp)){}
		}

	    fclose(fp);

	}

	int k;
	for(k = 0; k < THOTH_NUM_COLORS; k++){
	#ifdef SDL_COMPILE
		cfg->colors[k].r = (float)defaultColors[k].r/255.0f;
		cfg->colors[k].g = (float)defaultColors[k].g/255.0f;
		cfg->colors[k].b = (float)defaultColors[k].b/255.0f;

	
	#else
	#ifdef WINDOWS_COMPILE
		cfg->colors[k].r = (int)defaultColors[k].r << 16;
		cfg->colors[k].g = (int)defaultColors[k].g << 8;
		cfg->colors[k].b = defaultColors[k].b & 0xFF;
	#else
		cfg->colors[k].r = defaultColors[k].r*1000/255;
		cfg->colors[k].g = defaultColors[k].g*1000/255;
		cfg->colors[k].b = defaultColors[k].b*1000/255;

	#endif
	#endif
	}

}
