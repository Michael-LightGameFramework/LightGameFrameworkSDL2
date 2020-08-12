# Populate a field of grass Framework Test

I recently got hold of an excellent book "Game Programming Patterns" by Robert Nystrom (Also available free online)

In this demo I take his explanation of a flyweight and apply it to a field of grass. We remove our framerate limiter, and add 10% more grass per frame. When you hit a point at which the game appears un-playable hit exit.

A report of how many blades of grass were on display before exit will be displayed. 



## Versions:
Base - This program is meant to crash before anything is displayed. Use it to determine how many basic items your OS Memory Manager will allow to be created. 

FlyWeight - Utilize resource sharing in order to save memory and then test how many items can/should be displayed before the game is considered unplayable. Set "fps" in flyweight.cpp to change your desired frames per second. 
This will give a general idea of how many items you can place on a level before a massive overhaul must be considered.


