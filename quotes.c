#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "quotes.h"

char quotes[][500] = {"So many books, so little time.", "A room without books is like a body without a soul.", "You only live once, but if you do it right, once is enough.", "Be the change that you wish to see in the world.", "If you tell the truth, you don't have to remember anything.", "You've gotta dance like there's nobody watching, Love like you'll never be hurt, Sing like there's nobody listening, And live like it's heaven on earth.", "Don't walk in front of me... I may not follow Don't walk behind me... I may not lead Walk beside me... just be my friend", "I believe that everything happens for a reason. People change so that you can learn to let go, things go wrong so that you appreciate them when they're right, you believe lies so you eventually learn to trust no one but yourself, and sometimes good things fall apart so better things can fall together.", "The opposite of love is not hate, it's indifference. The opposite of art is not ugliness, it's indifference. The opposite of faith is not heresy, it's indifference. And the opposite of life is not death, it's indifference."};

/**
 * Generates a random quote
 * srand(time(NULL));
 * To introduce variability and avoid the same sequence of random numbers
 * on each program run, this function seeds the random number generator
 * using the current time as a seed. This ensures that a different seed
 * is used for each execution, leading to a unique sequence of random numbers.
 */
char *get_quote()
{
    srand(time(NULL));
    int length_of_quotes = sizeof(quotes) / sizeof(quotes[0]);
    int random_index = rand() % length_of_quotes;
    return quotes[random_index];
}