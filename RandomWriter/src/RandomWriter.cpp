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
using namespace std;

/**
 * Request the file name and open the file if the name is valid.
 *
 * @param inputFile - The file for open.
 */
void openFile(ifstream &inputFile) {
    promptUserForFile(inputFile, "Enter the file name: ");
}

/**
 * Close the file.
 *
 * @param inputFile - The file for close.
 */
void closeFile(ifstream &inputFile) {
    inputFile.close();
}

/**
 * Request the model level.
 *
 * @return - The model level value.
 */
int getModelLevel() {
    int modelLevel;
    while (true) {
        modelLevel = getInteger("Enter model level (1-10): ");
        if ((modelLevel >= 1) && (modelLevel <= 10))
            break;
        cout << "Wrong input data." << endl;
    }
    return modelLevel;
}

/**
 * Read the first seed from the file beginning.
 *
 * @param inputFile - The file for read.
 * @param modelLevel - Set the seed length.
 * @param seed - The read seed.
 * @param nextChar - The next char after the seed.
 * @return - True if the seed and the next char are read, or false in other case.
 */
bool readFirstSeed(ifstream &inputFile, int modelLevel, string &seed, char &nextChar) {
    for (int i = 0; i < modelLevel; i ++) {
        char ch = inputFile.get();
        if (ch == EOF)
            return false;
        seed += ch;
    }
    if (!inputFile.get(nextChar))
        return false;
    return true;
}

/**
 * Read the next seed after the current seed.
 *
 * @param inputFile - The file for read.
 * @param seed - The current seed before reading. The next seed after reading.
 * @param nextChar - The next char after the seed.
 * @return - True if the seed and the next char are read, or false in other case.
 */
bool readNextSeed(ifstream &inputFile, string &seed, char &nextChar) {
    seed.erase(0, 1);
    seed += nextChar;
    if (!inputFile.get(nextChar))
        return false;
    return true;
}

/**
 * Read the all seeds from the file.
 *
 * @param inputFile - The file for read.
 * @param modelLevel - Set the seed length.
 * @return - The map of the read seeds and the next chars.
 */
Map<string, Vector<char>> readAllSeeds(ifstream &inputFile, int modelLevel) {
    Map<string, Vector<char>> result;
    string seed;
    char nextChar;
    cout << "The file is processed..." << endl;
    // Read the first seed. If the first seed exists in the file, add it to result map.
    if (readFirstSeed(inputFile, modelLevel, seed, nextChar))
        result[seed].add(nextChar);
    // Read the next seeds while they exists in the file.
    while (true) {
        if (!readNextSeed(inputFile, seed, nextChar))
            break;
        result[seed].add(nextChar);
    }
    return result;
}

/**
 * Find and return the most frequency seed.
 *
 * @param seeds - The map of the seeds and the next chars.
 * @return - The most frequency seed.
 */
string getFirstSeed(Map<string, Vector<char>> &seeds) {
    Vector<string> keys = seeds.keys();
    string result = keys[0];
    // Find the seed with the longest vector of the next chars.
    for (string key : seeds)
        if (seeds[key].size() > seeds[result].size())
            result = key;
    return result;
}

/**
 * Find the next seed after the current seed by add the next char.
 *
 * @param seeds - The map of the seeds and the next chars.
 * @param seed - The current seed before search. The next seed after search.
 * @return - The next char from the current seeds next chars vector.
 */
char getNextSeed(Map<string, Vector<char>> &seeds, string &seed) {
    Vector<char> nextChars = seeds[seed];
    // Choose the next char from the current seeds next chars vector.
    char result = nextChars[randomInteger(0, nextChars.size() - 1)];
    // Create the next seed by the current seeds one char shifting left and add the choosed next char.
    seed.erase(0, 1);
    seed += result;
    return result;
}

/**
 * Generate randomly english text lenth 2000 chars.
 *
 * @param seeds - The map of the seeds and the next chars.
 * @return - The randomly generated text.
 */
string generateText(Map<string, Vector<char>> &seeds) {
    string seed = getFirstSeed(seeds);
    string text = seed;
    while (true) {
        text += getNextSeed(seeds, seed);
        if (text.length() > 2000)
            break;
    }
    return text;
}

int main() {
    ifstream inputFile;
    openFile(inputFile);
    int modelLevel = getModelLevel();
    Map<string, Vector<char>> seeds = readAllSeeds(inputFile, modelLevel);
    closeFile(inputFile);
    string text = generateText(seeds);
    cout << text << endl;
	return 0;
}
