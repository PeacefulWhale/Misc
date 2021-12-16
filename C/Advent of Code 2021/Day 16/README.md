# Day 16 Challenge A & B

I'm free! The cave has no control over me!

Accept I'm still well below the waves, surrounded by darkness, and running dangerously low on edible rations. I'd rather pop open the submarine's windows than a can of Christmas themed frosting.

Hold on, I'm receiving some strange sort of message from the *SLEIGHT*. This is a rather weird method of transmitting data. Oh well, I guess I better decrypt it before I do anything else.

## **Code Writeup**

To parse the input (which is part A of today) we have to follow the rules below:

- First 3 Bits: Packet Version
- Second 3 Bits: Packet ID
  0. Sum.
  1. Product.
  2. Minimum.
  3. Maximum.
  4. Literal Value.
  5. Greater Than.
  6. Less Than.
  7. Equal To.
- Final Bits: Data
  - ID != 4:
    - 6th bit is 0 or 1:
      - `0`: The next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet.
      - `1`: The next 11 bits are a number that represents the number of sub-packets immediately contained by this packet.
  - ID = 4:
    - Number divided into groups of 5 bits.
      - First = Continue bit.
        - 1: More segments to come.
        - 0: Last segment.
      - 2-5 = Part of number.

Being lazy, I just converted our input into a binary string and treated them like characters instead of actual binary. I could have read it as actual hexadecimal but I really didn't feel like dealing with a lot of bit stuff, or writing functions to convert integers to bit arrays and vice versa.

My method functions by taking the binary string and recursively calling itself on every substring it encounters. As it traverses it keeps track of the version numbers (saved to a global variable called `partA`) and the actual sum of the packets (done through the return value of the recursive function).

I *think* it's O(N), and it runs instantly, and doesn't use that much space. Overall I think that it's a neat little function that works well.

I did have to spend about ~15 minutes trying to figure out why I was having some integer overflows. It turns that I had used `1` instead of `1ULL` in my bit-shift, which was overflowing.
