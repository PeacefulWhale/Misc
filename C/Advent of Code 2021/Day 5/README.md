# Day 5 Challenge A & B

Do I even need to summarize my day up to this point? You know where I am. Stranded deep in a blinked submarine searching for strange stars to power an antenna to find some keys to save Christmas or something. At this point I'm not really sure anymore.

What I do know is that I thought that almost getting crushed by a giant squid would be the worst of my problems.

That might still be true. I'm not sure if having to navigate a maze of explosive geothermal vents is better or worse...

Artyom has told me that they appear in lines, and that I only really have to be careful of avoiding spots in which they overlap. So going over the lines where they don't overlap is fine? Even though they are *explosive* geothermal vents? That spew water hot enough to melt *lead*?

Good news is it isn't so cold in the sub any more.

Wait, that actually might be bad news... Let's find these stars before I end up cooked alive in this stupid cheery Christmas can.

## **Code Writeup**

I found today actually easier than day 4. I just made a method to parse through our input and save it to the board. I did make a new function called `getNextNum` which I might be using more in the future. We'll see I guess.

This was an O(N) algorithm that didn't have to malloc any space, so I believe it's more efficient than my day 4 solution. I did break part A when I made it work with straight lines, and I realized that there is no good way of making part A and part B work at the same time without looping through the input twice, or saving the diagonal lines for last (increasing the required memory). So I just accepted that part A was dead to me.

Part B can actually solve part A if you just remove the diagonal lines from the input, so I think that's good enough, I didn't have to write any special handling for the diagonal files, it just works with my logic to add all the lines.
