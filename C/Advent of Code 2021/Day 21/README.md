# Advent of Code Day 21

As I descend deeper into the abyss, the submarine offers to play a game with me...

I'm sure that this will go wonderfully and not have any reality breaking consequences... Right?

## **Code Writeup**

Part A was pretty easy, one brute force later and it's done.

Part B was a little bit harder, I had to implement a cache to make sure that this program didn't take a few days to run.

After messing around with trying to optimize the cache I gave up and just made it mirror the function's arguments. It runs in less than half a second, but I feel like if I spent more time on this I would be able to get it to run faster. Sadly I am lazy and that will not be happening today.

Once I got the cache working it was pretty easy to just recursively go down all possible universes and add up the total wins.
