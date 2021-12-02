#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef struct Node
{
    bool isWord;            // whether or not this is a word
    struct Node **children; // pointers to children
    int wordCount;          // how many words start with this, so we can find common and uncommon words
} Node;

#define BS 15

/* Board Color (BC) Determines bonus points
 * T = Triple Word Score
 * D = Double Word Score
 *
 * t = Triple Letter Score
 * d = Double Letter Score
 *
 * Anything Else = Nothing
 */
const char BC[8][8] =
    {
        {'T', ' ', ' ', ' ', 'd', ' ', ' ', 'T'},
        {' ', 'd', ' ', ' ', ' ', 'd', ' ', ' '},
        {' ', ' ', 't', ' ', ' ', ' ', 't', ' '},
        {' ', ' ', ' ', 'D', ' ', ' ', ' ', ' '},
        {'d', ' ', ' ', ' ', 'D', ' ', ' ', 'd'},
        {' ', 'd', ' ', ' ', ' ', 'D', ' ', ' '},
        {' ', ' ', 't', ' ', ' ', ' ', 'D', ' '},
        {'T', ' ', ' ', ' ', 'd', ' ', ' ', 'T'},
};

void addWord(struct Node *root, char *word);
bool isChar(char c);
bool isNum(char c);
char toLower(char c);
Node *newNode();
void printTree(struct Node *root);
void printTreeR(struct Node *root, int depth);
int readFromFile(Node *root, char *filepath);
bool isWord(Node *root, char *word);
void freeNode(Node *root);
void displayBoard(char *board);
void loadColors();
void printColor(char s[], char color);
char getColor(int x, int y);
int wordScore(Node *root, char *word, int score);
int letterScore(char letter);
void wordScoreMenu(Node *root);
void strCopy(char *a, char *b);
void strCopyEx(char *a, char *b, int x);
bool isIn(char a, char *b);
int len(char *string);
void bestWordMenu(Node *root);
char *bestWord(Node *root, Node *currentNode, char *currentWord, char *letters, int target);
void debug(char *a);
int *getCoord(char input, int x, int temp, int step, int substep);

void freeNode(Node *root)
{
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            freeNode(root->children[i]);
        }
    }
    free(root);
    root = NULL;
}

bool isWord(struct Node *root, char *word)
{
    // more recursion...
    // I'm surprised tbh at how often I'm using recursion...
    // printf("Root is %s null. Index: %d, Char: %d\n", (root == NULL) ? "" : "not", toLower(*word) - 'a', *word);
    if (root == NULL || (!isChar(*word) && !((*word == '\0' || *word == '\n') || *word == 0)))
    {
        return false;
    }
    else
    {
        if ((*word == '\0' || *word == '\n') || *word == 0)
        {
            return root->isWord;
        }
        else
        {
            int index = toLower(*word) - 'a';
            word = word + 1;
            return isWord(root->children[index], word);
        }
    }
}

int readFromFile(struct Node *root, char *filepath)
{
    int words = 0;

    FILE *inFile;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    inFile = fopen(filepath, "r");
    if (inFile == NULL)
    {
        printf("Error with finding file D;\n");
        exit(-1);
    }

    while ((read = getline(&line, &len, inFile)) != -1)
    {
        addWord(root, line);
        words++;
    }

    fclose(inFile);
    if (line)
    {
        free(line);
    }
    line = NULL;
    return (words);
}

char toLower(char c)
{
    if ((('A' <= c && 'Z' >= c) && isChar(c)))
    {
        c = 'a' + (c - 'A');
    }
    return c;
}

bool isChar(char c)
{
    if ((('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c)))
    {
        return true;
    }
    else
        return false;
}

bool isNum(char c)
{
    if ('0' <= c && c <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

Node *newNode()
{
    Node *node;
    node = malloc(sizeof *node);
    node->children = malloc(sizeof *node * 26);
    for (int i = 0; i < 26; i++)
    {
        node->children[i] = NULL;
    }
    node->isWord = false;
    node->wordCount = 0;
    return node;
}

void addWord(struct Node *root, char *word)
{
    char temp = toLower(*word);
    if (root != NULL)
    {
        root->wordCount++;
        if ((temp == '\0' || temp == '\n') || temp == 0)
        {
            root->isWord = true;
        }
        else
        {
            if (isChar(temp))
            {
                int index = temp - 'a';
                if ((root->children[index]) == NULL)
                {
                    (root->children[index]) = newNode();
                }
                addWord((root->children[index]), word + 1);
            }
        }
    }
}

void printTree(struct Node *root)
{
    // top down print of tree, so this it is partially recursive
    printf("Root:\n");
    int depth = 1;
    printTreeR(root, depth);
}

void printTreeR(struct Node *root, int depth)
{
    for (int i = 0; i < 26; i++)
    {
        if ((root->children[i]) != NULL)
        {
            for (int x = 0; x < depth; x++)
            {
                printf("-");
            }
            printf("%c\n", (char)'a' + i);
            printTreeR(root->children[i], depth + 1);
        }
    }
}

void displayBoard(char *board)
{
    printColor("|-------------------------------|\n", ' ');
    // Splitting up the for loop into 2 to make this prettier and easier
    for (int y = 0; y < BS; y++)
    {
        printColor("| ", ' ');
        for (int x = 0; x < BS; x++)
        {
            char str[2];
            str[0] = board[(y * BS) + x];
            str[1] = '\0';
            printColor(str, getColor(x, y));
            printw(" ");
        }
        printColor("|\n", ' ');
    }
    printColor("|-------------------------------|\n", ' ');
    refresh();
}

void printColor(char s[], char color)
{
    attron(COLOR_PAIR(color));
    printw("%s", s);
    attroff(COLOR_PAIR(color));
}

void loadColors()
{
    init_pair('T', COLOR_WHITE, COLOR_RED);
    init_pair('D', COLOR_WHITE, COLOR_YELLOW);
    init_pair('t', COLOR_WHITE, COLOR_BLUE);
    init_pair('d', COLOR_WHITE, COLOR_CYAN);
    init_pair(' ', COLOR_WHITE, COLOR_BLACK);
}

char getColor(int x, int y)
{
    // Only the first quadrant, remap all the other points to it via reflections!
    x = abs(x - 7);
    y = abs(y - 7);

    return BC[y][x];
}

int wordScore(Node *root, char *word, int score)
{
    if (root == NULL || word == NULL)
    {
        return -1;
    }
    else if (toLower(*word) == '\0')
    {
        if (root->isWord)
        {
            return score;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        int index = toLower(*word) - 'a';
        word = word + 1;
        return wordScore(root->children[index], word, score += letterScore(toLower(*word)));
    }
}

int letterScore(char letter)
{
    letter = toLower(letter);
    switch (letter)
    {
    default:
        return 0;
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
    case 's':
    case 't':
    case 'l':
    case 'r':
    case 'n':
        return 1;
    case 'd':
    case 'g':
        return 2;
    case 'b':
    case 'c':
    case 'm':
    case 'p':
        return 3;
    case 'f':
    case 'h':
    case 'v':
    case 'w':
    case 'y':
        return 4;
    case 'k':
        return 5;
    case 'j':
    case 'x':
        return 8;
    case 'q':
    case 'z':
        return 10;
    }
}

void wordScoreMenu(Node *root)
{
    attron(COLOR_PAIR(' '));
    unsigned char word[16];
    for (int i = 0; i < 15;) // max word length = 15 characters
    {
        erase();
        printw("| Please Enter Your Word |\n");
        printw("\"");
        for (int ii = 0; ii < i; ii++)
        {
            printw("%c", word[ii]);
        }
        printw("\"\n");
        refresh();

        word[i] = getch();
        flushinp();

        if (isChar(word[i]))
        {
            word[i] = toLower(word[i]);
            i++;
        }
        else if ((word[i] == 127 || word[i] == 8) || word[i] == 224) // delete or backspace
        {
            i = (i > 0) ? i - 1 : 0;
        }
        else if (word[i] == '\n') // new line
        {
            word[i] = '\0';
            break;
        }
    }
    word[15] = '\0';
    erase();

    int score = wordScore(root, (char *)word, 0);
    if (score <= 0)
    {
        printw("| \"%s\" is not a word! |\n", word);
    }
    else
    {
        printw("| \"%s\" is worth %d points! |", word, score);
    }
    getch();
    attroff(COLOR_PAIR(' '));
}

void strCopy(char *a, char *b)
{
    // to ensure that no issues happen, realloc space
    a = realloc(a, sizeof *a * (len(b) + 1));
    // copy b to a, assume that a has enough space for b
    int i = 0;
    for (; i < len(b); i++)
    {
        a[i] = b[i];
    }
    a[i + 1] = '\0';
}

void strCopyEx(char *a, char *b, int x)
{
    a = realloc(a, sizeof *a * len(b));
    // copy b to a, except for index x
    int i = 0, shift = 0;
    for (; i < len(b); i++)
    {
        if (i == x)
        {
            shift = 1;
        }
        a[i] = b[i + shift];
    }
    a[i + 1] = '\0';
}

bool isIn(char a, char *b)
{
    for (int i = 0; i < len(b); i++)
    {
        if (a == b[i] && a != '\0' && b[i] != '\0') // we should never call this with \0, but just in case
        {
            return true;
        }
    }
    return false;
}

int len(char *string)
{
    int i = 0;
    for (; string[i] != '\0' && string + i != NULL; i++)
        ;
    return i;
}

void bestWordMenu(Node *root)
{
    attron(COLOR_PAIR(' '));
    unsigned char word[BS + 1];
    // Get Word Loop
    for (int i = 0; i < BS;) // max word length = 15 characters
    {
        erase();
        printw("| Please Enter Your Letters |\n");
        printw("\"");
        for (int ii = 0; ii < i; ii++)
        {
            printw("%c", word[ii]);
            if (ii < i - 1)
            {
                printw(" ");
            }
        }
        printw("\"\n");
        refresh();

        word[i] = getch();
        flushinp();

        if (isChar(word[i]))
        {
            word[i] = toLower(word[i]);
            i++;
        }
        else if ((word[i] == 127 || word[i] == 8) || word[i] == 224) // delete or backspace
        {
            i = (i > 0) ? i - 1 : 0;
        }
        else if (word[i] == '\n') // new line
        {
            word[i] = '\0';
            break;
        }
    }
    word[15] = '\0';
    erase();

    int target = 0;
    unsigned char number[5];
    int i = 0;
    for (; i < 4;)
    {
        erase();
        printw("| Please Enter the Target Score |\n");
        printw("|    Must be Less than 9999     |\n");
        printw("\"");
        for (int ii = 0; ii < i; ii++)
        {
            printw("%c", number[ii]);
        }
        printw("\"\n");
        refresh();

        number[i] = getch();
        flushinp();

        if (number[i] >= '0' && number[i] <= '9')
        {
            i++;
        }
        else if ((number[i] == 127 || number[i] == 8) || number[i] == 224) // delete or backspace
        {
            i = (i > 0) ? i - 1 : 0;
        }
        else if (number[i] == '\n') // new line
        {
            number[i] = '\0';
            break;
        }
    }
    for (; i <= 4; i++)
    {
        number[i] = '\0';
    }
    // character array to integer
    // The number can be in the following formats 1.... 10... 100.. 1000.
    // we start from the back and count the null terminators
    int powi = 0;
    int pow[4] = {1, 10, 100, 1000};
    for (i = 4; i >= 0; i--)
    {
        if (number[i] != '\0')
        {
            target += (number[i] - '0') * pow[powi++];
        }
    }

    erase();
    printw("| Finding Best Word for... |\n");
    printw("Letters: %s\n", word);
    printw("Target: %d\n", target);
    refresh();

    char *currentWord;
    currentWord = malloc(sizeof *currentWord * (BS + 1));
    for (int i = 0; i < BS + 1; i++)
    {
        currentWord[i] = '\0';
    }
    currentWord = bestWord(root, root, currentWord, (char *)word, target);

    flushinp();
    erase();
    int bestWordScore = wordScore(root, currentWord, 0);
    if (bestWordScore != -1)
    {
        printw("| Found Best Word: |\n");
        printw("%s\n", currentWord);
        printw("| Score: %d Target: %d |\n", bestWordScore, target);
    }
    else
    {
        printw("| Could not find word from \"%s\" |\n", word);
    }
    refresh();
    free(currentWord);
    getch();
    flushinp();
    attroff(COLOR_PAIR(' '));
}

void debug(char *a)
{
    printw("%s\n", a);
    refresh();
}

char *bestWord(Node *root, Node *currentNode, char *currentWord, char *letters, int target)
{
    /* Recursive Algorithm:
     *
     * Check:
     *  - root & currentNode & currentWord & letters != null.
     *  - currentWord is not the best word possible and is larger than 0 (if it is, return it).
     *
     * Take letter from letters and recur with it if:
     *  - Letter has not been used.
     *  - The new word is on the tree.
     *
     * Save recurred word to current word if:
     *  - is closer to target than best.
     *  - && is a word.
     *  - || best is not a word && current is.
     *
     * Clean up the memory.
     *
     * Return the best word.
     *  - If the best word is not a word, return null.
     */

    // Check:
    if (root == NULL)
    {
        printw("root is null... wtf did you do?\n");
        refresh();
        return NULL;
    }
    else if (root == NULL || currentNode == NULL || currentWord == NULL || letters == NULL)
    {
        return currentWord; // if any of the above are true, then we've got an error...
        // but we can just return the currentWord to avoid memory issues
        // also it will just say -1
    }
    int currentWordLen = len(currentWord);
    int letterLen = len(letters);
    if (wordScore(root, currentWord, 0) == target || letterLen == 0)
    {
        return currentWord;
    }
    // Memory / Var Stuff
    char *cbw;
    cbw = malloc(sizeof *cbw * (BS + 1)); // current best word
    strCopy(cbw, currentWord);            // which always starts out as currentWord
    char *usedLetters;
    usedLetters = malloc(sizeof *usedLetters * (letterLen + 1));
    for (int i = 0; i < letterLen; i++)
    {
        usedLetters[i] = '0';
    }
    usedLetters[letterLen] = '\0';

    // Take letter from letters and recur with it:
    for (int i = 0; i < letterLen; i++)
    {
        if (isIn(letters[i], usedLetters))
        {
            continue; // skip this letter as we've already done it!
        }
        char *tempWord;
        tempWord = malloc(sizeof *tempWord * (currentWordLen + 2)); // +2 for the extra word, and the ending \0
        strCopy(tempWord, currentWord);
        tempWord[currentWordLen] = letters[i];
        tempWord[currentWordLen + 1] = '\0';

        char *tempLetters;
        tempLetters = malloc(sizeof *tempLetters * (letterLen));
        strCopyEx(tempLetters, letters, i);
        tempLetters[letterLen] = '\0';

        // recur with tempWord
        // leave the checking for null stuff to the child recur
        tempWord = bestWord(root, currentNode->children[letters[i] - 'a'], tempWord, tempLetters, target);

        // tempWord is now the best word of child recur tree
        // check temp and cbw, save the new cbw
        int tempWordScore = wordScore(root, tempWord, 0);
        int cbwScore = wordScore(root, cbw, 0);
        if (tempWordScore != -1) // only copy tempWord to cbw if tempWord is a word
        {
            if (abs(cbwScore - target) > abs(tempWordScore - target)) // temp word is closer to the target!
            {
                strCopy(cbw, tempWord);
            }
        }

        free(tempWord);
        tempWord = NULL;
        free(tempLetters);
        tempLetters = NULL;
        // check for special case
        if (tempWordScore == target || cbwScore == target)
        {
            // we have the best word, leave early!
            break;
        }
    }
    // copy the cbw to the currentWord, free everything, then return
    strCopy(currentWord, cbw);
    refresh();
    free(cbw);
    cbw = NULL;
    free(usedLetters);
    usedLetters = NULL;
    return currentWord;
}

int *getCoord(char input, int x, int temp, int step, int substep)
{
    int returnInfo[4];
    int badBoy = 0; // for use if the user doesn't do good input
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
            // ok I originally had a goto here
            // and while I think it was a good use of a goto
            // it is not a good use of a goto when I have to have 2 of them, because that is no good
            // so here I am, rewriting this so it no longer uses gotos
            if (temp != 1)
            {
                badBoy = 1; // someone has been bad D:<
            }
            else
            {
                x = (temp * 10) + (input - '0');
                temp = 0;
                substep = 0;
                step++;
            }
        }
    }
    else if (input == '\n' || input == ' ')
    {
        if (substep == 0)
        {
            badBoy = 1;
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
    if (badBoy)
    {
        substep = 0;
        temp = 0;
        erase();
        printw("You must enter a number between 0-14, or a letter a-%c\n", 'a' + 14);
        getch();
        flushinp();
    }
    // int x, int temp, int step, int substep
    returnInfo[0] = x;
    returnInfo[1] = temp;
    returnInfo[2] = step;
    returnInfo[3] = substep;
    return returnInfo;
}