/* File RandomWriter.cpp
 * --------------------------------------------------------
 *
 * This program generates a random text based on existing text
 * using "Markov models" of a different order.
 * First the program asks the user for the name of a text file and
 * order of Markov model, which will be used for analysis.
 * Then separates text on seeds equal to the order of Markov model
 * and writes the characters coming after them with the corresponding
 * probability. After this generate random text length of 2000
 * characters based on analysis results.
 *
 */
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

/* Function prototypes */
int readOrder();
string generateRandomText(ifstream &infile, int order);
Map<string, string> readSeeds(ifstream &infile, int order);
Vector<string> findFrequentSeeds(Map<string, string> &seeds);

/* Constant for number of the characters in the generated text */
const int RANDOM_TEXT_CHARACTERS = 2000;

/* Main program */
int main() {

    ifstream infile;
    promptUserForFile(infile, "Source file: ");
    int order = readOrder();
    cout << "Processing... " << endl << endl;
    string randomText = generateRandomText(infile, order);
    infile.close();

    cout << randomText << endl;
	return 0;
}


/* Function: readOrder
 * Usage: int order = readOrder();
 * -----------------------------------------------------------------------
 *
 * This function asks user for integer number between 1 and 10 inclusive
 * and repeat this question while user not enter a valid number/
 *
 * @return Integer number between 1 and 10 inclusive.
 */
int readOrder(){
    int order = 0;

    while (true) {
        cout << "Order [1-10]: ";
        cin >> order;
        if (order >= 1 && order <= 10) break;
        cout << "Order must be between 1 and 10." << endl;
    }
    return order;
}


/* Function: generateRandomText
 * Usage: string randomText = generateRandomText(infile, order);
 * -----------------------------------------------------------------------
 *
 * This function generates random text besed on the text recived
 * in the input file stream using Markov model of the received order.
 *
 * @param &infile Link to the input file stream with opened file
 * @param order Integer number of Markov model order
 * @return Generated random text in the string format
 */
string generateRandomText(ifstream &infile, int order){
    string result = "";
    Map<string, string> seeds;
    Vector<string> mostFrequentSeeds;
    string currentSeed = "";

    seeds = readSeeds(infile, order); //analyze text and write patterns

    mostFrequentSeeds = findFrequentSeeds(seeds); //find most frequently used seeds
    currentSeed = mostFrequentSeeds[randomInteger(0, mostFrequentSeeds.size() - 1)]; //randomly select one popular seed

    result += currentSeed; //add this seed to the text

    while (result.size() <= RANDOM_TEXT_CHARACTERS){ //write 2000 characters text
        result += seeds[currentSeed][randomInteger(0, seeds[currentSeed].size() - 1)]; //add to the text randomly selected character that follows this seed
        currentSeed = result.substr(result.size() - order); //move to the last seed in new text
    }
    return result;
}

/* Function: readSeeds
 * Usage: seeds = readSeeds(infile, order);
 * --------------------------------------------------------------------------
 *
 * This function read seeds with received order for Markov model from received
 * input file stream. It read number of characters equal to @order at file begining
 * and write it as a key in Map, next character adds to value of this key. Then shifts
 * cursor on a one character and repeat operatio until file ends.
 *
 * @param &infile Link to the input file stream with opened file
 * @param order Integer number of Markov model order
 * @return Map with all seeds as a keys and all characters after this seeds as a value
 */
Map<string, string> readSeeds(ifstream &infile, int order) {
    Map<string, string> seeds;
    string seed = "";

    char ch;
    while (infile.get(ch)) {
        if (seed.size() < order){ //read first characters to seed
            seed += ch;
        } else {
            seeds[seed] += ch; //add next character tu value
            seed = seed.erase(0, 1) + ch; //shift seed on the one character
        }
    }
    return seeds;
}


/* Function: findFrequentSeeds
 * Usage: mostFrequentSeeds = findFrequentSeeds(seeds);
 * ----------------------------------------------------------------------------
 *
 * This function finds most frequently used seeds in the text.
 * The frequency of use is determined by the number of characters
 * in the value of the corresponding seed.
 *
 * @param &seeds Map with all seeds as a keys and characters after them as value
 * @return Vector of string with all most popular seeds
 */
Vector<string> findFrequentSeeds(Map<string, string> &seeds){
    Vector<string> result;
    string popularSeed = "";
    int maxLength = 0;

    for (string key: seeds){ // go through all the keys of Map
        if(seeds[key].size() > maxLength){
            maxLength = seeds[key].size();
            popularSeed = key; //write most popular seed at this moment
        }
    }
    for (string key: seeds){ //write all seeds with the same length to array
        if (seeds[key].size() == seeds[popularSeed].size()){
            result.add(key);
        }
    }

    return result;
}
