#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include <queue>
using namespace std;

Queue<Vector<string>> myqueue;
Lexicon dictionary("EnglishWords.dat");
Vector<string> diffOneLetterWord(string);
Vector<string> wordLadder(string, string);

int main() {
    while(true){
        string startWord = getLine("Enter start word - ");
        if(startWord.length() == 0){
            break;
        }
        string endWord = getLine("Enter destination word - ");
        Vector<string> res;
        if(startWord.length() == endWord.length()){
            res = wordLadder(startWord, endWord);
        }
        if(res.isEmpty()){ //if returned vector is empty print "no ladder"
            cout << "No ladder found!" << endl;
        }
        for(int i = 0; i < res.size(); i++){     //formating result
            if(i == (res.size() - 1)){
                cout << res[i] << endl;
            }
            if(i!=(res.size() - 1)){
                cout << res[i] << " -> ";
            }

        }
        //if don't clear arry, we get the same array
        res.clear();
    }
    return 0;
}

/*
* Make an vector of words from startWord to endWord
*
* @param user input - startWord, endWord
* @return Vector-Ladder of words
*/

Vector<string> wordLadder(string startWord, string endWord){
    Lexicon usedWords;
    usedWords.add(startWord);
    Vector<string> res;
    res.add(startWord);
    myqueue.enqueue(res);
    res.clear();

    while (!myqueue.isEmpty()) {
        //get first ladder from queue
        Vector<string> vec = myqueue.dequeue();
        //if last word is equal to endWord
        if(vec[vec.size() - 1] == endWord){
            res = vec;
            return res;
        }
        //make an array of different words for last word in vec
        Vector<string> tmpque = diffOneLetterWord(vec[vec.size() - 1]);
        for(string diffWord : tmpque){
            //if current word is not used earlier. Add new Vector to queue
            if(!usedWords.contains(diffWord)){
                usedWords.add(diffWord);
                Vector<string> nVec = vec;
                nVec.add(diffWord);
                myqueue.enqueue(nVec);
            }
        }
    }
    return res;
}

/*
* Make an array of words that have one different letter
*
* @param user input - startWord
* @return Array of words
*/
Vector<string> diffOneLetterWord(string startWord){
    Vector<string> res;
    Vector<char> newWord;
    //make from startWord an array newWord to change letter by letter
    for(int w = 0; w < startWord.length(); w++){
        newWord.add(startWord[w]);
    }
    //cout << newWord << endl;
    string curWord = "";
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    //start to change each letter of word (array)
    for(int i = 0; i < newWord.size(); i++){
        for(int a = 0; a < alphabet.length(); a++){
            newWord.set(i, alphabet[a]);
            //convert array into word
            for(int r = 0; r < newWord.size(); r++){
                curWord += newWord[r];
            }
            if(curWord != startWord){
                if(dictionary.contains(curWord)){
                    res.add(curWord);
                }
            }
            curWord = "";
        }
        newWord.set(i, startWord[i]);
    }
    return res;
}
