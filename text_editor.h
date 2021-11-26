#ifndef TEXT_EDITOR_DEF
 #define TEXT_EDITOR_DEF
#include <sys/types.h>
#include "types.h"
#include "graphics.h"
#include "config.h"
#include "file_browser.h"

#define EDIT_CTRL_KEY    (unsigned int)0x100
#define EDIT_ALT_KEY     (unsigned int)0x200
#define EDIT_SHIFT_KEY   (unsigned int)0x400
#define EDIT_ARROW_UP    (unsigned int)0x800
#define EDIT_ARROW_DOWN  (unsigned int)0x1000
#define EDIT_ARROW_LEFT  (unsigned int)0x2000
#define EDIT_ARROW_RIGHT (unsigned int)0x4000
#define EDIT_ENTER_KEY   (unsigned int)0x8000

// #include "window.h"
// #include "text.h"


// // { "keys": ["ctrl+h"], "command": "move", "args": {"by": "characters", "forward": false} },
// // { "keys": ["ctrl+l"], "command": "move", "args": {"by": "characters", "forward": true} },
// // { "keys": ["ctrl+j"], "command": "move", "args": {"by": "lines", "forward": false} },
// // { "keys": ["ctrl+k"], "command": "move", "args": {"by": "lines", "forward": true} },
// // { "keys": ["ctrl+alt+h"], "command": "move", "args": {"by": "words", "forward": false} },
// // { "keys": ["ctrl+alt+l"], "command": "move", "args": {"by": "word_ends", "forward": true} },
// // { "keys": ["ctrl+shift+l"], "command": "expand_selection", "args": {"to": "line"} },

// // // { "keys": ["ctrl+shift+m"], "command": "expand_selection", "args": {"to": "brackets"} },
// // // { "keys": ["ctrl+m"], "command": "move_to", "args": {"to": "brackets"} },

// // { "keys": ["shift+alt+ctrl+h"], "command": "move", "args": {"extend": true, "by": "words", "forward": false} },
// // { "keys": ["shift+alt+ctrl+l"], "command": "move", "args": {"extend": true, "by": "word_ends", "forward": true} },


 typedef struct {


  int startCursorPos;
  int len;

 } TextEditorSelection;


 typedef struct TextEditor TextEditor;
 typedef struct TextEditorCommand TextEditorCommand;
 typedef struct TextEditorCursor TextEditorCursor;

 struct TextEditorCommand {

  unsigned int *keyBinding;
  char *keys;
  int num;
  unsigned char scroll;
  
  TextEditorCursor *savedCursors;
  int nSavedCursors;

  void (*Execute)(TextEditor *, TextEditorCommand *c);
  void (*Undo)(TextEditor *, TextEditorCommand *c);
 };

 struct TextEditorCursor {


  TextEditorSelection selection;

  //no longer used, now clipboard is from system, lines seperated by \n
  char *clipboard;
  int sClipboard;
  char *savedText;
  int addedLen;

  int pos;
 };

#define MAX_AUTO_COMPLETE_STRLEN 35
#define MAX_AUTO_COMPLETE 20

 enum {
  LOGMODE_NUM = 1,
  LOGMODE_TEXT,
  LOGMODE_TEXT_INSENSITIVE,
  LOGMODE_SAVE,
  LOGMODE_OPEN,
  LOGMODE_SWITCH_FILE,
  LOGMODE_FILEBROWSER,
  LOGMODE_MODES_INPUTLESS,
  LOGMODE_ALERT,
  LOGMODE_CONSOLE,
 };

typedef struct {
  int offset;
  int len;
} AutoCompleteOffset;

typedef struct {
  int                     scroll;
  int                     cursorPos;
  int                     historyPos;

  TextEditorCommand       **history;
  int                     sHistory;


  char                    *text;
  int                     textLen;
  char                    name[MAX_FILENAME];
  char                    path[MAX_PATH_LEN];
} TextEditorFile;

 struct TextEditor {

  Config                  *cfg;
  Graphics                *graphics;
  TextEditorCommand       **commands;
  int                     nCommands;

  int                     selectNextWordTerminator; // "select" not get it in the phrase selecting

  int                     autoCompleteSearchLen;
  int                     autoCompleteLen;
  AutoCompleteOffset      autoComplete[MAX_AUTO_COMPLETE];
  int                     autoCompleteIndex;

  int                     logging;
  int                     logIndex;
  char                    *loggingText;

  TextEditorFile          **files;
  int                     nFiles;
  TextEditorFile          *file;

  TextEditorCursor        *cursors;
  int                     mouseSelection;
  int                     nCursors;
  int                     logX;
  int                     logY;

  int                     quit;
  FileBrowser             fileBrowser;

  int                     ttyPid;
  int                     ttyMaster;
  int                     ttySlave;
  int                     _stdout;
  int                     _stderr;
 };


void TextEditor_LoadFile(TextEditor *t, char *path);
void TextEditor_Draw(TextEditor *t);
void TextEditor_Event(TextEditor *t,unsigned int key);
int TextEditor_Destroy(TextEditor *t);
void TextEditor_Init(TextEditor *t, Graphics *g, Config *cfg);
void TextEditor_SetCursorPos(TextEditor *t, int x, int y);
int TextEditor_SetCursorPosSelection(TextEditor *t, int x, int y);
void TextEditor_SetCursorPosDoubleClick(TextEditor *t, int x, int y);
 #endif