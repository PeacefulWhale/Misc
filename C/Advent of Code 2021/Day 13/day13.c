#include <stdio.h>
#include <stdlib.h>

#define MAXMAPSIZE 2000
#define EMPTY ' '
#define FILLED '#'

int buffIndex;
char *buffer;

int getNextNum();
int isNum(const char c);
int printMap(const char map[MAXMAPSIZE][MAXMAPSIZE], const int x, const int y, const int print); // Part A solution (also prints the map).

int main(int argc, char **argv)
{
    buffIndex = 0;
    if (argv[1] == NULL)
    {
        printf("Invalid Input!\n");
        return 1;
    }
    FILE *input;
    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Input filepath is invalid!\n");
        return 1;
    }
    // Get file size.
    int fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    // Malloc file.
    buffer = malloc(sizeof *buffer * fileSize);
    if (buffer == NULL)
    {
        printf("Could not allocate space!\n");
        return 1;
    }
    fread(buffer, fileSize, 1, input);
    fclose(input);

    // Parse Input.
    char map[MAXMAPSIZE][MAXMAPSIZE];
    for (int y = 0; y < MAXMAPSIZE; y++)
    {
        for (int x = 0; x < MAXMAPSIZE; x++)
        {
            map[x][y] = EMPTY;
        }
    }
    struct size
    {
        int x;
        int y;
    } size;
    size.x = 0;
    size.y = 0;
    // Read until the folding steps.
    while (buffer[buffIndex] != '\n')
    {
        // Loop through and get all the dots.
        int x, y;
        x = getNextNum();
        y = getNextNum();
        map[x][y] = FILLED;
        size.x = (size.x <= x) ? x : size.x;
        size.y = (size.y <= y) ? y : size.y;
        // Skip newline.
        buffIndex++;
    }
    // Skip newline again.
    buffIndex++;

    // Read the folding steps and fold.
    int folds = 0;
    while (buffIndex < fileSize)
    {
        buffIndex += 11;
        char direction = buffer[buffIndex];
        int position = getNextNum();
        // Skip until next line.
        while (buffer[buffIndex] != '\n' && buffIndex < fileSize)
        {
            buffIndex++;
        }
        buffIndex++;
        // Fold!
        int edgeBorder = (direction == 'x') ? size.y : size.x;
        int distanceBorder = (direction == 'x') ? size.x : size.y;

        for (int edgePos = 0; edgePos <= edgeBorder; edgePos++)
        {
            for (int edgeDist = position; edgeDist <= distanceBorder; edgeDist++)
            {
                struct coords
                {
                    int x;
                    int y;
                } current, target;
                // Get distance it needs to move up.
                if (direction == 'x') // X-Fold.
                {
                    // Current Position.
                    current.y = edgePos;
                    current.x = edgeDist;
                    // Target Position.
                    target.y = edgePos;
                    target.x = (2 * position) - edgeDist;
                }
                else // Y-Fold.
                {
                    // Current Position.
                    current.x = edgePos;
                    current.y = edgeDist;
                    // Target Position.
                    target.x = edgePos;
                    target.y = (2 * position) - edgeDist;
                }
                
                if (map[current.x][current.y] == FILLED)
                {
                    map[target.x][target.y] = FILLED;
                }
            }
        }
        // Update the size of the board.

        if (direction == 'x')
        {
            size.x = position - 1;
        }
        else
        {
            size.y = position - 1;
        }
        folds++;
        if(folds == 1)
        {
            int partA = printMap(map, size.x, size.y, 0);
            printf("Part A Solution: %d\n", partA);
        }
    }

    // Part 2 Requires intelligent vision systems.
    printf("Part Two:\n");
    printMap(map, size.x, size.y, 1);
    free(buffer);
    return 0;
}

int printMap(const char map[MAXMAPSIZE][MAXMAPSIZE], const int x, const int y, const int print)
{
    int returnVal = 0;
    for (int yy = 0; yy <= y; yy++)
    {
        for (int xx = 0; xx <= x; xx++)
        {
            char c = map[xx][yy];
            if(print)
            {
                printf("%c", c);
            }
            if(c == FILLED)
            {
                returnVal++;
            }
        }
        if(print)
        {
            printf("\n");
        }
    }
    return returnVal;
}

int getNextNum()
{
    // Skip to the next number.
    while (!isNum(buffer[buffIndex]))
    {
        buffIndex++;
    }
    // Get the number.
    int num = 0;
    while (isNum(buffer[buffIndex]))
    {
        num *= 10;
        num += (buffer[buffIndex] - '0');
        buffIndex++;
    }
    return num;
}

int isNum(const char c)
{
    return (c >= '0' && c <= '9');
}