#include <stdio.h>
#include <curses.h>
#include <string.h>

#define GREEN_TEXT 1
#define RED_TEXT 2
#define RESET_COLOR 3
#define SPACE ' '
#define UNDERSCORE '_'

const char short_text[] = "hello world";

int main(void)
{
    size_t short_text_length = strlen(short_text);
    int user_char;
    initscr();
    keypad(stdscr, TRUE);                            // Enable special keys, such as function keys
    start_color();                                   // Enable color support
    init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK); // Define color pair for green
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);     // Define color pair for red

    noecho(); // hide user input echo
    int x = 0;
    int y = 0;

    printw("%s\n", short_text);
    move(y, x); // move cursor to top of first letter

    while ((user_char = getch()) != '\n')
    {
        //  clear();
        // y--; UP
        // y++; DOWN
        // x--; LEFT
        // x++; RIGHT

        // todo clean up nesting
        // if RIGHT char
        if (user_char == short_text[x])
        {
            attron(COLOR_PAIR(GREEN_TEXT)); // Turn on color
            printw("%c", user_char);
            attroff(COLOR_PAIR(GREEN_TEXT)); // Turn off color
            x++;
        }
        // if WRONG char
        else if (user_char != short_text[x])
        {
            if (user_char == KEY_BACKSPACE)
            {
                // todo don't move cursor if on first character
                x--;
                move(y, x);
                printw("%c", short_text[x]);
            }
            else if (user_char == SPACE)
            {
                attron(COLOR_PAIR(RED_TEXT));
                printw("%c", short_text[x]);
                attroff(COLOR_PAIR(RED_TEXT));
                x++;
            }
            else
            {
                if (short_text[x] == SPACE)
                {
                    /* code */
                    attron(COLOR_PAIR(RED_TEXT));
                    printw("%c", UNDERSCORE);
                    attroff(COLOR_PAIR(RED_TEXT));
                    x++;
                }
                else
                {
                    attron(COLOR_PAIR(RED_TEXT));
                    printw("%c", short_text[x]);
                    attroff(COLOR_PAIR(RED_TEXT));
                    x++;
                }
            }
        }

        // break if end of text
        if (x == short_text_length)
        {
            break;
        }

        // Move the cursor to the new position
        move(y, x);

        // Refresh the screen
        // refresh();
    }

    endwin();

    return 0;
}