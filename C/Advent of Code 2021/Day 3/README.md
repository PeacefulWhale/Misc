# Day 3 Challenge A & B

Another day, another another nightmarish venture into the deep. At least this time they've provided me with a map and some better instructions. This time I slept through the jingle bells and only found my way down to the submarine well in the afternoon. They still haven't found someone small enough other than me to fit inside it. The control team thinks they've found two more possible star locations. I don't know how they find them, and why its always two stars, but that's not really my problem.

My problem was originally allowing myself to get roped into this whole "save Christmas" cliche. Now my problem is the sounds this submarine is making as I hover at 1000 meters below the surface.

I'm not afraid of the darkness, the inky depths, or the moving shadows at the corners of my eye.

I'm more concerned with the increasingly likely possibility that this poorly made can crumples like wet cardboard and I turn into a softball sized lump of "very dead me".

Control tells me that I can confirm systems are nominal by utilizing the diagnostic report. For some ungodly reason it's outputted to my computer via airdrop, and is only accessible as a string of ones and zeroes... At least control can tell me how to read it, but it would take to long to go through manually. I guess it's time to write some more code...

Couldn't the submarine's computer have done this? Why are all their systems convoluted puzzles? Why aren't we to the north pole yet? What just swam by me?

I better get this working, find the stars, and get out of here quickly...

## **Code Writeup**

Again for this challenge I've been given a text file with some information. This time it's "binary" and I've got to mess around with it a little bit.

Overall this wasn't too complicated. Part A was pretty easy, just a counter of the total was needed. I didn't even have to do any binary stuff because the `stdlib.h` has `strtol`.

Part B was a bit more challenging, mostly because I read it wrong the first time and thought that I had to find the one that matched the most / least significant bit of *all* the inputs, and not just the remaining ones. Having to do it for just the remaining ones meant I had to loop through and remove a bunch.

I solved both with an O(N) algorithm and I think my approach was solid. It's definitely not nearly as short as what some people have been putting out, though in my defense many are utilizing languages that have stuff like array slicing, or more built in functions that deal with strings, or even just more libraries...

At least that's what I tell myself so I don't feel too bad about how big and fat my code is.

I think I'm going to start a tradition of A: solving parts A and B together, and B: making a "cursed" file in which I remove all the comments and whitespace, and user input checking.

Now my code is *also* a 1 liner!
