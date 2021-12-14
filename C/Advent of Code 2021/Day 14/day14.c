#include <stdlib.h>
#include <stdio.h>

int buffIndex;
char *buffer;

int main(int argc, char **argv)
{
    buffIndex = 0;
    FILE *input;
    input = fopen(argv[2], "r");
    // Get file size.
    int fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    // Malloc file.
    buffer = malloc(sizeof *buffer * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);

    int tempIndex = 0;
    int generations = 0;
    while (argv[1][tempIndex] >= '0' && argv[1][tempIndex] <= '9')
    {
        generations *= 10;
        generations += (argv[1][tempIndex] - '0');
        tempIndex++;
    }

    // Step 1: Parse Input!
    // We'll use a "hashmap" of our possible polymer combinations.
    // It's not *really* a hashmap, but it will work.
    // ORDER IS IMPORTANT
    // map[FIRST][SECOND]
    char map[26][26];
    // Now for our actual pairs.
    unsigned long long data1[26][26], data2[26][26];
    unsigned long long(*oldPairs)[26][26], (*newPairs)[26][26];
    oldPairs = &data1;
    newPairs = &data2;
    for (int y = 0; y < 26; y++)
    {
        for (int x = 0; x < 26; x++)
        {
            map[x][y] = '\0';
            (*oldPairs)[x][y] = 0;
            (*newPairs)[x][y] = 0;
        }
    }
    
    // Read the first line to get the initial pairs.
    char first, last; // For the off by 1 errors
    first = buffer[buffIndex];
    while (buffer[buffIndex + 1] != '\n')
    {
        (*oldPairs)[buffer[buffIndex] - 'A'][buffer[buffIndex + 1] - 'A']++;
        buffIndex++;
    }
    last = buffer[buffIndex];
    buffIndex += 3; // Skip newline.

    // Get the mappings.
    while (buffer[buffIndex] != '\0' && buffIndex < fileSize)
    {
        // Input Format:
        // XX -> Y
        map[buffer[buffIndex] - 'A'][buffer[buffIndex + 1] - 'A'] = buffer[buffIndex + 6];
        buffIndex += 8;
    }

    // Now simulate the generations.
    // 10 for part A.
    // 40 for part B.
    for (int gen = 0; gen < generations; gen++)
    {
        // Update pairs!
        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 26; x++)
            {
                char destination = map[x][y];
                if (destination != '\0')
                {
                    // Valid pair!
                    (*newPairs)[x][destination - 'A'] += (*oldPairs)[x][y];
                    (*newPairs)[destination - 'A'][y] += (*oldPairs)[x][y];
                    (*oldPairs)[x][y] = 0;
                }
            }
        }
        // Swap newPairs and oldPairs.
        void *tempPointer;
        tempPointer = newPairs;
        newPairs = oldPairs;
        oldPairs = tempPointer;
    }

    // Print out the solution:

    unsigned long long count[26];
    for (int i = 0; i < 26; i++)
    {
        count[i] = 0;
    }
    // Add all occurrences together.
    for (int y = 0; y < 26; y++)
    {
        for (int x = 0; x < 26; x++)
        {
            count[x] += (*oldPairs)[x][y];
            count[y] += (*oldPairs)[x][y];
        }
    }

    // Convert total numbers occurred to actual numbers in the string.
    for (int i = 0; i < 26; i++)
    {
        if(count[i] != 0)
        {
            count[i] = (count[i] / 2) + (count[i] % 2);
        }
    }

    if(first == last)
    {
        // The formula I made is off by 1 when the first character and last character are equal.
        count[first - 'A']++;
    }
    // Calculate the least and greatest values.
    unsigned long long least, greatest;
    least = greatest = 0;
    for (int i = 0; i < 26; i++)
    {
        if ((count[i] < least || least == 0) && count[i] != 0)
        {
            least = count[i];
        }
        if ((count[i] > greatest || greatest == 0) && count[i] != 0)
        {
            greatest = count[i];
        }
    }
    printf("Solution: %llu\n", greatest - least);
    // Part A: 2937
    // Part B: 3390034818249
    free(buffer);
    return 0;
}