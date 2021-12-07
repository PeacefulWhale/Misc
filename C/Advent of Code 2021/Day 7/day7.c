#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 // Max file size.
#define MAXNUMBERS 2000 // Max number of numbers.

int buffIndex;
char buffer[BUFFSIZE];

int isNum(char c);
int getNextNum();

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

    struct Data
    {
        int numbers[MAXNUMBERS];
        int length;
        int min;
        int max;
    } data;
    for(int i = 0; i < MAXNUMBERS; i++)
    {
        data.numbers[i] = 0;
    }
    data.length = 0;
    data.min = -1;
    data.max = -1;

    int num = getNextNum();
    // Parse our input.
    while (num != -1)
    {
        data.numbers[data.length++] = num;
        if(num > data.max || data.max == -1)
        {
            data.max = num;
        }
        else if(num < data.min || data.min == -1)
        {
            data.min = num;
        }
        num = getNextNum();
    }
    // Now that we have our input, and our min / max values, we can create our map.
    int distance = data.max - num;
    distance = (distance >= 0) ? distance : -distance; // I trust the compiler to optimize this, screw bitmath.
    unsigned long solution[distance];
    // Set solution to 0.
    for(int i = 0; i < distance; i++)
    {
        solution[i] = 0;
    }
    
    // I'm feeling lazy right now so it's brute force time!
    // Part A:
    for(int i = 0; i < data.length; i++)
    {
        // Calculate distance from our start point and the current number.
        int tempNum = data.numbers[i];
        int tempDistance = data.min - tempNum;
        tempDistance = (tempDistance >= 0) ? tempDistance : -tempDistance;
        // Getting closer to tempNum.
        for(int j = 0; j < tempNum; j++)
        {
            solution[j] += tempDistance--;
        }
        // Getting further from tempNum.
        for(int j = tempNum; j < distance; j++)
        {
            solution[j] += tempDistance++;
        }
    }
    // Find smallest index and print it.
    unsigned long smallest = -1;
    int smallestIndex = -1;
    for(int i = 0; i < distance; i++)
    {
        if(solution[i] < smallest || smallest == -1)
        {
            smallestIndex = i;
            smallest = solution[i];
        }
    }
    printf("Part A solution:\n");
    printf("Index: %d, Fuel Spent: %lu\n", smallestIndex, smallest);
    // Index: 323, Fuel Spent: 336040

    // Another brute force...
    // Part B:
    // Set solution to 0.
    for(int i = 0; i < distance; i++)
    {
        solution[i] = 0;
    }
    for(int i = 0; i < data.length; i++)
    {
        // Calculate distance from our start point and the current number.
        // This time we start from our current crabby boi and work outward (to make keeping track of fuel super easy);
        int tempNum = data.numbers[i];
        int tempDistance = data.min - tempNum;
        tempDistance = (tempDistance >= 0) ? tempDistance : -tempDistance;
        long fuelCost = 0; // Long just in case.
        int helper = 1;
        // From crab to end.
        for(int j = tempNum + 1; j < distance; j++)
        {
            fuelCost += helper++;
            solution[j] += fuelCost;
        }
        fuelCost = 0;
        helper = 1;
        // From crab to start.
        for(int j = tempNum - 1; j >= 0; j--)
        {
            fuelCost += helper++;
            solution[j] += fuelCost;
        }
    }

    // Find smallest index and print it.
    smallest = -1;
    smallestIndex = -1;
    for(int i = 0; i < distance; i++)
    {
        if(solution[i] < smallest || smallest == -1)
        {
            smallestIndex = i;
            smallest = solution[i];
        }
    }
    printf("\nPart B solution:\n");
    printf("Index: %d, Fuel Spent: %lu\n", smallestIndex, smallest);
    // Index: 463, Fuel Spent: 94813675
    return 0;
}

int isNum(char c)
{
    return (c >= '0' && c <= '9');
}

int getNextNum()
{
    while (!isNum(buffer[buffIndex]))
    {
        // EOF
        if (buffer[buffIndex] == '\0')
        {
            return -1;
        }
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
