#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "filelib.h"
#include "simpio.h"
#include "tokenscanner.h"
using namespace std;


/*
The Function is to get the user level model in the form of an integer from 1 -10
param modelLevel - the variable which will contain modelLevel
*/
void getModelLevel(int &modelLevel){
    while(true){
        modelLevel = getInteger("Enter model level from 1-10: ");
        if (modelLevel < 1 || modelLevel >10){
            cout<<"You enter the wrong model level!"<<endl;
        } else break;
    }

}

/*
Function reads the first seed from the file according to the received level model.
param scanner - token scanner for reading characters
param modelLevel - the variable which will contain modelLevel
param seedKey - key first seed to build Map
param nextChar - the next character after seed
*/
void readFirstSeed(TokenScanner &scanner,int &modelLevel,string &seedKey,char &nextChar){

        for(int i = 0; i < modelLevel; i++){
            nextChar = scanner.getChar();
            if(nextChar == EOF )break;
            seedKey += nextChar;
        }
}

/*Function moves seed by one character
param param seedKey - key first seed to build Map
param nextChar - the next character after seed
param modelLevel - the variable which will contain modelLevel
*/
void readNextSeeds(string &seedKey, char &nextChar,int &modelLevel){

    if(seedKey != ""){
        seedKey = seedKey.substr(1);
        seedKey += nextChar;
    }
 }


/*
 The Function reads the file one character and fills out the Map according to the Markov model
 param seedMap - map in which seed are the keys and values are vectors of characters which may be present after seed
 param fileForAnalize - which may be present after the oxide
 param modelLevel - the variable which will contain modelLevel

*/
void readFileToBuildSeedsMap(Map<string, Vector<char>> &seedMap, ifstream &fileForAnalize, int &modelLevel){
    string seedKey;
    char nextChar;

    TokenScanner scanner(fileForAnalize);

    readFirstSeed(scanner,modelLevel,seedKey,nextChar);
    while(true){
        readNextSeeds(seedKey,nextChar,modelLevel);
        nextChar = scanner.getChar();
        if(nextChar == EOF){
            break;
        }
        /*At each iteration adding to the map characters which is the current Sid*/
        seedMap[seedKey].add(nextChar);
    }
    fileForAnalize.close();

}


/*The Function of analyzing the map, and choose seed with the highest number of possible characters
 param seedMap - map in which seed are the keys and values are vectors of characters which may be present after seed
 return seedKey return found the most popular seed
*/
string findMostPopularSeed(Map<string, Vector<char>> &seedMap){
    Vector<string> seedKeyCopy = seedMap.keys();
    string result = seedKeyCopy[0];
    for (string key: seedMap){
        if (seedMap[key].size() > seedMap[result].size()){
            result = key;
        }
    }
    return result;
}

/*
 Function randomly selects the next character of the possible from prepared seed
 param seedMap - map in which seed are the keys and values are vectors of characters which may be present after seed
 param startSeed - the most popular seed
 param modelLevel - the variable which will contain modelLevel
 return character the most popular character from prepared seed
*/
char getNextChar(Map<string, Vector<char>> &seedMap,string &startSeed, int &modelLevel){
    char result;
    Vector<char> nextChar = seedMap[startSeed];
    result = nextChar[randomInteger(0, nextChar.size()-1)];
    readNextSeeds(startSeed,result,modelLevel);
    return result;
}

/*
 Function generates a random text length of 2000 characters from the collected data in the map by Markov model
 param seedMap - map in which seed are the keys and values are vectors of characters which may be present after seed
*/
void printRandomText(Map<string, Vector<char>> &seedMap,int &modelLevel){
    const int MAX_TEXT_SIZE = 2000;
    string resultText = "";
    string startSeed = findMostPopularSeed(seedMap);

    resultText += startSeed;
    while (true) {

        if (resultText.length() > MAX_TEXT_SIZE) break;

        resultText += getNextChar(seedMap, startSeed,modelLevel);
    }

    cout<<resultText<<endl;
}


int main() {
    ifstream fileForAnalize;
    int modelLevel;
    Map<string, Vector<char>> seedMap;

    while (true) {
        promptUserForFile(fileForAnalize, "Please enter the your file name: ");
        if (fileForAnalize.is_open()){
        break;
        }
    }

    getModelLevel(modelLevel);

    readFileToBuildSeedsMap(seedMap,fileForAnalize,modelLevel);

    printRandomText(seedMap,modelLevel);

    return 0;
}
