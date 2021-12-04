# Day 4 Challenge A & B

It is as I feared.

The movement in the shadows.

The brief flashes of something leviathan.

The unnerving sensation of being watched.

The back of my mind telling me that going under in a submarine covered in blinking LEDs is just asking everything around to try to eat you.

My had gone just like the others. Wake up. Take a shower. Eat. And get shoved into the submersible again and sent hurtling to the terrible depths. At least this time I had been given a space heater. Though in my current predicament it didn't do me much good.

"Stay calm, we have scenarios for this" a high pitched voice said from my earpiece. Don't let their voice fool you, I had met Artyom earlier today as he told me how we'd be running missions from now on. Though he sounded like tinkerbell, he stood 7 feet tall, had forearms that put Popeye's to shame, and a beard that rivaled any Mall Santa's. His hair was pitch black though. And he spoke with a thick russian accent. He also didn't have pointy ears, but he did wear the elf uniform like everyone else on the ship accept for me. I've caught glimpses of an elf following me, holding up a tape measurer and eyeing it against my silhouette. I've hidden a back up pair of clothes in case the rest of mine are mysteriously replaced with a bright green, red, and white jumpsuit.

"Ok, here's what you've got to do. *TWINKLES*" (The name of the submarine, I know it's a bit on the nose but I didn't name it) "is equipped with advanced countermeasures, follow my instructions to boot it up".

As I followed Artyom's instructions the giant squid that currently was attempting to crush my submarine squeezed tighter.

"Wait, did you say bingo?" I asked, stopping momentarily to take a better look at the screen in front of me. It now displayed many different games such as tic-tac-toe, bingo, and many others.

"Yes, hurry we have little time".

I hoped that these were code names for deadly weapons I could utilize against this aggressive piece of calamari. But I was half worried that these were actual games and that Artyom would shortly say something along the lines of "Good, now you will not die bored. Goodbye comrade". I don't think we know each other well enough to be "comrades", but one can hope can't they?

Shoving my fears aside I activated the BINGO program and squeezed my eyes shut, preparing for the worst.

The groans of stressed metal stopped, and I opened my eyes hesitantly.

The squid was in front of the submarine now, and a holographic bingo board was projected in front of it.

*Huh* I thought, *They should have named it SQUID, there's enough letters for it*

"You must keep it distracted until it grows bored and leaves" Artyom told me.

"I have given you the order in which the numbers will be called, try not to win too much".

"Comrade squid does not take kindly to loosing".

Why did the squid get a comrade, and I don't?

## **Code Writeup**

For challenge A and B I calculated the total "turns" until each board was "BINGO-ed". I utilized a linked list because I only need to go one way through all the variables and it was an easy and efficient way of allocating and keeping track of memory.

Originally I went through all boards every time I added a play for part A, and stopped when I found the winner. But for part B I went through each board until it was "BINGO-ed" and added a variable for keeping track of how many turns it took to solve that board.

This is (I think) an O(N) algorithm for solving parts A and B.

This one also runs instantly (less than 0.01 seconds) like the others, so I'll take that as a good sign that I'm not doing this terribly.
