#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
using namespace std;


/*
Function selects all the possible words that are different from the input one character.
param - startWord word to start the search
param allValidWord - a list of all the words
param usedWords - list has previously used the words
return vector all found words
*/
Vector<string> findSimilarWord(string startWord, Lexicon &allValidWord, Lexicon &usedWords){
    Vector<string> result;
    if(!usedWords.contains(startWord))
        usedWords.add(startWord);

    for (int i = 0; i < startWord.length(); i++){
        string wordCopy = startWord;
        for (char c = 'a'; c <= 'z'; c++){
            wordCopy.replace(i,1,1,c);
            if (allValidWord.contains(wordCopy) & !usedWords.contains(wordCopy)) {
                result.add(wordCopy);
                usedWords.add(wordCopy);
            }
        }
    }

    return result;
}


/*
 method adds the word to the queue
 param word - the word to be added
 validLadders - queue all valid ladders
*/
void addWordToQueue (string word, Queue<Vector<string>> &validLadders){
    Vector<string> shortestLadder(1, word);
    validLadders.enqueue(shortestLadder);
}


/*
Function displays the shortest ladder
param  shortestLadder - a vector that contains the shortest ladder
*/
void printResultLadder(Vector<string> &shortestLadder){
    for (int i = 0; i < shortestLadder.size(); i++){
        if ( i != shortestLadder.size()-1 ){
            cout<<shortestLadder[i]<<"->";
        } else{
            cout<<shortestLadder[i]<<endl;
        }

    }

}


/*
Function finds the shortest ladder for two words
param startWord - word to start the search
param destWord - the word which should be built ladder
*/
void findeWordLaddes(string startWord, string destWord){
    Lexicon allValidWord("EnglishWords.txt");
    Lexicon usedWords;
    Queue<Vector<string>> validLadders;
    addWordToQueue(startWord, validLadders);
    bool result = false;

    while (!validLadders.isEmpty()) {
        Vector<string> shortestLadder = validLadders.dequeue();
        // If the last word of the vector is equal to destWord way to stop searching
        if (shortestLadder[shortestLadder.size() -1] == destWord) {
            printResultLadder(shortestLadder);
            result = true;
            break;
        }
        //displace a search on the last word on the ladder
        string lastLadderWord = shortestLadder[shortestLadder.size() -1];

        Vector<string> allSimilarWords = findSimilarWord(lastLadderWord, allValidWord, usedWords);

        for(int i=0; i < allSimilarWords.size(); i++){
            Vector<string> copyShortestLadder = shortestLadder;
            copyShortestLadder.add(allSimilarWords[i]);
            validLadders.enqueue(copyShortestLadder);
        }
        //If the ladder is not built stop and display a message
        if (shortestLadder.size() == 0) break;

    }
    if (!result) cout<<"Any ladders could not be found"<<endl;

}


int main() {
    while (true) {
        string startWord = getLine("Enter start word: ");
        string destWord = getLine("Enter destination word: ");

        if ((startWord.length() ==0) || (destWord.length() == 0)){
            cout<<"Enter incorrect word!"<<endl;
            continue;
        } else findeWordLaddes(startWord, destWord);

    }
    return 0;
}
