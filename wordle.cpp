
#include <iostream>
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

// for orange hints
bool charInString(string src, char target){
    for (char c: src)
    {
        if (c==target) return true;
    }
    return false;
}

bool validWord(string src, list<string> list){
    for(const auto& element : list){
        if (element == src)
        {
            return true;
        }
    }
    return false;
}

int main(){

    // choose a random word from list
    string word = "hello";
    string guessedWords[5] = {"abcde","abcde","","abcde","edcba"};
    string enteredWord;


    //////////////////COMPLETED//////////////////
    // convert root word to capital
    for (char& c: word)
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

    // for loop to 5 / for 5 chances in guessing
    for (size_t i = 0; i < 5; i++) // 5 chances
    {
        
        // print the entered words
        for (size_t j = 0; j < 5; j++) // prints each guess word
        {
            for (char& c : guessedWords[j])
            {   
                if (charInString(enteredWord, c))
                {
                    
                    cout<< YELLOW<< c << " " << COLOR_RESET;
                }else
                {
                    cout<< c << " ";
                }
                
                
            }
            cout<<endl;
        }
        cout<<endl<<endl;

        // prompt
        cout << ">";
        cin>>enteredWord;
        
        // check if entered word is valid
        if (validWord(enteredWord, listOfWords))
        {
            guessedWords[i] = enteredWord;
        }else
        {
            cout << "Not a word"<<endl;
        }
        
        
    }
    
}
