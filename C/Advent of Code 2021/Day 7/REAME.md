# Day 7 Challenge A & B

Explosive vents. Giant squids. Life support running low. And now there's a whale threatening to munch through my submarine (oh the irony of my github name).

I thought I was doomed, but crabs are here to save me! Surface dwelling *crabs* sent by SANTA'S SLEIGHT in miniature submarines? What!? With no weapons? And they can only move horizontally? And there's a cave system they want to break into? Instead of using their "cave breaking power" to break the whale chasing me? Where did these crabs come from? Why am I looking for the stars instead of these apparently intelligent crabs?

What is happening?

## **Code Writeup**

So my first thought was to pure brute force this by creating an array that fit the range of the numbers and then calculating the fuel cost to reach this step. However this not optimal as it will always take O(N^2)...

I know that there exist many sorting methods that can sort an array quicker than this, such as quick and merge sorting. If I were to implement a method like this, with additional checking to see if the current value is a median then I would be able to find the median value quicker!

However I decided I wanted to see what part B was before I wrote out this algorithm. So I plugged my input into an online calculator and found the median, and wrote a one line python script to output the fuel cost so I could get part B, and then write an algorithm to solve both (hopefully).

Part B is where things get interesting... We are still finding a median, accept now the fuel cost is squared so the most efficient point is probably *not* the median. A single outlier can *drastically* change the optimal solution. We could just calculate all possible points from MIN to MAX... But this feels terribly inefficient. We know that a better solution exists for part A, and I find it difficult to believe that what we utilized in part A cannot be utilized for part B.

We could try to utilize the quicksort method, but maybe change the method of flipping so it utilizes the "stretchy" distance ^ 2 method? I original thought that utilizing a method to minimize a loss function might work, but the issue is once we calculate the loss we know where exactly to move...

To be honest my brain is a bit smooth right now, and I've got a lot of homework to do. So I'll just write a brute force method ;_;

Please forgive me Gods of coding, for I have failed you.

Even though it runs near instantly (0.01s for both) I know that this is not nearly the most efficient method. I may come back to this later and implement a faster approach (for part A at least, as I know that a modified quick or merge sort is faster than this for finding the linear median)
