#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "Vector.h"

using namespace std;

Vector<string> oneLesnitsa(string startWord, Vector<string> wordStorage, string finalWord );
bool checkWord(string wordStart, string wordEnd);
Vector<string> makeVectorWithSpeciallyWords(string mainWord, Lexicon & lex);
Vector<string> searchNextStage(Vector<string> top, Vector<string> & wordStorage);
bool contains(Vector<string> vector, string word);


int main() {
    string startWord;
    string destinationWord;

    Vector<string> similarWords;

    Lexicon lex("EnglishWords.dat");

    startWord = getLine("Enter start word (RETURN to quit): ");
    destinationWord = getLine("Enter destination word: ");

    if(startWord == destinationWord){
        cout << "Booom..." << endl;
    }

    cout << oneLesnitsa(startWord, makeVectorWithSpeciallyWords(startWord, lex), destinationWord);


    return 0;
}

/*Check two words.
* @return true if words has one different char.
*/
bool checkWord(string wordStart, string wordEnd){
    // count different chars in the words
    int counter = 0;
    for (int i = 0; i < wordEnd.size(); ++i) {
        if(wordStart[i] == wordEnd[i]){
            continue;
        }
        else{
            counter++;
        }

        if(counter > 1) return false;
    }
    return true;
}

/*Search all words which length = length start word
*
* @param startWord - Start a word which was input
* @param lex - Lexicon which has all english words
* @return - vector with words. which length = startWord length
*/
Vector<string> makeVectorWithSpeciallyWords(string startWord, Lexicon & lex ){
    Vector<string> rez;
    int size = startWord.size();
    for(string value : lex){
        if(value.size() == size)
            rez.add(value);
    }
    return rez;
}


/*
* Push start data in vector
* @return - vector with start and temd words
*/

Vector<string> makePair(string startWord, string tempWord){
    Vector<string> rezVector;
    rezVector.add(startWord);
    rezVector.push_back(tempWord);
    return rezVector;
}



Vector<string> searchNextStage(Vector<string> top, Vector<string> & wordStorage){
    Vector<string> retVal;

    string topWord = top.get(top.size()-1);
    for(string value : wordStorage){
        if(!contains(top, value))
            if(checkWord(value, topWord))
                retVal.add(value);
    }
    return retVal;
}

/*
* Check if the word contains in the vector.
* @return true if the vector has the word
* @return false if -
*/

bool contains(Vector<string> vector, string word){
    for(string value : vector){
        if(value == word) return true;
    }
    return false;
}


/*
*@return shortest lesnitsa betwen words
*/
Vector<string> oneLesnitsa(string startWord, Vector<string> wordStorage, string finalWord ){
    Queue<Vector<string>> mainQue;
    Vector<string> tempVect;
    //start data
    tempVect.add(startWord);
    mainQue.add(tempVect);

    while (!mainQue.isEmpty()) {

        Vector<string> top = mainQue.dequeue();
        tempVect = searchNextStage(top, wordStorage);
        for(string value : tempVect){
            Vector<string> copyTop = top;
            copyTop.push_back(value);
            if(value == finalWord){
                return copyTop;
            }
            mainQue.enqueue(copyTop);
        }
    }

    Vector<string> nul;
    return nul;
}
