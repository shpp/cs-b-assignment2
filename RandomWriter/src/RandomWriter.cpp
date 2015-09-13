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
#include "hashmap.h"

using namespace std;
/* == Version 3 (improved productivity of program - reading == */
/* FILE READING */
string fileInput(string promptText);
int levelInput(string promptText);

Map<string, Vector<string>> getFileMap(istream & infile, int modelLevel);
string getLevelSeed(istream & infile,
                    int modelLevel);
string getSymbolAfterSeed(istream & infile);
void returnCharsToStream(istream & infile, Stack<char>& stack);

void modifyMap(Map<string, Vector<string>>& mainMap,
               string seed,
               string nextSymbol);
/* Level 0-model Text GENERATION */
void generate_0_levelText(Map<string, Vector<string>>& fileMap, int printSymbQty);
Map<string, Vector<string>> getStatisticMap(Map<string, Vector<string>>& fileMap);
void generate_0_levelText_v2(Map<string, Vector<string>>& fileMap, int printSymbQty);
HashMap<string, float> getStatisticMap_v2(Map<string, Vector<string>>& fileMap);

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
    //string fileName = "Hamlet.txt";
    //string fileName = "TomSawyer.txt";

    int modelLevel = levelInput("Enter model level range (0-10): ");
    /* Generated text symbols qty */
    int resultTextSymbolsQty = 2000;
    /* Start of main process and timing */
    cout << "Processing..." << endl;
    Timer timer1(true);
    /* Program reads some text file, processes it, and generate new text due to data */
    cout << "=========================================" << endl;
    /* Prepare input stream object   */
    ifstream infile;
    /* Lunch stream of file reading   */
    infile.open(fileName.c_str());
    /* Main read text process - seeds map creating */
    Map<string, Vector<string>> fileMap = getFileMap(infile, modelLevel);
    /* If whole file is read program closes stream */
    infile.close();
    /* Map building time stops */
    timer1.stop();
    cout << "MAP BUILD TIME IS " << timer1.elapsed() << " ms" << endl;;
    /* New text generating time starting */
    Timer timer2(true);
    cout << "=========================================" << endl;
    /* Acidentally I make 0-level process too */
    if(modelLevel > 0){
        generate_N_levelText(fileMap, resultTextSymbolsQty);
    }else{
        generate_0_levelText_v2(fileMap, resultTextSymbolsQty);
    }
    /* End of generation - timing stops */
    cout << endl;
    cout << "=========== END OF GENERATION ==========="<< endl;
    timer2.stop();
    cout << "TEXT BUILD TIME IS " << timer2.elapsed() << " ms" << endl;

    return 0;
}

/* Main read text process - seeds map creating */
Map<string, Vector<string>> getFileMap(istream & infile, int modelLevel){
        Map<string, Vector<string>> resultMap;
        /* Create start seed due to maodel level */
        string seed = getLevelSeed(infile, modelLevel);

        char nextChar;
        while ((nextChar = infile.get()) != EOF) {
            /* Get next symbol after such seed */
            string nextSymbol = charToString(nextChar);
            /* Fills resultMap by obtained seed as key and nextSymbol as result  */
            modifyMap(resultMap, seed, nextSymbol);
            /* Modify seed - take away first sumbol and add nextSymbol to end */
            seed.erase(0,1);
            seed += nextSymbol;
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

/* Builds first seed of required length */
string getLevelSeed(istream & infile, int modelLevel){
    string result = "";
    char nextChar;
    /* Take one symbol from stream anyway, for 0 model or 1 model */
    if((nextChar = infile.get()) != EOF){
        result += nextChar;
    }
    /* For 2 model and higher - seed chars quantity equal model level value */
    if(modelLevel > 1){
        for (int i = 1; i < modelLevel; i++){
            if((nextChar = infile.get()) != EOF){
                result += nextChar;
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
void returnCharsToStream(istream & infile, Stack<char>& stack){
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
    /* Text generation begins */
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

/* Mdified for version 2 - improved productivity */
/* Next symbol is get from seed statistic vector due to its probability in vector */
string getRandomSymbol(string key, Map<string, Vector<string>>& map){
    if(map.containsKey(key)){
        int thisKeyVecSize = map[key].size();
        int randomInt = randomInteger(0, (thisKeyVecSize - 1));
        //Vector<string> vec = map[key];
        string result = map[key].get(randomInt);
        return result;
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

/* Attempt to improve function productivity
 * For 0-level text generating we have rule:
 * - every new symbol of text appear after seed due to this symbol
 *   statistic probability */
void generate_0_levelText_v2(Map<string, Vector<string>>& fileMap, int printSymbQty){
    string startSeed = getFrequentSeed(fileMap);
    cout << "THE MOST FREQUENT SEED IS: \"" << startSeed << "\"" << endl;
    cout << "============= GENERATED TEXT ============" << endl;

    /* First text symbol generation */
    cout <<startSeed;

    int counter = 0;
    /* For Level-0 Model every next symbol apear in text due to it's probability
     * I will try to improve productivity of function new algoryth:
     * - Statistic map symbol value of it's statistic probability
     * - We will iterate throgh this map randomly, and print symbols
     *   due to their probabilites values  */
    HashMap<string, float> statisticMap = getStatisticMap_v2(fileMap);
    /* Main text generation process */
    while(counter < printSymbQty){
        for (string key: statisticMap){
            if(counter < printSymbQty){
                /* Decide if this symbol will be printed to text */
                float probability = statisticMap[key];
                string nextPrintSymbol = key;
                bool isPrint = randomChance(probability);
                if(isPrint){
                    cout << nextPrintSymbol;
                    counter++;
                }
            }else{
                break;
            }
        }
    }
}

/* Returns map of all kind of symbols in text with their probabilites as values */
HashMap<string, float> getStatisticMap_v2(Map<string, Vector<string>>& fileMap){
    HashMap<string, float> result;
    Map<string, int> statisticTable;
    int symbolsInTextQty = 0;
    /* Count total quantity of symbols in text, and
     * quantity of their apearence - as a map - statisticTable */
    for(string key: fileMap){
        int keyVectorSize = fileMap[key].size();
        symbolsInTextQty += keyVectorSize;
        /* We supose that apearence of every symbol equal it's vector size */
        statisticTable.put(key, keyVectorSize);
    }
    /* Now we get total quantity of symbols in text
     * Create map of symbols with their probabilites as values */
    for(string key: statisticTable){
        int thisSymbolQty = statisticTable[key];
        float thisSymbolProbability = (thisSymbolQty /((float) symbolsInTextQty));
        result.put(key, thisSymbolProbability);
    }
    return result;
}
