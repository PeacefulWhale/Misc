# Day 6 Challenge A & B

I finally gathered the courage to ask Artyom how many stars we had left. 40. I've only gotten 1/5 of the stars, and at this suspicious 2 stars a day rate, I've got at least 20 days left if you include today.

Dear lord, is Christmas really worth this?

At least today is nice and easy, chill and relaxed. No giant squids. No dangerous vents. I don't even have to map our route or plot our depth.

In fact it's almost boring down here without the constant worry of death.

Well I guess that's not quite true. There's still the constant worry of death, but it's not as pressing as it usually is. I'm just buried deep under the waves in a vehicle assembled by elves who forgot to make it big enough for one of themselves to sit in. I'm sure they didn't forget anything else. No pressure. Well, a lot of pressure... Roughly 2000 meters of water worth of pressure.

Hey, at least there are a lot of pretty fish to look at.

I wonder how many there are...

Huh, would you look at that. The sub has read my mind is has collected the age of the fish and their breeding cycles for me to compute. Wait. It can do that? I wonder what sort of sensors it's using to find the fish.

Wait a second.

What sensors is it using to read my mind?

## **Code Writeup**

This *was* easy until it required utilizing *very* large numbers. However thankfully some *very* kind hearted individual has developed [GMP](https://gmplib.org).

To solve this I utilized my `getNextNum` function that I used last time to get our input. From here it's a simple algorithm to calculate the Nth generation.

The algorithm is very simple. An array of `mpz_t` is created with 9 indices. I first thought of swapping the elements down one, but then I realized that we could just utilize an offset instead instead, and increment it. `+` and `%` are quicker than `mpz_set` I'd assume so at least.

Actually I'll test it. Utilizing `mpz_set` took on average 0.012 seconds to calculate 10,000 generations.

Utilizing my offset method it takes on average 0.012 seconds...

They're exactly the same?

What about at 1,000,000 generations? And let's run them a few times to make sure.

`mpz_set` took 11 seconds.

```C
// 1: Get a temp integer of the fish at 0
mpz_set(temp, fish[0]);
mpz_add(bigNum, bigNum, temp);
// 2: "Age" all the fish.
for (int i = 1; i <= 8; i++)
{
    mpz_set(fish[i - 1], fish[i]);
}
// 3: Add temp back to the fish.
mpz_add(fish[6], fish[6], temp);
mpz_set(fish[8], temp);
```

`offset` took 9 seconds.

```C
// 1: Get a temp integer of the fish at 0
mpz_set(temp, fish[offset % 9]);
mpz_add(bigNum, bigNum, temp);
// 2: "Age" all the fish.
offset++;
// 3: Add temp back to the fish.
mpz_add(fish[(offset + 6) % 9], fish[(offset + 6) % 9], temp);
mpz_set(fish[(offset + 8) % 9], temp);
```

So it looks like my offset method *is* quicker, by ~15%

This problem is remarkably similar to the fibonacci rabbit problem, which makes me think that an equation for this is very likely possible. However I am currently too lazy to try to figure it out, so I won't.

Either way my method is speedy (thank you `gcc -03` and `gmp -h`).

Here are some calculated numbers:

```text
1,000 generations:
// 24657915758996082910171105512555077825557
10,000 generations:
// We get a number with 381 digits.
1,000,000 generations:
// (1 minute later)
// It returns a number with 37838 digits... Neat!
```
