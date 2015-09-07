#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "hashmap.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
using namespace std;


// Size of the displayed random text
const int TEXT_SIZE = 2000;



/*
 * Prompts the user for a filename and tries to open file. If an unsuccessful attempt
 * to open a file again prompts the user for a filename. Otherwise returns a filename.
 */
string requestForFileName(ifstream &);


/*
 * Reads a file by single characters and creates all possible sequences of a given length and
 * for all of these sequences creates a list of following characters which corresponds to each sequence.
 * @param ifstream &readFile takes a stream from which it reads the file.
 * @param HashMap <string, Vector <char> > &listOfSequences takes an associative array to store sequences as a keys
 * and an arrays of chars with following characters which corresponds to each sequence as a values.
 * @param const int order takes an entered model order for random text which characterizes the length of the sequence.
 */
void readSequencesFromFile(ifstream & readFile, HashMap <string, Vector <char> > & listOfSequences, const int & order);



/*
 * Finds seed for the begining of the random text.
 * @param HashMap <string, Vector <char> > &listOfSequences takes an associative array to store sequences and their following characters.
 * @return string with one of the most common sequence from wich a random text is started.
 */
string findSeed(HashMap <string, Vector <char> > & listOfSequences);



/*
 * Displayed a random text of a predetermined size by constant.
 * In case of getting sequences which there is only at the end of file then displayed text has smaller size.
 * @param HashMap <string, Vector <char> > &listOfSequences takes an associative array to store sequences and their following characters.
 * @param string seed takes a sequence of characters that begins the random text.
 */
void outputRandomText(HashMap <string, Vector <char> > & listOfSequences, string seed);


/*
 *  Prompts the user for a filename based on which the random text will be drawn up and model order for the random text.
 *
 */
int main() {
    ifstream readFile; // create an object of stream class to open a file
    requestForFileName(readFile);

    int order = 0;
    cout << "What number of order do you want to generate text? [1-10]: ";
    cin >> order;

    while ((order < 1) || (order > 10)) {
        cout << "The number of order must be within [1-10]: ";
        cin >> order;
    }

    cout << "Processing..." << endl;

    // create an associative array to store sequences as a keys and an arrays of chars with corresponding following characters as a values
    HashMap <string, Vector <char> > listOfSequences;

    readSequencesFromFile(readFile, listOfSequences, order);
    string seed = findSeed(listOfSequences);    // one of the most common sequence
    outputRandomText(listOfSequences, seed);

    return 0;
}


string requestForFileName(ifstream & readFile){
    while (true){
        string fileName = "";
        cout << "Please, enter file name: " << endl;
        cin >> fileName;
        readFile.open(fileName.c_str()); //leads a string to a c-style to open a file
        if(readFile){
            return fileName;
        }
        readFile.clear(); // resets the status indecator in the stream for reuse it.
        cout << "The file can not be found. Check the correctness of the file name." << endl;
    }
}



void readSequencesFromFile(ifstream & readFile, HashMap <string, Vector <char> > & listOfSequences, const int & order){
    char symbol; // declare a variable for reading a file by single character
    int numOfChars = 0;
    string sequence = "";

    // reads the file while it has next character
    while (readFile.get(symbol)){
        if (numOfChars < order){
            sequence += symbol; // create a first sequence of characters with length which is equal entered order
            ++numOfChars;
        }
        else {
            if (listOfSequences.containsKey(sequence)){
                listOfSequences[sequence].add(symbol); // for existing sequence add new following character to the list of values
            }
            else {
                Vector <char> nextSign;
                nextSign.add(symbol); // creates a new list of following characters which corresponds to the new sequence
                listOfSequences.add(sequence, nextSign);
            }
             // creates next sequence which consists of a previous sequence except the first character and current character
            sequence = sequence.substr(1) + symbol;
        }
    }
    readFile.close();
}


string findSeed(HashMap <string, Vector <char> > & listOfSequences){
    int maxFrequency = 0; // declare a variable for determining the largest list of following characters
    string seed = ""; // declare a variable for one of the most common sequence
    for (string sequence : listOfSequences){
        int frequency = listOfSequences[sequence].size();
        if (frequency >=  maxFrequency) {
            maxFrequency = frequency;
            seed = sequence;
        }
    }
    return seed;
}



void outputRandomText(HashMap <string, Vector <char> > & listOfSequences, string seed){
    int numOfChars = seed.length();
    cout << seed;
    while (numOfChars <= TEXT_SIZE) {
        if(listOfSequences.containsKey(seed)){
            Vector <char> variations = listOfSequences[seed]; //gets a list of characters that are following after current sequence
            int numVariations = variations.size();
            int nextSymbol = randomInteger(0, numVariations - 1); //determines the position of the next symbol randomly
            cout << variations[nextSymbol];
            seed = seed.substr(1) + variations[nextSymbol]; // creates next sequence which will replace the previous seed
            ++numOfChars;
        }
        else{
            break; // if getting sequences there is only at the end of file then stops text output
        }
    }
    return;
}
