# Day 13 Challenge A & B

More good and bad news. The submarine is moving along fine, and I might get out of these caves within a week at this rate. I might also become a little bit more cooked than I would like to be. The water here is getting *pretty* toasty...

Thankfully I've found a way for checking how to avoid extra crispy caves. This thing is equipped with a thermal imaging system that should allow me to avoid caves that are hot enough to be dangerous. The only problem is that the imaging system isn't set up. I've managed to locate the manual for it, amongst many similar manuals buried deep in the cargo hold of this vessel, but it appears that I've got a *slight* leak because the manual is terribly soggy and in danger of disintegrating with too much movement.

I've managed to get the first page open, but going any further may damage the book beyond repair. Thankfully the code to activate the system is on the first page. And un-thankfully, it's a convoluted method to get the code, and involves far too much folding to attempt to put this soggy page through. It doesn't help that this page specifically is made out of an extra thing and slightly transparent paper that has become one with the pages behind it.

I may be able to do this another way. You know, by copying the page down onto a dry piece of paper and folding that. However this thing requires a lot of folds, and a lot of markings... I don't think trying to copy it to a normal piece of paper is going to work.

## **Code Writeup**

This day was pretty easy. However interestingly enough I ran into some segfault issues with my standard way of reading the buffer, and had to include a new `fileSize` variable to avoid this.

I created a 2 array of characters for my input, and a simple method for going through and folding the paper. I unknowingly solved part B before part A, because I went and folded it all the way and saw the 4 characters before I solved part A.

I think my method is decent, and I doubt there's any significantly more efficient methods to do it.

I wrote my method to run for both X and Y directions easily, and to convert between the "universal" folding coordinates I used in the method to the specific `x` and `y` coordinates I used in the grid itself.

**NOTE:** Part B requires partially-intelligent vision systems!

*It is suggested that the vision systems are able to read latin characters.*

Characters for partially-intelligent vision systems:

```text
#### #### #    ####   ##  ##  ###  #### 
#    #    #    #       # #  # #  # #    
###  ###  #    ###     # #    #  # ###  
#    #    #    #       # # ## ###  #    
#    #    #    #    #  # #  # # #  #    
#### #    #### #     ##   ### #  # #    
```
