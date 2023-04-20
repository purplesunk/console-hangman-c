#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void setPartialWord(char *partial_word, const char *choosen_word);

int main(int argc, char **argv) {
    srand(time(NULL));

    if (argc != 2) {
        printf("Incorrect amount of arguments. USAGE: hangman {wordsfile}.txt\n");
        return 1;
    }

    FILE *words = fopen(argv[1], "r");
    if (words == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    const int MAX_WORD_LENGTH = 128;

    int line_count = 0;
    int word_length = 0;

    char c;
    while (fread(&c, sizeof(char), 1, words)) {
        if (word_length >= MAX_WORD_LENGTH) {
            printf("Max word length surpassed.\n");
            fclose(words);
            return 1;
        }
        if (c == '\n' || c == EOF) {
            ++line_count;
            word_length = 0;
        } else {
            ++word_length;
        }
    }

    if (line_count == 0 && word_length == 0) {
        printf("Could not read a word to choose.\n");
        fclose(words);
        return 1;
    }

    char choosen_word[MAX_WORD_LENGTH];
    choosen_word[0] = '\0';
    int words_gotten = 0;

    while (strlen(choosen_word) == 0) {
        // Put the pointer of the file in the beggining
        fseek(words, 0, SEEK_SET);

        int choose_random = rand() % line_count;
        int word_index = 0;
        int i = 0;

        while (fread(&c, sizeof(char), 1, words)) {
            if (word_index > choose_random) {
                choosen_word[i] = L'\0';
                break;
            }

            if (c == '\n' || c == EOF) {
                ++word_index;
            } else if (word_index == choose_random && i < MAX_WORD_LENGTH) {
                choosen_word[i] = toupper(c);
                ++i;
            }
        }

        ++words_gotten;
        // If it checks more times than there are lines in the file, just quit.
        if (words_gotten > line_count) {
            printf("Could not read any words. Use other file.\n");
            fclose(words);
            return 1;
        }
    }

    char partial_word[128];
    const int choosen_word_length = strlen(choosen_word);
    setPartialWord(partial_word, choosen_word);

    int tries = 0;

    // Gameplay
    while (1) {
        printf("Tries left: '%i'\n", 5 - tries);
        printf("The word is: '%s'\n", partial_word);
        printf("Choose a letter: ");

        char letter;
        int char_read = scanf("%c", &letter);

        letter = toupper(letter);

        while ((c = getchar()) != '\n' && c != EOF) {
        };

        if (char_read == 1) {
            int change = 0;
            for (int i = 0; i < choosen_word_length; ++i) {
                if (choosen_word[i] == letter) {
                    partial_word[i] = choosen_word[i];
                    change = 1;
                }
            }

            if (!change) {
                ++tries;
            }
        }

        if (!strcmp(choosen_word, partial_word) || tries == 5) {
            break;
        }
    }

    if (tries == 5) {
        printf("You lose.\n");
        printf("The word was: '%s'\n", choosen_word);
    } else {
        printf("You win!!\n");
        printf("The word was: '%s'\n", choosen_word);
    }

    fclose(words);

    return 0;
}

void setPartialWord(char *partial_word, const char *choosen_word) {
    int i;
    for (i = 0; choosen_word[i] != '\0'; ++i) {
        partial_word[i] = '_';
    }
    partial_word[i] = '\0';
}
