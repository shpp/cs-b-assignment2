#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"
using namespace std;

FILE* file_pointer;

/*
* Show mess about check order
* @return input digit
*/
int getOrder(){
    int order = getInteger("Order [1-10]: ");

    if (  order < 0 || order > 10) {
        while(order < 0 || order > 10){
            cout << "Order must be between 1 and 10." << endl;
            order = getInteger("Order [1-10]: ");
        }
    }
    return order;
}

/*
* Make data, which we can to take for make text.
* @param infile - file for work
* @param order - length for seed
* @return Map<string, Vector<char>> - which has seed and vector with chars
*/

Map<string, Vector<char>> makeData(ifstream & inFile, int order){
    Map<string, Vector<char>> rezult;
    Vector<char> tempDataVect;
    char tempChar;
    string seed = "";

    //start seed
    while( order  != seed.size()){
        inFile.get(tempChar);
        seed += tempChar;
    }

    inFile.get(tempChar);
    tempDataVect.add(tempChar);
    rezult.put(seed, tempDataVect);



    while (inFile.get(tempChar)) {
        seed.erase(0,1);
        seed += tempChar;

        inFile.get(tempChar);
        rezult[seed].add(tempChar);
    }
    return rezult;
}

/*
*   Make text
*   Search most popular seed. Add him to the rezult text, and add random char from vector
*   Get next seed and to continue work.
*   @param data - map for work, which has seeds and vector of chars, for each seed
*/

void makeText(Map<string, Vector<char>> & data){
    cout << "Processing..." << endl;
    int maxSize = 0;
    string tempSeed = "";
    string rezultText = "";
    //search most popular seed in the text
    for(string key : data.keys()){
        if(data[key].size() > maxSize){
                maxSize = data[key].size();
                tempSeed = key;
            }
    }

    //add this seed in the rezultText
    rezultText += tempSeed;
    //get most popular char, after seed
    char lastChar = data[tempSeed].get(randomInteger(0, maxSize-1));
    //add this char in the rezultText
    rezultText += lastChar;

    while(rezultText.size() < 2000){
       tempSeed.erase(0,1);
       tempSeed +=lastChar;
       cout << "[" << tempSeed << "]" << endl;
       if(data.containsKey(tempSeed)){
           rezultText += tempSeed;
           int vectSize = data[tempSeed].size()-1;
           lastChar =data[tempSeed].get(randomInteger(0, vectSize));
           rezultText += lastChar;
       }
       else{
           cout << "Boom!!!" << endl;
           break;
       }
    }

    cout << rezultText;



}


int main() {

    Map<string, Vector<char>> storage;

    string fileName = "";

    ifstream inFile;

    fileName = promptUserForFile(inFile, "Source file: ", "Could not open source file.");

    int order = getOrder();

    storage = makeData(inFile, order);

    cout << storage["     "];

    makeText(storage);



    return 0;
}
