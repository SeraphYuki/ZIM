![Screenshot](Screenshots/thoth.bmp)<br>
https://rotatedev.itch.io/zim BINARIES<br>
New <b>colorscheme</b> is: https://github.com/morhetz/gruvbox<br>
<b>font</b>: https://github.com/larsenwork/monoid<br>
![Screenshot](Screenshots/Screenshot.png)
<br><hr>
    <b>SelectNextWord</b> Find/Replace, CTRL+D In sublime text ||| <b>AddCursorCommand</b> Up/Down<br>
    <p>![Screenshot](Screenshots/selectcursors.gif) ![Screenshot](Screenshots/cursors.gif)<br></p>
    <b>MoveLines</b> Up/Down ||| <b>MoveByWords</b> Left/Right <b>MoveByChars</b> Left/Right/Up/Down<br>
    <p>![Screenshot](Screenshots/movelines.gif)  ![Screenshot](Screenshots/navigation.gif)<br></p>
    <b>ExpandSelectionWords</b> Left/Right ||| <b>IndentLine</b> Left/Right<br>
    <p>![Screenshot](Screenshots/selections.gif) ![Screenshot](Screenshots/indenting.gif)<br></p>
    <b>Undo Redo Cut Copy Paste</b>  ||| <b>AutoComplete</b><br>
    <p>![Screenshot](Screenshots/undo.gif) ![Screenshot](Screenshots/autocomplete.gif)<br></p>
    
    
    
![Screenshot](Screenshots/Screenshot.gif)<br>
<hr>
<b>TODO</b>:<br>
windows crashing <br>
windows keybindings interfere with system bindings ctrl+alt+left<br>
Faster undoing: done. Needs testing <br>
element rendering in graphics.c save gpu mem.<br>
have only the active file in memory AND swap files<br>
minimap<br>
keybindings config<br>
highlighting for more languages than C, and general lua (function is a keyword)<br>
} auto indent down <br>
check for file change outside editor and reload on window focus event<br>
find replace<br>
code folding<br>
<br>
<br>
Example thothconfig.cfg</b><br>
# gruvbox <br>
COLOR_CYAN 0x8e 0xc0 0x7c<br>
COLOR_RED 0xfb 0x49 0x34<br>
COLOR_YELLOW 0xfa 0xbd 0x2f<br>
COLOR_BLUE 0x83 0xa5 0x98<br>
COLOR_GREEN 0xb8 0xbb 0x26<br>
COLOR_MAGENTA 0xd3 0x86 0x9b<br>
COLOR_WHITE 0xeb 0xdb 0xb2<br>
COLOR_BLACK 0x28 0x28 0x28<br>
COLOR_GREY 0x92 0x83 0x74<br>
COLOR_BG 0x28 0x28 0x28<br><br>
<b>COMMANDS</b>:<br>
ctrl+a (select all)<br>
ctrl+- (zoom out)<br>
ctrl+= (zoom in)<br>
ctrl+q (quit)<br>
escape (closes find/goto/console, removes extra cursors/selections)<br>
ctrl+b (compile (runs "make"))<br>
ctrl+y (redo)<br>
ctrl+z (undo)<br>
ctrl+x (cut)<br>
ctrl+c (copy)<br>
ctrl+v (paste)<br>
arrow keys (movement)<br>
ctrl+h (move left) ctrl+l (move right) ctrl+j (move up) ctrl+k (move down)<br>
shift+arrow up/down (scroll screen up/down)<br>
ctrl+alt+arrow right/left (expand selection by words right/left)<br>
ctrl+alt+[ (indent backward) <br>
ctrl+alt+] (indent forward) <br>
ctrl+alt+h (move by words left) ctrl+alt+l (move by words right)<br>
ctrl+shift+l (expand selection by a line)<br>
ctrl+shift+k (delete line)<br>
ctrl+arrow up (add cursor up)<br>
ctrl+arrow down (add cursor down)<br>
ctrl+d (select word under cursor if no selection. continued pressing selects the next occurance of word or selection)<br>
ctrl+g (goto line)<br>
ctrl+f (search) (enter to search forward, ctrl+enter to search backward)<br>
ctrl+F (case senstive search)<br>
ctrl+m (move brackets) (moves cursor between the { }, ( ), [ ], of the current scope) (either to the end, or to the beginning if its at the end)<br>
ctrl+shift+j (select brackets) (selects everything between the brackets)<br>
ctrl+/ (toggle comment) (adds or removes // for the line to comment) (todo: mutli-line)<br>
ctrl+shift+arrow up/down (move line up/down) (moves the entire line the cursors on, or every line in the selection by a line)<br>
ctrl+o (open file)<br>
ctrl+shift+o (file browser)<br>
ctrl+s (save file)<br>
ctrl+shift+s Save As file<br>
ctrl+n New file <br>
ctrl+p Switch file, (lists open files)<br>
ctrl+w Close file<br>

<br>
Deveoloped while bedridden after I got hit by a car on a moped.<br>

