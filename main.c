
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
- If the bird collides with a pipes the program stops

Pipes:
- The pipes of the 3 different types: 'up' , 'middle' , 'down'
- These pipes will appear into the frame at randomly and will come towards the bird
- if the pipes's type and the bird's position match the program will continue to run and the pipes will eventually move out of frame

The game needs to be in real-time, fgets() is not a good choice 


*/

# define HEIGHT 16
# define WIDTH 32
# define NoOfPipes 3 // the amount of pipes visible in an instances

// center of mass of components
typedef struct {
    int x;
    int y;

} coord;

// global variables
coord bird;
coord pipes[NoOfPipes];
char screen[HEIGHT][WIDTH];


void draw_border() {
    int i, j;

    // Top border
    for (i = 0; i < WIDTH; i++) {
        screen[0][i] = '#';
    }

    // Side borders and fill inside with spaces
    for (i = 1; i < HEIGHT - 1; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Left and right borders
            if (j == 0 || j == WIDTH - 1) {
                screen[i][j] = '#';
            } 
            // Inside area
            else {
                screen[i][j] = ' ';
            }
        }
    }

    // Bottom border
    for (i = 0; i < WIDTH; i++) {
        screen[HEIGHT - 1][i] = '#';
    }
}

void draw_bird(){
    //int i, j;
    screen[bird.y][bird.x] = '@';
    
}

void print_screen(){
    int y,x;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}

void draw_pipes(){

}

void draw(){
    // draw border in rectangle
    draw_border();
    
    //TODO: draw bird

    draw_bird();


    //TODO: draw pipes

    draw_pipes();
    
    
    print_screen();
}

int collision(){

    for (size_t i = 0; i < NoOfPipes; i++)
    {
        
        if (bird.x == pipes[i].x && bird.y != pipes[i].y){
            return 1;
        } else{
            return 0;
        }
    }
}

int quit(char move){
    if (move == 'q')
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
    
}

void main(){
    
    bird.x = 4;
    bird.y = 4;
    int i,j,frame = 0;
    char move;
    




    /* 
        A     B     OUT     (A||B) !(A||B)   !A && !B
       ==============================================
        0     0       1        0      1         1
        0     1       0        1      0         0
        1     0       0        1      0         0
        1     1      1/0       0      1         0



    */
    while (!collision() && !quit(move)) // keep running if no collision and no quit
    {
        draw();

        printf("\nEnter you move:  ");
        scanf("%c",&move);
        getchar();

        if (move == 'w')
        {
            bird.y -= 2; //  move bird up

            for (i = 0; i < NoOfPipes; i++)
            {
                pipes[i].x--; // move the 3 pipes closer
            }
            

        }
        


    }
}