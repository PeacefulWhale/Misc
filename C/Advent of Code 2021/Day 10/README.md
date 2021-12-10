# Day 10 Challenge A & B

Another gosh darned day in a cave, exploring with no exit in sight.

I'm going mad... The frosting cans are looking extra good after another 3 meals of strange christmas themed MRIs...

Perhaps this stupid thing has a `auto-cave-escaper`.

Who could have guessed. The submarine that can magically generate obscure puzzles in the most dire of circumstances *does* have a solution to my current problem.

Only one problem...

`Syntax error in navigation subsystem on line: all of them`

Whoever programmed this just *had* to give this rust bucket sass didn't they?

## **Code Writeup**

This day was really simple, one of the easiest days yet tbh.

I created a "stack" and pushed stuff and popped stuff to get part A and part B.

I did implement a quick-sort for finding the median in part B, and by implement I mean calling `qsort` from `stdlib.h`. I was feeling extra lazy today, and I refuse to apologize for it!

One neat thing I did do was checking if the symbols matched by checking to see if they were within 3 integers of each other

```C
// Char     Int
// {        123
// }        125
// [        91
// ]        93
// <        60
// >        62
// (        40
// )        41
```

I wonder, what's between a lot of them? After having checked, it looks like `\|=`, neat.
