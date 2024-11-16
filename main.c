
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
# define WIDTH 64
# define NoOfPipes 3 // the amount of pipes visible in an instances
# define DIST 18     // distance btween two pipes

#define GREEN       "\33[32m"                        // ANSI code for green output
#define YELLOW      "\33[33m"                        // ANSI code for yellow output
#define WHITE       "\33[0m"                         // ANSI code for uncolored output

#define Q        0x51
#define W        0x57

// center of mass of components
typedef struct {
    int x;
    int y;

} entity;

// global variables
entity bird;
entity pipes[3];
char screen[HEIGHT][WIDTH];

void initializePipes(){
    //assume these coordinates of pipes
    pipes[0].x = DIST;
    pipes[1].x = DIST + pipes[0].x;
    pipes[2].x = DIST + pipes[1].x;

    //the heights are meant to be random
    pipes[0].y = (rand()%7) +5;     // random height between 5 and 11
    pipes[1].y = (rand()%7) +5;     // random height between 5 and 11
    pipes[2].y = (rand()%7) +5;     // random height between 5 and 11
}

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
            if (screen[y][x] == '@')
            {
                printf("%s%c", YELLOW,screen[y][x]);
            } else if (screen[y][x] == '|' || screen[y][x] == '-')
            {
                printf("%s%c", GREEN,screen[y][x]);
            } else
            {
                printf("%s%c",WHITE,screen[y][x]);
            }
        }
        printf("\n");
    }
    printf("\33[16A"); // moves the cursor 16 lines up
    
}

void draw_pipes(){
    int i,j;
    // 3 pipes, using their coordinates, draw them
    for(i=0;i<NoOfPipes;i++){
        for (j = 1; j < HEIGHT-1; j++)
        {
            if (j != pipes[i].y)
            {
                screen[j][pipes[i].x] = '|';
            }
        // TODO add '_' to the aperture
        }
        
    }
}

void draw(){
    // draw border in rectangle
    draw_border();
    draw_bird();
    draw_pipes();
    print_screen();
}

int collision(){
    int i;
    for (i = 0; i < NoOfPipes; i++)
    {
        // the coordinate of a pipe defines the pathway between two pipes, aka aperture, hence the player MUST match it's coordinate
        if (bird.x == pipes[i].x && bird.y != pipes[i].y){
            return 1;
        } else{
            return 0;
        }
    }
}

int quit(){
    if (GetAsyncKeyState(Q))
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
    
    initializePipes();

    srand(time(NULL));



    /* 
        A     B     OUT     (A||B) !(A||B)   !A && !B
       ==============================================
        0     0       1        0      1         1
        0     1       0        1      0         0
        1     0       0        1      0         0
        1     1      1/0       0      1         0



    */
    while (!collision() && !quit()) // keep running if no collision and no quit
    {
        draw();

        // the method of scanf for input is intrusive,                          // https://cboard.cprogramming.com/cplusplus-programming/112970-getasynckeystate-key-pressed.html
        //printf("\nEnter you move:  ");                                          // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
        //scanf("%c",&move);
        //getchar();

        if (GetAsyncKeyState(W))
        {
            bird.y -= 2; //  move bird up

            bird.y += 4; //  move bird down
            
        for (i = 0; i < NoOfPipes; i++)
        {
            pipes[i].x--; // move the 3 pipes closer
        }

        }
        Sleep(100);
    }
}