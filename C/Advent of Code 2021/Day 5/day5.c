#include <stdlib.h>
#include <stdio.h>

#define BUFFSIZE 8192 * 2 // Max file size.
#define MAPSIZE 1000      // Max size of map.

int isNum(char c);
int getNextNum();
void printMap(int x, int y);

// These are now global variables.
int buffIndex;
char buffer[BUFFSIZE];
int vents[MAPSIZE][MAPSIZE];

int main(int argc, char **argv)
{
    buffIndex = 0;
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
    fread(buffer, BUFFSIZE, 1, input);

    // Initialize our map.
    for (int x = 0; x < MAPSIZE; x++)
    {
        for (int y = 0; y < MAPSIZE; y++)
        {
            vents[x][y] = 0;
        }
    }

    // Add the vents to our map when parsing through the input.
    struct coord
    {
        int x, y;
    } max, coord1, coord2; // Our lines will be from these two coordinates
    max.x = 0;
    max.y = 0;
    int solution = 0;
    // Coordinates can be 0-999, and come in 2 numbers (x and y) seperated by a comma.
    // Lines are 2 coordinates seperated by " -> ".
    while (buffer[buffIndex] != '\0')
    {
        // Get coordinates.
        coord1.x = getNextNum();
        coord1.y = getNextNum();
        coord2.x = getNextNum();
        coord2.y = getNextNum();
        // Enable if you want to see the coordinates read and added.
        // printf("%d,%d -> %d,%d\n", coord1.x, coord1.y, coord2.x, coord2.y);
        // Update max.x and may.y (for the print function).
        max.x = (coord2.x > max.x) ? coord2.x : max.x;
        max.y = (coord2.y > max.y) ? coord2.y : max.y;
        // Add lines.
        int x = coord1.x;
        int y = coord1.y;
        vents[x][y]++;
        do
        {
            // Add to part A and B
            if(vents[x][y] == 2)
            {
                solution++;
            }
            if (coord1.x < coord2.x)
            {
                x++;
            }
            else if(coord1.x > coord2.x)
            {
                x--;
            }
            if (coord1.y < coord2.y)
            {
                y++;
            }
            else if (coord1.y > coord2.y)
            {
                y--;
            }
            vents[x][y]++;
        } while (x != coord2.x || y != coord2.y);
        
        if(vents[x][y] == 2)
        {
            solution++;
        }
    }
    fclose(input);

    // Enable this if you want to see the map.
    // printMap(max.x, max.y);

    // Part A:
    // Part A no longer works because we're adding diagonals as well. RIP Part A.
    // I'm too lazy right now to make it work or to fix it ;_;
    // printf("Part A Solution: %d\n", solution);
    // 5774

    // Part B:
    printf("Part B Solution: %d\n", solution);
    // 18423
    
    return 0;
}

void printMap(int x, int y)
{
    printf("Map:\n");
    for (int i = 0; i <= y; i++)
    {
        for (int j = 0; j <= x; j++)
        {
            if (vents[j][i] == 0)
            {
                printf(".");
            }
            else
            {
                printf("%d", vents[j][i]);
            }
        }
        printf("\n");
    }
}

int isNum(char c)
{
    return (c >= '0' && c <= '9');
}

int getNextNum()
{
    // I kinda want to put the buffIndex++ inside the buffer call, because it's identical to what is done here.
    // But that wouldn't be as readable, and who really cares about line length in the end?
    // Readability > Compactness
    // Skip all "not numbers"
    while (!isNum(buffer[buffIndex]))
    {
        buffIndex++;
    }
    int returnVal = 0;
    while (isNum(buffer[buffIndex]))
    {
        returnVal *= 10;
        returnVal += (buffer[buffIndex] - '0');
        buffIndex++;
    }
    return returnVal;
}
