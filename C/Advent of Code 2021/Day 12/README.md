# Day 11 Challenge A & B

Avast, it appears that this cave system goes on forever.

And this gosh darn submarine refuses to do anything that is suboptimal!

After sending out probes it has now mapped the entire cave system from start to finish, and yet it refuses to move forward until I provide it with an optimal path. And it refuses to accept the *obviously* shortest path. It insists upon me proving that this path is the shortest, but won't accept simple proofs that utilize the length of the path and the shortest possible length of the given map. Apparently it is a stupid submarine and insists that the shortest path is proven by being shown *all* possible paths. And for whatever reason the submarine itself is incapable of generating such paths.

What choices have I made that have led me to dealing with a temperamental submarine?

## **Code Writeup**

This was a pretty simple "generate all paths" problem, that had a few rules enforced (that differed for part A and B).

I was able to make a recursive function that went through all possible paths.

I don't think it's possible to do this in any other way than brute forcing it because the challenge requires generating *all* paths, and not the optimal one or longest one, or something like that.

The most challenging part of this problem was making the data structure and parsing the input, but that didn't take me that long if I'm honest.

The data system I ended up utilizing was the following:

```C
typedef struct Node
{
    char *id;
    struct Node **connected;
    int connections;
} Node;
```

In addition I utilized a `char **` variable to keep track of which node's I had visited in the recursive function.

It was interesting, and I think my solution is near optimal.
