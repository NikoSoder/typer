#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>
#include "quotes.h"

#define GREEN_TEXT 1
#define RED_TEXT 2
#define RESET_COLOR 3
#define SPACE ' '
#define UNDERSCORE '_'

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

void calculate_words_per_minute(double seconds, const char *quote, int mistakes, int correct_words)
{
    int total_words = count_words(quote); // todo remove this if not needed
    printf("Total words %d\n", total_words);
    total_words = total_words - mistakes;
    printf("Total words after mistakes %d\n", total_words);
    printf("Total correct words %d\n", correct_words);

    // Convert time to minutes and calculate WPM
    double time_taken_minutes = seconds / 60.0;
    int wpm = round((double)correct_words / time_taken_minutes);

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
    const char *quote = get_quote();
    size_t quote_length = strlen(quote);
    bool timer_started = false;
    int max_width, max_height;
    int user_char;
    initscr();
    getmaxyx(stdscr, max_height, max_width);         // Get terminal height and width
    keypad(stdscr, TRUE);                            // Enable special keys, such as function keys
    start_color();                                   // Enable color support
    init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK); // Define color pair for green
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);     // Define color pair for red

    noecho(); // hide user input echo
    int x = 0;
    int y = 0;
    int quote_char_index = 0;

    printw("%s\n", quote);
    move(y, x); // move cursor to top of first letter

    int total_correct_words = 0;
    int total_mistakes = 0;
    // variable to store the current word's length
    int word_index = 0;
    // variable to store the earliest index where a mistake occurs within the current word
    int mistake_on_word = -1;

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

        // don't go back if on first letter
        if (user_char == KEY_BACKSPACE && word_index == 0)
        {
            continue;
        }

        // todo clean up nesting
        // if RIGHT char
        if (user_char == quote[quote_char_index])
        {
            handle_color_change(user_char, GREEN_TEXT);
            x++;
            quote_char_index++;
            word_index++;
            // reset mistake variables if next word
            if (user_char == SPACE)
            {
                total_correct_words = mistake_on_word == -1 ? total_correct_words += 1 : total_correct_words;
                total_mistakes = mistake_on_word > -1 ? total_mistakes += 1 : total_mistakes;
                word_index = 0;
                mistake_on_word = -1;
            }
        }
        // if WRONG char
        else if (user_char != quote[quote_char_index])
        {
            // track earliest mistake on word
            if (user_char != KEY_BACKSPACE && mistake_on_word == -1)
            {
                mistake_on_word = word_index;
            }

            if (user_char == KEY_BACKSPACE && x == 0)
            {
                y--;
                x = max_width - 1;
                word_index--;
                quote_char_index--;
                move(y, x);
                printw("%c", quote[quote_char_index]);
                mistake_on_word = mistake_on_word == word_index ? -1 : mistake_on_word;
            }
            else if (user_char == KEY_BACKSPACE)
            {
                // don't move cursor if on first character
                // if (x > 0)
                // {
                x--;
                quote_char_index--;
                word_index--;
                move(y, x);
                printw("%c", quote[quote_char_index]);
                // reset mistake_on_word if backspacing earliest mistake
                mistake_on_word = mistake_on_word == word_index ? -1 : mistake_on_word;
                // }
            }
            else if (user_char == SPACE)
            {
                handle_color_change(quote[quote_char_index], RED_TEXT);
                quote_char_index++;
                x++;
                word_index++;
            }
            else
            {
                if (quote[quote_char_index] == SPACE)
                {
                    handle_color_change(UNDERSCORE, RED_TEXT);
                    quote_char_index++;
                    x++;
                    word_index++;
                }
                else
                {
                    handle_color_change(quote[quote_char_index], RED_TEXT);
                    quote_char_index++;
                    x++;
                    word_index++;
                }
            }
        }
        if (x == max_width && user_char != KEY_BACKSPACE)
        {
            y++;
            x = 0;
            // move(y, x);
        }

        // break if end of text
        if (quote_char_index == quote_length)
        {
            break;
        }

        // Move the cursor to the new position
        move(y, x);

        // Refresh the screen
        // refresh();
    }

    endwin();

    // Add mistake on final word if there is any
    if (mistake_on_word > -1)
    {
        total_mistakes++;
    }
    if (mistake_on_word == -1)
    {
        total_correct_words++;
    }

    //  Get the final time-stamp
    gettimeofday(&end, NULL);

    // Calculate the elapsed time in milliseconds
    double t_ms = (double)(end.tv_sec - start.tv_sec) * 1000.0 +
                  (double)(end.tv_usec - start.tv_usec) / 1000.0;

    // Convert time to seconds with one decimal place
    double t_seconds = round(t_ms / 1000.0 * 10.0) / 10.0;

    printf("Took %.1f milliseconds to execute \n", t_ms);
    printf("Took %.1f seconds to execute \n", t_seconds);
    printf("Total mistakes: %d\n", total_mistakes);
    printf("Terminal width: %d\n", max_width);
    calculate_words_per_minute(t_seconds, quote, total_mistakes, total_correct_words);

    return 0;
}