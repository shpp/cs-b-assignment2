#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "strlib.h"

using namespace std;

/*Functions prototypes  */
void wordLadderGame();
void userInput(string& startWord, string& endWord, Lexicon englishWords);
string validInput(string prompt, Lexicon englishWords);
void addLadderToQueue(Vector<string> vec,
                      Queue<Vector<string>> & mainQueue,
                      Lexicon & wordsInQueue);
Vector<string> getNewValidWords(string inputWord, Lexicon & englishWords);
string nextFromAlphabet(string inputWord, int index);
Vector<string> mainQueueProcess(Queue<Vector<string>> & mainQueue,
                      Lexicon & wordsInQueue,
                      Lexicon & englishWords,
                      string endWord);
void programOutput(Vector<string>& resultLadder);

/* Program console settings */
void consoleSettings();
const string programTitle = "WordLadder";
const bool consoleEcho = false;

int main() {
    consoleSettings();
    /* "WordLadder game" function
     * The main task is to modify words from user due to game rules
     * At the end of process result cosequence of word have be printed
     */
    wordLadderGame();

	return 0;
}

/* "WordLadder game" function
 * The main task is to get pair of words from user - start word and end word
 * Function search variant to modify start word into end word
 * The main rules of game modifications is:
 * - each new mofification of start word can change only one letter in it
 * - each word is valid english word
 * Words from user must have same length
 * At the end of process result cosequence of word have be printed
 */
void wordLadderGame(){
    Lexicon englishWords("EnglishWords.txt");

    /* User input */
    string startWord = "";
    string endWord = "";
    userInput(startWord, endWord, englishWords);

    /* Main program store - stores all posible dicisions of the main task */
    Queue<Vector<string>> mainQueue;
    /* Creates lexicon of all words which queue contain */
    Lexicon wordsInQueue;
    /* Creates start possible variant of the task */
    Vector<string> newLadder(1, startWord);
    /* Fills main program queue by start possible variant of the task */
    addLadderToQueue(newLadder, mainQueue, wordsInQueue);
    /* Main program process */
    Vector<string> resultLadder = mainQueueProcess(mainQueue,
                                                   wordsInQueue,
                                                   englishWords,
                                                   endWord);
    programOutput(resultLadder);
}

/* Main program process
 * - get next ladder
 * - check there last word for end case
 * - get mofified variants of this word
 * - adds them into vectors
 * - adds these vectors to the queue
 * @param mainQueue - reference of main queue of program - store of possible
 *                    program decision variants
 * @param wordsInQueue - reference - store of  all posible words from queue
 * @param wordsInQueue - reference - store of possible valid english words
 * @param endWord - copy of the result user word
 *
 * @return - vector of first program decision, ladder of words from start word to end word
 */
Vector<string> mainQueueProcess(Queue<Vector<string>> & mainQueue,
                      Lexicon & wordsInQueue,
                      Lexicon & englishWords,
                      string endWord){

    Vector<string> resultLadder;
    while (!mainQueue.isEmpty()){
        /* Gets next queue element */
        Vector<string> queueLastLadder = mainQueue.dequeue();
        /* Gets word from this ladder, which have being put last there */
        string lastLadderWord = queueLastLadder[(queueLastLadder.size() - 1)];
        /* Checks an end case of programm */
        if (lastLadderWord == endWord){
            /* put this ladder into the programm output */
            resultLadder = queueLastLadder;
            break;
        }else{
            /* Gets vector of all new possible variants of this word */
            Vector<string> newWordsVec = getNewValidWords(lastLadderWord, englishWords);
            /* Adds all new values to new ladders, and adds them to the queue */
            for(string modifiedWord: newWordsVec){
                /* Each new ladder is made as a copy of the taken ladder */
                Vector<string> nextNewLadder = queueLastLadder;
                /* Put new mofified word into new copy */
                nextNewLadder.add(modifiedWord);
                /* Adds such new ladder into the queue */
                addLadderToQueue(nextNewLadder, mainQueue, wordsInQueue);
            }
        }
    }
    return resultLadder;
}

/* Adds each new ladder to main queue
 * Creates lexicon of the queue's words
 *
 * Modifies mainQueue and queueWordList by reference
 *
 * @param vec - ladder to put into queue
 * @param mainQueue - main queue of the programm
 * @param queueWordsList - queue lexicon, saves all kinds of words from queue  */
void addLadderToQueue(Vector<string> vec,
                      Queue<Vector<string>> & mainQueue,
                      Lexicon & queueLexicon){
    /* For this version suppose that such vector has only one new kind of word - last word */
    string lastWord = vec[(vec.size() - 1)];
    /* If queue lexicon doesn't contain such word... */
    if(!queueLexicon.contains(lastWord)){
        /* ...then this word is put to queue lexicon */
        queueLexicon.add(lastWord);
        /* And add this vec to queue */
        mainQueue.enqueue(vec);
    }
}

/* Creates list of all possible input word modifications
 * Get word and modifies one letter from it
 * If modified word is valid - save it into result vector
 * inputWord isn't saved into result
 *
 * @param inputWord - simple word
 * @param englishLexicon - vocabulary of valid english words
 *
 * @return - vector of all possible modifications */
Vector<string> getNewValidWords(string inputWord, Lexicon & englishLexicon){
    Vector<string> resultVector;
    /* Counter of modification proccess */
    int modificationIndex = 0;
    string modificatedWord = "defalut";
    /* loop of new words generating
     * It stops if last modification was equal empty string */
    while (modificatedWord != ""){
        /* Gets new modification of input word */
        modificatedWord = nextFromAlphabet(inputWord, modificationIndex);
        /* Checks new modification
         * If it's starts from letter - for case of damaged words
         * like "-ba" during input */
        if(isalpha(modificatedWord[0])){
            /* If new modification dosen't equal input word */
            if(modificatedWord != inputWord){
                /* If new modification is valid english word */
                if(englishLexicon.contains(modificatedWord)){
                   resultVector.add(modificatedWord);
                }
            }
        }else{
            break;
        }
        /* Shifts counter to direct modification process */
        modificationIndex++;
    }
    return resultVector;
}

/* Modifies each letter in input word due to alphabet
 *
 * @param inputWord - simple word
 * @param index - gives position of modification. If value of index bigger
 * then possible variants of modifications - function returns ""
 *
 * @return - modification of input word or "" if there are no modifications for
 * such index */
string nextFromAlphabet(string inputWord, int index){
    string alphabet = "abcdefghijklmnopqrstuvwqxyz";
    /* Initiate result string */
    string outputWord = inputWord;
    /* Index of the next letter posirion to be changed */
    int letterIndex = index / alphabet.length();
    /* Index of the next letter from alphabet to be put into modification position */
    int alphabetIndex = index % alphabet.length();
    /* While process index alows to generate modifications */
    if(index < ((alphabet.length()) * (inputWord.length()))){
        /* Create new modification - change letter at this position */
        outputWord[letterIndex] = alphabet[alphabetIndex];
        return outputWord;
    }else{
        /* If process index value is more then possible return end case value  */
        return "";
    }
}

void userInput(string& startWord, string& endWord, Lexicon englishWords){
    //startWord = "sleep";
    //endWord = "awake";

    //startWord = "airplane";
    //endWord = "tricycle";

    //startWord = "work";
    //endWord = "play";

    //startWord = "code";
    //endWord = "data";

    //startWord = "never";
    //endWord = "event";

    startWord = validInput("Enter start word: ", englishWords);
    endWord = validInput("Enter end word: ", englishWords);
}

string validInput(string prompt, Lexicon englishWords){
    while(true){
            string result = getLine(prompt);
            if(englishWords.contains(result)){
                return result;
                break;
            }else{
                cout << "Unfortunately your input is failed" << endl;
            }
     }
}

/* Programm output - prints results of process */
void programOutput(Vector<string>& resultLadder){
    /* End case - if there no variants of modification */
    if(resultLadder.isEmpty()){
        cout << "No ladder found";
    }else{
        /* Loop of the result printing */
        for(int i = 0; i < (resultLadder.size() - 1); i++){
            cout << resultLadder[i];
            cout << " -> ";
        }
        cout << resultLadder[(resultLadder.size() - 1)];
    }
}

/* Make console output window more convenient */
void consoleSettings(){
    setConsoleWindowTitle(programTitle);
    setConsoleEcho(consoleEcho);
}
