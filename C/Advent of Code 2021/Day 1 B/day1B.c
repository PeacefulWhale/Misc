#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2 // This should be enough buffer.
int toInt(char *c, int len);

int main(int argc, char **argv)
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
    int numbers[BUFFSIZE];
    int numbersIndex = 0;
    fread(buffer, BUFFSIZE, 1, input);

    int index = 0;
    char numBuff[32];
    int len = 0;
    // go through the entire buffer
    while (buffer[index] != '\0' && index < BUFFSIZE)
    {
        char c = buffer[index];
        // if it is a number, add it to our numBuffer
        if(c >= '0' && c <= '9')
        {
            numBuff[len] = c;
            len++;
        }
        // if it is not a number, check the current number
        else
        {
            numbers[numbersIndex++] = toInt(numBuff, len);
            len = 0;
        }
        index++;
    }
    fclose(input);

    int returnValue = 0;
    int oldNum = 0;
    int newNum = 0;
    // go through the numbers array and check the sums!
    for(int i = 0; i < numbersIndex - 3; i++)
    {
        // a + b + c > b + c + d
        // is the same thing as
        // a > d
        oldNum = numbers[i];
        newNum = numbers[i + 3];
        if(newNum > oldNum)
        {
            returnValue++;
        }
        oldNum = newNum;
    }
    printf("Measurements larger than previous measurement: %d\n", returnValue);
    return 0;
}

int toInt(char *c, int len)
{
    int returnValue = 0;
    int a = 1;
    for (int i = len - 1; i >= 0; i--)
    {
        returnValue += (c[i] - '0') * a;
        a *= 10;
    }
    return returnValue;
}