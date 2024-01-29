#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <ctype.h>
#include <menu.h>
#include "quotes.h"
#include "menu_interface.h"

#define GREEN_TEXT 1
#define RED_TEXT 2
#define RESET_COLOR 3
#define SPACE ' '
#define UNDERSCORE '_'

// Define the states of the program
enum State
{
    STARTING_SCREEN,
    TYPING,
    ENDING
};

enum State current_state = STARTING_SCREEN;
ITEM *menu_option; // returns option from run_menu()

struct timeval start, end;

void initialize_ncurses()
{
    initscr();
    keypad(stdscr, TRUE);                            // Enable special keys, such as function keys
    start_color();                                   // Enable color support
    init_pair(GREEN_TEXT, COLOR_GREEN, COLOR_BLACK); // Define color pair for green
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);     // Define color pair for red
    noecho();                                        // hide user input echo
}

void handle_color_change(char character, int color)
{
    attron(COLOR_PAIR(color)); // Turn on color
    printw("%c", character);
    attroff(COLOR_PAIR(color)); // Turn off color
}

void clear_and_refresh()
{
    clear();
    refresh();
}

void get_type_again_option(bool *type_again)
{
    while (true)
    {
        int type_again_user_input = getch();

        if (type_again_user_input == 'n')
        {
            *type_again = false;
            // jump to starting screen if type again is no
            current_state = STARTING_SCREEN;
            break;
        }
        else if (type_again_user_input == 'y')
        {
            clear_and_refresh();
            break;
        }
    }
}

bool handle_typing(char *quote, size_t quote_length, int max_width, int *total_correct_words, int *total_mistakes)
{
    int user_char;
    int x = 0;
    int y = 0;
    bool timer_started = false;
    size_t quote_char_index = 0;
    int word_index = 0;
    // variable to store the earliest index where a mistake occurs within the current word
    int mistake_on_word = -1;

    printw("%s\n", quote);
    move(y, x); // move cursor to top of first letter

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
                // if no mistakes on word
                if (mistake_on_word == -1)
                {
                    (*total_correct_words)++;
                }
                else
                {
                    (*total_mistakes)++;
                }

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

    // break loop if enter key is pressed
    if (user_char == '\n')
    {
        return true;
    }

    // Add mistake on final word if there is any
    if (mistake_on_word > -1)
    {
        (total_mistakes)++;
    }
    if (mistake_on_word == -1)
    {
        (*total_correct_words)++;
    }

    return false;
}

// not the best solution to count words but works for now
int count_words(const char *text)
{
    int word_count = 0;
    for (size_t i = 0; i < strlen(text); i++)
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

void calculate_statistics(struct timeval start, struct timeval end, char *quote, int total_mistakes, int total_correct_words)
{
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
}

// main typing function
void type(const char *game_option)
{
    clear_and_refresh();

    bool type_again = true;

    while (type_again)
    {
        int max_width, max_height;
        getmaxyx(stdscr, max_height, max_width);
        char *quote = get_quote();
        size_t quote_length = strlen(quote);
        int total_correct_words = 0;
        int total_mistakes = 0;

        bool enter_pressed = handle_typing(quote, quote_length, max_width, &total_correct_words, &total_mistakes);
        if (enter_pressed)
        {
            current_state = STARTING_SCREEN;
            break;
        }

        clear_and_refresh();

        struct timeval end;
        gettimeofday(&end, NULL);

        calculate_statistics(start, end, quote, total_mistakes, total_correct_words);

        printw("Go again? (y/n): ");
        get_type_again_option(&type_again);
    }
}

// program structure
void run_program()
{
    while (current_state != ENDING)
    {
        clear_and_refresh();

        switch (current_state)
        {
        case STARTING_SCREEN:
            menu_option = run_menu();
            if (menu_option != NULL && strcmp(item_name(menu_option), "Exit") != 0)
            {
                current_state = TYPING;
            }
            else
            {
                current_state = ENDING;
            }
            break;
        case TYPING:
            const char *option = item_name(menu_option);
            type(option);
            break;
        }

        clear_and_refresh();
    }
}

int main(void)
{
    initialize_ncurses();

    run_program();

    endwin();

    return 0;
}