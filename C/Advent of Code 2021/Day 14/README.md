# Day 14 Challenge A & B

Crap, this thing is going to pop. Even with avoiding extra hot rooms it's getting *very* uncomfortable and the ASS (artificial stupid submarine) is complaining that the hull is going to break unless it can synthesize some sort of polymer to cover critical areas.

Sadly it appears that I'm going to die, there's no way that I can make anything like that when stuck at the bottom of the ocean. Maybe I'll have just a *little* frosting before I die...

Wait a second. This submarine *does* have an advanced chemical laboratory and a method for developing heat resistance polymers!

What *doesn't* this stupid thing have?

## **Coding Writeup**

Originally I did this as saving the entire string, but when I saw part B I realized that this wouldn't work.

I had saved the whole string because I was worried that the position of stuff would be important (IE something about finding where a certain pair occurs last). However when I saw that part B was just a *lot* of generations I realized that it would be better to instead save pairs and the total number of pairs that there are.

Implementing this was pretty easy to do, however I was stumped for a little while tying to figure out how to get the total characters in all the pairs to the actual number of characters in the string. I quickly realized that division by 2 and rounding up functioned for almost all of them. I realized that it did not function for ones in which the first and last character are equal. When that is the case it is off by 1. This was an easy fix to implement.

By creating a `unsigned long long data[26][26]`, I was able to keep track of all of the pairs. I utilized a `char map[26][26]` to map the insertions to. If `map[x][y]` is not `\0` than it maps to a character that needs to be inserted.

I think that my method works pretty well. It runs near instantly and handles very large sums and generations. It's at the speed where running it for 10, or 100 generations has no noticeable difference with the `time` command. Both stay around `0.009` seconds.
