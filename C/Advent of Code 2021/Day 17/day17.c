#include <stdlib.h>
#include <stdio.h>

typedef struct Data
{
    int x;
    int y;
} Data;

int simulate(int x, int y, Data min, Data max);
int isIn(Data a, Data min, Data max);

int main(int argc, char **argv)
{
    // We aren't using the file buffer today because I'm lazy.
    // Test input: target area: x=20..30, y=-10..-5
    // Our input: target area: x=248..285, y=-85..-56
    int x1 = 248, x2 = 285, y1 = -85, y2 = -56;
    if(argc > 1) // If you want to enter input just put *some* flag in the call to the exectuable.
    {
        printf("Enter x1, x2, y1, and y2\n");
        scanf("%d", &x1);
        scanf("%d", &x2);
        scanf("%d", &y1);
        scanf("%d", &y2);
    }
    // Swap the values so they're in the right order.
    if(x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if(y1 < y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    Data targetMin, targetMax;
    targetMin.x = x1;
    targetMin.y = y2;
    targetMax.x = x2;
    targetMax.y = y1;
    printf("Target square set to:\n X: %d to %d, Y: %d to %d\n", x1, x2, y1, y2);
    // Calculate min and max values.
    Data min, max;
    // Min / Max y is really easy.
    min.y = y2;
    max.y = -y2;
    // Min / Max x is a bit harder.
    max.x = x2;
    int sum = 0;
    for(int x = 0; x < max.x; x++)
    {
        // Keep going until the sum of x is equal or greater than x1.
        sum += x;
        if(sum >= x1)
        {
            min.x = x;
            break;
        }
    }
    printf("Max: x = %d, y = %d\n", max.x, max.y);
    printf("Min: x = %d, y = %d\n", min.x, min.y);
    // Part A is really easy, I originally solved it on a TI-84, but we mine as well solve it here too I guess.
    // Simply sum the value of our max Y
    int partA = 0;
    for(int y = 0; y < max.y; y++)
    {
        partA += y;
    }
    printf("\nPart A solution: %d\n", partA);
    // 3570

    // Part B is a little bit harder. I'm sure there's a way to make a formula for this, but I figured scripting it would be quicker.
    int partB = 0;
    for(int x = min.x; x <= max.x; x++)
    {
        for(int y = min.y; y <= max.y; y++)
        {
            partB += simulate(x, y, targetMin, targetMax);
        }
    }
    printf("Part B solution: %d\n", partB);
    // 1919
    return 0;
}

int simulate(int x, int y, Data min, Data max)
{
    Data position;
    position.x = 0;
    position.y = 0;

    while(1) // Loop until our function has exceeded how far it's supposed to go.
    {
        // Update position and velocity.
        if(x != 0)
        {
            position.x += x--;
        }
        position.y += y--;

        // Now check and see if our position is valid.
        if(isIn(position, min, max))
        {
            return 1; // This path is valid.
        }
        else if (x == 0 && position.x <= min.x)
        {
            // X is 0 and we're not in the correct boundary yet.
            return 0;
        }
        else if (position.x > max.x || position.y < min.y)
        {
            // We've gone out of the boundaries, give up.
            return 0;
        }
    }
}

int isIn(Data a, Data min, Data max)
{
    return (
        (min.x <= a.x && a.x <= max.x) &&
        (min.y <= a.y && a.y <= max.y)
    );
}