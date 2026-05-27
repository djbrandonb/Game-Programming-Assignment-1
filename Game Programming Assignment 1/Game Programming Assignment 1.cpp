// Game Programming Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <allegro5/allegro.h>
using namespace std;

bool finished = false;
bool timeOut = false;

class logic {
public:
    //constructor
    logic();
    //Provides a basic introduction to the user
    void introduction();
    //Opens a file and reads in the list of words that can be scrambled
    //Returns: true if words inserted and false if not
    bool createLists();
    //Carries out the actual word scrambling
    //Returns: true if user wins and false if not
    bool playGame();
    //Takes in a word and scrambles it
    //Returns a scrambled version of the parameter
    string scrambler(string word);
    //Displays the endgame information
    void end();

private:
    int numCorrect,             //the number of correct words
        smallWordLength,       //length of word arrays
        mediumWordLength,
        largeWordLength;
    vector<string> smallWords;   //array of words of length 4-5
    vector<string> mediumWords;   //array of words of length 6-7
    vector<string> largeWords;   //array of words of length 8+   
};//end class logic

logic::logic() {
    numCorrect = 0;
    smallWordLength = 0;
    mediumWordLength = 0;
    largeWordLength = 0;
    smallWords = {};
    mediumWords = {};
    largeWords = {};
}

void logic::introduction() {
    cout << "Welcome to Scramble!" << endl << "You will be shown a scrambled word. You have 60 seconds to unscramble the word." << endl;
}

bool logic::createLists() {
    ifstream inFS;
    string word;
    bool wordsInserted = false;
    inFS.open("dictionary.txt", std::ios::in);
    if (!inFS.is_open()) {
        cerr << "Could not open file dictionary.txt" << endl;
        return 1;
    }

    while (!inFS.eof()) {
        inFS >> word;
        if (word.length() >= 4 && word.length() <= 5) {
            smallWords.push_back(word);
        }
        else if (word.length() >= 6 && word.length() <= 7) {
            mediumWords.push_back(word);
        }
        else if (word.length() >= 8) {
            largeWords.push_back(word);
        }
    }

    if (inFS.fail()) {
        cout << "Input failure before reaching end of file." << endl;
    }
    smallWordLength = smallWords.size();
    mediumWordLength = mediumWords.size();
    largeWordLength = largeWords.size();
    if (smallWordLength > 0 && mediumWordLength > 0 && largeWordLength > 0) {
        wordsInserted = true;
    }
    return wordsInserted;
}

bool logic::playGame() {
    
    bool win = false;

    string word;
    string guess;
    for (int i = 1; i <= 5; ++i) {
        cout << "GET READY FOR LEVEL " << i << endl;
        cout << "The word to guess is: ";
        if (i == 1 || i == 2) {
            word = smallWords.at(rand() % smallWordLength);
        }
        else if (i == 3 || i == 4) {
            word = mediumWords.at(rand() % mediumWordLength);
        }
        else if (i == 5) {
            word = largeWords.at(rand() % largeWordLength);
        }
        cout << scrambler(word) << endl;
        cin >> guess;
        if (guess == word) {
            cout << "Horray ";
            numCorrect++;
        }
        else {
            cout << "I am sorry ";
        }
        cout << "the word was " << word << endl;
    }

    if (finished) {
        win = true;
    }
    return win;
}

string logic::scrambler(string unscrambled_word) {
    string scrambled_word;
    string temp = unscrambled_word;
    while (temp.length() != 0) {
        int index = rand() % temp.length();
        scrambled_word += unscrambled_word.at(index);
        temp.erase(temp.begin()+index);
    }
    return scrambled_word;
}

void logic::end() {
    cout << "Thank you for playing Scramble." << endl << endl << "You managed to get " << numCorrect;
    cout << " out of 5 correct!" << endl;
}

void* input(ALLEGRO_THREAD* ptr, void* arg) {
    logic* l = (logic*)arg;
    finished = false;
    l->playGame();
    finished = true;
    return NULL;
}

void* timer(ALLEGRO_THREAD* ptr, void* arg) {
    time_t startTime, currentTime; //times used to measure elapsed time
    startTime = time(NULL);
    currentTime = time(NULL);
    while (currentTime - startTime < 60 && !finished) {
        currentTime = time(NULL);
    }
    timeOut = true;
}

int main()
{
    logic l;

    void* input(ALLEGRO_THREAD * ptr, void* arg);
    void* timer(ALLEGRO_THREAD * ptr, void* arg);

    ALLEGRO_THREAD* create1 = NULL, * create2 = NULL; //used for return value from thread creation

    create1 = al_create_thread(input, &l);
    create2 = al_create_thread(timer, NULL);

    l.introduction();
    l.createLists();
    while (!finished && !timeOut) {
       if (!finished && !timeOut) {
           al_start_thread(create1);
           al_start_thread(create2);
       }
       else
       {
           al_destroy_thread(create1);
           al_destroy_thread(create2);
       }

    }

    l.end();
}