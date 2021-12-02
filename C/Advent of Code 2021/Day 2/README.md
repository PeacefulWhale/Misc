# Day 2 Challenge A & B

I awoke to the sound of ringing Christmas bells. Groaning I opened my eyes and saw nothing but the pitch black of midnight. Rolling to my side I reached for my phone and slammed my hand right into solid steel. Oh right... I'm still on this cursed cheery boat. I blinked the sleep from my eyes and tried to make out my surroundings. Last night I had gotten back to the SLEIGHT late and tired. I wasn't exhausted from writing the code to manuever the submarine, but rather from the sheer pressure of being in what appeared to be a a submarine made of gingerbread under thousands of feet of water. Regardless I got the two stars and the SLEIGHT-DIVE control room decided to pull me up for rest and refueling. And now I found myself in a room I have little recollection of entering. In the near pitch black darkness I could make out the glim glow of the door that led to one of the many halls that traversed this vessel. Groaning I stood up and reached for the door, only to find myself falling rapidly to the floor.

I had tripped over my backpack and other assorted luggage, that had apparently been moved to my new room as I had slept. As I staggered back upright thousands of Christmas lights strung across the ceiling of my room flashed on, as the cacophony of Christmas bells slowly faded. Evidently the crew had a morning wake up call that suited their nightmarish Christmas theme.

After getting ready and aimlessly wandering around the brightly painted hallways I found myself directed towards the mess hal by a surprisingly short elf. They still stood nearly half a foot taller than me, but compared to the seven and eight foot elves I was accustomed to they could almost pass for a normal person. If you ignored the pointed ears and cat like pupils of their vivid red eyes. Red eyes. Why hadn't I noticed that earlier? And why was the submarine they sent me down in yesterday too small for the average elf?

Regardless of the strange circumstances I found myself in, breakfast was refreshing and I was almost enjoying myself until I was hurried back into the submarine and sent hurtling along some sort of preprogrammed instructions towards the possible location of two other stars.

However it would have been too easy for the instructions to just work, wouldn't it have? At least some elf had the foresight to shove my trusty laptop into my arms before the submarine sank below the waves.

## **Code Writeup**

Another text file, and a very similar problem to the one we were given before. This time we have to keep track of 2 values that can be changed depending on what the input is. Each input is separated by non UTF-8 byte and only has a number that is 1 digit long. This means we can go through our buffer with preset index increments that depend on the length of the command. Each command starts with a different letter, so we just need to check 2 characters for each command, one to tell which command it is, and one for the actual digit it corresponds to.

Both A and B can be done in the same loop, with very minor changes made to the structure I'm using to keep track of our values, and the method I use to increment them.

I wrote an O(N) algorithm that solves both, so it's a bit better than my solution for yesterday.
