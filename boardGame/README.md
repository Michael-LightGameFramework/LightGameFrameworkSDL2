# Plugins:
Check out the item class headers for the most complete API.

## Getting Started

List.txt contains the list of plugins you wish to include/launch in your program. 


Here are some of the key functions;

create() - called by main, should return a new instance of your item

constructor - Create an empty constructor so the item can be created.
void preloop() - load assets before the main game loop
void handleEvent(SDL_Event * event)  - check if event should change plugin's status
void update(int gameTick) - Adjust to time based input, and react to status changes
void draw() - Draw to item to screen
void cleanup() - prepare to exit to main menu, be ready for a re-launch, cleanup pointers.

destroy(item* yours) - also called by main, asks you to destroy your instance of your plugin item

## Examples

src/waves.cpp - Shows how to call an animation class from an item pointer that is in fact the child of an animation. (Check out the update function)

src/dual.cpp - Class names are now dissambiguated. Feel free to create any class name that you want. Dual.so has a wave class which loads perfectly fine beside the original wave plugin.

src/template.cpp - a quickstart file, copy it and use it as the base to a new plugin.

src/ext.cpp - example of sending an event to cause activity in the main function. (In this case a request to close the program.)


