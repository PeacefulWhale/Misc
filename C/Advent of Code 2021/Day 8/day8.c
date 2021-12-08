#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 8192 * 4 // Today is an extra big file.

int buffIndex;
char buffer[BUFFSIZE];

int getNum(int key); // Basically a hash map.

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

    unsigned int solution = 0; // Part B solution.
    // Main Loop.
    while (buffer[buffIndex] != '\0')
    {
        // We need to find the correct mapping!
        int mapping[7]; // a, b, c, d, e, f, g -> what number they go to.
        for(int i = 0; i < 7; i++)
        {
            mapping[i] = -1; // Set to -1 so we know which values are unknown.
        }
        // Our first step is to find the total characters, as 3 of our mappings can be found with this information.
        // We need to keep track of these for later.
        char one[2];
        char four[4];

        int totalCharacters[7];
        for (int i = 0; i < 7; i++)
        {
            totalCharacters[i] = 0;
        }
        int length = 0;
        while (buffer[buffIndex] != '|')
        {
            if (buffer[buffIndex] != ' ')
            {
                length++;
                totalCharacters[buffer[buffIndex] - 'a']++;
            }
            else 
            {
                if (length == 2) // Save one.
                {
                    one[0] = buffer[buffIndex - 2];
                    one[1] = buffer[buffIndex - 1];
                }
                else if (length == 4) // Save four.
                {
                    four[0] = buffer[buffIndex - 4];
                    four[1] = buffer[buffIndex - 3];
                    four[2] = buffer[buffIndex - 2];
                    four[3] = buffer[buffIndex - 1];
                }
                length = 0;
            }
            buffIndex++;
        }
        for (int i = 0; i < 7; i++)
        {
            if (totalCharacters[i] == 4)
            {
                mapping[i] = 4;
            }
            else if (totalCharacters[i] == 6)
            {
                mapping[i] = 1;
            }
            else if (totalCharacters[i] == 9)
            {
                mapping[i] = 5;
            }
        }

        // The second step is to find which maps to 2.
        // We know this because one maps to 2 and 5 only.
        for(int i = 0; i < 2; i++)
        {
            if(mapping[one[i] - 'a'] == -1)
            {
                mapping[one[i] - 'a'] = 2;
                break;
            }
        }

        // The third step is to find which maps to 3.
        // We know this because four maps to 1, 2, 3, and 5.
        for(int i = 0; i < 4; i++)
        {
            if(mapping[four[i] - 'a'] == -1)
            {
                mapping[four[i] - 'a'] = 3;
                break;
            }
        }
        // The only unknown mappings left are 0 and 6.
        // 0 appears 8 times.
        // 6 appears 7 times.
        for(int i = 0; i < 7; i++)
        {
            if(mapping[i] == -1)
            {
                if(totalCharacters[i] == 8)
                {
                    mapping[i] = 0;
                }
                else if(totalCharacters[i] == 7)
                {
                    mapping[i] = 6;
                }
            }
        }
        buffIndex += 2; // Skip "| ".
        
        int output = 0;
        int offset = 1000;
        int numbers = 0;
        while(numbers < 4 && (buffer[buffIndex] != ' ' && buffer[buffIndex] != '\n' && buffer[buffIndex] != '\0'))
        {
            int key = 1;
            int addKey = 0;
            while((buffer[buffIndex] != ' ' && buffer[buffIndex] != '\n') && buffer[buffIndex] != '\0')
            {
                int temp = mapping[buffer[buffIndex] - 'a'];
                if(temp == 0)
                {
                    addKey += 1;
                }
                else if(temp == 1)
                {
                    addKey += 2;
                }
                else
                {
                    key *= temp;
                }
                buffIndex++;
            }
            key += addKey;
            // We have a number, add it to the solution!
            output += (getNum(key)) * offset;
            offset /= 10;
            numbers++;
            buffIndex++;
        }
        solution += output;
    }
    printf("Part B solution: %d\n", solution);
    // Part B solution: 908067.
    return 0;
}

int getNum(int key)
{
    switch(key)
    {
        case 243:
        return 0;

        case 10:
        return 1;

        case 145:
        return 2;

        case 181:
        return 3;

        case 32:
        return 4;

        case 93:
        return 5;

        case 363:
        return 6;

        case 11:
        return 7;

        case 723:
        return 8;

        case 183:
        return 9;
    }
    printf("Error! %d is unknown!\n", key);
    return -1;
}