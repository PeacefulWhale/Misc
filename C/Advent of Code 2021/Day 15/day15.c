#include <stdlib.h>
#include <stdio.h>

#define PRINTMAP 0 // Set to 1 to print final map path to terminal.
#define EMPTY '.'  // What to use as the empty space in the map.

int buffIndex;
char *restrict buffer;

int dijkstra(int *restrict costs, int *restrict nodes, int *restrict visited, const int width, const int height);
void dijStep(int *restrict costs, int *restrict nodes, int *restrict visited, const int currentX, const int currentY, const int width, const int height);
void printMap(const int *restrict nodes, const int width, const int height);

int main(int argc, char **argv)
{
    buffIndex = 0;
    FILE *input;
    input = fopen(argv[1], "r");
    // Get file size.
    int fileSize = 0;
    fseek(input, 0, SEEK_END);
    fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);
    // Malloc file.
    buffer = malloc((sizeof *buffer) * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);

    // From our input buffer make a map.
    int width, height;
    width = height = 0;
    // Get width.
    while (buffer[width] != '\n')
    {
        width++;
    }
    height = (fileSize / (width + 1)) + 1;
    // Initial map from which all other maps come from.
    int *restrict costs;
    int *restrict nodes;
    int *restrict visited;
    costs = malloc((sizeof *costs) * (width * height)); // Cost to get to each node.
    nodes = malloc((sizeof *nodes) * (width * height)); // Current values of nodes.
    visited = malloc((sizeof *visited) * (width * height)); // Whether or not we've visited the nodes before.
    // Copy buffer to map.
    int tempX, tempY;
    tempX = tempY = 0;
    while (buffIndex < fileSize)
    {
        costs[(tempY * width) + tempX] = (buffer[buffIndex]) - '0';
        nodes[(tempY * width) + tempX] = -1;
        visited[(tempY * width) + tempX] = 0;
        tempX++;
        if (tempX == width)
        {
            // New line!
            tempY++;
            tempX = 0;
            buffIndex++;
        }
        buffIndex++;
    }
    free(buffer);
    int partA = dijkstra(costs, nodes, visited, width, height);
    if(PRINTMAP)
    {
        printMap(nodes, width, height);
    }
    printf("Part A solution: %d\n", partA);
    free(visited);
    free(nodes);

    // We need to make our new 5x larger map for part B.
    int oldHeight = height;
    int oldWidth = width;
    height *= 5;
    width *= 5;
    int *restrict newCosts; // The only one we need a new name of.
    newCosts = malloc((sizeof *costs) * (width * height)); // Cost to get to each node.
    nodes = malloc((sizeof *nodes) * (width * height)); // Current values of nodes.
    visited = malloc((sizeof *visited) * (width * height)); // Whether or not we've visited the nodes before.
    // Ok, now to get our new costs / nodes / visited!
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            nodes[(y * width) + x] = -1;
            visited[(y * width) + x] = 0;
            if(x < oldWidth && y < oldHeight)
            {
                // direct copy!
                newCosts[(y * width) + x] = costs[(y * oldWidth) + x];
            }
            else
            {
                // Follow the weird rules for generating the new input.
                int oldY, oldX;
                oldY = (y % oldHeight) * oldWidth;
                oldX = (x % oldWidth);
                int newValue = costs[(oldY) + oldX];
                newValue += x / oldWidth;
                newValue += y / oldHeight;
                if(newValue > 9)
                {
                    newValue %= 10;
                    newValue += 1;
                }
                newCosts[(y * width) + x] = newValue;
            }
        }
    }
    int partB = dijkstra(newCosts, nodes, visited, width, height);
    if(PRINTMAP)
    {
        printMap(nodes, width, height);
    }
    printf("Part B solution: %d\n", partB);
    
    free(costs);
    free(newCosts);
    free(visited);
    return 0;
}

int dijkstra(int *restrict costs, int *restrict nodes, int *restrict visited, const int width, const int height)
{
    // Dijkstra's Algorithm time!
    // Start in the top left corner, move towards the bottom right.
    visited[0] = 1;
    nodes[0] = 0;
    // While we have not visited the destination.
    int done = 0;
    while (!done)
    {
        // Get the smallest value.
        struct smallest
        {
            int x;
            int y;
            int value;
        } smallest;
        smallest.value = -1;
        smallest.x = 0;
        smallest.y = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                // If we have not yet visited this spot.
                if (visited[(y * width) + x] == 0)
                {
                    if (nodes[(y * width) + x] != -1)
                    {
                        // If both values are equal, choose the one that is closest to the bottom corner.
                        if (nodes[(y * width) + x] == smallest.value)
                        {
                            if(x + y > smallest.x + smallest.y)
                            {
                                smallest.value = nodes[(y * width) + x];
                                smallest.x = x;
                                smallest.y = y;
                            }
                        }
                        // If this spot is less than our current smallest (and the spot is not -1 either)
                        else if ((nodes[(y * width) + x] < smallest.value || smallest.value == -1))
                        {
                            smallest.value = nodes[(y * width) + x];
                            smallest.x = x;
                            smallest.y = y;
                        }
                    }
                }
            }
        }
        // Now call the dijStep with the smallest value.
        dijStep(costs, nodes, visited, smallest.x, smallest.y, width, height); 
        if(visited[(height * width) - 1] == 1)
        {
            done = 1;
        }
        else // only 1 undone node!
        {
            // See if the unvisited nodes are all -1.
            int tempDone = 1;
            for(int i = 0; i < (width * height); i++)
            {
                if(visited[i] == 0 && nodes[i] != -1)
                {
                    tempDone = 0;
                    break;
                }
            }
            done = tempDone;
            if(done)
            {
                printMap(visited, width, height);
                printf("\n");
                printMap(nodes, width, height);
            }
        }
    }
    return nodes[(height * width) - 1];
}

void dijStep(int *restrict costs, int *restrict nodes, int *restrict visited, const int currentX, const int currentY, const int width, const int height)
{
    // The distance to the nodes next to our node is the current value of this node added to the value of the other nodes.
    for (int y = 1; y >= -1; y--)
    {
        for (int x = 1; x >= -1; x--)
        {
            // It is part of the valid '+' shape.
            if (x + y != 0 && -x + y != 0)
            {
                // Check if it fits the height and width restraints.
                if (
                    currentX + x >= 0 &&
                    currentX + x <= width + 1 &&
                    currentY + y >= 0 &&
                    currentX + y <= height + 1)
                {
                    // Make sure we haven't visited this node already.
                    if (visited[((currentY + y) * width) + (currentX + x)] == 0)
                    {
                        // Now calculate the distance to this node from our current node.
                        int tempValue = nodes[((currentY)* width) + (currentX)] + costs[((currentY + y) * width) + (currentX + x)];
                        // Check and see if this is the new best path to the current value.
                        if (tempValue < nodes[((currentY + y) * width) + (currentX + x)] || nodes[((currentY + y) * width) + (currentX + x)] == -1)
                        {
                            // It is, set the value of this node to the new best value.
                            nodes[((currentY + y) * width) + (currentX + x)] = tempValue;
                        }
                    }
                }
            }
        }
    }
    // Set the current node as visited.
    visited[(currentY * width) + currentX] = 1;
}

void printMap(const int *restrict nodes, const int width, const int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (nodes[(y * width) + x] == -1)
            {
                printf("%4c", EMPTY);
            }
            else
            {
                printf("%4d", nodes[(y * width) + x]);
            }
        }
        printf("\n");
    }
}