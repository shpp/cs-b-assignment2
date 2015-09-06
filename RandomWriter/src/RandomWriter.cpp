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

void inputModelLevel(int &modelLevel);
void analysisOfTheModelText(Map<string, Vector<char>> &seeds, ifstream &infile, int &modelLevel);
void findingFirstSeed(TokenScanner &scanner,int &modelLevel,string &seed,char &nextChar);
void generationNextSeed(string &seed,char &nextChar,int &modelLevel);
void findingTheInitialSeed(string &initialSeed,Map<string, Vector<char>> &seeds);
char nextChar(Map<string, Vector<char>> &seeds,string &firstSeed,int &modelLevel);
void generateText(Map<string, Vector<char>> &seeds,int &modelLevel);



int main() {
    /*Variable storage:
    feed stream to generate a random text,
    line number generation model,
    collections storage seeds.*/
    ifstream infile;
     int modelLevel;
    Map<string, Vector<char>> seeds;

    //Prompts the user for the name of the source to generate random text generation and model number.
    promptUserForFile(infile, "Enter the file name: ");
    inputModelLevel(modelLevel);

    /*The function takes the parameters: source file to generate random text,
    line number generation models, clothing collection for storing the results
    of analysis of the model text.*/
    analysisOfTheModelText(seeds,infile,modelLevel);

    /*Function based on the model obtained from the analysis of the source code
    and a user-specified number of order model generates random text*/
    generateText(seeds,modelLevel);

	return 0;
}


//***Determining the validity of a user-entered numbers in a row models generate a random text.***
void inputModelLevel(int &modelLevel){
    while(true){
        modelLevel = getInteger("Enter model level (1-10): ");
        if(modelLevel && (modelLevel>=1 && modelLevel <= 10)){
            break;
        }
      cout<<"Enter the wrong number!!!"<<endl;
    }

}

/*The function analyzes the source file for the presence of seeds in it
, the size of which depends on the order of numbers entered by the user model.
Selected seeds in Map will be recorded to the keys and the character that follows them as the value*/
void analysisOfTheModelText(Map<string, Vector<char>> &seeds, ifstream &infile, int &modelLevel){
    string seed;
    char nextChar;

    TokenScanner scanner(infile);
    findingFirstSeed(scanner,modelLevel,seed,nextChar);
    while(true){
        generationNextSeed(seed,nextChar,modelLevel);
        if((nextChar = scanner.getChar()) == EOF){
            break;
        }

        seeds[seed].add(nextChar);
    }
    infile.close();

}

/*An additional feature that is used in the analysis of the model text.
Finding the first seed in the text. Depending on the model number of
the order entered by the user to generate the file to read the same
number of characters is equal to the number of the order of the model.
A string of characters is the first seed.*/
void findingFirstSeed(TokenScanner &scanner,int &modelLevel,string &seed,char &nextChar){

        for(int i(0);i<modelLevel;i++){
            nextChar = scanner.getChar();
            if(nextChar == EOF ){
                break;
            }
           seed += nextChar;
        }
}

/*The function receives the seed source and symbol located after him.
Seed subtract one character from the beginning, and one character is added at the end.
The result is the next seed.*/
void generationNextSeed(string &seed,char &nextChar,int &modelLevel){
     if(seed != ""){
         if(modelLevel>1){
         seed = seed.substr(1);
         seed += nextChar;
         }
         else{
          seed = "";
          seed += nextChar;
         }
     }
 }


/*The function that generates a random text based on the model obtained
from the analysis of the text (which the Collection MAP keys are seeds
and the values that are characters after them with a certain probability).*/
void generateText(Map<string, Vector<char>> &seeds,int &modelLevel){
    Vector<string> keys = seeds.keys();
    string result = "";
    string initialSeed = keys[0];

    //Finding the initial seed to start generating random text
    findingTheInitialSeed(initialSeed,seeds);
    result += initialSeed;

        while (true) {
              result += nextChar(seeds, initialSeed,modelLevel);
              if (result.length() > 2000){
                  break;
              }
          }

    cout<<result<<endl;
}

//Initial is the seed from which the largest size values. Since it is most common in text.
void findingTheInitialSeed(string &initialSeed,Map<string, Vector<char>> &seeds){
    for (string key : seeds){
        if (seeds[key].size() > seeds[initialSeed].size()){
            initialSeed = key;
        }
    }
}

/*Randomly searching for a symbol, which will be located after the seed.
Search occurs in the probability vector values of the characters that are after the seeds.*/
char nextChar(Map<string, Vector<char>> &seeds,string &initialSeed, int &modelLevel){
    char ch;
    Vector<char> value = seeds[initialSeed];
    int n;
        if(value.size()>0){
         n = randomInteger(0, value.size()-1);
        }
        else{
            n = 0;
        }
        ch = value[n];
        generationNextSeed(initialSeed,ch,modelLevel);
    return ch;
}
