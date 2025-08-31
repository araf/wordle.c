#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Uppercase a string in place
void to_uppercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

char *get_random_word() // Took help from Gemini for this block function
{
    FILE *file;
    static char word[7];
    int total_words = 0;
    int random_index;
    int current_index = 0;

    file = fopen("wordlist/selected_words.txt", "r");
    if (file == NULL)
    {
        return NULL;
    }

    while (fgets(word, sizeof(word), file))
    {
        total_words++;
    }

    if (total_words == 0)
    {
        fclose(file);
        return NULL;
    }

    srand(time(NULL));
    random_index = rand() % total_words;
    rewind(file);

    while (fgets(word, sizeof(word), file))
    {
        if (current_index == random_index)
        {
            word[strcspn(word, "\n")] = 0;
            fclose(file);
            return word;
        }
        current_index++;
    }

    fclose(file);
    return NULL;
}

bool word_found(const char *target_word)
{
    if (strlen(target_word) != 5)
    {
        return false;
    }
    FILE *file;
    char buffer[6];
    file = fopen("wordlist/all_words.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }
    while (fread(buffer, 1, 6, file) == 6)
    {
        if (strncmp(buffer, target_word, 5) == 0)
        {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    printf("Not a valid 5-letter word!\n");
    return false;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear"); // add
#endif
}

int main(int argc, char *argv[])
{
    char *ans;

    if (argc == 1 || (argc == 2 && strcmp(argv[1], "play") == 0))
    {
        ans = get_random_word();
    }
    else if (argc == 3 && strcmp(argv[1], "debug") == 0)
    {
        if (word_found(argv[2]))
        {
            ans = argv[2];
        }
        else
        {
            printf("Word must be 5-letter and in the list.\n");
            return 1;
        }
    }
    else
    {
        printf(
            "Usecases: \nPlay: ./wordle OR ./wordle play\nDebug: ./wordle debug [5-letter_word]\n");
        return 1;
    }

    char txt[500] = "";
    const char *cols[] = {"\x1b[90m", "\x1b[32m", "\x1b[33m"};
    char opt[64];
    const char *title = "\n\t\t\t===========\n \t\t\t WORDLE .C \n\t\t\t===========\n\n";

    for (int k = 0; k < 6; k++)
    {
        snprintf(txt + strlen(txt), sizeof(txt) - strlen(txt), "\t\t\t ");
        if (k == 0)
        {
            clearScreen();
            printf("%s", title);
            for (int i = 0; i < 6; i++)
            {
                printf("\t\t\t ");
                for (int j = 0; j < 5; j++)
                {
                    printf("_ ");
                }
                printf("\n");
            }
            printf("\n");
        }
        else
        {
            printf("\n\x1b[37m[Hints] Grey: incorrect, Yellow: incorrect place, Green: "
                   "correct.\x1b[0m");
        }

        while (1)
        {
            printf("\nEnter a 5-letter word: ");
            if (scanf("%5s", opt) != 1)
            {
                printf("Input error.\n");
                // clear input and retry
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) { }
                continue;
            }
            // discard rest of the line to keep input clean if user typed more
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            if (word_found(opt))
                break;
        }

        clearScreen();
        int signs[6] = {0, 0, 0, 0, 0, 0};
        int used_oranges[6] = {0, 0, 0, 0, 0, 0};

        for (int i = 0; i < 5; ++i)
        {
            if (opt[i] == ans[i])
            {
                signs[i] = 1;
            }
        }
        for (int i = 0; i < 5; ++i)
        {
            if (signs[i] == 0) // only if not green
            {
                for (int j = 0; j < 5; ++j)
                {
                    if (ans[j] == opt[i] && used_oranges[j] == 0 && signs[j] == 0)
                    {
                        signs[i] = 2;
                        used_oranges[j] = 1;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < 5; ++i)
        {
            snprintf(txt + strlen(txt), sizeof(txt) - strlen(txt),
                     "%s%c\x1b[0m ", cols[signs[i]], toupper((unsigned char)opt[i]));
        }
        snprintf(txt + strlen(txt), sizeof(txt) - strlen(txt), "\n");
        printf("%s%s", title, txt);
        for (int i = k + 1; i < 6; i++)
        {
            printf("\t\t\t ");
            for (int j = 0; j < 5; j++)
            {
                printf("_ ");
            }
            printf("\n");
        }
        if (!strcmp(ans, opt))
        {
            printf("\n\t\t\t %sYOU WON!!\x1b[0m\n\n\n", cols[1]);
            return 0;
        }
    }
    to_uppercase(ans);
    printf("\n\t\t\t %sYOU LOST! \x1b[0m\n\t\t\t Ans: %s\n\n", cols[0], ans);
    return 1;
}
