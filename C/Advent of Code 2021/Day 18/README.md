# Day 18 Challenge A & B

This cannot get any weirder. I am now certain that I have died, and that I am now cursed to live my life in this *hell*.

Crabs.

Giant squids.

Hostile whales.

And now...

Snails...

Snails that need help with their math problems.

There are sentient, nay, sapient snails, and one of them requires my assistance with their math homework, and the submarine refuses to leave this snail city until I render assistance.

No one is going to believe me if I ever make it out of here.

The elves on the ship weren't even surprised, they even knew the name of this place.

They call it "Cochlea Utopia". It kinda rhymes if you say it right. It doesn't seem like a great name for a city though, naming anything "Utopia" seems really cliché to me.

Well, I guess I don't know that. Many in whatever language they speak it makes more sense, perhaps the translation isn't quite as good as it could be.

## **Code Writeup**

Rules:

1. If any pair is nested inside four pairs, the leftmost such pair explodes.
2. If any regular number is 10 or greater, the leftmost such regular number splits.
3. During reduction, at most one action applies, after which the process returns to the top of the list of actions.
4. Once no action in the above list applies, the snailfish number is reduced.

Also:

ALL EXPLOSIONS GO OFF BEFORE ANY SPLITS

I originally wrote my method to do the left most of either explosions or splits, and once I learned that this was *not* how one was supposed to do it I quickly changed it.

Also you have to reduce after ever addition because you'll get it wrong if you wait until the end.

I utilized nodes and a tree system with:

```C
typedef struct Node
{
    int number; // If this is -1, then this has children.
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;
```

This made my methods for exploding, adding, printing, and calculating the sum of rather elegant I think. I utilized a fair bit of recursion.

I didn't do recursion for part B though, just a nested loop and copy / pasting the code and swapping `a` and `b`.
