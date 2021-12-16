#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAXSUBS 200 // No way we have more subpackets than this at once right?

int buffIndex, fileSize;
char *restrict buffer;

// Part A solution.
unsigned int partA;

// Yes, I'm too lazy to do actually work with these like bits.
// Sue me.
char binary[16][5] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

unsigned long long int parseData();
int hexToInt(const char c);
unsigned long long int binToInt(const char *restrict c, const int len);
int isDone();
unsigned long long int mathStuff(const unsigned long long int data[MAXSUBS], const int dataCount, const int id);

int main(int argc, char **argv)
{
    buffIndex = 0;
    FILE *input;
    input = fopen(argv[1], "r");
    // Get file size.
    fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    // Malloc file.
    buffer = malloc((sizeof *buffer) * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);

    // Convert buffer to binary string because I'm lazy!
    char *tempBuffer;
    tempBuffer = malloc(sizeof *tempBuffer * ((fileSize * 4) + 1));
    tempBuffer[0] = '\0';
    for (int i = 0; i < fileSize; i++)
    {
        strcat(tempBuffer, binary[hexToInt(buffer[i])]);
    }
    // Copy tempBuffer to buffer, and update filesize!
    char *temp;
    temp = buffer;
    buffer = tempBuffer;
    free(temp);
    fileSize *= 4;
    buffIndex = 0;
    tempBuffer[fileSize] = '\0';
    partA = 0;
    unsigned long long int partB = parseData();

    printf("Part A Solution: %d\n", partA);
    // 957
    printf("Part B Solution: %llu\n", partB);

    free(buffer);
    return 0;
}

unsigned long long int parseData()
{
    // Starting position for use in relative offsets!
    // Recursive method for reading packets and stuff.
    // Returns the sum of the sub packets.
    int version = binToInt((buffer + buffIndex), 3);
    partA += version;
    buffIndex += 3;
    // 3-5: Type ID.
    int id = binToInt((buffer + buffIndex), 3);
    buffIndex += 3;
    // Here things get a *little* bit more complicated...

    if (id == 4)
    {
        // Get size of number.
        int length = 1;
        for (int i = 0; buffer[buffIndex + i] != '0'; i += 5)
        {
            length++;
        }
        char *tempStr;
        tempStr = malloc(sizeof tempStr * ((4 * length) + 1));
        for (int i = 0; i < length; i++)
        {
            buffIndex++;
            for (int j = 0; j < 4; j++)
            {
                tempStr[(i * 4) + j] = buffer[buffIndex++];
            }
        }
        length *= 4;
        tempStr[length] = '\0';

        unsigned long long int returnVal = binToInt(tempStr, length);

        free(tempStr);
        return returnVal;
    }
    else
    {
        // We have to get the length number, and then read the packets.
        int bitLen;
        char lengthID = buffer[buffIndex];
        if (lengthID == '0')
        {
            // Next 15 bits are the number.
            bitLen = 15;
        }
        else
        {
            // Next 11 bits are the number.
            bitLen = 11;
        }
        buffIndex++;
        int length = binToInt((buffer + buffIndex), bitLen);
        buffIndex += bitLen;

        // Data for math stuff!
        unsigned long long int data[MAXSUBS]; 
        int dataCount = 0;
        if (length != 0)
        {
            if (lengthID == '0') // Total length in bits.
            {
                int startPos = buffIndex;
                while (buffIndex - startPos < length && !isDone())
                {
                    data[dataCount++] = parseData();
                }
            }
            else // Number of immediate sub-packets.
            {
                for (int i = 0; i < length; i++)
                {
                    data[dataCount++] = parseData();
                }
            }
        }
        // Do the math stuff!
        return mathStuff(data, dataCount, id);
    }
}

unsigned long long int mathStuff(const unsigned long long int data[MAXSUBS], const int dataCount, const int id)
{
    unsigned long long int returnVal = 0;
    if (id == 0) // Sum.
    {
        // Sum of all items in data.
        returnVal = 0;
        for(int i = 0; i < dataCount; i++)
        {
            returnVal += data[i];
        }
    }
    else if (id == 1) // Product.
    {
        // Product of all items in data.
        returnVal = 1;
        for(int i = 0; i < dataCount; i++)
        {
            returnVal *= data[i];
        }
    }
    else if (id == 2) // Minimum.
    {
        // Return the minimum value.
        int minIndex = 0;
        for(int i = 0; i < dataCount; i++)
        {
            if(data[i] < data[minIndex])
            {
                minIndex = i;
            }
        }
        returnVal = data[minIndex];
    }
    else if (id == 3) // Maximum.
    {
        // Return the maximum value.
        int maxIndex = 0;
        for(int i = 0; i < dataCount; i++)
        {
            if(data[i] > data[maxIndex])
            {
                maxIndex = i;
            }
        }
        returnVal = data[maxIndex];
    }
    else if (id == 5) // Greater-Than.
    {
        // Is the first element greater than the second?
        returnVal = data[0] > data[1];
    }
    else if (id == 6) // Less-Than.
    {
        // Is the first element less than the second?
        returnVal = data[0] < data[1];
    }
    else if (id == 7) // Equal-To.
    {
        // Is the first element equal to the second?
        returnVal = data[0] == data[1];
    }
    return returnVal;
}

int isDone()
{
    // Return true if there are only padded 0s left, or we're over fileSize.
    if (buffIndex > fileSize)
    {
        return 1;
    }
    else
    {
        for (int i = buffIndex; i < fileSize; i++)
        {
            if (buffer[i] != '0')
            {
                return 0;
            }
        }
        return 1;
    }
}

unsigned long long int binToInt(const char *restrict c, const int len)
{
    // Convert string of 0s and 1s to integer.
    unsigned long long int returnVal = 0;
    for (int i = 0; i < len; i++)
    {
        if (c[i] == '1')
        {
            unsigned long long int one = 1;
            returnVal += (one << ((len - 1) - i));
        }
    }
    return returnVal;
}

int hexToInt(const char c)
{
    // Convert hexadecimal character to integer.
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else
    {
        return (c - 'A') + 10;
    }
}