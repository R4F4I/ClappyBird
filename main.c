/*
Functionalities:
- Main
    - fetch scores from scores file
    - show menu
        - Play Game
            * View Score
            - during game play show the user's scores

            * Controls / buttons:
            - press q button to close the game
            - press up button to move the 'bird' 
            - (after it moves up the bird will start descending at a constant rate)

            * environment:
            - The bird will exist in a 'frame'
            - This frame will have set height / width
            - This frame will update at a constant rate

            * components:
            - Inside the frame there will exist a bird and 'pipes'
            - There will be 1 type of bird and 3 types of pipes

            * Bird:
            - The bird will either move up or down
            - The user will make the bird go up, the bird will automatically go down
            - If the bird collides with a pipes the program stops

            * Pipes:
            - The pipes of the 3 different types: 'up' , 'middle' , 'down'
            - These pipes will appear into the frame at randomly and will come towards the bird
            - if the pipes's type and the bird's position match the program will continue to run and the pipes will eventually move out of frame

        - Show scores
            - display scores
            - highlight section if it matches the user's name
        - quit
    - save scores to scores file
    - credits
        - print credits
    - quit
        - exit program




*/

#include <windows.h>                            // for windows API
#include <string.h>                             // for string functions
#include <stdio.h>                              // for input/output functions
#include <time.h>                               // for time functions mostly scores


# define HEIGHT             16                  // screen height
# define WIDTH              64                  // screen width
# define NoOfPipes          3                   // the amount of pipes visible in an instances
# define DIST               25                  // distance between two pipes
# define APERTURE           2                   // size of the aperture, if 0 the pipe has one character width '- -' if one '-   -' etc.
# define MAX_SCORES         10                  // max amount of scores to be saved
# define FILENAME           "scores_list.txt"   // file name to save the scores

# define COLOR_RESET        "\33[0m"            // ANSI code for COLOR_RESET output
# define GREEN_BG           "\x1b[102m"         // ANSI code for green background output
# define YELLOW             "\33[33m"           // ANSI code for yellow output
# define GREEN              "\33[32m"           // ANSI code for green output
# define RED                "\33[31m"           // ANSI code for red output
# define HIDE_CURSOR        "ESC[?25l"      

//High intensity background colors
# define BLKHB              "\e[0;100m"
# define REDHB              "\e[0;101m"
# define GRNHB              "\e[0;102m"
# define YELHB              "\e[0;103m"
# define BLUHB              "\e[0;104m"
# define MAGHB              "\e[0;105m"
# define CYNHB              "\e[0;106m"
# define WHTHB              "\e[0;107m"

// all are lowercase inputs

# define B                  0x42            // ANSI code for 'B' key
# define O                  0x4F            // ANSI code for 'O' key
# define Q                  0x51            // ANSI code for 'Q' key
# define W                  0x57            // ANSI code for 'W' key

// center of mass of components
typedef struct {
    int x;
    int y;

} entity;

typedef struct {
    char name[3];
    int score;
} list;

list scores_list[MAX_SCORES];

// global variables
entity bird;                                // handles bird's position
entity pipes[3];                            // handles the 3 pipe's position
char screen[HEIGHT][WIDTH];                 // handles the game screen
char player_name[3];                        // handles the player's name
time_t game_start;                          // handles the game start time

void hide_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Set the cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}


// ?playing game functions

void update_border() {
    int i, j;
    // create the border OUTSIDE of the screen
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

void update_bird(){
    screen[bird.y][bird.x] = '@';
}

void print_screen(){
    int y,x;
    for (y = 0; y < HEIGHT; y++) {
        // print the vertical sides
        printf("%s|",COLOR_RESET);
        for (x = 1; x < WIDTH-1; x++) {
            if (screen[y][x] == '@')
            {
                printf("%c",screen[y][x]);
            } else if (screen[y][x] == '|' || screen[y][x] == '-') 
            {
                printf("%s%c", GREEN_BG,screen[y][x]);
            } 
            else if (screen[y][x] == '#')
            {
                printf("%s%c",COLOR_RESET,screen[y][x]);
            } 
           
             else if (screen[y][x] == screen[y][x])
            {
                printf("%s%c",CYNHB,screen[y][x]);
            }
        }
        // print the vertical sides
        printf("%s|",COLOR_RESET);
        printf("\n");
    }
    printf("\33[18A"); // moves the cursor 18 lines up 2 more lines for the score
    
}

void update_pipes(){
    int i,j,k;
    // 3 pipes, using their coordinates, draw them, only draw them if they are inside the WIDTH
    for(i=0;i<NoOfPipes;i++){
        if (pipes[i].x< WIDTH){
            for (j = 1; j < HEIGHT-1; j++)
            {
                if  (
                        j < pipes[i].y - APERTURE ||
                        j > pipes[i].y + APERTURE
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

void update_aperture(){
    for (int i = 0; i < NoOfPipes; i++){
        if (pipes[i].x < WIDTH){
            screen[pipes[i].y][pipes[i].x] = 'x';
        }
    }
}

void update_score(){
    time_t current_time = time(NULL);
    int curr_score=current_time - game_start;
    printf("%sSCORE: %d%s               \n\n",GREEN,curr_score,COLOR_RESET);
}

void draw(){

    // draw border in rectangle
    update_border();
    update_bird();
    update_pipes();
    // update_aperture();
    update_score();
    print_screen();

}

// ?/playing game functions

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

// ? get async wrapper functions

int pressed_W(){
    return GetAsyncKeyState(W) ? 1 : 0;
}

int pressed_Q(){
    return GetAsyncKeyState(Q) ? 1 : 0;
}

int pressed_O(){
    return GetAsyncKeyState(O) ? 1 : 0;
}

int pressed_B(){
    return GetAsyncKeyState(B) ? 1 : 0;
}

int pressed_enter(){
    return GetAsyncKeyState(VK_RETURN) ? 1 : 0;
}

// ? /get async wrapper functions

// ? show functions

void show_title(){
    printf("%s    )   ___                      ______              \n",YELLOW);
      printf("(__/_____) /)                (, /    ) ,       /) \n");
      printf("  /       // _  __  __         /---(     __  _(/  \n");
      printf(" /       (/_(_(_/_)_/_)_(_/_) / ____)_(_/ (_(_(_  \n");
      printf("(______)     .-/ .-/   .-/ (_/ (                  \n");
      printf("            (_/ (_/   (_/         %s\n\n\n\n\n\n\n\n\n",COLOR_RESET);
}

void show_game_instructions(){
    printf("Press %s'W'%s to start or %s'Q'%s to quit\n\n\n\n\n",RED,COLOR_RESET,GREEN,COLOR_RESET);
}

int show_menu(){
    int choice=0;

    printf("(use UP & DOWN  arrow keys to navigate)\n");
    printf("Choose (by pressing 'o') either of the two below\n\n\n");
    while(!pressed_O()){

        switch (choice)
        {
        case 0:
            printf("%sPLAY GAME  <%s\n",RED,COLOR_RESET);
            printf("SEE SCORES          %s\n",COLOR_RESET); // added whitespaces to clear artifacts after refresh
            printf("%sQUIT                \n",COLOR_RESET);
            break;
        case 1:
            printf("PLAY GAME           %s\n",COLOR_RESET); // added whitespaces to clear artifacts after refresh
            printf("%sSEE SCORES <%s\n",RED,COLOR_RESET);
            printf("%sQUIT                \n",COLOR_RESET);
            break;
        case 2:
            printf("PLAY GAME           %s\n",COLOR_RESET); // added whitespaces to clear artifacts after refresh
            printf("SEE SCORES          %s\n",COLOR_RESET);
            printf("%sQUIT       <%s\n",RED,COLOR_RESET);
            break;
        default:
            break;
        }


        // virtual keycode for up arrow
        if(GetAsyncKeyState(VK_UP)){
            choice--;
            // circle back to bottom
            if (choice<0)
            {
                choice = 2;
            }
            Sleep(100);
            
        }
        // virtual keycode for down arrow
        if(GetAsyncKeyState(VK_DOWN)){
            choice++;
            // circle back to top
            if (choice>2)
            {
                choice = 0;
            }
            Sleep(100);
            
        }
        
        printf("\33[3A"); // moves the cursor 2 lines up
        Sleep(50);
    }
    return choice;
}

// ? /show functions

void clear_screen(){
    system("cls");
}

int play_game(){
    // printf("%s",HIDE_CURSOR); 

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

    draw();
    //printf("%s",COLOR_RESET);

    while (!(pressed_W()) && !pressed_Q()) {
        // Do nothing, just wait for the key press
    }
    

    game_start = time(NULL);

    while (!collision() && !pressed_Q()) // keep running if no collision and no quit
    {
        draw();
        //                                          V``only move up if the height is greater than one
        if (pressed_W()&& bird.y<HEIGHT && bird.y>1)
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
    int score = end - game_start;
    clear_screen();
    //printf("\33[16A\n\n\n %s%s SECONDS SURVIVED: %d\n",COLOR_RESET,YELLOW,score);
    return score;
}

void print_credits(){
    printf("----------------------\n");
    printf("Thank You For Playing!\n");
    printf("----------------------\n\n\n");
    printf("Credits:\n");
    printf("Rafay Siddiqui\n");
    printf("Masoom Khan\n");
    printf("Dev Kumar\n");
}

// ? score handling functions

void fetch_scores(){
    FILE *fptr;
    fptr = fopen(FILENAME, "r");
    int i;

    if (fptr == NULL){
        printf("Could not open file %s\n", FILENAME);
    }

    for (i = 0; i < MAX_SCORES; i++)
    {
        fscanf(fptr,"%[^,],%d\n",scores_list[i].name,&scores_list[i].score);
    }
    fclose(fptr);
}

void show_scores(){
    int i;
    printf("--------------------\n");
    printf("  NAME   |   SCORE  \n");
    printf("--------------------\n");
    for (i = 0; i < MAX_SCORES; i++)
    {
        if(strcmp(scores_list[i].name,player_name)==0){
            printf("%s  %s    |     %d  %s< YOU\n",GRNHB,scores_list[i].name,scores_list[i].score,COLOR_RESET);
        } else{
            printf("  %s    |     %d\n",scores_list[i].name,scores_list[i].score);
        }
        
    }

    printf("\nPress B to go back\n");
    while (!pressed_B()){
        Sleep(100);
    }
}

// inserts the value of the score into the scores_list structure
void insert_score(int score,char player_name[]){
    int i,j;
    for (i = 0; i < MAX_SCORES; i++)
    {
        if (scores_list[i].score<score)
        {
            // make space for the new score
            for (j = MAX_SCORES-1; j > i; j--)
            {
                strcpy(scores_list[j].name,scores_list[j-1].name);
                scores_list[j].score = scores_list[j-1].score;
            }
            // insert the score
            scores_list[i].score=score;
            strcpy(scores_list[i].name,player_name);
            return; // after inserting the player name, exit
        }
        
    }
}

void write_to_file(){
    FILE *fptr;
    fptr = fopen(FILENAME, "w");
    int i;


    if (fptr == NULL){
        printf("Could not open file %s\n", FILENAME);
    }

    for (i = 0; i < MAX_SCORES; i++)
    {
        fprintf(fptr,"%s,%d\n",scores_list[i].name,scores_list[i].score);
    }
    fclose(fptr);
}

// ? /score handling functions

void main(){
    int score;
    int option=-1;
    hide_cursor();

    // get scores from the file
    fetch_scores();

    // keeping loop between options until the user chooses to exit
    // option 2 will exit the program
    while(option!=2){
        clear_screen();
        show_title();
        option = show_menu();
        // option 0 is to play game
        if (option == 0)
        {
            // initialise the game
            clear_screen();
            show_title();
            printf("Enter your name (of 3 char) to start the game: ");
            scanf("%s",player_name);
            show_game_instructions();
            score = play_game();
            // run this subroutine regardless of the score, it will also check if the score is high enough to be stored in the scores_list structure
            insert_score(score,player_name);

        } 
        // option 1 is to show scores
        else if (option == 1)
        {
            clear_screen();
            show_title();
            show_scores();
        } 
    }

    // exit the program
    clear_screen();
    show_title();
    print_credits();
    // write the scores to file before exiting
    write_to_file();

}