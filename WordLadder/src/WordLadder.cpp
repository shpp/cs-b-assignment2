#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

// read lexicon file
Lexicon lex("EnglishWords.txt");

// local word's dictionary
Vector<string> dic;

// declare source and final words
string source_word;
string final_word;

/**
 * @brief fillDic select words from lexicon depending to required word length
 * @param wordLength - int length of words to select
 */
void fillDic(int wordLength){
    dic.clear();
    for(string word:lex){
        if((word.length()==wordLength) && (word!=source_word))
            dic.add(word);
    }
}

/**
 * @brief wordAlreadyInOldChain check if word already present in current chain
 * @param wordToCheck - word to check
 * @param Chain - vector of words to check in
 * @return true - if present
 */
bool wordAlreadyInOldChain(string wordToCheck, Vector<string> & Chain){
    bool res=false;
    for(string usedWord:Chain){
        if(usedWord==wordToCheck){
            res=true;
            break;
        }
    }
    return res;
}

/**
 * @brief numberDifferentLetters count number of different letters between words
 * @param wordToCheck - word to check:)
 * @param lastWord - word to compare with
 * @return int number of differences
 */
int numberDifferentLetters(string wordToCheck,string lastWord){
    int res=0;
    for(int i=0; i<wordToCheck.size();i++)
        if(!(wordToCheck[i]==lastWord[i]))
            res++;
    return res;
}

/**
 * @brief findNewWordsInDic find new words in dictionary with one letter changed
 * @param lastWord - current base word
 * @param oldChain - current chain with used words
 * @return vector of new words (string)
 */
Vector<string> findNewWordsInDic(string lastWord, Vector<string> & oldChain){
    Vector<string> res;
    // look for words, that not equ lastWord & has 1 difference in letters from it & is'nt in oldChain Vector.
    for(string word:dic){
        if((word==lastWord)||(word=="")||(wordAlreadyInOldChain(word,oldChain))||(numberDifferentLetters(word,lastWord)>1))
            continue;
        res.add(word);
    }
    return res;
}

/**
 * @brief showChain show chain of words to screen
 * @param currentChain - chain to show
 */
void showChain(Vector<string> & currentChain){
    for(auto word:currentChain){
        cout << word << " ";
    }
    cout << endl;
}

/**
 * @brief takeWord ask user to enter word
 * @param msg - prompt message
 * @return string, entered by user.
 */
string takeWord(string msg){
    string res;
    cout << msg;
    cin >> res;
    return res;
}

/**
 * @brief check if destination word not present in local dictionary.
 * @return false - if present.
 */
bool finalWordNotInDictionary(){
    bool res=true;
    for(string word:dic){
        if(word==final_word){
            res=false;
            break;
        }
    }
    return res;
}

/**
 * @brief removeChainWordsFromDic - remove deadlock words from local dictionary
 * @param chain - vector of <string> words to be removed
 */
void removeChainWordsFromDic(Vector<string> & chain){
    int i=0;
    for(string dicElem:dic){
        if(dicElem=="") // already removed
            continue;
        for(string chainElem:chain)
            if(chainElem==dicElem){
                dic.set(i,"");
                break;
            }
        i++;
    }
}


int main()
{
    while(true){
        source_word=takeWord("Source word ('q' to exit): ");

        if(source_word.length()==1)
            break;

        final_word=takeWord("Destination word: ");

        cout << "I'm thinking..." << endl;

        if(final_word.length()!=source_word.length()){
            cout << "Detination word has incorrect length" << endl;
            continue;
        }

        // fill local dictionary
        fillDic(source_word.length());
        cout << "Local dictionary: " << dic.size() << "words.\n";

        // check for corrent length source and final words
        if(finalWordNotInDictionary()){
            cout << "Detination word is absent in dictionary" << endl;
            continue;
        }

        // init first chain
        Vector <string> chain;
        chain.add(source_word);

        // init chainsQueue
        Queue<Vector<string>> chainsQueue;
        chainsQueue.enqueue(chain);

        bool chainFound=false;

        // main search loop
        while(chainsQueue.size()>0){
            Vector <string> currentChain=chainsQueue.dequeue();
            string lastWordOfCurrentChain=currentChain[currentChain.size()-1];

            Vector <string> expandingWords=findNewWordsInDic(lastWordOfCurrentChain,currentChain);

            if(expandingWords.size()==0){   // deadlock words chain found
                removeChainWordsFromDic(currentChain);
            } else {                        // expand the chainwith new words
                for(auto word:expandingWords){
                    Vector <string> newChain(currentChain);
                    newChain.add(word);
                    //check, if final_word found?!
                    if(word==final_word){
                        showChain(newChain);
                        chainFound=true;
                        break;
                    }
                    chainsQueue.enqueue(newChain);
                }
            }
            if(chainFound)
                break;
        }
        // message if not found chain...
        if(chainsQueue.size()==0)
            cout << "No chain between words:(" << endl;
    }
    return(0);
}
