
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/*
Functionalities:

Controls / buttons:
- press q button to close the game
- press up button to move the 'bird' 
- (after it moves up the bird will start descending at a constant rate)

environment:
- The bird will exist in a 'frame'
- This frame will have set height / width
- This frame will update at a constant rate

components:
- Inside the frame there will exist a bird and 'pipes'
- There will be 1 type of bird and 3 types of pipes

Bird:
- The bird will either move up or down
- The user will make the bird go up, the bird will automatically go down
- If the bird collides with a pipe the program stops

Pipes:
- The pipes of the 3 different types: 'up' , 'middle' , 'down'
- These pipes will appear into the frame at randomly and will come towards the bird
- if the pipe's type and the bird's position match the program will continue to run and the pipe will eventually move out of frame


*/


void main(){

    // Windows-specific command to change console window title
    system("title \"My Program\"");

    // VK_UP is a virtual key code for the up button
    if (GetAsyncKeyState(VK_UP)){
        printf("Up button");
    }
    
}