#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"
#include "stack.h"
#include "queue.h"
#include "timer.h"

using namespace std;

/* FILE READING */
string fileInput(string promptText);
int levelInput(string promptText);

Map<string, Vector<string>> getFileMap(istream & infile, int modelLevel);
string getLevelSeed(istream & infile,
                    char seedChar,
                    int modelLevel,
                    Stack<char>& stack);
string getSymbolAfterSeed(istream & infile);
void returnCharsToStream(istream & infile,Stack<char>& stack);

void modifyMap(Map<string, Vector<string>>& mainMap,
               string seed,
               string nextSymbol);
/* Level 0-model Text GENERATION */
void generate_0_levelText(Map<string, Vector<string>>& fileMap, int printSymbQty);
Map<string, Vector<string>> getStatisticMap(Map<string, Vector<string>>& fileMap);

/* Level N-model Text GENERATION */
void generate_N_levelText(Map<string, Vector<string>>& fileMap, int printSymbQty);
string getFrequentSeed(Map<string, Vector<string>>& fileMap);
string getRandomSymbol(string key, Map<string, Vector<string>>& map);

// Console settings
void consoleSettings();
const string programTitle = "RandomWriter";
const bool consoleEcho = false;

int main() {
    consoleSettings();

    /* User enters name of file from project directory */
    string fileName = fileInput("Enter file name: ");
    //string fileName = "MiddleMarch.txt";
    //string fileName = "Readme.txt";

    int modelLevel = levelInput("Enter model level range (1-10): ");
    /* Generated text symbols qty */
    int resultTextSymbolsQty = 2000;

    cout << "Processing..." << endl;
    Timer timer1(true);
    cout << "=========================================" << endl;
    /* Prepare input stream object   */
    ifstream infile;
    /* Lunch stream of file reading   */
    infile.open(fileName.c_str());
    /* Main read text process - seeds map creating */
    Map<string, Vector<string>> fileMap = getFileMap(infile, modelLevel);
    /* If whole file is read program closes stream */
    infile.close();

    cout << "MAP BUILD TIME IS " << timer1.elapsed() << endl;

    timer1.stop();
    Timer timer2(true);

    cout << "=========================================" << endl;

    if(modelLevel > 0){
        generate_N_levelText(fileMap, resultTextSymbolsQty);
    }else{
        generate_0_levelText(fileMap, resultTextSymbolsQty);
    }

    cout << endl;
    cout << "=========== END OF GENERATION ==========="<< endl;
    cout << "TEXT BUILD TIME IS " << timer2.elapsed() << endl;
    timer2.stop();

    return 0;
}

/* Main read text process - seeds map creating */
Map<string, Vector<string>> getFileMap(istream & infile, int modelLevel){
        Map<string, Vector<string>> resultMap;
        char startSeedChar;
        while ((startSeedChar = infile.get()) != EOF) {
            /* Stack for chars from seed */
            Stack<char> stack;
            /* Take chars from stream to build seed for required level */
            string seed = getLevelSeed(infile, startSeedChar, modelLevel, stack);
            /* Get next symbol after such seed */
            string nextSymbol = getSymbolAfterSeed(infile);
            /* Return chars from stack to stream */
            returnCharsToStream(infile, stack);
            /* Fills resultMap by obtained seed as key and nextSymbol as result  */
            modifyMap(resultMap, seed, nextSymbol);
        }
        return resultMap;
}

/* This function calls after seed getting from stream
 * Returns next symbol, after seed chars, from stream */
string getSymbolAfterSeed(istream & infile){
    string result = "";
    char nextChar = infile.peek();
    if(nextChar != EOF){
        result = charToString(nextChar);
    }
    return result;
}

/* Get seedChar - next char from stream, and builds seed of
 * required length */
string getLevelSeed(istream & infile,
                    char seedChar,
                    int modelLevel,
                    Stack<char>& stack){
    /* seed begins from seedChar */
    string result = charToString(seedChar);
    if(modelLevel > 0){
        for(int i = 1; i < modelLevel; i++){
            char nextChar;
            if ((nextChar = infile.get()) != EOF){
                /* Adds next text symbols to seed */
                result += charToString(nextChar);
                /* Stores taken seed chars to stack
                 * It'll enables to return them back to stream letter */
                stack.push(nextChar);
            }
        }
    }
    return result;
}

/* Modifies main file map - adds seeds and modifies nexy chars vectors */
void modifyMap(Map<string, Vector<string>>& mainMap,
               string seed,
               string nextSymbol){   
    if(mainMap.containsKey(seed)){
        /* Modifies map entry - adds next symbol to vector */
        mainMap[seed].add(nextSymbol);
    }else{
        /* Adds new map entry with such next symbol value */
        Vector<string> newSeedVector;
        newSeedVector.add(nextSymbol);
        mainMap.put(seed, newSeedVector);
    }
}

/* This function calls after seed creation by getLevelSeed
 * Return chars ffrom stack to stream */
void returnCharsToStream(istream & infile,Stack<char>& stack){
    while(!stack.isEmpty()){
        char symbol = stack.pop();
        infile.putback(symbol);
    }
}

/* Unfortunately I haven't managed to make it more rapid - dedaline time is coming
 *
 * 0-level generating depends of
 * - every new symbol of text appear after seed due to it's statistic probability */
void generate_0_levelText(Map<string, Vector<string>>& fileMap, int printSymbQty){
    string startSeed = getFrequentSeed(fileMap);
    cout << "THE MOST FREQUENT SEED IS: \"" << startSeed << "\"" << endl;
    cout << "============= GENERATED TEXT ============" << endl;
    cout <<startSeed;

    int counter = 0;
    /* For Level-0 Model every next symbol apear in text due to it's probability */
    /* Statistic map saves all next symbols as a value vector
     * Randomly taking from this vector gives probability for this symbol */
    Map<string, Vector<string>> statisticMap = getStatisticMap(fileMap);

    while(counter < printSymbQty){
        string nextSymbol = getRandomSymbol("someDefalutKey", statisticMap);
        if(nextSymbol == "no symbols"){
            cout << endl;
            cout << "PROGRAM STOPS DUE TO BREAK CONDITION";
            break;
        }
        cout << nextSymbol;
        counter++;
    }
    cout <<  endl;
}

/* Prepares map with single key and single next symbols vector
 * Vector stores all possible next symbols from text
 * Random taking from this vector returns symbols due to their probabylity */
Map<string, Vector<string>> getStatisticMap(Map<string, Vector<string>>& fileMap){
    /* This key is not important for our tasks */
    string key = "someDefalutKey";
    /* Vector of all next symbols */
    Vector<string> totalVec;
    for(string key: fileMap){
        Vector<string> keyVec = fileMap[key];
        for(string i: keyVec){
            totalVec.add(i);
        }
    }
    Map<string, Vector<string>> result;
    result.put(key, totalVec);
    return result;
}

/* N-level generating depends of
 * - every new symbol of text appear after such seed more frequently */
void generate_N_levelText(Map<string, Vector<string>>& fileMap, int printSymbQty){
    string startSeed = getFrequentSeed(fileMap);
    cout << "THE MOST FREQUENT SEED IS: \"" << startSeed << "\"" << endl;
    cout << "============= GENERATED TEXT ============" << endl;
    cout <<startSeed;

    int counter = 0;
    while(counter < printSymbQty){
        string nextSymbol = getRandomSymbol(startSeed, fileMap);
        if(nextSymbol == "no symbols"){
            cout << endl;
            cout << "NO NEXT SYMBOL CONDITION";
            break;
        }
        cout <<nextSymbol;
        startSeed.erase(0,1);
        startSeed += nextSymbol;
        counter++;
    }
}

/* The most frequent seed is seed with bigest vector */
string getFrequentSeed(Map<string, Vector<string>>& fileMap){
    int maxSize = 0;
    string result = "";
    for(string key: fileMap){
        int keySize = fileMap[key].size();
        if(keySize > maxSize){
            result = key;
            maxSize = keySize;
        }
    }
    return result;
}

/* Next symbol is get from seed statistic vector due to its probability in vector */
string getRandomSymbol(string key, Map<string, Vector<string>>& map){
    if(map.containsKey(key)){
        int thisKeyVecSize = map[key].size();
        int randomInt = randomInteger(0, (thisKeyVecSize - 1));
        Vector<string> vec = map[key];
        return vec[randomInt];
    }else{
        return "no symbols";
    }
}


/* User input function
 * Defines if this file exist in project directory */
string fileInput(string promptText){
    while(true){
        string result = getLine(promptText);
        if(fileExists(result)){
            return result;
            break;
        }else{
            cout << "Unfortunately your input is failed" << endl;
        }
    }
}

/* User input function
 * Defines if this model level value is correct */
int levelInput(string promptText){
    while(true){
        int result = getInteger(promptText);
        if((result >= 0) && ((result <= 10))){
            return result;
            break;
        }else{
            cout << "Unfortunately your input is failed" << endl;
        }
    }
}

void consoleSettings(){
    setConsoleWindowTitle(programTitle);
    setConsoleEcho(consoleEcho);
}
