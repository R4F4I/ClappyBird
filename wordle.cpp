
#include <iostream>
#include <stdlib.h>
#include <list>

# define COLOR_RESET        "\33[0m"            // ANSI code for COLOR_RESET output
# define CYAN               "\033[36m"
# define GREEN_BG           "\x1b[102m"         // ANSI code for green background output
# define YELLOW             "\33[33m"           // ANSI code for yellow output
# define GREEN              "\33[32m"           // ANSI code for green output
# define RED                "\33[31m"           // ANSI code for red output
# define HIDE_CURSOR        "ESC[?25l"      


using namespace std;        

list <string> listOfWords= {
"Apple",
"Brave",
"Chair",
"Dream",
"Eagle",
"Flock",
"Grape",
"House",
"Ideal",
"Joker",
"Knife",
"Light",
"Mouse",
"Night",
"Ocean",
"Paper",
"Queen",
"River",
"Snake",
"Table",
"Under",
"Vowel",
"Water",
"Yacht",
"Zebra"
};

void cursor_up(int X){
    printf("\033[%dA",X); // Move up X lines;
}

void cursor_left(int X){
    printf("\033[%dD",X); // Move left X lines;
}

// for orange hints
bool charInString(string src, char target){

    for (char& c: src)
    {
        c=toupper(c);
    }

    for (char c: src)
    {
        if (c==target) return true;
    }
    return false;
}

bool validWord(string src, list<string> list){
    
    for(const auto& element : list){
        string newElement = element;
        for (char& c: src)
        {
            c= toupper(c);
        }
        
        for (char& c: newElement)
        {
            c= toupper(c);
        }
        


        // cout<< newElement <<" " << src << endl;
        if (newElement == src)
        {
            return true;
        }
    }
    return false;
}

bool matchPosition(char c, char d){
 return c==d;
}

int main(){

    // choose a random word from list
    string secretWord = "apple";
    string guessedWords[5] = {"     ","     ","     ","     ","     "};
    string enteredWord;


    //////////////////COMPLETED//////////////////
    // convert root word to capital
    for (char& c: secretWord)
    {
        c = toupper(c);
    }
    //////////////////COMPLETED//////////////////


    //////////////////COMPLETED//////////////////
    // convert all guessed words in to CAPITAL
    for (size_t i = 0; i < 5; i++)
    {
        for (char& c: guessedWords[i])
        {
            c = toupper(c);
        }
    }
    //////////////////COMPLETED//////////////////

    
    

    //////////////////:: MAIN LOOP :://////////////////
    system("cls");

    // for loop to 5 / for 5 chances in guessing
    for (size_t i = 0; i < 5; i++) // 5 chances
    {
        cursor_up(8);
        cursor_left(11);
        
        // print the entered words
        for (size_t j = 0; j < 5; j++) // prints each guess word
        {
            int k = 0; // ued to comapare the 'word' and entered word char side by side
            for (char& c : guessedWords[j])
            {   

                c = toupper(c);


                if (matchPosition(c,secretWord[k++]))
                {
                    cout<< GREEN<< c << COLOR_RESET<< " ";
                }                
                else if (charInString(secretWord, c))
                {
                    
                    cout<< YELLOW<< c << COLOR_RESET<< " ";
                }
                else
                {
                    cout<< c << " ";
                }
                
                
            }
            cout<<endl;
        }
        cout<<endl<<endl;

        // prompt
        cout << "\r\033[2K>>>>>>";
        cin>>enteredWord;

        for (char& c: enteredWord)
        {
            c=toupper(c);
        }
        
        
        if (enteredWord == secretWord)
        {
            cout<<"CONGRATS!\n";
            break;
        }
        

        // check if entered word is valid
        if (validWord(enteredWord, listOfWords))
        {
            guessedWords[i] = enteredWord;
        }else
        {
            cout << "Not a word"<<endl;
        }
        
        
    }

    
    cursor_up(9);
    cursor_left(6);

    // print the entered words
    for (size_t j = 0; j < 5; j++) // prints each guess word
    {
        int k = 0; // ued to comapare the 'word' and entered word char side by side
        for (char& c : guessedWords[j])
        {   

            c = toupper(c);


            if (matchPosition(c,secretWord[k++]))
            {
                cout<< GREEN<< c << COLOR_RESET<< " ";
            }                
            else if (charInString(secretWord, c))
            {
                
                cout<< YELLOW<< c << COLOR_RESET<< " ";
            }
            else
            {
                cout<< c << " ";
            }
            
            
        }
        cout<<endl;
    }
    

    cout<<CYAN<< "---------"<<endl;
    
    for (char& c : secretWord)
    {   
        c = toupper(c);
        cout<< CYAN<< c << COLOR_RESET<< " ";
        
        
        
    }
    
}