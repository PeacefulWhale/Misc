#include <stdlib.h>
#include <stdio.h>

#define CONFIDENCE 3 // Number of beacons that must match for it to be considered the correct alignment.
                     // 3 solves my input, and the test case. The max this can be is 12.
                     // This doesn't effect speed noticeably.

typedef struct Coord
{
    int x, y, z;
} Coord;

typedef struct Scanner
{
    Coord pos;
    // pos X, Y, Z: Location in 3D space.
    //              Location is relative to the first scanner, which is set to 0,0,0
    Coord *beacons; // Position of beacons.
    int length;     // Number of beacons.
} Scanner;

Scanner *getScanner(char *restrict buffer, int *buffIndex, const int fileSize); // Returns new scanner and it's attached beacons.
int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize);      // Get's the next number in the buffer.
int isNum(const char c);                                                        // Simple truth function.
int align(Scanner *a, Scanner *b);                                              // Aligns b to a. Returns the alignment (for debugging stuff).
Coord alignCoord(Coord a, int dir);                                             // A function that returns a coordinate aligned to a specified direction.
Coord subCoord(Coord a, Coord b);                                               // Subtracts coord b from a.
Coord addCoord(Coord a, Coord b);                                               // Adds coord a and b together.
int manhattan(Coord a, Coord b);                                                // Returns the manhattan distance between coord a and b.
int matches(Coord a, Coord b);

int main(int argc, char **argv)
{
    int buffIndex, fileSize;
    char *restrict buffer;
    buffIndex = 0;
    FILE *input;
    input = fopen(argv[1], "r");
    fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    buffer = malloc((sizeof *buffer) * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);
    // Parse input into datastructure.
    Scanner **scanners;
    // First find how many scanners we need to go through.
    int totalScanners = 0;
    while (buffIndex < fileSize)
    {
        // Check to see if this matches the '--- Scanner X ---' line all scanners start with.
        if (buffer[buffIndex] == '-' && buffer[buffIndex + 1] == '-')
        {
            totalScanners++;
        }
        // Skip to new line.
        while (buffIndex < fileSize && buffer[buffIndex] != '\n')
        {
            buffIndex++;
        }
        buffIndex++;
    }
    buffIndex = 0;
    printf("%d Total Scanners\n", totalScanners);
    scanners = malloc(sizeof *scanners * totalScanners);
    // Get first scanner.
    scanners[0] = getScanner(buffer, &buffIndex, fileSize);
    // Get all our scanners.
    for (int tempIndex = 1; tempIndex < totalScanners; tempIndex++)
    {
        scanners[tempIndex] = getScanner(buffer, &buffIndex, fileSize);
    }
    // Now match up scanners.
    int totalSolved = 1; // The first scanner (0,0,0) can be considered solved!
    // An array that tells us if a certain index is solved or not.
    int solved[totalScanners];
    for (int i = 0; i < totalScanners; i++)
    {
        solved[i] = 0;
    }
    solved[0] = 1;
    printf("Total Scanners: %d\n", totalScanners);
    while (totalSolved < totalScanners)
    {
        int start = scanners[0]->length;
        for (int i = 1; i < totalScanners; i++)
        {
            // Only check unsolved scanners.
            if (solved[i] == 0)
            {
                int tempAdded;
                int alignment = align(scanners[0], scanners[i]);
                if (alignment != 24)
                {
                    solved[i] = 1;
                    totalSolved++;
                    printf("Solved scanner %2d with alignment %2d and offset %d,%d,%d\n", i, alignment, scanners[i]->pos.x, scanners[i]->pos.y, scanners[i]->pos.z);
                    printf("%3d Total Beacons. %1.0f%% done\n", scanners[0]->length, 100 * ((float)totalSolved / (float)totalScanners));
                }
            }
        }
    }

    printf("Part A solution: %d\n", scanners[0]->length);

    // Part B is finding the largest manhattan distance.
    int partB = 0;
    for (int x = 0; x < totalScanners; x++)
    {
        for (int y = 0; y < totalScanners; y++)
        {
            int dist = manhattan(scanners[x]->pos, scanners[y]->pos);
            if(dist > partB)
            {
                partB = dist;
            }
        }
    }
    printf("Part B solution: %d\n", partB);
    // Free all our memory!
    free(buffer);
    for (int i = 0; i < totalScanners; i++)
    {
        free(scanners[i]->beacons);
        free(scanners[i]);
    }
    return 0;
}

int manhattan(Coord a, Coord b)
{
    return (
        abs(a.x - b.x) +
        abs(a.y - b.y) +
        abs(a.z - b.z)
    );
}

Coord alignCoord(Coord a, int dir)
{
    Coord aligned;
    // I think my method for programmatically generating this is broken, so I'm just doing this for now...
    // Bite me D:<
    switch (dir)
    {
    default:
    case 0:
        aligned.x = a.x;
        aligned.y = a.y;
        aligned.z = a.z;
        break;
    case 1:
        aligned.x = a.x;
        aligned.y = -a.z;
        aligned.z = a.y;
        break;
    case 2:
        aligned.x = a.x;
        aligned.y = -a.y;
        aligned.z = -a.z;
        break;
    case 3:
        aligned.x = a.x;
        aligned.y = a.z;
        aligned.z = -a.y;
        break;
    case 4:
        aligned.x = a.y;
        aligned.y = -a.x;
        aligned.z = a.z;
        break;
    case 5:
        aligned.x = a.y;
        aligned.y = -a.z;
        aligned.z = -a.x;
        break;
    case 6:
        aligned.x = a.y;
        aligned.y = a.x;
        aligned.z = -a.z;
        break;
    case 7:
        aligned.x = a.y;
        aligned.y = a.z;
        aligned.z = a.x;
        break;
    case 8:
        aligned.x = a.z;
        aligned.y = a.y;
        aligned.z = -a.x;
        break;
    case 9:
        aligned.x = a.z;
        aligned.y = a.x;
        aligned.z = a.y;
        break;
    case 10:
        aligned.x = a.z;
        aligned.y = -a.y;
        aligned.z = a.x;
        break;
    case 11:
        aligned.x = a.z;
        aligned.y = -a.x;
        aligned.z = -a.y;
        break;
    case 12:
        aligned.x = -a.x;
        aligned.y = -a.y;
        aligned.z = a.z;
        break;
    case 13:
        aligned.x = -a.x;
        aligned.y = -a.z;
        aligned.z = -a.y;
        break;
    case 14:
        aligned.x = -a.x;
        aligned.y = a.y;
        aligned.z = -a.z;
        break;
    case 15:
        aligned.x = -a.x;
        aligned.y = a.z;
        aligned.z = a.y;
        break;
    case 16:
        aligned.x = -a.y;
        aligned.y = a.z;
        aligned.z = -a.x;
        break;
    case 17:
        aligned.x = -a.y;
        aligned.y = a.x;
        aligned.z = a.z;
        break;
    case 18:
        aligned.x = -a.y;
        aligned.y = -a.z;
        aligned.z = a.x;
        break;
    case 19:
        aligned.x = -a.y;
        aligned.y = -a.x;
        aligned.z = -a.z;
        break;
    case 20:
        aligned.x = -a.z;
        aligned.y = a.y;
        aligned.z = a.x;
        break;
    case 21:
        aligned.x = -a.z;
        aligned.y = a.x;
        aligned.z = -a.y;
        break;
    case 22:
        aligned.x = -a.z;
        aligned.y = -a.y;
        aligned.z = -a.x;
        break;
    case 23:
        aligned.x = -a.z;
        aligned.y = -a.x;
        aligned.z = a.y;
        break;
    }
    return aligned;
}

int align(Scanner *a, Scanner *b)
{
    // Each scanner has at least 12 nodes matching with other scanners.
    // If we cannot get 12 scanners matching then we have the wrong alignment.
    int aligned = 0;
    int alignment = 0;
    Coord offset;
    // All possible alignments.
    // Debug.
    while (alignment < 24)
    {
        // All possible initial starting points of A and B (for offset calculation).
        for (int indexA = 0; indexA <= a->length && aligned < CONFIDENCE; indexA++)
        {
            for (int indexB = 0; indexB <= b->length && aligned < CONFIDENCE; indexB++)
            {
                aligned = 0;
                Coord alignedB = alignCoord(b->beacons[indexB], alignment);
                offset = subCoord(a->beacons[indexA], alignedB);
                Coord tempCoord = addCoord(alignedB, offset);

                // See how many of the other coordinates match.
                // We need at least 12 to match for this alignment and offset to the valid one.
                for (int matchA = 0; matchA < a->length; matchA++)
                {
                    for (int matchB = 0; matchB < b->length; matchB++)
                    {
                        Coord tempB = alignCoord(b->beacons[matchB], alignment);
                        tempB = addCoord(tempB, offset);
                        if (matches(a->beacons[matchA], tempB))
                        {
                            aligned++;
                        }
                    }
                }
            }
        }
        if (aligned < CONFIDENCE)
        {
            alignment++;
        }
        else
        {
            break;
        }
    }
    if (alignment != 24)
    {
        // Reallocate enough space for both.
        a->beacons = realloc(a->beacons, sizeof *a->beacons * (a->length + b->length));
        // Align all the coordinates to the new alignment.
        // And see if the beacon currently exists in the target.
        for (int i = 0; i < b->length; i++)
        {
            Coord newCoord = alignCoord(b->beacons[i], alignment);
            newCoord = addCoord(newCoord, offset);
            int present = 0;
            for (int j = 0; j < a->length; j++)
            {
                if (matches(a->beacons[j], newCoord))
                {
                    present = 1;
                    break;
                }
            }
            if (present == 0)
            {
                a->beacons[a->length] = newCoord;
                a->length++;
            }
        }
        // Set the scanner's offset.
        b->pos = offset;
    }
    return alignment;
}

int matches(Coord a, Coord b)
{
    return (
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z);
}

Coord addCoord(Coord a, Coord b)
{
    Coord coord;
    coord.x = a.x + b.x;
    coord.y = a.y + b.y;
    coord.z = a.z + b.z;
    return coord;
}

Coord subCoord(Coord a, Coord b)
{
    Coord coord;
    coord.x = a.x - b.x;
    coord.y = a.y - b.y;
    coord.z = a.z - b.z;
    return coord;
}

Scanner *getScanner(char *restrict buffer, int *buffIndex, const int fileSize)
{
    // Create new scanner.
    Scanner *new;
    new = malloc(sizeof *new);
    new->length = 0;
    new->pos.x = 0;
    new->pos.y = 0;
    new->pos.z = 0;

    // Skip to the next newline.
    while (*buffIndex < fileSize && buffer[*buffIndex] != '\n')
    {
        (*buffIndex)++;
    }
    (*buffIndex)++;
    int start = *buffIndex;
    // Now we can read the numbers!
    // First figure out how many beacons there are.
    int totalBeacons = 0;
    while (*buffIndex < fileSize && !(buffer[*buffIndex] == '-' && buffer[(*buffIndex) + 1] == '-'))
    {
        if (buffer[*buffIndex] == '\n')
        {
            totalBeacons++;
        }
        (*buffIndex)++;
    }
    if (*buffIndex >= fileSize)
    {
        // There's no extra newline at the end of the input.
        totalBeacons++;
    }
    else
    {
        totalBeacons--;
    }

    new->beacons = malloc(sizeof *new->beacons *totalBeacons);
    new->length = totalBeacons;
    *buffIndex = start;
    // Now read through the numbers and assign the beacon numbers to them.
    int currentBeacon = 0;
    while (currentBeacon < totalBeacons)
    {
        int sign = 1;
        if (buffer[*buffIndex] == '-')
        {
            sign = -1;
            (*buffIndex)++;
        }
        else
        {
            sign = 1;
        }
        new->beacons[currentBeacon].x = getNextNum(buffer, buffIndex, fileSize) * sign;
        (*buffIndex)++;
        if (buffer[*buffIndex] == '-')
        {
            sign = -1;
            (*buffIndex)++;
        }
        else
        {
            sign = 1;
        }
        new->beacons[currentBeacon].y = getNextNum(buffer, buffIndex, fileSize) * sign;
        (*buffIndex)++;
        if (buffer[*buffIndex] == '-')
        {
            sign = -1;
            (*buffIndex)++;
        }
        else
        {
            sign = 1;
        }
        new->beacons[currentBeacon].z = getNextNum(buffer, buffIndex, fileSize) * sign;
        (*buffIndex)++;
        currentBeacon++;
    }
    (*buffIndex)++;
    return new;
}

int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize)
{
    // Skip to the next number.
    while (*buffIndex < fileSize && !isNum(buffer[*buffIndex]))
    {
        (*buffIndex)++;
    }
    // Get the number.
    int num = 0;
    while (*buffIndex < fileSize && isNum(buffer[*buffIndex]))
    {
        num *= 10;
        num += (buffer[*buffIndex] - '0');
        (*buffIndex)++;
    }
    return num;
}

int isNum(const char c)
{
    return (c >= '0' && c <= '9');
}