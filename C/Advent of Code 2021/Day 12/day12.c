#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define BUFFSIZE 8192 * 2
#define IDSIZE 8         // Max length that unique Node IDs can be.
#define MAXCONNECTIONS 8 // Max number of connections each node can connect to.
#define MAXNODES 64      // Max number of nodes in the input file.
#define MAXLENGTH 128    // Max length of each path.

int buffIndex;
char buffer[BUFFSIZE];

typedef struct Node
{
    char *id;
    struct Node **connected; // Pointers to pointers
    int connections;
} Node;

Node *getNode(char *id, Node **allNodes, int length);
Node *newNode(char *id);
void connect(Node *a, Node *b);
int findPaths(Node *currentNode, char **traveled, int count, int small, int part); // Find both part A and part B paths.
int isSmall(char *id);
void getNextChar(char *tempBuffer);
int isChar(char c);

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

    Node **allNodes; // List of all nodes.
    allNodes = malloc(sizeof *allNodes * MAXNODES);
    int length = 0;

    while (buffer[buffIndex] != '\0')
    {
        // Read through the input file for nodes and add them to the list, while updating their connections.
        char startID[IDSIZE];
        getNextChar(startID);
        Node *startNode = getNode(startID, allNodes, length);
        if (startNode == NULL)
        {
            // Start Node does not exist, create it.
            startNode = newNode(startID);
            allNodes[length++] = startNode;
        }

        char endID[IDSIZE];
        getNextChar(endID);
        Node *endNode = getNode(endID, allNodes, length);
        if (endNode == NULL)
        {
            // Start Node does not exist, create it.
            endNode = newNode(endID);
            allNodes[length++] = endNode;
        }

        // Update the connections.
        connect(startNode, endNode);

        buffIndex++;
    }

    // Part A: Find how many ways there are to get to the end!
    int solution = 0;
    Node *start = getNode("start", allNodes, length);
    char **traveled;
    traveled = malloc(sizeof *traveled * MAXLENGTH);
    solution = findPaths(start, traveled, 0, 0, 0);
    printf("Part A Solution: %d\n", solution);
    solution = findPaths(start, traveled, 0, 0, 1);
    printf("Part B Solution: %d\n", solution);
    // Part A Solution: 4241
    // Part B Solution: 122134

    // Free the nodes!
    free(traveled);
    for (int i = 0; i < length; i++)
    {
        if (allNodes[i] != NULL)
        {
            if (allNodes[i]->id != NULL)
            {
                free(allNodes[i]->id);
            }
            if (allNodes[i]->connected != NULL)
            {
                free(allNodes[i]->connected);
            }
            free(allNodes[i]);
        }
    }
    free(allNodes);
    return 0;
}

int findPaths(Node *currentNode, char **traveled, int count, int small, int part)
{
    if (strcmp(currentNode->id, "end") == 0)
    {
        // We've reached the end!
        return 1;
    }

    // First check to see if we have already been here (and it is a small node).
    for (int i = 0; i < count; i++)
    {
        if (strcmp(traveled[i], currentNode->id) == 0)
        {
            if (isSmall(traveled[i]))
            {
                if (part) // Part B
                {
                    // Can't go here again if small != 0, or if it's start.
                    if (small != 0 || (strcmp("start", currentNode->id) == 0))
                    {
                        return 0;
                    }
                    else
                    {
                        small = 1; // Now we know that we've been to a small place 2x already.
                    }
                }
                else
                {
                    // Part A.
                    return 0;
                }
            }
        }
        // If we've been here before but it's a large room we don't need to worry, keep going!
    }

    // Recursively go through all the paths!
    int goodPaths = 0;
    // Create new traveled list.
    char **newTraveled;
    newTraveled = malloc(sizeof *newTraveled * MAXLENGTH);
    for (int j = 0; j < count; j++) // Copy current path over!
    {
        newTraveled[j] = traveled[j];
    }
    // Copy current node to the newTraveled.
    newTraveled[count] = malloc(sizeof newTraveled[count] * IDSIZE);
    strcpy(newTraveled[count], currentNode->id);
    for (int i = 0; i < currentNode->connections; i++)
    {
        Node *newNode = currentNode->connected[i];
        // Add current node to the new path.
        goodPaths += findPaths(newNode, newTraveled, count + 1, small, part);
    }
    // Free the added newTraveled node.
    free(newTraveled[count]);
    // Return the calculated good paths.
    return goodPaths;
}

int isSmall(char *id)
{
    if (id[0] >= 'a' && id[0] <= 'z') // It's lowercase...
    {
        return 1;
    }
    return 0; // Otherwise it's not!
}

void connect(Node *a, Node *b)
{
    // Connect A and B together!
    a->connected[a->connections] = b;
    a->connections++;

    b->connected[b->connections] = a;
    b->connections++;
}

Node *newNode(char *id)
{
    Node *node;
    node = malloc(sizeof *node);
    node->id = malloc(sizeof node->id * IDSIZE);
    node->connected = malloc(sizeof *node * MAXCONNECTIONS);
    for (int i = 0; i < MAXCONNECTIONS; i++)
    {
        node->connected[i] = NULL;
    }
    strcpy(node->id, id);
    node->connections = 0;
    return node;
}

Node *getNode(char *id, Node **allNodes, int length)
{
    for (int i = 0; i < length && allNodes[i] != NULL; i++)
    {
        if (strcmp(id, allNodes[i]->id) == 0)
        {
            return allNodes[i];
        }
    }
    return NULL;
}

int isChar(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Much like my earlier function "getNextNum".
void getNextChar(char *tempBuffer)
{
    // Skip non-characters.
    while (!isChar(buffer[buffIndex++]))
        ;
    buffIndex--;
    int i = 0;
    while (isChar(buffer[buffIndex]))
    {
        tempBuffer[i++] = buffer[buffIndex++];
    }
    tempBuffer[i] = '\0';
}