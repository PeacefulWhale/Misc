#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2 // This should be enough buffer.

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
    fread(buffer, BUFFSIZE, 1, input);

    // Variables for part A.
    struct A
    {
        int depth;
        int horizontal;
    } a;
    a.depth = 0;
    a.horizontal = 0;

    // Variables for part B.
    struct B
    {
        int depth;
        int horizontal;
        int aim;
    } b;
    b.depth = 0;
    b.horizontal = 0;
    b.aim = 0;

    // Because we know the size of all these inputs, we can just read the first character and then the 1 character digit associated with it.
    int index = 0;
    while(index < BUFFSIZE && buffer[index] != '\0')
    {
        int temp = 0;
        switch (buffer[index])
        {
        case 'f':
            index += 8;
            temp = (buffer[index] - '0');
            a.horizontal += temp;
            b.horizontal += temp;
            b.depth += (b.aim * temp);
            break;
        case 'd':
            index += 5;
            temp = (buffer[index] - '0');
            a.depth += temp;
            b.aim += temp;
            break;
        case 'u':
            index += 3;
            temp = (buffer[index] - '0');
            a.depth -= temp;
            b.aim -= temp;
            break;
        }
        index += 2;
    }
    printf("A Solution: %d\nB Solutions: %d\n", a.depth * a.horizontal, b.depth * b.horizontal);
    fclose(input);
}