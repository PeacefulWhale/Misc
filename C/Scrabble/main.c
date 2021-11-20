#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "func.h" // put functions here so the file is pretty

/* ToDo:
 * Serialize the text file and modify readFromFile accordingly.
 * Create Scrabble algorithm (this is the biggest one XD)
 * Create "Add Play" menu and function
 * Much more stuff
 */

void mainLoop(char *board, Node *root);
void addPlayMenu(char *board, Node *root);
void addPlay(char *board, int x, int y, int dir, char *word);

int main()
{
    // Init Stuff:
    Node *root = newNode();
    printf("Loading Dictionary\n");
    int loadedWords = readFromFile(root, "./dictionary.txt");
    printf("Loaded %d words\n", loadedWords);

    // board init
    char *board;
    board = malloc(sizeof *board * (BS * BS));

    for (int x = 0; x < BS * BS; x++)
    {
        board[x] = ' ';
    }
    printf("Board Created\n");

    // Initialize nCurses
    initscr();
    cbreak();
    noecho();
    start_color();
    loadColors();

    // Controll Loop:
    mainLoop(board, root);

    // Exit Stuff:
    endwin();

    printf("Freeing Memory\n");
    freeNode(root);
    free(board);
    root = NULL;
    board = NULL;
    printf("Memory Freed\n");

    return 0;
}

void mainLoop(char *board, Node *root)
{
    // Let's separate the gameplay loops into their own functions so we can debug them better.
    bool menu = true; // are the booleans coming from stdlib or ncurses? I'll include stdbool just in case.
                      // It feels wrong to use booleans instead of ints in C, but it improves readability so I'll use them.
    clear();
    while (menu)
    {
        erase();
        attron(COLOR_PAIR(' '));
        displayBoard(board);
        printw("| Choose Action:             |\n");
        printw("|----------------------------|\n");
        printw("| 1: Calculate Best Word     |\n");
        printw("| 2: Calculate Best Play     |\n");
        printw("| 3: Add Play to Board       |\n");
        printw("| 4: Calculate Score of Word |\n");
        printw("| `: Quit                    |\n");
        attroff(COLOR_PAIR(' '));

        refresh();
        char input = getch();

        switch (input)
        {
        case '1':
            bestWordMenu(root);
            break;

        case '2':

            break;

        case '3':
            addPlayMenu(board, root);
            break;

        case '4':
            wordScoreMenu(root);
            break;

        case '`':
            menu = false;
            break;
        }
    }
}

void addPlayMenu(char *board, Node *root)
{
    // char *board, int x, int y, int dir, char *word
    int x = -1, y = -1, dir = -1, temp = 0; // dir = 0 = down, dir = 1 = right
    char *word;
    word = malloc(sizeof *word * (BS + 1));
    word[0] = '\0';
    char *tempX, *tempY;
    tempX = malloc(sizeof *tempX * 3); // temp for this only needs to be 3 characters long (including \0)
    tempY = malloc(sizeof *tempY* 3);

    strCopy(tempX, "??");
    strCopy(tempY, "??");

    int step = 0;
    int substep = 0;
    while (step < 4)
    {
        // the online scrabble game decides if a play is down / right depeneding on whether the letter or number is first
        // we will just make our users input the actual direction, otherwise this can get confusing
        erase();
        displayBoard(board);
        attron(COLOR_PAIR(' '));
        printw("| Enter Word Play |\n");
        printw("| X  Y  Dir  Word |\n");
        if(x == -1)
        {
            strCopy(tempX, "??");
        }
        else
        {
            sprintf(tempX, "%d", x);
        }

        if(y == -1)
        {
            strCopy(tempY, "??");
        }
        else
        {
            sprintf(tempY, "%d", y);
        }
        char *dirStr = (dir == -1) ? "???" : (dir == 0) ? "Down" : "Right"; // sorry for the fat one liner ternary thing
        char *wordStr = (len(word) > 0) ? word : "???";
        printw("| %s %s %s %s |\n\n", tempX, tempY, dirStr, wordStr);
        printw("| Press \"`\" to quit\n");
        refresh();
        // get X / Y coords
        // accept number or letter for either of them
        char input = getch();
        flushinp();
        if (input == '`')
        {
            break;
        }
        else if (step == 0)
        {
            if (isChar(input) && substep == 0)
            {
                input = toLower(input);
                // they've entered a character, which makes this nice and easy
                x = input - 'a';
                step++;
            }
            else if (isNum(input) && input - '0' < BS)
            {
                // they've entered a number, those darn twats...
                if (substep == 0)
                {
                    // save to temp instead
                    temp = input - '0';
                    substep++;
                }
                else
                {
                    if (temp != 1)
                    {
                        goto badBoy; // bite me, gotos have legitimate uses, and this one is super reasonable, it's basically just a jmp a few lines down D:<
                    }
                    x = (temp * 10) + (input - '0');
                    temp = 0;
                    substep = 0;
                    step++;
                }
            }
            else if(input == '\n' || input == ' ')
            {
                if(substep == 0)
                {
                    goto badBoy; // sorry for the goTos
                }
                else
                {
                    // 1 digit word
                    x = temp;
                    temp = 0;
                    substep = 0;
                    step++;
                }
            }
            else
            {
                badBoy:
                substep = 0;
                temp = 0;
                erase();
                printw("You must enter a number between 0-14, or a letter a-%c\n", 'a' + 14);
                getch();
                flushinp();
            }
        }
        else if (step == 1)
        {
            if (isChar(input) && substep == 0)
            {
                input = toLower(input);
                // they've entered a character, which makes this nice and easy
                y = input - 'a';
                step++;
            }
            else if (isNum(input) && input - '0' < BS)
            {
                // they've entered a number, those darn twats...
                if (substep == 0)
                {
                    // save to temp instead
                    temp = input - '0';
                    substep++;
                }
                else
                {
                    if (temp != 1)
                    {
                        goto badBoy; // bite me, gotos have legitimate uses, and this one is super reasonable, it's basically just a jmp a few lines down D:<
                    }
                    y = (temp * 10) + (input - '0');
                    temp = 0;
                    substep = 0;
                    step++;
                }
            }
            else if(input == '\n' || input == ' ')
            {
                if(substep == 0)
                {
                    goto badBoy; // sorry for the goTos
                }
                else
                {
                    // 1 digit word
                    y = temp;
                    temp = 0;
                    substep = 0;
                    step++;
                }
            }
            else
            {
                badBoy:
                substep = 0;
                temp = 0;
                erase();
                printw("You must enter a number between 0-14, or a letter a-%c\n", 'a' + 14);
                getch();
                flushinp();
            }
        }
        refresh();
        attroff(COLOR_PAIR(' '));
    }
    free(word);
    free(tempX);
    free(tempY);
    tempX = tempY = word = NULL;
}