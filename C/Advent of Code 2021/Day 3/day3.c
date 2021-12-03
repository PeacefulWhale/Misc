#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2 // This should be enough buffer.

// I am lazy and have built these values into my code.
// Bite me.
#define COLUMNS 12
#define ROWS 1000

typedef struct Data
{
    char *bits[ROWS];
    int length;
} Data;

char sigBit(char *a[], int length, int index, char bit);
Data partB(Data data, char bit);

int main(int arc, char **argv)
{
    // argv[1] should contain our input file.
    if (argv[1] == NULL)
    {
        printf("No input file selected!\n");
        return 1;
    }
    // Now let's open our file and read through all the lines.
    // How many measurements are larger than the previous measurement?
    FILE *input;
    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Input filepath is invalid!\n");
        return 1;
    }

    char buffer[BUFFSIZE];
    fread(buffer, BUFFSIZE, 1, input);

    // Keep track of all the columns (there are 12).
    int bits[COLUMNS];
    // We will also keep track of how many rows we've gone through.
    int offset = 0;
    // For part B to keep track of all our integers.
    char *binaryArray[ROWS];
    for (int i = 0; i < ROWS; i++)
    {
        binaryArray[i] = malloc(sizeof *binaryArray[i] * COLUMNS);
    }
    for (int index = 0; index < BUFFSIZE && buffer[index] != '\0'; index++)
    {
        if (buffer[index] == '0')
        {
            // Every row we need to take 1 off our index for accessing the bit array.
            bits[(index - offset) % COLUMNS]++;
            binaryArray[(int)(index - offset) / COLUMNS][(index - offset) % COLUMNS] = '0';
        }
        else if (buffer[index] == '1')
        {
            binaryArray[(int)(index - offset) / COLUMNS][(index - offset) % COLUMNS] = '1';
        }
        else
        {
            offset++;
        }
    }
    fclose(input);

    // Find our greatest bits and make an integer out of them.
    // Gamma is made out of the most common bits.
    // Epsilon is the inverse of gamma.
    // Both are unsigned.
    char gammaStr[COLUMNS + 1];
    char epsilonStr[COLUMNS + 1];
    for (int i = 0; i < COLUMNS; i++)
    {
        // If bitArray[i] > ROWS / 2, then 0 was the most common.
        gammaStr[i] = (bits[i] > ROWS / 2) ? '0' : '1';
        epsilonStr[i] = (bits[i] <= ROWS / 2) ? '0' : '1';
    }
    gammaStr[COLUMNS] = '\0';
    epsilonStr[COLUMNS] = '\0';
    // We can just use strtol().
    long gamma = strtol(gammaStr, NULL, 2);
    long epsilon = strtol(epsilonStr, NULL, 2);
    printf("gammaStr: %s, epsilonStr: %s\n", gammaStr, epsilonStr);
    printf("Part A Solution\nGamma: %lu, Epsilon: %lu, Solution: %lu\n\n", gamma, epsilon, gamma * epsilon);
    // Part A Solution:
    // Gamma: 199, Epsilon: 3896, Solution: 775304

    // Time for part B now.
    // This *would* have been just finding which strings matched epsilon and gamma the best.
    // However we get the next "most / least common" bit from the *remaining* integers.
    // Which makes this quite a bit harder.
    // In addition carbon and oxygen are not guaranteed to be opposites, so we have to calculate both.
    Data data;
    data.length = ROWS;
    for (int row = 0; row < ROWS; row++)
    {
        data.bits[row] = binaryArray[row];
    }

    Data carbon = partB(data, '0');
    Data oxygen = partB(data, '1');
    char *carbonStr = carbon.bits[0];
    char *oxygenStr = oxygen.bits[0];
    long carbonInt = strtol(carbonStr, NULL, 2);
    long oxygenInt = strtol(oxygenStr, NULL, 2);
    printf("oxygenStr: %s, carbonStr: %s\n", oxygenStr, carbonStr);
    printf("Part B Solution\nOxygen: %lu, Carbon: %lu, Solution: %lu\n\n", oxygenInt, carbonInt, oxygenInt * carbonInt);

    return 0;
}

Data partB(Data data, char bit)
{
    // This is an in place algorithm for getting the best bits.
    // The best bits will be saved to data.bits[0].
    int index = 0;
    while (data.length > 1 && index < COLUMNS)
    {
        Data temp;
        temp.length = 0;
        // Initialize mask.
        char mask;
        mask = sigBit(data.bits, data.length, index, bit);
        // Main logic loop.
        for(int row = 0; row < data.length; row++)
        {
            if(data.bits[row][index] == mask)
            {
                temp.bits[temp.length] = data.bits[row];
                temp.length++;
            }
        }
        // Copy temp to data.
        for(int row = 0; row < temp.length; row++)
        {
            data.bits[row] = temp.bits[row];
        }
        data.length = temp.length;
        index++;
    }
    return data;
}

char sigBit(char *a[], int length, int index, char bit)
{
    // Return the correct bit for the bit mask based off the given bit and input array.
    int ones = 0;
    for (int row = 0; row < length; row++)
    {
        if (a[row][index] == '1')
        {
            ones++;
        }
    }
    // (int) rounds down!
    // And we want to favor the bit when we return it!
    // If there are equal numbers, then we return our bit.
    // What we return depends on the bit.
    if(ones > length - ones)
    {
        // This means that 1 is the most common bit.
        return (bit == '0') ? '0' : '1';
    }
    else if(ones == length - ones)
    {
        // Special Case, return our current bit.
        return bit;
    }
    else
    {
        // This means that 0 is the most common bit.
        return (bit == '0') ? '1' : '0';
    }
}