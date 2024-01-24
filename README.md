## Introduction
Terminal typing game written in C

## How to run 

### on Linux
Requirements:
- GCC (GNU Compiler Collection)
- Make
- ncurses `sudo apt-get install libncurses5-dev libncursesw5-dev`

Compile with:

`make`

If you want to perform a clean build, you can run: `make clean`

run your program with:

`bin/type`

## How is WPM calculated?

$
WPM = \frac{{words \, typed \, correctly}}{{time \, taken \, in \, minutes}}
$

> [!NOTE]
> For now, any sequence of characters separated by a space is counted as a word.

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