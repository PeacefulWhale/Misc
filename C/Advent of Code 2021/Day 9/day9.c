#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2

int buffIndex;
char buffer[BUFFSIZE];

int basinSize(char *map, int x, int y, int width, int height);

int main(int argc, char **argv)
{
    buffIndex = 0;
    if (argv[1] == NULL)
    {
        printf("Invalid Input!\n");
        printf("Please call with \"day7 [input file]\n");
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
    fclose(input);

    // Get width of file to allocate our map.
    // Sadly it is not guaranteed to be square ;_;
    int width = 0;
    int height = 0;
    while (buffer[width] != '\n')
    {
        width++;
    }
    while (buffer[width * (height + 1)] != '\0')
    {
        height++;
    }
    char map[width * height]; // We *could* parse the input at the same time we solve part A.
    // But I think part B might require some more math.
    // And this also makes it easier to read.
    // Now that we know our width we can go through our buffer and solve part A.
    int offset;
    while (buffer[buffIndex] != '\0')
    {
        if (buffer[buffIndex] == '\n')
        {
            buffIndex++;
            continue;
        }
        else
        {
            map[offset] = buffer[buffIndex];
            buffIndex++;
            offset++;
        }
    }
    int partA = 0;
    int partB = 0;
    int basins[3];
    basins[0] = basins[1] = basins[2] = 0;
    // For part B we have to find the size of the largest basin and return it.
    // We can integrate this with part A.

    // Main Loop:
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int currentChar = map[(y * width) + x];
            int isSmallest = 1;
            if (x != 0) // Left wall.
            {
                if(map[(y * width) + (x - 1)] <= currentChar) // Check left.
                {
                    isSmallest = 0;
                }
            }
            if (x != width - 1) // Right wall.
            {
                if(map[(y * width) + (x + 1)] <= currentChar) // Check right.
                {
                    isSmallest = 0;
                }
            }
            if (y != 0) // Top wall.
            {
                if(map[((y - 1) * width) + x] <= currentChar) // Check top.
                {
                    isSmallest = 0;
                }
            }
            if (y != height - 1) // Bot wall.
            {
                if(map[((y + 1) * width) + x] <= currentChar) // Check bottom.
                {
                    isSmallest = 0;
                }
            }
            if(isSmallest)
            {
                partA += (currentChar - '0') + 1;
                // We can actually overwrite our map with 9s and not change the end answer.
                int size = basinSize(map, x, y, width, height);
                // Add it to the basin list.
                // Yes this is an insertion sort, but it's only 3 indices so it's the quickest.
                for(int i = 0; i < 3; i++)
                {
                    if(basins[i] < size)
                    {
                        int temp = basins[i];
                        basins[i] = size;
                        size = temp;
                    }
                }
            }
        }
    }

    printf("Part A solution: %d\n", partA);
    // Part A solution: 423
    printf("Part B solutions: %d\n", basins[0] * basins[1] * basins[2]);
    // Part B solution: 1198704
    return 0;
}

int basinSize(char *map, int x, int y, int width, int height)
{
    // Find the basin size.
    // A basin grows recursively unless the value is 9.
    int currentChar = map[(y * width) + x];
    int size = 0;
    if(currentChar == '9')
    {
        return 0;
    }
    else
    {
        map[(y * width) + x] = '9';
        size++;
        if (x != 0) // Left wall.
        {
            // Check if the left is 9.
            size += basinSize(map, x - 1, y, width, height);
        }
        if (x != width - 1) // Right wall.
        {
            // Check if the right is 9.
            size += basinSize(map, x + 1, y, width, height);
        }
        if (y != 0) // Top wall.
        {
            // Check if the top is 9
            size += basinSize(map, x, y - 1, width, height);
        }
        if (y != height - 1) // Bot wall.
        {
            // Check if the bot is 9.
            size += basinSize(map, x, y + 1, width, height);
        }
    }
    return size;
}