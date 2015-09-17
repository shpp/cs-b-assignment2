
#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"

// used for testing of task resolve time
#include "time.h"

using namespace std;

// shortcut for oftenly used data type
typedef Vector<string> StrArray;
// functions declaration
StrArray getNexladders(StrArray baseladder, Lexicon & wordsBank, Lexicon &usedWords);
bool singleLetterDifference(string & word1, string & word2);
Vector<string> findShortestladder(string & startWord, string & destinationWord, Lexicon & similarWords);

// main funciton
int main() {
    // data variables declaration
    int         startWordSize;
    string      startWord = "",
                destinationWord = "";
    Lexicon     lex("EnglishWords.dat"),
                similarWords;
    
    // main loop
    while (true) {
    	
    	// ask user for a start word input
        startWord = getLine("Enter start word (RETURN to exit): ");
        if (startWord == "") return 0;
        if (!lex.contains(startWord)) {
            cout << "\tThere is no sutch word, try again.\n";
            continue;
        }

        // ask user for a destination word input
        destinationWord = getLine("Enter destination word (RETURN to exit): ");
        if (destinationWord == "") return 0;
        if (!lex.contains(destinationWord)) {
            cout << "\tThere is no sutch word, try again.\n";
            continue;
        }

        // saving timestamp before general job will start
        time_t startTime = time(NULL);

        startWordSize = startWord.size();
	
	// trim from all english words bank all words with length, different from start word's length
	// we will use only words with appropriate length
        for(string temp : lex){
            if(startWordSize == temp.length()){
                similarWords.add(temp);
            }
        }
	
	// start general function, get shortest data to the variable
        auto result = findShortestladder(startWord, destinationWord, similarWords);
	// echo passed time in seconds
        cout << "Processing time: " << time(NULL) - startTime << "\n";
	
	// echo readable and anderstandable results
        if (result.isEmpty()) {
            cout << "There is no appropriate ladder, sorry.\n";
        } else {
            cout << "Shortest ladder found:\n\t" << result[0];
            for (int i = 1; i < result.size(); i++) {
                cout << " > " << result[i];
            }
            cout << "\n------------\n";
        }

    }

    return 0;
}

/**
 * getNexladders
 * function scan word bank, and searching for all words, that can be the sequel of the base ladder 
 * (only one letter difference between last ladder word and searched word)
 * except the wards in the usedWords Lexicon
 * 
 * @param baseladder - a ladder to continue
 * @param wordsBank - lexicon of words to scan
 * @param usedWords - lexicon of allready checked words
 * @result - Vector of ladders with different sequels
 **/
StrArray getNexladders(StrArray baseladder, Lexicon & wordsBank, Lexicon & usedWords) {

    StrArray retVal;

    string TopWord = baseladder[baseladder.size()-1];

    for (string value : wordsBank)
        if (!usedWords.contains(value) && singleLetterDifference(TopWord, value)) {
            retVal.push_back(value);
            usedWords.add(value);
        }
    return retVal;
}

/**
 * singleLetterDifference
 * function checks if the words have one different letter in the same position
 * 
 * @param word1 - first word to compare
 * @param word2 - second word to compare
 * @result - boolean
 **/
bool singleLetterDifference(string &word1, string &word2) {

    int diff_letters = 0;
    int limit = word1.size();
    
    // if they are equal - they are NOT different =)
    if (word1 == word2) return false;
    
    // check every character position and count differences
    for (int i=0; i < limit; i++) {
        if (word1[i] != word2[i])
            diff_letters++;
        // if there is more then one difference - there is not a ONE difference =)
        if (diff_letters > 1) return false;
    }
    
    // we reach this statement if we fiond one difference, not more and not less
    return true;
}

/**
 * findShortestladder
 * function is the wrapper for ladder building loop
 * returnes the shurtest word ladder between source and destination words
 * 
 * @param startWord - the beginning of word ladder
 * @param destinationWord - the end of word ladder
 * @param similarWords - words bank (to search intermediate words from)
 * @result - Vector of words (ladder) in regular order
 **/
StrArray findShortestladder(string & startWord, string & destinationWord, Lexicon & similarWords) {

    Queue<StrArray>     baseQueue;
    StrArray            containerVector,
                        topElement,
                        ladderList;
    Lexicon             usedWords;
    
    // pack start word into vector and put it to queue
    containerVector.push_back(startWord);
    baseQueue.enqueue(containerVector);
    usedWords.add(startWord);
    
    // queue processing loop
    while (!baseQueue.isEmpty()) {
    	
    	// take an element from queue
        topElement =  baseQueue.dequeue();
        
        // get all words, that can be a sequel of a current ladder
        ladderList = getNexladders(
            topElement,
            similarWords,
            usedWords
        );
       
        // forming sequel ladders 
        for (auto value : ladderList){
            StrArray backup = topElement;
            backup.push_back(value);
	    
	    // return result, if we reached destination word
            if (value == destinationWord)
                return backup;
	    
	    // add formed ladder to a queue
            baseQueue.enqueue(backup);
        }
    }
    
    // if we reached the end of queue - there is no possible ladders for current word pair
    // empty vector must be returned
    StrArray emptyVec;
    return emptyVec;
}
