#include <stdlib.h>
#include <stdio.h>

// Here's a dirty little macro, please forgive me.
#define ABS(n) (((n) > 0) ? (unsigned long)(n) : (unsigned long)(n) * -1UL)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct Cube
{
	int faces[6];
} Cube;

typedef struct cubeList
{
	int size, length;
	Cube **items;
} cubeList;

Cube *newCube(int *faces);
void append(cubeList *list, Cube *cube);
void getCommand(char *restrict buffer, int *buffIndex, const int fileSize, int *faces, int *on);
int isIn(Cube *cubeOne, Cube *cubeTwo);
int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize);
int isNum(const char c);
void freeList(cubeList *list);
void runCommand(Cube *cube, cubeList *list, int on);
void printList(cubeList *list);

int main(int argc, char **argv)
{
	int buffIndex = 0, fileSize;
	char *restrict buffer;
	FILE *input;
	input = fopen(argv[1], "r");
	fileSize = 0;
	fseek(input, 0, SEEK_END);
	fileSize = ftell(input);
	fseek(input, 0, SEEK_SET);
	buffer = malloc((sizeof *buffer) * (fileSize + 1));
	fread(buffer, fileSize, 1, input);
	fclose(input);

	// Reset the buffIndex.
	buffIndex = 0;
	// For parsing the commands.
	int faces[6];
	int on;
	int pos[3] = {0, 0, 0};
	// Main Logical Loop for part A.
	char partAMemory[101][101][101];
	// Clear it out.
	for (int x = -50; x <= 50; x++)
	{
		for (int y = -50; y <= 50; y++)
		{
			for (int z = -50; z <= 50; z++)
			{
				partAMemory[x + 50][y + 50][z + 50] = 0;
			}
		}
	}
	// Now go through and run the commands.
	while (buffIndex < fileSize)
	{
		getCommand(buffer, &buffIndex, fileSize, faces, &on);
		// A lot of nested loops / checking stuff.
		for (int x = -50; x <= 50; x++)
		{
			if (x >= faces[0] && x <= faces[1])
			{
				for (int y = -50; y <= 50; y++)
				{
					if (y >= faces[2] && y <= faces[3])
					{
						for (int z = -50; z <= 50; z++)
						{
							if (z >= faces[4] && z <= faces[5])
							{
								partAMemory[x + 50][y + 50][z + 50] = on;
							}
						}
					}
				}
			}
		}
	}
	// Get solution for part A.
	unsigned long partA = 0;
	for (int x = -50; x <= 50; x++)
	{
		for (int y = -50; y <= 50; y++)
		{
			for (int z = -50; z <= 50; z++)
			{
				if (partAMemory[x + 50][y + 50][z + 50])
				{
					partA++;
				}
			}
		}
	}
	printf("Part A Solution: %lu\n", partA);
	// 600458

	// Part B time!
	// Reset buffIndex.
	buffIndex = 0;
	// List for part B.
	cubeList *list;
	list = malloc(sizeof *list);
	list->length = 0;
	list->size = 100;
	list->items = malloc(sizeof list->items * list->size);
	// Main Logical Loop for part B.
	int temp = 0;
	while (buffIndex < fileSize)
	{
		getCommand(buffer, &buffIndex, fileSize, faces, &on);
		Cube *new = newCube(faces);
		runCommand(new, list, on);
	}
	// Count everything up for part B, long long just in case.
	unsigned long partB = 0;
	for (int i = 0; i < list->length; i++)
	{
		partB +=
			((ABS(list->items[i]->faces[1] - list->items[i]->faces[0]) + 1) *
			 (ABS(list->items[i]->faces[3] - list->items[i]->faces[2]) + 1) *
			 (ABS(list->items[i]->faces[5] - list->items[i]->faces[4]) + 1));
	}
	printf("Part B Solution: %lu\n", partB);
	// 1334275219162622
	// If you want to see all the cubes, you can enable this line to print them.
	// printList(list);
	// In my input there are 9047 cubes.
	freeList(list);
	free(buffer);
	return 0;
}

void printList(cubeList *list)
{
	for (int i = 0; i < list->length; i++)
	{
		printf("Cube %d:\n", i);
		printf("\tX:%d..%d\n", list->items[i]->faces[0], list->items[i]->faces[1]);
		printf("\tY:%d..%d\n", list->items[i]->faces[2], list->items[i]->faces[3]);
		printf("\tZ:%d..%d\n", list->items[i]->faces[4], list->items[i]->faces[5]);
	}
}

void runCommand(Cube *cube, cubeList *list, int on)
{
	// Only go through the list of elements before we started intersecting.
	int oldLength = list->length;
	for (int i = 0; i < oldLength; i++)
	{
		// If we are intersecting any cubes we "break" them into 27 pieces.
		// We delete any pieces that are negative, and the piece we intersect.
		if (isIn(cube, list->items[i]))
		{
			int faces[6];
			int numbers[3] = {0, 0, 0};
			for (numbers[0] = 0; numbers[0] < 3; numbers[0]++)
			{
				for (numbers[1] = 0; numbers[1] < 3; numbers[1]++)
				{
					for (numbers[2] = 0; numbers[2] < 3; numbers[2]++)
					{
						if (numbers[0] == 1 && numbers[1] == 1 && numbers[2] == 1)
						{
							// We skip the center most, as that's our "intersection".
							continue;
						}
						int valid = 1;
						for (int j = 0; j < 6; j += 2)
						{
							// Left row.
							if (numbers[j / 2] == 0)
							{
								faces[j] = list->items[i]->faces[j];
								faces[j + 1] = cube->faces[j] - 1;
							}
							// Center row.
							else if (numbers[j / 2] == 1)
							{
								faces[j] = cube->faces[j];
								faces[j] = MAX(faces[j], list->items[i]->faces[j]);
								faces[j + 1] = cube->faces[j + 1];
								faces[j + 1] = MIN(faces[j + 1], list->items[i]->faces[j + 1]);
							}
							// Right row.
							else
							{
								faces[j] = cube->faces[j + 1] + 1;
								faces[j + 1] = list->items[i]->faces[j + 1];
							}

							// Check to see if this square is invalid.
							if (faces[j] > faces[j + 1] ||
								faces[j] < list->items[i]->faces[j] ||
								faces[j + 1] > list->items[i]->faces[j + 1])
							{
								valid = 0;
								continue;
							}
						}
						if (valid)
						{
							// Add this cube to the list only if it's valid.
							Cube *new = newCube(faces);
							append(list, new);
						}
					}
				}
			}
			// We have to delete this cube now.
			free(list->items[i]);
			list->items[i] = NULL;
			list->length--;
			oldLength--;
			for (int j = i; j < list->length; j++)
			{
				list->items[j] = list->items[j + 1];
			}
			i--;
		}
	}
	if (on)
	{
		// Add the "on" cube.
		// We don't add "off" cubes.
		append(list, cube);
	}
}

void freeList(cubeList *list)
{
	// Free the malloced stuff.
	for (int i = 0; i < list->length; i++)
	{
		free(list->items[i]);
	}
	free(list->items);
	free(list);
}

Cube *newCube(int *faces)
{
	Cube *new;
	new = malloc(sizeof *new);
	for (int i = 0; i < 6; i++)
	{
		new->faces[i] = faces[i];
	}
	return new;
}

void append(cubeList *list, Cube *cube)
{
	if (list->length >= list->size)
	{
		list->size = list->size * 1.5;
		list->items = realloc(list->items, sizeof list->items * list->size);
	}
	list->items[list->length] = cube;
	list->length++;
}

int isIn(Cube *cubeOne, Cube *cubeTwo)
{
	// Our cubes (rectangular prisms) are aligned to each other.
	// We can just check to see if their faces are all within each other.
	// Check all 3 pairs of faces.
	for (int i = 0; i < 6; i += 2)
	{
		// Check to see if these faces are outside of the other cube.
		if ((cubeOne->faces[i] < cubeTwo->faces[i] && cubeOne->faces[i + 1] < cubeTwo->faces[i]) ||
			cubeOne->faces[i] > cubeTwo->faces[i + 1] && cubeOne->faces[i + 1] > cubeTwo->faces[i + 1])
		{
			return 0;
		}
	}
	// None of the faces are outside, ∴ the cube intersects.
	return 1;
}

void getCommand(char *restrict buffer, int *buffIndex, const int fileSize, int *faces, int *on)
{
	// We should start on the first character of the newline.
	// Return 0 for off, 1 for on.
	(*on) = (buffer[(*buffIndex) + 1] == 'n');
	// Write the coordinates to the faces.
	for (int i = 0; i < 6; i++)
	{
		faces[i] = getNextNum(buffer, buffIndex, fileSize);
	}
	(*buffIndex)++;
}

int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize)
{
	// Skip to the next number.
	while (!isNum(buffer[*buffIndex]) && *buffIndex < fileSize && buffer[*buffIndex] != '-')
	{
		(*buffIndex)++;
	}
	// Get the number.
	int sign = 1;
	if (buffer[*buffIndex] == '-')
	{
		sign = -1;
		(*buffIndex)++;
	}
	int num = 0;
	while (isNum(buffer[*buffIndex]) && *buffIndex < fileSize)
	{
		num *= 10;
		num += (buffer[*buffIndex] - '0');
		(*buffIndex)++;
	}
	return num * sign;
}

int isNum(const char c)
{
	return (c >= '0' && c <= '9');
}