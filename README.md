## How to run 

Compile:

`gcc main.c quotes.c -o bin/main -lncurses -lm`

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
- [ ] Create starting screen. (start and quit)
- [ ] Show personal best (wpm)
- [x] Add multiple quotes
- [ ] Write score(wpm) to file

## Known bugs

- Quote on multiple lines doesn't work