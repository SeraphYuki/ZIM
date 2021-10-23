#ifndef TEXT_EDITOR_DEF
 #define TEXT_EDITOR_DEF
#include <sys/types.h>

#define EDIT_CTRL_KEY    (unsigned int)0x100
#define EDIT_ALT_KEY     (unsigned int)0x200
#define EDIT_SHIFT_KEY   (unsigned int)0x400
#define EDIT_ARROW_UP    (unsigned int)0x800
#define EDIT_ARROW_DOWN  (unsigned int)0x1000
#define EDIT_ARROW_LEFT  (unsigned int)0x2000
#define EDIT_ARROW_RIGHT (unsigned int)0x4000

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

  TextEditorCursor *savedCursors;
  int nSavedCursors;
  char **savedTexts;
  int *addedLens;

  void (*Execute)(TextEditor *, TextEditorCommand *c);
  void (*Undo)(TextEditor *, TextEditorCommand *c);
 };

 struct TextEditorCursor {


  TextEditorSelection selection;

  char *clipboard;
  int sClipboard;

  int pos;
 };

#define MAX_AUTO_COMPLETE 20

typedef struct {
  char pair;
  int offset;
} ColoredTextOffset;

 struct TextEditor {



  TextEditorCommand **history;
  int sHistory;


  int autoComplete;

  int searching;
  char *searchingText;
  int scroll;
  int historyPos;

  TextEditorCommand **commands;
  int nCommands;

  TextEditorCursor *cursors;
  int nCursors;

  char *text;
  int textLen;

  ColoredTextOffset *coloredText;
  int coloredTextLen;

  int quit;
 };

 void TextEditor_Draw(TextEditor *t);
 void TextEditor_Event(TextEditor *t,unsigned int key);
 void TextEditor_Destroy(TextEditor *t);
 void TextEditor_Init(TextEditor *t);



 #endif