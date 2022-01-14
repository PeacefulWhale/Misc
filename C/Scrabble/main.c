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
int playScore(char *board, int x, int y, int dir, char *word);

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
    tempY = malloc(sizeof *tempY * 3);

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
        if (x == -1)
        {
            strCopy(tempX, "??");
        }
        else
        {
            sprintf(tempX, "%2d", x);
        }

        if (y == -1)
        {
            strCopy(tempY, "??");
        }
        else
        {
            sprintf(tempY, "%2d", y);
        }
        // If dir is -1, it hasn't been set.

        char *dirStr = (dir == -1) ? "???" : (dir == 0) ? "Down"
                                                        : "Right"; // sorry for the fat one liner ternary thing
        char *wordStr = (len(word) > 0) ? word : "???";
        printw("| %s %s %s %s |\n\n", tempX, tempY, dirStr, wordStr);
        printw("| Press \"`\" to quit\n");
        // Input Instructions:
        switch (step)
        {
        case 0:
            printw("Enter X Coord\n");
            break;
        case 1:
            printw("Enter Y Coord\n");
            break;
        case 2:
            substep = 0; // Reset substep so we can use it later for the word.
            printw("Enter Direction with 0/d or 1/r\n");
            break;
        case 3:
            printw("Enter Word\n");
            break;
        }
        refresh();
        // get X / Y coords
        // accept number or letter for either of them
        char input = getch();
        flushinp();
        if (input == '`')
        {
            break;
        }
        else
        {
            switch (step)
            {
            // I really don't like how Prettier handles switch statements...
            case 0: // X Coordinate.
            {
                int xInfo[4];
                getCoord(xInfo, input, x, temp, step, substep);
                x = xInfo[0];
                temp = xInfo[1];
                step = xInfo[2];
                substep = xInfo[3];
                break;
            }
            case 1: // Y Coordinate.
            {
                int yInfo[4];
                getCoord(yInfo, input, y, temp, step, substep);
                y = yInfo[0];
                temp = yInfo[1];
                step = yInfo[2];
                substep = yInfo[3];
                break;
            }
            case 2: // Direction.
            {
                switch (input)
                {
                case '0':
                case 'd':
                    dir = 0;
                    break;
                case '1':
                case 'r':
                    dir = 1;
                    break;
                default:
                    clear();
                    printw("You must enter 0/d or 1/r!\n");
                    refresh();
                    getch();
                }
                if (dir != -1)
                {
                    step++;
                }
            }
            case 3: // Word.
            {
                if (substep == BS || input == '\n' || input == ' ')  // New line / max word size.
                {
                    step++;
                }
                else if (isChar(input))
                {
                    word[substep++] = input;
                }
                else if (isDel(input)) // Delete or backspace.
                {
                    substep = substep ? substep - 1 : 0;
                }
                word[substep] = '\0';
            }
            }
        }
        refresh();
        attroff(COLOR_PAIR(' '));
    }

    // We now have our input, and can actually add the play to the board!
    // We will calclate the score of the play, display it, and then ask the user if they want to add it to the board.
    free(word);
    free(tempX);
    free(tempY);
    tempX = tempY = word = NULL;
}