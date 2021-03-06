# Day 1 Challenge B

Gah, this pathetic bucket of bolts isn't accurate enough to find the second star which should be nearby, if my support team of elves high above me in *SANTA'S SLEIGHT* are correct in their assessment... They tell me that there is too much error in the depth sensor to be accurate, and that we'll possibly never find which crevice the star has fallen into at this rate. If only there was a way to look at many samples together, or perhaps in larger timestamps between samples...

I better delete some whitespace to save room, who knows how much memory this submarine has.

## **Code Writeup**

The challenge can be seen [here](https://adventofcode.com/2021/day/1#part2)

Alas, one key is not a saved Christmas... We must venture further into the depths! But here is just one problem, the radar isn't accurate enough to find the second key... If only there was a way to merge our samplings together!

This is essentially the same problem, but it's been changed around a little bit. We are given this to help understand it:

```text
199  A      
200  A B    
208  A B C  
210    B C D
200  E   C D
207  E F   D
240  E F G  
269    F G H
260      G H
263        H
```

The new goal is to count `the number of times the sum of measurements in this sliding window increases`.

So if I understand this correctly, we just have to implement this "sliding" window. I think the easiest way is going to be to create an integer array of all the values and slide through it. We might be able to get this to work as it collects the numbers, instead of having another loop at the end.

But to be honest, that would be a lot of effort, and O(2N) ≈ O(N)

So I just modified `day1A.c` to save all the numbers, and then go through at the end calculating the sum. It still runs instantly and I got `1235`, which earned me another star!
