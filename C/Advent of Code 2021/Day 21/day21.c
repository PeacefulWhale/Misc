#include <stdlib.h>
#include <stdio.h>

#define TARGETSCORE 21 // Part B Target score.
#define MAXSCORE 30   // Max score possible for the cache.

typedef struct Player
{
    int pos, score;
} Player;

// I'm too lazy to deal with pointers, sue me.
typedef struct doubleLong
{
    unsigned long value[2];
} doubleLong;

doubleLong cache[10][10][MAXSCORE][MAXSCORE][2]; // Cache for part B.

long playUntilPartA(const int target, Player playerOne, Player playerTwo); // Method for simulating plays.
void turnPartA(Player *restrict player, int *restrict dice);
void turnpartBRecur(Player *restrict player, const int dice);
doubleLong addDoubleLong(doubleLong a, doubleLong b);
doubleLong partB(Player one, Player two, int player);
int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize);
int isNum(const char c);

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
    buffer = malloc((sizeof *buffer) * fileSize);
    fread(buffer, fileSize, 1, input);
    fclose(input);

    Player playerOne, playerTwo;
    int startPosOne, startPosTwo;
    getNextNum(buffer, &buffIndex, fileSize); // Skip the Player '1'.
    startPosOne = getNextNum(buffer, &buffIndex, fileSize);
    getNextNum(buffer, &buffIndex, fileSize); // Skip the Player '2'.
    startPosTwo = getNextNum(buffer, &buffIndex, fileSize); 
    free(buffer);

    // Does not work if a player position is 10, sorry!
    playerOne.pos = startPosOne;
    playerOne.score = 0;
    playerTwo.pos = startPosTwo;
    playerTwo.score = 0;
    printf("Part A: %lu\n", playUntilPartA(1000, playerOne, playerTwo));
    // 752745

    // For part B we need to discuss *parallel universes*...
    doubleLong wins;
    wins.value[0] = 0;
    wins.value[1] = 0;
    // Refresh players for part B.
    playerOne.pos = startPosOne;
    playerOne.score = 0;
    playerTwo.pos = startPosTwo;
    playerTwo.score = 0;

    // Set the cache to 0.
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int x = 0; x < MAXSCORE; x++)
            {
                for (int y = 0; y < MAXSCORE; y++)
                {
                    cache[i][j][x][y][0].value[0] = 0;
                    cache[i][j][x][y][0].value[1] = 0;
                    cache[i][j][x][y][1].value[0] = 0;
                    cache[i][j][x][y][1].value[1] = 0;
                }
            }
        }
    }
    // We start rolling from player 2, because that will skip player 2's turn and 1 will actually roll first.
    wins = partB(playerOne, playerTwo, 1);
    // Get the lesser value.
    wins.value[0] = (wins.value[0] >= wins.value[1]) ? wins.value[0] : wins.value[1];
    printf("Part B: %lu\n", wins.value[0]);
    // 309196008717909
    return 0;
}

doubleLong partB(Player one, Player two, int player)
{
    doubleLong returnValue;
    returnValue.value[0] = returnValue.value[1] = 0;
    Player *currentPlayer = player ? &two : &one;

    if (currentPlayer->score >= TARGETSCORE)
    {
        // We have reached a winning universe.
        returnValue.value[player] = 1;
        returnValue.value[!player] = 0;
    }
    else if (cache[one.pos - 1][two.pos - 1][one.score][two.score][player].value[0] != 0 &&
             cache[one.pos - 1][two.pos - 1][one.score][two.score][player].value[1] != 0)
    {
        // This universe, while not winning, is cached.
        returnValue = cache[one.pos - 1][two.pos - 1][one.score][two.score][player];
    }
    else
    {
        // This universe is not a winning one, and is not cached. We have to traverse down.
        for (int x = 1; x <= 3; x++)
        {
            for (int y = 1; y <= 3; y++)
            {
                for (int z = 1; z <= 3; z++)
                {
                    // x + y + z is our dice roll.
                    Player tempOne, tempTwo;
                    tempOne = one;
                    tempTwo = two;
                    // We now swap the current player.
                    currentPlayer = player ? &tempOne : &tempTwo;
                    // Find the new player position and score.
                    currentPlayer->pos += (x + y + z);
                    currentPlayer->pos = ((currentPlayer->pos - 1) % 10) + 1;
                    currentPlayer->score += currentPlayer->pos;
                    // Traverse the possible universes!
                    returnValue = addDoubleLong(returnValue, partB(tempOne, tempTwo, !player));
                }
            }
        }
    }
    // Cache the returnValue.
    cache[one.pos - 1][two.pos - 1][one.score][two.score][player] = returnValue;
    // Also cache the "mirror universe".
    // I don't know if this actually does anything, and if it's even correct.
    cache[two.pos - 1][one.pos - 1][two.score][one.score][!player].value[0] = returnValue.value[1];
    cache[two.pos - 1][one.pos - 1][two.score][one.score][!player].value[1] = returnValue.value[0];
    return returnValue;
}

doubleLong addDoubleLong(doubleLong a, doubleLong b)
{
    doubleLong returnValue;
    returnValue.value[0] = a.value[0] + b.value[0];
    returnValue.value[1] = a.value[1] + b.value[1];
    return returnValue;
}

long playUntilPartA(const int target, Player playerOne, Player playerTwo)
{
    long diceRolls = 0;
    int dice = 1;
    while (1)
    {
        // Player one rolls.
        turnPartA(&playerOne, &dice);
        diceRolls += 3;
        if (playerOne.score >= target)
        {
            return diceRolls * playerTwo.score;
        }
        // Player two rolls.
        turnPartA(&playerTwo, &dice);
        diceRolls += 3;
        if (playerTwo.score >= target)
        {
            return diceRolls * playerOne.score;
        }
    }
}

void turnPartA(Player *restrict player, int *restrict dice)
{
    for (int i = 0; i < 3; i++)
    {
        player->pos += *dice;
        player->pos = ((player->pos - 1) % 10) + 1; // Gosh darn modulus shifts.
        (*dice)++;
        if (*dice == 101)
        {
            *dice = 1;
        }
    }
    player->score += player->pos;
}

int getNextNum(char *restrict buffer, int *buffIndex, const int fileSize)
{
    // Skip to the next number.
    while (!isNum(buffer[*buffIndex]) && *buffIndex < fileSize)
    {
        (*buffIndex)++;
    }
    // Get the number.
    int num = 0;
    while (isNum(buffer[*buffIndex]) && *buffIndex < fileSize)
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