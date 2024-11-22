
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

# define HEIGHT             16              //
# define WIDTH              64              //
# define NoOfPipes          3               // the amount of pipes visible in an instances
# define DIST               25              // distance between two pipes
# define APERTURE           2               // size of the aperture, if 0 the pipe has one character width '- -' if one '-   -' etc.

# define COLOR_RESET        "\33[0m"        // ANSI code for COLOR_RESET output
# define GREEN              "\x1b[102m"       // ANSI code for green output
# define YELLOW             "\33[33m"       // ANSI code for yellow output
# define HIDE_CURSOR        "ESC[?25l"      

//High intensity background 
# define BLKHB              "\e[0;100m"
# define REDHB              "\e[0;101m"
# define GRNHB              "\e[0;102m"
# define YELHB              "\e[0;103m"
# define BLUHB              "\e[0;104m"
# define MAGHB              "\e[0;105m"
# define CYNHB              "\e[0;106m"
# define WHTHB              "\e[0;107m"


# define Q                  0x51            // ANSI code for 'q' key
# define W                  0x57            // ANSI code for 'w' key

// center of mass of components
typedef struct {
    int x;
    int y;

} entity;

// global variables
entity bird;
entity pipes[3];
char screen[HEIGHT][WIDTH];

void hide_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Set the cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo);
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
            } else if (screen[y][x] == '#')
            {
                printf("%s%c",BLKHB,screen[y][x]);
            }
             else if (screen[y][x] == screen[y][x])
            {
                printf("%s%c",CYNHB,screen[y][x]);
            }
        }
        printf("\n");
    }
    printf("\33[16A"); // moves the cursor 16 lines up
    
}

void draw_pipes(){
    int i,j,k;
    // 3 pipes, using their coordinates, draw them, only draw them if they are inside the WIDTH
    for(i=0;i<NoOfPipes;i++){
        if (pipes[i].x< WIDTH){
            for (j = 1; j < HEIGHT-1; j++)
            {
                if  (
                        j < pipes[i].y - APERTURE ||
                        j > pipes[i].y + APERTURE //&& j != pipes[i].y 
                    )
                    {
                        screen[j][pipes[i].x] = '|';
                    }
                if (
                    j == pipes[i].y+1+APERTURE ||
                    j == pipes[i].y-1-APERTURE  
                )
                {
                    screen[j][pipes[i].x] = '-';
                    screen[j][pipes[i].x-1] = '-';
                    screen[j][pipes[i].x+1] = '-';
                }
            }
        }
    }
}

void draw_aperture(){
    for (size_t i = 0; i < NoOfPipes; i++)
    {
        screen[pipes[i].y][pipes[i].x] = 'x';
    }
    
}

void draw(){

    // draw border in rectangle
    draw_border();
    draw_bird();
    draw_pipes();
    draw_aperture();
    print_screen();

}

int collision(){
    int i;
    for (i = 0; i < NoOfPipes; i++)
    {
        // the coordinate of a pipe defines the pathway between two pipes, aka aperture, hence the player MUST match it's coordinate
        if (            
        // a more robust collision detection
        // the coordinates for the bird happen to contain pipes as well
        screen[bird.y][bird.x] == '|' || 
        screen[bird.y][bird.x] == '-'
        )
        {   
            return 1;
        }
        // collision with ground
        if (bird.y>HEIGHT)
        {
            return 1;
        }
         
        
    return 0;
    }
}

int quit(){
    return GetAsyncKeyState(Q) ? 1 : 0;
}

void main(){

    
    //hide_cursor();

    printf("%s",HIDE_CURSOR); 


    bird.x = 4;
    bird.y = 4;
    int i,j; 
    


    srand(time(NULL));
    //   initialise pipes
    //   assume these coordinates of pipes
    pipes[0].x = DIST;
    pipes[1].x = DIST + pipes[0].x;
    pipes[2].x = DIST + pipes[1].x;

    //the heights are meant to be random
    for (i = 0; i < NoOfPipes; i++)
    {
        pipes[i].y = (rand()%7) +5;     // random height between 5 and 11
    }




    /* 
        A     B     OUT     (A||B) !(A||B)   !A && !B
       ==============================================
        0     0       1        0      1         1
        0     1       0        1      0         0
        1     0       0        1      0         0
   //   1     1      1/0       0      1         0



    */

   /*
   events:
   1. bird descends 
   2. pipes approach
   3. pressing 'W' ascends the bird
   4. bird colliding with pipes OR with the ground ends the game
   5. colliding with ceiling negates the change in altitude


   */


    for (i = 0; i < HEIGHT; i++)
    {
        printf("\n");
    }


    printf("    )   ___                      ______              \n");
    printf("(__/_____) /)                (, /    ) ,       /) \n");
    printf("  /       // _  __  __         /---(     __  _(/  \n");
    printf(" /       (/_(_(_/_)_/_)_(_/_) / ____)_(_/ (_(_(_  \n");
    printf("(______)     .-/ .-/   .-/ (_/ (                  \n");
    printf("            (_/ (_/   (_/         \n\n\n\n\n\n\n\n\n");
    printf("Press 'W' to start or 'Q' to quit\n");
    
    draw();


    while (!(GetAsyncKeyState(W) & 0x8000)) {
        // Do nothing, just wait for the key press
    }
    

    time_t start = time(NULL);

    while (!collision() && !quit()) // keep running if no collision and no quit
    {
        draw();
        //                                          V``only move up if the height is greater than one
        if (GetAsyncKeyState(W)&& bird.y<HEIGHT && bird.y>1)
        {
            bird.y -= 2; //  move bird up
        }


        for (i = 0; i < NoOfPipes; i++)
        {
            pipes[i].x--; // move the 3 pipes closer
            
            if (pipes[i].x == 2) // to circle the pipe back to the start
            {
                pipes[i].x = WIDTH + DIST;

                // change the leaving pipe's height
                pipes[i].y = (rand()%7) +5;     // random height between 5 and 11
            }
            
        }

        bird.y++; //  move bird down by 1 unit, will happen regardless
        Sleep(80); // millisecond delay
    }

    time_t end = time(NULL);
    int score = end - start;

    printf("\33[16A\n\n\n %s%s SECONDS SURVIVED: %d\n",COLOR_RESET,YELLOW,score);

}