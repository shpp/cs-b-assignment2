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

using namespace std;

FILE* file_pointer;

int summMap(Map<char,int> values ){
    int rezult = 0;
    Vector<char> keys = values.keys();
    for (char key : keys) {

        rezult += values[key];
    }
    return rezult;
}


void data(string sid, char nextSymbol, Map<string,Map<char,int>> & storage){
    if (storage.containsKey(sid)) {
        int tempValue = storage.get(sid).get(nextSymbol);
        storage[sid].add(nextSymbol, ++tempValue);
    }
    else{
        Map<char, int> valueInside;
        valueInside.add(nextSymbol, 1);
        storage.add(sid,valueInside);
    }

}


Map<Vector<int>, char> convertMap(Map<char, int> data){
    Vector<int> range;
    Map<Vector<int>, char> rezult;
    Vector<char> keys = data.keys();
    int curKey = 0;
    for(char key : keys){
        range.clear();
        range.push_back(curKey);
        curKey += data[key];
        range.push_back(curKey);
        rezult.add(range,key);
    }
    return rezult;
}

char randomChar(Map<char, int>  data){
    Map<Vector<int>, char> convertedMap = convertMap(data);
    //cout << "ConvertedMap :: " << convertedMap << endl;
    int maxVal = summMap(data);
    int value = randomInteger(0, maxVal);
    Vector<Vector<int>> keys = convertedMap.keys();
    for (auto key : keys) {
        if (value >= key[0] && value < key[1])
            return convertedMap[key];
    }
    return '\0';
}


bool spacing(string seed, char nextSymbol) {
    if (nextSymbol == ' ')
        if (seed.at(seed.size()-1) == ' ' || seed.at(seed.size()-1) == '\n')
            return true;
    return false;
}

void dumpMap(Map<string, Map<char, int>> storage) {
    Vector<string> keys = storage.keys();
    for (string key : keys) {
        cout << "Seed: '" << key << "' > " << storage[key] << endl << "--------" << endl;
    }
}

int main() {
    Map<string, Map<char, int>> storage;
    Map<char, int> valueInside;
    char temp_char;

    string letters = " \nabcdefghijklmnopqrstuvwxyz";

    ifstream inFile;
    string fileName, seed;
    fileName = promptUserForFile(inFile, "Source file: ", "Could not open source file.");

    int order = 8;

    while( order  != seed.size()){
        inFile.get(temp_char);
        seed += temp_char;
    }

    inFile.get(temp_char);
    do {
        data(seed, temp_char, storage);
        seed.erase(0,1);
        seed += temp_char;
    } while (inFile.get(temp_char));

    dumpMap(storage);
    Vector<string> keys = storage.keys();
    int maxInput = 0;
    string maxSeed;
    for(string key : keys){
        int temp = summMap(storage[key]);
        if(temp > maxInput){
            maxInput = temp;
            maxSeed = key;
        }
    }

    string final = maxSeed;
    for (int counter = 0; counter < 100; counter++) {
//        cout << "Final :: " << final << endl;
        int size = final.size();
        string seed = final.substr(size-order,size);
        if (storage.containsKey("xample. ")) cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        if (storage.containsKey(seed)) {
            final += randomChar(storage[seed]);
        } else {
            cout << "!W can't find key: '" << seed << "' Dump: " << storage[seed] << endl;
        }
//        cout << "Seed: " << seed << " RRRRR: " << storage[seed] << endl << endl << endl;

    }
    return 0;
}
