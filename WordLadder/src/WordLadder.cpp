#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

/* Function prototypes */
string getStartWord(Lexicon &dictionary);
string getDestinationWord(Lexicon &dictionary, int length);
Vector<string> findMetagrams(string word, Lexicon &dictionary);
Vector<string> findWordLadder(string startWord, string destinationWord, Lexicon &dictionary);
void printWordLadder(Vector<string> &ladder);

int main() {
    /* Lexicon of English words. */
    const string WORD_LIST = "EnglishWords.dat";
    Lexicon dictionary(WORD_LIST);

    Vector<string> wordLadder;

    while(true){
        /* Get the initial and final words */
        string startWord = getStartWord(dictionary);
        string destinationWord = getDestinationWord(dictionary, startWord.length());
        /* Find and print word ladder */
        wordLadder = findWordLadder(startWord, destinationWord, dictionary);
        printWordLadder(wordLadder);
    }
    return 0;
}
/* Function prompts the user for the initial word, checks its presence in the dictionary.
 * If the user enters a blank line terminates the program.
 */
string getStartWord(Lexicon &dictionary){
    string startWord = "";
    while (true) {
        startWord = toLowerCase(getLine("Enter start word (RETURN to quit): "));
        if (startWord == ""){
            exit(0);
        } else if (!dictionary.contains(startWord)){
            cout << "Please enter a valid word!" << endl;
        } else break;
    }
    return startWord;
}
/* The function prompts the user for the final word,
 * check for it in the dictionary and matching the length of the first word.
 */
string getDestinationWord(Lexicon &dictionary, int length){
    string destinationWord = "";
    while (true) {
        destinationWord = toLowerCase(getLine("Enter destination word: "));
        if (dictionary.contains(destinationWord) && destinationWord.length() == length){
            break;
        } else if (!dictionary.contains(destinationWord) && !(destinationWord.length() == length)){
            cout << "Please enter a valid word of " << length << " letters!" << endl;
        } else if (!dictionary.contains(destinationWord)){
            cout << "Please enter a valid word!" << endl;
        } else if (!(destinationWord.length() == length)){
            cout << "Destination word must be " << length << " letters!" << endl;
        }
    }
    return destinationWord;
}
/* The function takes an original word and finds it Metagrams in the dictionary.
 * Returns vector Metagrams.
 */
Vector<string> findMetagrams(string parentalWord, Lexicon &dictionary){
    Vector<string> metagrams;
    string tmp = "";

    for (int i = 0; i < parentalWord.length(); i++){
        for (char ch = 'a'; ch <= 'z'; ch++){
            tmp = parentalWord;
            tmp[i] = ch; // generation word
            if (tmp != parentalWord && dictionary.contains(tmp)){ // check in dictionary and unique
                metagrams.add(tmp); // add to vector
            }
        }
    }
    return metagrams;
}
/* Function finds the word ledder between two words
 * using Breadth-first search algorithm.
 */
Vector<string> findWordLadder(string startWord, string destinationWord, Lexicon &dictionary){

    Vector<string> wordLadder;
    Vector<string> emptyWordLadder;
    Lexicon usedWords;
    Queue<Vector<string>> ladders;

    wordLadder.add(startWord);
    usedWords.add(startWord);
    ladders.enqueue(wordLadder);

    while (!ladders.isEmpty()){
        wordLadder = ladders.dequeue(); // Removes and returns the first item in the queue.
        // short ladder found if last element of word ladder == destination word
        if (wordLadder[wordLadder.size() - 1] == destinationWord){
            return wordLadder; //
        }
        // Looking Metagrams for the word standing at the end of the current ladder.
        Vector<string> metagrams = findMetagrams(wordLadder[wordLadder.size() - 1], dictionary);
        // Looking at Metagrams words have not been used before and are adding to the ladders
        for(string word : metagrams){
            if(!usedWords.contains(word)){
                usedWords.add(word);
                Vector<string> tmp = wordLadder;
                tmp.add(word);
                ladders.enqueue(tmp);
            }
        }
    }
    return emptyWordLadder;
}
/* The function displays the ladder of words or a message about her absence. */
void printWordLadder(Vector<string> &ladder){
    if(ladder.size() > 0){
        for (int i = 0; i < ladder.size(); i++){
            if (i < ladder.size() - 1){
                cout << ladder[i] << " -> ";
            }
            else {
                cout << ladder[i] << endl;
            }
        }
    }
    else {
        cout << "Word ladder not found." << endl;

    }
}
