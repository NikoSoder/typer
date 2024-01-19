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
    // printw("Total words %d\n", total_words);
    total_words = total_words - mistakes;
    // printw("Total words after mistakes %d\n", total_words);
    // printw("Total correct words %d\n", correct_words);

    // Convert time to minutes and calculate WPM
    double time_taken_minutes = seconds / 60.0;
    int wpm = round((double)correct_words / time_taken_minutes);

    printw("Words per minute wpm: %d\n", wpm);
}

void handle_color_change(char character, int color)
{
    attron(COLOR_PAIR(color)); // Turn on color
    printw("%c", character);
    attroff(COLOR_PAIR(color)); // Turn off color
}

void type()
{
    bool type_again = true;
    while (type_again)
    {
        // reset these variables every loop
        int max_width, max_height;
        getmaxyx(stdscr, max_height, max_width); // Get terminal height and width
        char *quote = get_quote();
        size_t quote_length = strlen(quote);
        bool timer_started = false;
        int user_char;
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

        // quote loop
        while ((user_char = getch()) != '\n')
        {
            /**
             * directions
             *
             * y--; UP
             * y++; DOWN
             * x--; LEFT
             * x++; RIGHT
             */

            // Start timer if first loop
            if (!timer_started)
            {
                gettimeofday(&start, NULL);
                timer_started = true;
            }

            // don't go back if on first letter
            if (user_char == KEY_BACKSPACE && word_index == 0)
            {
                continue;
            }

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
            // todo clean up nesting
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
                    x--;
                    quote_char_index--;
                    word_index--;
                    move(y, x);
                    printw("%c", quote[quote_char_index]);
                    // reset mistake_on_word if backspacing earliest mistake
                    mistake_on_word = mistake_on_word == word_index ? -1 : mistake_on_word;
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
            // jump to next line if end of line
            if (x == max_width && user_char != KEY_BACKSPACE)
            {
                y++;
                x = 0;
            }

            // break loop if end of quote
            if (quote_char_index == quote_length)
            {
                break;
            }

            // Move the cursor to the new position
            move(y, x);

            // Refresh the screen
            // refresh();
        }

        // break loop if enter
        if (user_char == '\n')
        {
            break;
        }

        // Add mistake on final word if there is any
        if (mistake_on_word > -1)
        {
            total_mistakes++;
        }
        if (mistake_on_word == -1)
        {
            total_correct_words++;
        }

        clear();
        refresh();
        //  Get the final time-stamp
        gettimeofday(&end, NULL);

        // Calculate the elapsed time in milliseconds
        double t_ms = (double)(end.tv_sec - start.tv_sec) * 1000.0 +
                      (double)(end.tv_usec - start.tv_usec) / 1000.0;

        // Convert time to seconds with one decimal place
        double t_seconds = round(t_ms / 1000.0 * 10.0) / 10.0;

        // printw("Took %.1f milliseconds to execute \n", t_ms);
        printw("Took %.1f seconds to type \n", t_seconds);
        // printw("Total mistakes: %d\n", total_mistakes);
        // printw("Terminal width: %d\n", max_width);
        calculate_words_per_minute(t_seconds, quote, total_mistakes, total_correct_words);
        /**
         * todo
         * read file and return biggest wpm
         * then compare biggest wpm from file to wpm this round
         * show new record if this round bigger wpm than in file
         * otherwise show personal best
         *
         * what happens if no txt file
         */
        // clear();
        // refresh();

        printw("(s) to stop or any other key to type again\n");
        printw("Go again? ");
        refresh();

        // Get user input to determine whether to type again or quit
        int type_again_user_input = getch();

        if (type_again_user_input == 's')
        {
            type_again = false;
        }
        else
        {
            clear();
            refresh();
        }
    }
}

int main(void)
{
    initscr();
    keypad(stdscr, TRUE);                            // Enable special keys, such as function keys
    start_color();                                   // Enable color support
    init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK); // Define color pair for green
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);     // Define color pair for red
    noecho();                                        // hide user input echo

    type();

    endwin();

    return 0;
}