# The System

Every level in this game is itself a plugin. Add a plugin to the game by adding it's .so/.dll to the file called packages. 
Plugins in the pakages list run out of order, and they are referenced by the board variable "name".

There are three names that get run-time priority as seen in game.cpp;
1. "test" Name your board "test" to get top run priority.
2. "start" if test is not found, then "start" is run at startup. 
3. "overworld" this is the default board if nothing else is provided.

Currently each board has a variable that triggers a swapping out of boards called "fin". If you set fin to true, then you can also set a request board using the "request" string. The game loop will then try to load your requested board, or will default to the overworld board.
The variable "fin" should always be set to false on a board "init()" function to make them replayable.

#BUILD AND INSTALL
Call the following to build and install:
  make all
  make install
