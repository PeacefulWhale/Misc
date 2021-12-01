# Day 1 Challenge A

The challenge can be seen [here](https://adventofcode.com/2021/day/1)

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
