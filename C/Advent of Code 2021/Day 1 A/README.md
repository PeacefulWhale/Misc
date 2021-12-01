# Day 1 Challenge A

Man, this is not how I imagined my Christmas break going... It had started out normal, fleeing another country to evade INTERPOL and seeking refuge on a covert vessel heading somewhere where no one could find me. I wanted a small, unobtrusive vessel that had a legitimate reason for going where it was going, and one who's crew knew not to ask a lot of questions. At least, that's what I told my intermediary what I wanted. Where I ended up with was not what I was expecting.

A Christmas themed, larger international cargo liner strung from the keel to the bow with blinding LED Christmas lights, and crewed by a multitude of tall, pointy eared individuals clad in red, green, and white felt clothes. By some cursed twist of fate I had found myself onboard *SANTA'S SLEIGHT* manned by nightmarish hybrids of tolkien elves and stereotypical Christmas elfs. The freighter itself was noticeably a custom job, with strange cuts in the side of the hull, and with "SL" sloppily painted over the "FR" in the original "FREIGHT" on the side of the ship. Not to mention the use of Christmas lights instead chains or ropes to tie everything down.

This in and of itself wouldn't have been that big of a problem, after all it met all of my requirements. It was heading to the north pole, a place rarely visited by *anyone*, and with such an expansive cargo even if the ship was searched odds are I'd be fine hiding among the coal. On second thought, perhaps the stuffed bears would be a better place to conceal myself, I'm still not sure if the coal is fed to the engines or not.

Even the journey itself was going smoothly, as we sailed right past patrolling military vessels and into international waters I let myself relax. Until I found myself shook from my stupor by a blaring alarm.

"She's not going to hold!" A deep voice bellowed as I turned my head towards the commotion. Elves scurried across the deck as a shipping container threatened to fall from it's position stacked high.

"ALL LEDs to red!" a much higher voice came from a much taller elf as they grabbed hold of a snapped LED line that resembled an angry sugar cane snake (don't ask, I still have trauma).

I'm not sure if this was to alert crew members who had gone deaf from the siren that was currently assaulting my senses, or if the red lights were stronger, but regardless the increased state of alarm and potentially stronger light lines were unable to stop the catastrophe. As I watched in horror, the bright gold shipping container snapped the remaining strings of lights and fell, no, *floated* into the turbulent ocean beneath us.

"By sugarplums! There go the keys!" screamed someone behind me. As I turned to glance at the elf behind me they finished their proclamation with "And the stars too!"

Not sure what to say, or what to do, I quickly offered my condolences and my sincere apologizes that I was unable to help. And that is how I found myself two thousand meters below the surface, armed with a submarine that all the elves were too tall to sit in, and my trusty laptop. Do not ask my why the "stars" required to make the key finding antenna work were *with the keys*, or why a submarine to search for fallen keys even existed in the first place, or why keys were even needed when I saw *several* cutting instruments that could make quick work of anything that needed to be unlocked lying around the inner cargo hold.

At the time I didn't have time to worry about that though, with the whole "rapidly sinking" thing going on. The fact that my submarine was covered in blinking LEDs did *not* help my nerves either, as I had just watched a documentary about giant squids, and I was pretty sure that the blinking red LEDs would make for a very effective squid lure.

Regardless, we've got to do what we've got to do, and I've got to find these stars so I can find the keys so the ship can continue to the north pole and I can make my escape. Who cares about saving Christmas, I've got to save myself!

## **Code Writeup**

The challenge can be seen [here](https://adventofcode.com/2021/day/1)

The first part of this challenge appears to be to keep track of whether or not we have increased or decreased a count. We are given a text file that contains many stuff, and our only goal is to give an integer value that represents `How many measurements are larger than the previous measurement?`

The actual format of the file in binary are utf-8 values separated by `0A`.

My solution to part 1 is an O(n) algorithm (if I remember my O notation stuff correctly). It is as follows:

1. Get the entire buffer.
2. Loop through the entire buffer.
    1. If the current character is a number, add it to a character array (numBuffer) in the right spot.
    2. If the current character is not a number:
        - Convert the numBuffer to a number and check if it is greater than the last number, if it is increment the count.
        - Reset the numBuffer and update the last number.
    3. If the current character is `\0` or we've reached the end of our buffer, quit.
3. Return the count.

I got `1195` from this and got the star. I'm not sure if everyone gets the same input, and I wouldn't be surprised if they don't.
