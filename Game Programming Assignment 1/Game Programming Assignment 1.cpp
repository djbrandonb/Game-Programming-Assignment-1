// Game Programming Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class logic {
public:
    logic();
    //constructor
    void introduction();
    //Provides a basic introduction to the user
    bool createLists();
    //Opens a file and reads in the list of words that can be scrambled
    //Returns: true if words inserted and false if not
    bool playGame();
    //Carries out the actual word scrambling
    //Returns: true if user wins and false if not
    string scrambler(string word);
    //Takes in a word and scrambles it
    //Returns a scrambled version of the parameter
    void end();
    //Displays the endgame information
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
    cout << "Welcome to Scramble!" << endl << "You will be shown a scrambled word. You have 60 seconds to unscramble the word.";
}

bool logic::createLists() {
    ifstream inFS;
    string word;
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
}

bool logic::playGame() {
    string word;
    string guess;
    for (int i = 1; i <= 5; ++i) {
        cout << "GET READY FOR LEVEL " << i << endl;
        if (i >= 1 && i <= 2) {
            word = smallWords.at(rand() % smallWordLength);
        }
        else if (i >= 3 && i <= 4) {
            word = mediumWords.at(rand() % mediumWordLength);
        }
        else if (i == 5) {
            word = largeWords.at(rand() % largeWordLength);
        }
        cout << "The word to guess is: " << scrambler(word) << endl;
        cout << "Give it a guess: ";
        cin >> guess;
        if (guess == word) {
            cout << "Hooray the word was " << word << endl;
            numCorrect++;
        }
        else {
            cout << "Sorry, the word is " << word << endl;
        }
    }
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
    cout << " out of 5 correct!" << endl << "Press any key to continue . . .";
}

int main()
{
    logic l;

    l.introduction();
    l.createLists();
    l.playGame();
    l.end();
}