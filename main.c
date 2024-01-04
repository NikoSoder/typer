#include <stdio.h>
#include <curses.h>

int main(void)
{
    // code
    while ((user_char = getch()) != '\n')
    {
        //  clear();
        // y--; UP
        // y++; DOWN
        // x--; LEFT
        // x++; RIGHT

        if (user_char == short_text[x])
        {
            attron(COLOR_PAIR(GREEN_TEXT)); // Turn on color
            printw("%c", user_char);
            attroff(COLOR_PAIR(GREEN_TEXT)); // Turn off color
            x++;
        }
        else if (user_char != short_text[x])
        {
            attron(COLOR_PAIR(RED_TEXT));
            printw("%c", user_char);
            attroff(COLOR_PAIR(RED_TEXT));
            x++;
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
}