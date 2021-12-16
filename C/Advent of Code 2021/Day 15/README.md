# Day 15 Challenge A & B

Dear lord. There's not enough "cold" in the sub to deal with this heat.

Unless I get out of here quickly I'm going to turn into a steamed me. I do not like being steamed.

If *only* there was a way to determine the most efficient way to leave a system!

I'm kidding.

This stupid submarine refuses to move unless we plot even *more* optimal paths for it to take... We're fine. We could go right through the remaining caves. But the submarine insists that it must not waste fuel and that certain ares require it to run the cooling systems harder.

If you're wondering why I haven't noticed the cooling systems it's because this stupid thing has kept all of the cooling power directed at wherever it's processing unit is.

If I find it I'm going to steal all of it's sweet cold air for myself.

## **Code Writeup**

I was *very* lazy today.

This is just a dijkstra algorithm that prefers moving towards the bottom right corner.

I started with an A\* algorithm first and while that worked (quicker than dijkstra too) it wasn't producing optimal results unless I modified the heuristic. However at the point where it *was* producing optimal results it had essentially become a dijkstra algorithm but less efficient.

I decided to scratch it and just use a standard dijkstra algorithm.

This took about 2 minutes to run and it is *not* efficient.

I *could* add a sorted queue to make it so accessing the next element is much quicker, but as I said, I'm terribly lazy right now.
