# Advent of Code Day 19 Challenge A & B

Dear lord who made this submarine? I'm guessing the elves, but if that's true Christmas must already be doomed. No one who makes such a barely functioning machine would be able to keep Christmas running!

The scanners I need to map the depth are barely functional. The beacons don't even broadcast unique IDs so you can identify them! The scanners don't know which way is up!

At least it shouldn't be too difficult to get this thing working...

## **Code Writeup**

I was *extra* lazy this day, and instead of doing linear algebra, making fingerprints with the distances, or anything smart like that, I just brute forced the solution.

I also was too lazy to figure out how to create a function for the cube rotation, so I just hard coded it all. To be honest, it works, its quick, and it's only 75% spaghetti.

Well it's not that quick (on my i3 it takes ~30 seconds to run). It takes a solid 30 seconds to solve A and B. Solving B is actually instantly done, as I was lazy and brute forced all the beacons and scanners to find the answer to part A even though I didn't need to do that.

My method is really simple. Set `scanners[0]` to 0,0,0. I then loop through the other scanners and see if any combination of alignments and offsets (calculated by taking one point from `scanners[0]` and `scanners[i]`) leads to `X` matches. I found that 3 matches worked fine for the test case and my input. However it is possible to get an input in which you require more matches.

Part B was just 2 nested loops to find the greatest combination.
