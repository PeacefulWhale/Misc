# Day 22

The reactor needs to be rebooted? What? How? Why?

Am I even qualified to do this? What if I get it wrong? I don't want to become an irradiated sludge at the bottom of the ocean floor for some strange crab to eat. I want to live!

I better simulate this first, just to make sure I get it right.

## **Code Writeup**

For part A it's quite easy to just utilize a 3D array and go through and update every single cell, it took me less than 5 minutes to implement that. I was originally planning to just calculate all the cubes, and then apply a "filter" over the correct range.

However upon thinking about my planned methods to calculate the full length (at the time it was an Octree) I decided that brute forcing part A would probably be easier than implementing a filter, and it would allow me to ensure that I understood the logic of the problem. Really though, I just wanted the golden star to assure myself that my brain isn't as smooth as I fear it is.

For part B it gets a little bit harder. Originally I planned to use an octree to map it all. My first attempt to implement this did so by breaking cubes into as large as possible, and breaking them into smaller pieces if needed. The problem with this was that you would end up with _many_ 1x1x1 cubes, to the point where you'd never really be able to finish it. Even though a few number of the cubes were larger than 1x1x1, it wasn't enough.

My second attempt was to have cubes in a list, and going through and creating modifying cubes and creating new ones as the commands came in, treating them like boolean operations. I did this by essentially "breaking" the intersected cube into 27 parts, and adding the valid parts. By skipping the middle most one, and adding the operator cube in if it's an "on" cube, I was able to go through the input and generate an up to date list of all cubes.

Each x/y/z axis was divided into 3 rows, the left, center, and right. Based off the current position we're in utilizing 3 nested loops, we can calculate the "piece" of the intersected cube that we're looking at. If it's a valid cube (no negative lengths and within the bounds of the cutting and intersecting cubes) then we add it to the list. If it's not, we skip over it.

It turns out that I don't need to sort the cubes in a octree or anything, and looping through all of them is quick enough for me (under a tenth of a second, as it should be). In the end we had 9047 cubes, and a total volume of 1334275219162622.
