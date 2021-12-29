#include <stdlib.h>
#include <stdio.h>

#define LIGHT '#'
#define DARK '.'
#define UNTOUCHED 'X'

#define MAXSIZE 1024 // I am really lazy right now...
                     // The max that this can grow is 1 in each direction each simulation step.
                     // If we exceed this then we're in trouble.

typedef struct Coord
{
    int x, y;
} Coord;

int flip;                          // For inputs that have iea[0] == '.'
char (*map)[MAXSIZE][MAXSIZE];     // The current map.
char mapData[MAXSIZE][MAXSIZE];    // Actual data of map.
char (*tempMap)[MAXSIZE][MAXSIZE]; // Temp map used for simulating.
char tempData[MAXSIZE][MAXSIZE];   // Actual data of tempMap.
Coord botRight, topLeft;           // The size of the map.
char iea[513];                     // Image Enchancement Algorithm.

void simulate(int steps); // Main method.
int count();              // Counts how many lit pixels there are.
void printMap();          // Prints the map.

int main(int argc, char **argv)
{
    map = &mapData;
    tempMap = &tempData;
    int buffIndex, fileSize;
    char *restrict buffer;
    FILE *input;
    input = fopen(argv[1], "r");
    fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    buffer = malloc((sizeof *buffer) * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);
    for (int i = 0; i < 512; i++)
    {
        iea[i] = buffer[i];
    }
    iea[512] = '\0';

    topLeft.x = topLeft.y = MAXSIZE / 2;
    botRight.x = botRight.y = 0;

    // Initialize our map.
    for (int y = 0; y < MAXSIZE; y++)
    {
        for (int x = 0; x < MAXSIZE; x++)
        {
            (*map)[x][y] = UNTOUCHED;
            (*tempMap)[x][y] = UNTOUCHED;
        }
    }
    // Read input to map.
    buffIndex = 512 + 2; // First line of input + 2 newlines.
    for (int y = topLeft.y; buffIndex < fileSize; y++)
    {
        botRight.y = (y >= botRight.y) ? y + 1 : botRight.y; // Update botRight.y to y if it's less than the current y.
        for (int x = topLeft.x; buffIndex < fileSize && buffer[buffIndex] != '\n'; x++)
        {
            (*map)[x][y] = buffer[buffIndex++];
            botRight.x = (x >= botRight.x) ? x + 1 : botRight.x; // Update botRight.x to x if it's less than the current x.
        }
        buffIndex++; // Skip newline.
    }
    free(buffer);

    flip = 0;
    simulate(2);
    printf("Part A Solution: %d\n", count());
    // 5503
    
    simulate(48);
    printf("Part B Solution: %d\n", count());
    // 19156

    // Enable if you want to see what the final map looks like.
    // printMap();
    return 0;
}

void printMap()
{
    for (int y = topLeft.y - 1; y < botRight.y + 1; y++)
    {
        for (int x = topLeft.x - 1; x < botRight.x + 1; x++)
        {
            printf("%c", (*map)[x][y]);
        }
        printf("\n");
    }
}

int count()
{
    int returnVal = 0;
    if (flip == 1)
    {
        return -1; // Infinite lit pixels!
    }
    else
    {
        for (int y = topLeft.y; y < botRight.y; y++)
        {
            for (int x = topLeft.x; x < botRight.x; x++)
            {
                if ((*map)[x][y] == LIGHT)
                {
                    returnVal++;
                }
            }
        }
    }
    return returnVal;
}

void simulate(int steps)
{
    // The meat of this challenge.
    for (int step = 0; step < steps; step++)
    {
        // Increase bounds.
        topLeft.x--;
        topLeft.y--;
        botRight.x++;
        botRight.y++;

        // Calculate new values.
        for (int y = topLeft.y; y < botRight.y; y++)
        {
            for (int x = topLeft.x; x < botRight.x; x++)
            {
                // Convert the 3x3 filter to a 9 bit integer.
                int i = 8;
                int number = 0;
                for (int bitY = y - 1; bitY <= y + 1; bitY++)
                {
                    for (int bitX = x - 1; bitX <= x + 1; bitX++)
                    {
                        if ((*map)[bitX][bitY] == LIGHT ||                               // If it's lit.
                            ((*map)[bitX][bitY] == UNTOUCHED && flip == 1&& iea[0] == LIGHT) // Or our untouched bits are all lit.
                        )
                        {
                            number = (number | (0x01U << i));
                        }
                        i--;
                    }
                }
                // Update the temp map!
                (*tempMap)[x][y] = iea[number];
            }
        }

        // Flip tempMap and map!
        char(*tempPointer)[MAXSIZE][MAXSIZE];
        tempPointer = map;
        map = tempMap;
        tempMap = tempPointer;

        // Flip flip!
        flip = (flip == 1) ? 0 : 1;
    }
}