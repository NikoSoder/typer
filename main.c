#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>

#define GREEN_TEXT 1
#define RED_TEXT 2
#define RESET_COLOR 3
#define SPACE ' '
#define UNDERSCORE '_'

const char short_text[] = "hello world";
struct timeval start, end;

// not the best solution to count words but works for now
int count_words(const char *text)
{
    int word_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            word_count += 1;
        }
    }
    // count last word
    word_count++;

    return word_count;
}

void calculate_words_per_minute(double seconds)
{
    int total_words = count_words(short_text);
    printf("Total words %d\n", total_words);

    // Convert time to minutes and calculate WPM
    double time_taken_minutes = seconds / 60.0;
    int wpm = round((double)total_words / time_taken_minutes);

    printf("Words per minute wpm: %d\n", wpm);
}

void handle_color_change(char character, int color)
{
    attron(COLOR_PAIR(color)); // Turn on color
    printw("%c", character);
    attroff(COLOR_PAIR(color)); // Turn off color
}

int main(void)
{
    size_t short_text_length = strlen(short_text);
    bool timer_started = false;
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
        if (!timer_started)
        {
            // Get the initial time-stamp
            gettimeofday(&start, NULL);
            timer_started = true;
        }

        // todo clean up nesting
        // if RIGHT char
        if (user_char == short_text[x])
        {
            handle_color_change(user_char, GREEN_TEXT);
            x++;
        }
        // if WRONG char
        else if (user_char != short_text[x])
        {
            if (user_char == KEY_BACKSPACE)
            {
                // don't move cursor if on first character
                if (x > 0)
                {
                    x--;
                    move(y, x);
                    printw("%c", short_text[x]);
                }
            }
            else if (user_char == SPACE)
            {
                handle_color_change(short_text[x], RED_TEXT);
                x++;
            }
            else
            {
                if (short_text[x] == SPACE)
                {
                    handle_color_change(UNDERSCORE, RED_TEXT);
                    x++;
                }
                else
                {
                    handle_color_change(short_text[x], RED_TEXT);
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

    //  Get the final time-stamp
    gettimeofday(&end, NULL);

    // Calculate the elapsed time in milliseconds
    double t_ms = (double)(end.tv_sec - start.tv_sec) * 1000.0 +
                  (double)(end.tv_usec - start.tv_usec) / 1000.0;

    // Convert time to seconds with one decimal place
    double t_seconds = round(t_ms / 1000.0 * 10.0) / 10.0;

    printf("Took %.1f milliseconds to execute \n", t_ms);
    printf("Took %.1f seconds to execute \n", t_seconds);
    calculate_words_per_minute(t_seconds);

    return 0;
}