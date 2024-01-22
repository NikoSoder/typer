## How to run 

Compile:

`gcc main.c quotes.c menu_interface.c -o bin/main -lncurses -lmenu -lm`

run executable file with:

`bin/main`

## TODO

- [x] Handle wrong character when on SPACE
- [x] Handle SPACE on character
- [x] Handle backspace when on first char
- [x] Track mistakes
- [x] Track final word mistake
- [x] Don't go back to previous word
- [x] Track words per minute
- [x] Create starting screen
- [ ] Show personal best (wpm)
- [ ] Fix compile errors
- [ ] Add hard mode
- [ ] Better count words logic
- [x] Add multiple quotes
- [ ] Write score(wpm) to file
- [x] Fix quote on multiple lines
- [x] Fix backspace on multiple lines

## Known bugs