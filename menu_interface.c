#include <stdlib.h>
#include <menu.h>
#include "menu_interface.h"

char *choices[] = {
    "Normal mode",
    "Hard mode",
    "Exit",
};

// todo add parameters so menu variables are not hard coded
ITEM *run_menu()
{
    ITEM **my_items;
    int c;
    MENU *my_menu;
    int number_of_choices;
    ITEM *cur_item;

    number_of_choices = sizeof(choices) / sizeof(choices[0]);
    my_items = (ITEM **)calloc(number_of_choices + 1, sizeof(ITEM *));

    for (int i = 0; i < number_of_choices; i++)
    {
        my_items[i] = new_item(choices[i], "");
    }
    my_items[number_of_choices] = (ITEM *)NULL;

    my_menu = new_menu((ITEM **)my_items);

    mvprintw(LINES - 3, 0, "j/k: move down and up or with arrow keys");
    mvprintw(LINES - 2, 0, "F1 to Exit");
    post_menu(my_menu);

    while ((c = getch()) != KEY_F(1))
    {
        switch (c)
        {
        case KEY_DOWN:
        case 'j':
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
        case 'k':
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10: // enter
            cur_item = current_item(my_menu);
            return cur_item;
        }
    }

    free_menu(my_menu);
    for (int i = 0; i < number_of_choices; ++i)
    {
        free_item(my_items[i]);
    }
    return NULL;
}