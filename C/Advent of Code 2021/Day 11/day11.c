#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2

int buffIndex;
char buffer[BUFFSIZE];

unsigned int flashCount(char *map, int x, int y, int width, int height, int first);

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
    fread(buffer, BUFFSIZE, 1, input);
    fclose(input);

    // Map handling copied from day 9.
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
    char map[width * height];
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

    // Part A is simple, we just have to simulate everything and keep track of the number of flashes.
    // I'll do this recursively.
    unsigned int partA = 0;
    unsigned int partB = 0;
    int synced = 0;
    for (int i = 0; !synced || i < 100; i++)
    {
        // Increase octopi power! (and check if they're synced).
        synced = 1;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (synced && map[(y * width) + x] != '0')
                {
                    synced = 0;
                }
                map[(y * width) + x] += 1;
            }
        }
        // Flash the poor octopi!
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned int temp = flashCount(map, x, y, width, height, 1);
                if (i < 100)
                {
                    partA += temp;
                }
            }
        }
        partB++;
    }
    printf("Part A Solution: %u\n", partA);
    // 1721

    printf("Part B Solution: %u\n", partB - 1); // -1 because our generation count starts at 0
    // 298
}

unsigned int flashCount(char *map, int x, int y, int width, int height, int first)
{
    // Yes this is basically a copy of day9, don't blame me it works!
    unsigned int flashes = 0;
    // Don't flash "tired" octopi.
    // But we can increment them if this is this is the first call!
    if (map[(y * width) + x] == '0')
    {
        return 0;
    }
    else
    {
        if (!first)
        {
            map[(y * width) + x] += 1;
        }
        // Only "flash" if it has enough power.
        // We have to do diagonal guys too!
        if (map[(y * width) + x] > '9')
        {
            map[(y * width) + x] = '0';
            flashes += 1;

            // Recursively call flash on the surrounding tiles after they've had their power increased.
            int ul, ur, dl, dr; // Corner booleans.
            ul = ur = dl = dr = 0;
            if (x != 0) // Left wall.
            {
                ul++;
                dl++;
                // Check if the left >= 9.
                flashes += flashCount(map, x - 1, y, width, height, 0);
            }
            if (x != width - 1) // Right wall.
            {
                ur++;
                dr++;
                // Check if the right >= 9.
                flashes += flashCount(map, x + 1, y, width, height, 0);
            }
            if (y != 0) // Top wall.
            {
                ur++;
                ul++;
                // Check if the top >= 9
                flashes += flashCount(map, x, y - 1, width, height, 0);
            }
            if (y != height - 1) // Bot wall.
            {
                dr++;
                dl++;
                // Check if the bot is >= 9.
                flashes += flashCount(map, x, y + 1, width, height, 0);
            }
            if (ur == 2) // Top Right.
            {
                flashes += flashCount(map, x + 1, y - 1, width, height, 0);
            }
            if (ul == 2) // Top Left.
            {
                flashes += flashCount(map, x - 1, y - 1, width, height, 0);
            }
            if (dr == 2) // Bot Right.
            {
                flashes += flashCount(map, x + 1, y + 1, width, height, 0);
            }
            if (dl == 2) // Bot Left.
            {
                flashes += flashCount(map, x - 1, y + 1, width, height, 0);
            }
        }
    }
    return flashes;
}