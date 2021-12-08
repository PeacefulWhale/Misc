# Day 8 Challenge A & B

Crabs. Whale. Explosions.

That's about all I remember from yesterday...

With a splitting headache I found myself at the bottom of the ocean this morning, having apparently survived entering the cave system. The floating chunks of whale outside suggest that the whale did not.

The crabs are no where to be seen. I've lost contact with the SLEIGHT, and I've got no idea what I'm doing.

Thankfully this submarine has some emergency rations. Let's hope that I figure a way out of this cave soon though, otherwise it might be raw whale chunks for a while...

I see some seven-segment displays, but they appear scrambled. There might be a way to unscramble them though... Let's see.

## **Code Writeup**

In part A we are tasked with finding: `how many times do digits 1, 4, 7, or 8 appear?`

This is because 1, 4, 7, and 8 all have identifiable patterns.

**Unique Patterns:**

```text
Number | # of on segments
    1:   2
    4:   4
    7:   3
    8:   7
```

So if I had to guess the 10 inputs before the `|` tell us the patterns of the output digits, and the 4 numbers after `|` are digits. The pattern that they use to light up is not guaranteed to be the same I guess. However because 1, 4, 7 and 8 are the only digits with the specific numbers on, we know to just look for segments of that length.

Because I'm pretty sure that part B requires us to find out what all the digits are, I'm not going to write a lot of code for this, and instead I am going to utilize a quick and easy regex search to find the solution to part A.

To do this I did 2 regex stuff. I used `(?<=\|).*` to copy only the stuff after `|`. I could have also used the column select from VSCode but that would require scrolling a lot.

Then I used the regex command `(\s[a-z]{2}(?=\s))|(\s[a-z]{3}(?=\s))|(\s[a-z]{4}(?=\s))|(\s[a-z]{7}(?=\s))` to find all strings of the right length.

I found 301 matches, which was correct.

Thank you VSCode for regex support ;_;

For part B it appears that my initial thought that the stuff on the left is scrambled was correct, and I now must unscramble it.

```text
 0000
1    2
1    2
 3333
4    5
4    5
 6666
```

We are given a string of `abcdefg`. If it is one of the unique lengths (length is equal to 2, 3, 4, or 7) we know what the possibilities of `abcdefg` must map to.

When the lengths are equal to the following, their characters must map to:

```text
Num Length   Maps-To                     Key
0   6        0 | 1 | 2 | 4 | 5 | 6       243
1   2        2 | 5                       10
2   5        0 | 2 | 3 | 4 | 6           145
3   5        0 | 2 | 3 | 5 | 6           181
4   4        1 | 2 | 3 | 5               32
5   5        0 | 1 | 3 | 5 | 6           93
6   6        0 | 1 | 3 | 4 | 5 | 6       363
7   3        0 | 2 | 5                   11
8   7        0 | 1 | 2 | 3 | 4 | 5 | 6   723
9   6        0 | 1 | 2 | 3 | 5 | 6       183
```

We can also generate unique key for each number by following these rules:

- Multiply the numbers together (accept for 0).
- If it has 0 add 1.
- If it has 1 add 2.

From this we can find a few givens from our scrambled 10 digits:

- 0 will appear 8 times
- **1 will appear 6 times**
- 2 will appear 8 times
- 3 will appear 7 times
- **4 will appear 4 times**
- **5 will appear 9 times**
- 6 will appear 7 times

This means we will always be able to find out which characters map to 1, 4 and 5.

Because we know that which character is 5, we know which character is 2 because **number one** maps to 2 and 5 only.

With 1, 2, and 5 we know which maps to 3 because **number four** only maps to 1, 2, 3, and 5.

At this point we have 1, 2, 3, 4, and 5 mapped, which only leaves 0 and 6. And we know that 0 appears 1 more time than 6, so we can find these two as well.

Now we know where each character maps to, and we can find the number it represents. From here it's just simple addition and we find the solution to part B!

These numbers create a 4 digit number that we add to our solution.

After we get the numbers we have to repeat this for every line.

After implementing this (requires another loop) I was able to find that the solution for part B is `908067` (for my input at least).
