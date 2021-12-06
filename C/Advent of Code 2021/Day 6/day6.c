// This day accepts 2 arguments, the number of days to simulate, and the input file.

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h> // Thank you kind creators of this program.
// You'll likely have to add "-lgmp", and "-lgmpxx" if you compile this.

#define BUFFSIZE 8192 * 2 // Max file size.

int buffIndex;
char buffer[BUFFSIZE];

int isNum(char c);
int getNextNum();

int main(int argc, char **argv)
{
    buffIndex = 0;
    if (argv[1] == NULL || argv[2] == NULL)
    {
        printf("Invalid Input!\n");
        printf("Please call with \"day6 [generations to simulate] [input file]\n");
        return 1;
    }
    // Get number of generations to simulate.
    int tempIndex = 0;
    int generations = 0;
    while (isNum(argv[1][tempIndex]))
    {
        generations *= 10;
        generations += (argv[1][tempIndex] - '0');
        tempIndex++;
    }

    FILE *input;
    input = fopen(argv[2], "r");
    if (input == NULL)
    {
        printf("Input filepath is invalid!\n");
        return 1;
    }
    fread(buffer, BUFFSIZE, 1, input);
    fclose(input); // I've been putting this at the end for some reason.
    mpz_t fish[9];
    for (int i = 0; i <= 8; i++)
    {
        mpz_init(fish[i]);
    }
    int num = getNextNum();
    // Part A solution can be done while we parse input.
    // Initialize our bigNum
    mpz_t bigNum;
    mpz_init(bigNum);

    while (num != -1)
    {
        mpz_add_ui(fish[num], fish[num], 1);
        mpz_add_ui(bigNum, bigNum, 1);
        num = getNextNum();
    }
    // We now have our initial fish ready for simulation.
    int offset = 0;
    int zero = offset % 9;
    int six = (offset + 6) % 9;
    int eight = (offset + 8) % 9;
    for (int gen = 1; gen <= generations; gen++)
    {
        mpz_t temp;
        mpz_init(temp);
        
        // 1: Get a temp integer of the fish at 0
        mpz_set(temp, fish[offset % 9]);
        mpz_add(bigNum, bigNum, temp);
        // 2: "Age" all the fish.
        offset++;
        // 3: Add temp back to the fish.
        mpz_add(fish[(offset + 6) % 9], fish[(offset + 6) % 9], temp);
        mpz_set(fish[(offset + 8) % 9], temp);
        
    }
    printf("Solution:\n");
    gmp_printf("%Zd\n", bigNum);
    // 393019 for part A.
    // 1757714216975 for part B.
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
