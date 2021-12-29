# Advent of Code Day 20 Challenge A & B

Not only is this submarine a nightmare to manage with all this strange, nonstandard features, but it can't even come with a built in image display.

Of course it has to use a strange obscure image compression method that resembles a cursed conway's game of life...

End me.

## **Code Writeup**

This was pretty simple. I just made 2 maps that were 1024 x 1024 (probably way bigger than they needed to be) and just simulated it for 2 and then 48 generations to get the solutions to part A and part B.

My input had the cursed "lit 0" which means that on odd numbered simulation steps there are an "infinite" number of lit pixels. I solved this by having a "flip" value that kept track of whether the simulation step was odd or even, and then having 3 types of cells, lit, off, and untouched. All untouched cells were surrounded by untouched or off cells which allowed me to pretend that I was actually switching them on / off.
