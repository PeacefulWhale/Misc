#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2 // Max file size.
#define MAXPULL 150       // Max pull size.
#define BOARDSIZE 5       // Size of board.

typedef struct Board
{
    int turns;
    int *items;
    struct Board *next;
} Board;

int isNum(char c);
Board *newBoard(Board *prev);
void freeBoards(Board *board);
int score(Board *board);
int printBoard(Board *board);
int main(int argc, char **argv)
{
    if (argv[1] == NULL)
    {
        printf("No input file selected!\n");
        return 1;
    }
    FILE *input;
    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Input filepath is invalid!\n");
        return 1;
    }
    char buffer[BUFFSIZE];
    fread(buffer, BUFFSIZE, 1, input);
    int index = 0;

    // Get the numbers we've pulled in the correct order.
    struct pulledNumbers
    {
        int pull[MAXPULL];
        int length;
    } pulled;
    pulled.length = 0;
    while (isNum(buffer[index]) || buffer[index] == ',')
    {
        // All digits are less than 2 characters large.
        pulled.pull[pulled.length] = buffer[index] - '0';
        index++;
        if (isNum(buffer[index]))
        {
            pulled.pull[pulled.length] *= 10;
            pulled.pull[pulled.length] += buffer[index] - '0';
            index++;
        }
        index++;
        pulled.length++;
    }

    // Create boards.
    // We're using a single linked list as we just need to loop through all the boards in one direction.
    Board *rootBoard, *currentBoard;
    rootBoard = newBoard(NULL);
    currentBoard = rootBoard;
    int offset = 0; // Which location to put our value into the board.
    while (buffer[index] != '\0')
    {
        // This should skip spaces and newlines.
        // Very simillar to our method of getting numbers from the first line.
        // However this must be modified to save the temp to the correct index.
        if (isNum(buffer[index]))
        {
            // This is at the front instead of the end of this code segment so we don't create an empty "last board".
            if (offset == BOARDSIZE * BOARDSIZE)
            {
                // When offset is greater than or equal to total board size, it means that we need to make a new board.
                Board *new;
                new = newBoard(currentBoard);
                currentBoard->next = new;
                currentBoard = new;
                offset = 0;
            }

            currentBoard->items[offset] = buffer[index] - '0';
            index++;
            if (isNum(buffer[index]))
            {
                currentBoard->items[offset] *= 10;
                currentBoard->items[offset] += buffer[index] - '0';
                index++;
            }
            offset++;
        }
        index++;
    }
    // "currentBoard" now points to the last board saved.
    currentBoard->next = NULL;
    // We're done reading input, we can close it.
    fclose(input);

    // Variables for our least and greatest boards.
    // So we don't have to sort the array.
    Board *partA, *partB;
    currentBoard = rootBoard;
    partA = partB = currentBoard;
    while (currentBoard != NULL)
    {
        int won = 0;

        // The main logic for part A and part B.
        for (int playIndex = 0; playIndex < pulled.length && !won; playIndex++)
        {
            int play = pulled.pull[playIndex];
            for (int i = 0; i < BOARDSIZE * BOARDSIZE && !won; i++)
            {
                // Check to see if the play matches any square on the board.
                if (currentBoard->items[i] == play) // We have a match!
                {
                    // -1 == Erased / Scored
                    currentBoard->items[i] = -1;
                    // Check and see if this board is a winner (at this location!)
                    int sum;
                    int col = (int)(i % BOARDSIZE);
                    sum = 0;
                    for (int i = 0; i < BOARDSIZE; i++)
                    {
                        if (currentBoard->items[(i * BOARDSIZE) + col] == -1)
                        {
                            sum++;
                        }
                    }
                    if (sum == 5)
                    {
                        // This is a winner!
                        currentBoard->turns = playIndex;
                        won = 1;
                        break;
                    }
                    sum = 0;
                    int row = (int)(i / BOARDSIZE);
                    for (int i = 0; i < BOARDSIZE; i++)
                    {
                        if (currentBoard->items[(row * BOARDSIZE) + i] == -1)
                        {
                            sum++;
                        }
                    }
                    if (sum == 5)
                    {
                        // This is a winner!
                        currentBoard->turns = playIndex;
                        won = 1;
                        break;
                    }
                }
            }
        }
        if (currentBoard->turns < partA->turns || partA->turns == -1)
        {
            partA = currentBoard;
        }
        if (currentBoard->turns > partB->turns || partB->turns == -1)
        {
            partB = currentBoard;
        }
        currentBoard = currentBoard->next;
    }

    printf("\n===============================\n");
        printf("Part A won in %d\n", partA->turns);
    int partAScore = printBoard(partA);
    printf("Solution: %d\n", partAScore * pulled.pull[partA->turns]);
    // Solution: 10680
    printf("\nPart B won in %d\n", partB->turns);
    int partBScore = printBoard(partB);
    printf("Solution: %d\n", partBScore * pulled.pull[partB->turns]);
    // Solution: 31892
}

Board *newBoard(Board *prev)
{
    Board *board;
    board = malloc(sizeof *board);
    int *items;
    items = malloc(sizeof *items * BOARDSIZE * BOARDSIZE);
    for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++)
    {
        items[i] = -2; // If there is a -2 in our board, then something has gone wrong!
    }
    board->items = items;
    board->next = NULL;
    board->turns = -1;
    return board;
}

void freeBoards(Board *board)
{
    if (board != NULL)
    {
        if (board->items != NULL)
        {
            free(board->items);
            board->items = NULL;
        }
        freeBoards(board->next);
        free(board);
        board = NULL;
    }
}

int isNum(char c)
{
    return (c >= '0' && c <= '9');
}

int printBoard(Board *board)
{
    // Prints the board, and returns the score of the board.
    int score = 0;
    for (int i = 0; i < BOARDSIZE; i++)
    {
        for (int j = 0; j < BOARDSIZE; j++)
        {
            if (board->items[(i * BOARDSIZE) + j] == -1)
            {
                printf("X ");
            }
            else
            {
                printf("%d ", board->items[(i * BOARDSIZE) + j]);
                score += board->items[(i * BOARDSIZE) + j];
            }
        }
        printf("\n");
    }
    return score;
}