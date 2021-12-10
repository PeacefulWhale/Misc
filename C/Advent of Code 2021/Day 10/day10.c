#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 2
#define STACKSIZE 512 // Max size stack can be.
#define SCORESIZE 512 // Max size of file in lines.

int buffIndex;
char buffer[BUFFSIZE];

int match(char a, char b);
int scoreA(char c); // Part A score.
int scoreB(char c); // Part B score.
int isLeft(char c);
void swap(int* a, int* b); // For part B scoring.
int compare(const long *a, const long *b);

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
        char stack[STACKSIZE]; // If the lines are bigger than this I'm going to be very sad...
        int length;
    } data;
    
    data.length = 0; 
    data.stack[0] = '\0';

    long partA = 0; // Part A.

    struct Scores
    {
        long scores[SCORESIZE];
        int length;
    } scores;
    scores.length = 0;

    while (buffer[buffIndex] != '\0')
    {
        int firstIllegal = 0;
        while (buffer[buffIndex] != '\n' && buffer[buffIndex] != '\0')
        {
            if (isLeft(buffer[buffIndex])) // If it goes on the left, add it to the stack!
            {
                data.stack[++data.length] = buffer[buffIndex];
            }
            else if (buffer[buffIndex] != '\n' || buffer[buffIndex] != '\0') // It is a right side character. Check to see if it is valid!
            {
                if(match(data.stack[data.length], buffer[buffIndex]))
                {
                    data.stack[data.length--] = '\0';
                }
                else
                {
                    // Not a match!
                    // Part A!
                    if(firstIllegal == 0)
                    {
                        firstIllegal = 1;
                        partA += scoreA(buffer[buffIndex]);
                    }
                }
            }
            buffIndex++;
        }
        // Clear stack!
        if(firstIllegal == 0) // And do part B if this is a "not corrupted" line.
        {
            long tempScore = 0;
            for(int i = data.length; i > 0; i--)
            {
                tempScore *= 5;
                tempScore += scoreB(data.stack[i]);
            }
            scores.scores[scores.length++] = tempScore;
        }
        data.length = 0;
        buffIndex++;
    }

    printf("Part A Solution: %ld\n", partA);
    // 392367

    // Get the middle value for part B.
    // Find the median value!
    // I *could* use a modified quicksort.
    // But I'm feeling rather lazy right now, and stdlib has a quicksort!
    qsort(scores.scores, scores.length, sizeof(long), (void *) compare);
    long partB = scores.scores[(int) (scores.length / 2)];

    printf("Part B Solution: %ld\n", partB);
    // 2192104158
    return 0;
}

int compare(const long *a, const long *b) // For stdlib.h qsort.
{
    if(*a > *b)
    {
        return 1;
    }
    else if(*a == *b)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int scoreA(char c)
{
    // Return the proper value.
    switch (c)
    {
    case '<':
    case '>':
        return 25137;
    case '{':
    case '}':
        return 1197;
    case '[':
    case ']':
        return 57;
    case '(':
    case ')':
        return 3;
    }
    return -1; // This shouldn't ever happen, but if it does we know we've done something wrong!
}

int scoreB(char c)
{
        // Return the proper value.
    switch (c)
    {
    case '<':
    case '>':
        return 4;
    case '{':
    case '}':
        return 3;
    case '[':
    case ']':
        return 2;
    case '(':
    case ')':
        return 1;
    }
    return -1;
}

int isLeft(char c)
{
    // Wether or not it is the left side item.
    switch (c)
    {
    case '<':
    case '{':
    case '[':
    case '(':
        return 1;
    default:
        return 0;
    }
}

int match (char a, char b)
{
    if (b - a <= 3 && b - a >= -3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}