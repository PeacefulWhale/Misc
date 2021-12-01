# Day 1 Challenge A

The challenge can be seen [here](https://adventofcode.com/2021/day/1)

Gosh darn elves and their clumsy little legs. Why are the keys even in a place where they can go overboard? Why am I on this boat? Why do they have a submarine? How often has this happened? Elves and Santa are real? Dear lord, the implications of this are insane. I do hope I survive.

I must descend into the dark abyss and find the keys that have flown overboard! Armed with my trust submarine and C I shall save Christmas! I think... Not sure what the keys are for but I'm sure it's important.

Our submarine can return it's current height above the sea bed, but we must determine if we're getting deeper! Sadly it is far to dark down here to use light as a guide, and this submarine doesn't have any headlights, just a big magnet attached to the bottom. There are too many measurements to do this by hand, to GCC we go!

The first part of this challenge appears to be to keep track of whether or not we have increased or decreased a count. We are given a text file that contains many stuff, and our only goal is to give an integer value that represents `How many measurements are larger than the previous measurement?`

The actual format of the file in binary are utf-8 values separated by `0A`.

My solution to part 1 is an O(n) algorithm (if I remember my O notation stuff correctly). It is as follows:

1. Get the entire buffer.
2. Loop through the entire buffer.
    1. If the current character is a number, add it to a character array (numBuffer) in the right spot.
    2. If the current character is not a number:
        - Convert the numBuffer to a number and check if it is greater than the last number, if it is increment the count.
        - Reset the numBuffer and update the last number.
    3. If the current character is `\0` or we've reached the end of our buffer, quit.
3. Return the count.

I got `1195` from this and got the star. I'm not sure if everyone gets the same input, and I wouldn't be surprised if they don't.

Thankfully I have attained a key, and Christmas is saved! But wait, weren't there 50 that went overboard? This may be a problem, because it is getting quite cold in here.
