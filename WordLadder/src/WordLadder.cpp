#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "strlib.h"
using namespace std;


/* Constants */
const Lexicon DICTIONARY("EnglishWords.dat"); // create a set with all english words from EnglishWords.dat
const string ALPHABET = "abcdefghijklmnopqrstuvwxyz"; //create a string with all letters of English alphabet


/*
 * Creates a word ladder between two words by creating a connection with such words each of which differs
 * from the previous single letter.
 * @param string &start and string &finish takes start and distination words for which creats a ladder.
 * @return string with resulting word ladder or with absent message in case of impossibility of finding the connection between words.
 */
string createAWordLadders(string & start, string & finish);


/*
 * Finds new variations of last word in the word ladder and if this variation is a real word and it has not yet been added
 * to any of word ladders then this word ladder is increased by this finding word.
 * @param Queue< Vector<string> > &listOfLadders takes a queue with all creating word ladders.
 * @param Vector<string> &wordLadder takes a first word ladder of the queue to supplement it with a new word.
 * @param Lexicon &usedWords takes a set with words which have already been added to one of the word ladder to avoid duplications.
 * @param string &finishWord takes a distination words to compare it with the finding variation.
 */
void lengthenTheLadders(Queue< Vector<string> > & listOfLadders, Vector<string> & wordLadder, Lexicon & usedWords, string & finishWord);




/*
 * Asks the user to enter start and distination words for creating word ladder between them.
 * Checks the validity of the entered words and if words have equal length and don't contain spaces
 * returns a word ladder.
 */
int main() {
    while(true){
        string startWord = getLine("Enter a start word (RETURN to quit): ");
        if (startWord == "") break; // if user enters nothing then the program  stops execution
        string distinationWord = getLine("Enter a distination word: ");

        //leads words to lowercase and trims spaces at the beginning and the end
        startWord = trim(toLowerCase(startWord));
        distinationWord = trim(toLowerCase(distinationWord));

        if ((startWord.length() != distinationWord.length())
                || (startWord.find(" ") != string::npos) || (distinationWord.find(" ") != string::npos)) {
            cout << "Start and distination words must to be a single words with equal length." << endl;
            break;
        }

        cout << createAWordLadders(startWord, distinationWord) << endl;
    }
    return 0;
}


string createAWordLadders(string & startWord, string & distinationWord){
    Queue < Vector<string> > listOfLadders;  //creates a queue for all word ladders
    Vector<string> startLadder; // creates an array for word ladder
    Lexicon usedWords; // creates a set for added words to any word ladders

    usedWords.add(startWord);
    startLadder.add(startWord);
    listOfLadders.enqueue(startLadder);

    while(!listOfLadders.isEmpty()){
        Vector<string> wordLadder = listOfLadders.dequeue(); //takes a first word ladder from a queue
        // checks the last word from  the word ladder to equality with the distination word
        if (wordLadder[wordLadder.size() - 1] == distinationWord) {

            // makes a string from a Vector with resulting word ladder for returning
            string resultWordLadder = startWord;
            for(int i = 1; i < wordLadder.size() - 1; ++i){
                resultWordLadder += " -> " + wordLadder[i];
            }
            return resultWordLadder + " -> " + distinationWord;
        }
        // extends the word ladder with new words
        lengthenTheLadders(listOfLadders, wordLadder, usedWords, distinationWord);
    }

    // return a message about the absence of an existing word ladder
    return "There is no ladder from start word: \"" + startWord + "\" to distination word: \"" + distinationWord + "\"";
}



void lengthenTheLadders(Queue< Vector<string> > & listOfLadders, Vector <string> & wordLadder, Lexicon & usedWords, string & distinationWord){

    string lastWord = wordLadder[wordLadder.size() - 1];
    string variations = lastWord; // create a duplicate of the last word

    /*
     * Use nested loop to iterate through all possible variations which differ from the last word with single letter by
     * placing on the current position in the word one by one all the letters of the alphabet.
     */
    for (int i = 0; i < lastWord.length(); ++i){
        for (int j = 0; j < ALPHABET.length(); ++j){
            variations[i] = ALPHABET[j];

            if (DICTIONARY.contains(variations)){

                /*
                 * Checks whether the getting variation of the word is equal to distination word to more quickly obtain
                 * the desired word ladder at the beginning of the queue.
                 */
                if(variations == distinationWord){
                    wordLadder.add(variations); // adds variation of the word to the end of Vector with previous word ladder
                    listOfLadders.clear();      // clear the queue to add only the necessary word ladder that takes the first position
                    listOfLadders.enqueue(wordLadder);
                    return;
                }

                /*
                 * Checks whether the getting variation of the word is added to one of the word ladder to avoid duplications.
                 * And if it is not creates new word ladder by adding this variation to previous one.
                 */
                if(!usedWords.contains(variations)){
                    usedWords.add(variations);
                    Vector <string> newWordLadder = wordLadder;
                    newWordLadder.add(variations);
                    listOfLadders.enqueue(newWordLadder); // add a new word ladder to end of queue
                }
            }
        }
        variations = lastWord; // return the initial value of the last word for changing letter at the next position

    }
    return;
}



