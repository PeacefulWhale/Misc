#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    int number; // If this is -1, then this has children.
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

Node *getNodes(Node *parent, char *restrict buffer, int *buffIndex, const int fileSize); // Parse input into nodes.
Node *addNode(Node *a, Node *b);                                                         // Return a new node that has a as the left, and b as the right.
void reduceNode(Node *root);                                                             // Reduces the tree in place.
void splits(Node *root, Node *current, int *flag);                                       // Recursive Explosions.
void explosions(Node *root, Node *current, const int depth, int *flag);                  // Recursive splitting.
void freeNode(Node *a);                                                                  // Frees the stack.
void printNode(Node *a);                                                                 // Prints the tree.
void printNodeR(Node *a);                                                                // Recursive function for printNode.
unsigned long long magnitude(Node *root);                                                // Recursive Magnitude calculation. ULL just in case...
int isNum(const char c);

int main(int argc, char **argv)
{
    int buffIndex, fileSize;
    char *restrict buffer;
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
    if (buffer == NULL)
    {
        printf("Could not allocate space for buffer...\n");
        exit(1);
    }
    fread(buffer, fileSize, 1, input);
    fclose(input);

    // Get first line.
    Node *sum;
    sum = getNodes(NULL, buffer, &buffIndex, fileSize);
    buffIndex++;
    // Add the next lines to the sum.
    int lines = 0; // Part B
    while (buffIndex < fileSize)
    {
        Node *b = getNodes(NULL, buffer, &buffIndex, fileSize);
        sum = addNode(sum, b);
        reduceNode(sum);
        buffIndex++;
        lines++;
    }
    printf("Node after reduction:\n");
    printNode(sum);
    // Part A:
    unsigned long long partA = magnitude(sum);
    printf("Part A Solution: %llu\n", partA);
    // 3359
    freeNode(sum);

    // For part B find the largest possible sum of 2 numbers from the homework list.
    // We need to generate all possible permutations and figure out which one is the largest.
    // Order matters it appears...
    // Let's just do a nested loop.
    unsigned long long partB = 0;
    for (int i = 0; i < lines; i++)
    {
        for (int j = i + 1; j < lines; j++)
        {
            Node *a;
            Node *b;
            // Skip to line i.
            int firstIndex = 0;
            for (int skip = 0; skip < i; skip++)
            {
                while (buffer[firstIndex] != '\n' && firstIndex < fileSize)
                {
                    firstIndex++;
                }
                firstIndex++;
            }
            // Skip to line j.
            int secondIndex = 0;
            for (int skip = 0; skip < j; skip++)
            {
                while (buffer[secondIndex] != '\n' && secondIndex < fileSize)
                {
                    secondIndex++;
                }
                secondIndex++;
            }
            int first, second;
            first = firstIndex;
            second = secondIndex;
            // A + B
            a = getNodes(NULL, buffer, &firstIndex, fileSize);
            b = getNodes(NULL, buffer, &secondIndex, fileSize);
            a = addNode(a, b);
            reduceNode(a);
            unsigned long long temp = magnitude(a);
            if (temp > partB)
            {
                partB = temp;
            }
            freeNode(a);
            a = NULL;

            // I'm too lazy to actually do permutations lol.
            // Let's just swap a and b here.
            firstIndex = first;
            secondIndex = second;
            // B + A
            a = getNodes(NULL, buffer, &firstIndex, fileSize);
            b = getNodes(NULL, buffer, &secondIndex, fileSize);
            a = addNode(b, a);
            reduceNode(a);
            temp = magnitude(a);
            if (temp > partB)
            {
                partB = temp;
            }
            freeNode(a);
            a = NULL;
        }
    }
    printf("Part B Solution: %llu\n", partB);
    // 4616
    free(buffer);
    return 0;
}

void splits(Node *root, Node *current, int *flag)
{
    if (current != NULL)
    {
        int recalled = 0; // To keep track if this function has recursively called back to the start, to avoid calling the children roots too many times!
        if (current->number >= 10 && recalled == 0)
        {
            *flag = 1; // Flag to set to 1 if we've changed the tree.
            recalled = 1;
            // Create new node from these values.
            int left = current->number / 2;
            int right = current->number - left;
            current->number = -1;

            // Create left node.
            Node *leftNode;
            leftNode = malloc(sizeof *leftNode);
            if (leftNode == NULL)
            {
                printf("Could not allocate space for new node...\n");
                exit(1);
            }
            leftNode->number = left;
            leftNode->left = NULL;
            leftNode->right = NULL;
            leftNode->parent = current;

            // Create right node.
            Node *rightNode;
            rightNode = malloc(sizeof *rightNode);
            if (rightNode == NULL)
            {
                printf("Could not allocate space for new node...\n");
                exit(1);
            }
            rightNode->number = right;
            rightNode->left = NULL;
            rightNode->right = NULL;
            rightNode->parent = current;

            // Update current node.
            current->right = rightNode;
            current->left = leftNode;
            // Go back to the beginning.
            reduceNode(root);
        }

        // Call reduce on the children of this node.
        if (recalled == 0)
        {
            splits(root, current->left, flag); // Prefer left sided reducing action.
            splits(root, current->right, flag);
        }
    }
}

void explosions(Node *root, Node *current, const int depth, int *flag)
{
    if (current != NULL)
    {
        int recalled = 0; // To keep track if this function has recursively called back to the start, to avoid calling the children roots too many times!
        // Check for explosion.
        if (depth >= 4)
        {
            // May only explode if it contains 2 regular numbers
            if (current->right != NULL && current->left != NULL)
            {
                int left = current->left->number;
                int right = current->right->number;
                if (left != -1 && right != -1)
                {
                    *flag = 1; // Flag to set to 1 if we've changed the tree.
                    recalled = 1;
                    freeNode(current->left);
                    current->left = NULL;
                    freeNode(current->right);
                    current->right = NULL;
                    current->number = 0;
                    Node *tempNode = current;
                    Node *prev = NULL;

                    // Left:
                    tempNode = current;
                    prev = NULL;
                    while (tempNode->parent != NULL && tempNode->left == prev)
                    {
                        // Go up until the previous node is no longer on the left.
                        prev = tempNode;
                        tempNode = tempNode->parent;
                    }
                    if (tempNode->parent != NULL || tempNode->left != prev)
                    {
                        tempNode = tempNode->left;
                        while (tempNode->right != NULL && tempNode->number == -1)
                        {
                            // Go right until the current number is not -1.
                            tempNode = tempNode->right;
                        }
                        if (tempNode != NULL && tempNode->number != -1)
                        {
                            tempNode->number += left;
                        }
                    }
                    // Right:
                    tempNode = current;
                    prev = NULL;
                    while (tempNode->parent != NULL && tempNode->right == prev)
                    {
                        // Go up until the previous node is no longer on the right.
                        prev = tempNode;
                        tempNode = tempNode->parent;
                    }
                    if (tempNode->parent != NULL || tempNode->right != prev)
                    {
                        tempNode = tempNode->right;
                        while (tempNode->left != NULL && tempNode->number == -1)
                        {
                            // Go left until the current number is not -1.
                            tempNode = tempNode->left;
                        }
                        if (tempNode != NULL && tempNode->number != -1)
                        {
                            tempNode->number += right;
                        }
                    }
                    reduceNode(root);
                    // explosions(root, root, 0, flag);
                }
            }
        }
        // Call reduce on the children of this node.
        if (recalled == 0)
        {
            explosions(root, current->left, depth + 1, flag); // Prefer left sided reducing action.
            explosions(root, current->right, depth + 1, flag);
        }
    }
}

void reduceNode(Node *root)
{
    if (root != NULL)
    {
        int changed = 0;
        do
        {
            changed = 0;
            explosions(root, root, 0, &changed);
            if (changed == 0)
            {
                splits(root, root, &changed);
            }
        } while (changed != 0);
    }
}

Node *getNodes(Node *parent, char *restrict buffer, int *buffIndex, const int fileSize)
{
    Node *new;
    new = malloc(sizeof *new);
    if (new == NULL)
    {
        printf("Could not allocate space for new node...\n");
        exit(1);
    }
    new->left = NULL;
    new->right = NULL;
    new->parent = parent;
    new->number = -1;
    if (isNum(buffer[*buffIndex]))
    {
        new->number = 0;
        while (isNum(buffer[*buffIndex]))
        {
            new->number *= 10;
            new->number += (buffer[*buffIndex] - '0');
            (*buffIndex)++;
        }
        return new;
    }
    // New left node.
    if (buffer[*buffIndex] == '[')
    {
        (*buffIndex)++;
        new->left = getNodes(new, buffer, buffIndex, fileSize);
    }
    // New right node.
    if (buffer[*buffIndex] == ',')
    {
        (*buffIndex)++;
        new->right = getNodes(new, buffer, buffIndex, fileSize);
    }
    // See if we're done with this current node.
    if (buffer[*buffIndex] == ']')
    {
        (*buffIndex)++;
        return new;
    }
    return new;
}

void printNode(Node *a)
{
    printNodeR(a);
    printf("\n");
}

void printNodeR(Node *a)
{
    if (a->number != -1)
    {
        printf("%d", a->number);
    }
    else
    {
        if (a->left != NULL)
        {
            printf("[");
            printNodeR(a->left);
        }
        if (a->right != NULL)
        {
            printf(",");
            printNodeR(a->right);
            printf("]");
        }
    }
}

unsigned long long magnitude(Node *root)
{
    unsigned long long total = 0;
    if (root == NULL)
    {
        return 0;
    }
    else if (root->number != -1)
    {
        return root->number;
    }
    else
    {
        total += (3ULL * magnitude(root->left));
        total += (2ULL * magnitude(root->right));
    }
    return total;
}

Node *addNode(Node *a, Node *b)
{
    // Add b to a.
    Node *new;
    new = malloc(sizeof *new);
    if (new == NULL)
    {
        printf("Could not allocate space for new node...\n");
        exit(1);
    }
    new->parent = NULL;
    new->left = a;
    new->right = b;
    a->parent = new;
    b->parent = new;
    new->number = -1;
    return new;
}

void freeNode(Node *a)
{
    if (a != NULL)
    {
        if (a->right != NULL)
        {
            freeNode(a->right);
        }
        if (a->left != NULL)
        {
            freeNode(a->left);
        }
        a->number = -1;
        free(a);
        a = NULL;
    }
}

int isNum(const char c)
{
    return (c >= '0' && c <= '9');
}