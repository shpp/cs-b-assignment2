/* File WordLadder.cpp
 * -------------------------------------------------
 *
 * This program implements Word Ladder game.
 * A word ladder puzzle begins with two words, and to solve the puzzle
 * one must find a chain of other words to link the two, in which two adjacent words
 * (that is, words in successive steps) differ by one letter.
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

/* Function prototypes */
Vector<string> findSimilarWords(string word, Lexicon &dictionary);
Vector<string> findLadder(string startWord, string endWord, Lexicon &dictionary);
string getStartWord(Lexicon &dictionary);
string getDestinationWord(Lexicon &dictionary, int size);
void printLadder(Vector<string> &ladder);

/* Main program */
int main() {
    const string DICTIONARY_FILE = "EnglishWords.dat";

    Lexicon dictionary(DICTIONARY_FILE);
    Vector<string> wordLadder;

    while(true){

        string startWord = getStartWord(dictionary);

        if (startWord.size() == 0) break; // implementation of "RETURN to quit"

        string destWord = getDestinationWord(dictionary, startWord.size());
        wordLadder = findLadder(startWord, destWord, dictionary);
        printLadder(wordLadder);

    }


    return 0;
}

/* Function: printLadder
 * Usage: printLadder(wordLadder);
 * -------------------------------------------------------------------
 *
 * This function displays received word ladder in format:
 * "startword -> word1 -> word2 -> endword"
 * or display mesage "No ladder found." if
 * ladder is empty.
 *
 * @param &ladder WordLadder to display
 */
void printLadder(Vector<string> &ladder){
    if(ladder.size() > 0){
        for (int i = 0; i < ladder.size(); i++){
            if (i < ladder.size() - 1){
                cout << ladder[i] << " -> "; //for words before last
            }
            else {
                cout << ladder[i] << endl; //for last word
            }
        }
    }
    else {
        cout << "No ladder found." << endl;

    }
}

/* Function: getStartWord
 * Usage: string startWord = getStartWord(dictionary);
 * -------------------------------------------------------------
 *
 * This fuction asks user to enter the start word or press enter
 * with blank field to quit the programm. It repeat this question
 * while not entered a valid word from dictionary or ENTER presed
 * in the blank field.
 *
 * @param &dictionary Link to the instanse of the Lexicon class
 * with English words dictionary
 *
 * return@ Valid English word
 */
string getStartWord(Lexicon &dictionary){
    string word = "";
    while (true) {
        word = getLine("Enter start word (RETURN to quit): ");
        word = toLowerCase(word);
        if (dictionary.contains(word) || word.size() == 0) break;
        cout << "Please enter a valid English word!" << endl;
    }
    return word;
}

/* Function: getDestinationWord
 * Usage: string destWord = getDestinationWord(dictionary, startWord.size());
 * --------------------------------------------------------------------------
 *
 * This function asks user to enter the destination word. It repeat this question
 * while not entered a valid word from dictionary with received size
 *
 * @param &dictionary Link to the instanse of the Lexicon class
 * with English words dictionary
 * @param size Required length of the returned word
 * @return Valid English word with required lenth
 */
string getDestinationWord(Lexicon &dictionary, int size){
    string word = "";
    while (true) {
        word = getLine("Enter destination word: ");
        word = toLowerCase(word);
        if (dictionary.contains(word) && word.size() == size) break;
        cout << "Enter a valid destination word of " << size << " letters!"<< endl;
    }
    return word;
}

/* Function: findLadder
 * Usage: wordLadder = findLadder(startWord, destWord, dictionary);
 * ------------------------------------------------------------------
 *
 * This function finds the shortest ladder between two words
 * using Breadth-first search algorithm.  It starts at the start word
 * and explores the neighbor words first, before moving to the next level neighbors.
 *
 * @param startWord The word to start the ladder
 * @param endWord The wod to end the ladder
 * @param &dictionary Link to the instanse of the Lexicon class
 * with English words dictionary
 * @return Resulted word ladder
 */
Vector<string> findLadder(string startWord, string endWord, Lexicon &dictionary){

    Vector<string> ladder;
    Vector<string> nullVector;
    Lexicon usedWords;
    Queue<Vector<string>> ladders;

    ladder.add(startWord);
    usedWords.add(startWord);
    ladders.enqueue(ladder);

    while (!ladders.isEmpty()){ // go through all over the queue
        ladder = ladders.dequeue(); //read end remove the top of the queue

        if(ladder[ladder.size() - 1] == endWord) return ladder; // if shortest ladder is found

        Vector<string> similarWords = findSimilarWords(ladder[ladder.size() - 1], dictionary);

        /* Adds all words with difference in one letter to the ladders and push its to the queue */
        for(string word : similarWords){

            if(!usedWords.contains(word)){
                usedWords.add(word);
                Vector<string> temp = ladder;
                temp.add(word);
                ladders.enqueue(temp);
            }
        }

    }

    return nullVector;
}

/* Function: findSimilarWords
 * Usage: Vector<string> similarWords = findSimilarWords(ladder[ladder.size() - 1], dictionary);
 * ------------------------------------------------------------------------------
 *
 * This function finds all words in the English dictionary different from
 * the received word by one letter.
 *
 * @param word The word to start search
 * @param &dictionary Link to the instanse of the Lexicon class
 * with English words dictionary
 *
 * @return Vector with found word
 */
Vector<string> findSimilarWords(string word, Lexicon &dictionary){
    Vector<string> result;
    string temp = "";

    for(int i = 0; i < word.size(); i++){
        for(char ch = 'a'; ch <= 'z'; ch++){ //go through all the letters of the English alphabet
            temp = word;
            temp[i] = ch;
            if(dictionary.contains(temp) && temp != word){
                result.add(temp);
            }
        }
    }
    return result;
}
