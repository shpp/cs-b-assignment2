#include <iostream>
#include <fstream>
#include <string>
#include "simpio.h"
#include "strlib.h"
#include "console.h"
#include "tokenscanner.h"
using namespace std;

void enterFileName(ifstream & infile);
void fleschKincaidTest(ifstream & infile);
int syllablesIn(string word);
bool isVowel(char ch);
double calculateGradeLevel(int words, int syllables, int sentences);

/* Main program */
int main() {
    ifstream infile;
    enterFileName(infile);
    fleschKincaidTest(infile);
    infile.close();
    return 0;
}

/* Function: enterFileName
 * Usage: enterFileName(infile);
 * --------------------------------------------------------------
 *
 * This function asks user for filename, try to open file with this
 * filename and if it wrong asks user again. Function is interrupted
 * if file opened successfully.
 *
 * @param &infile Link to the input file stream
 */
void enterFileName(ifstream & infile){

    while (true) {
        string fileName = getLine("Enter input file name: ");
        infile.open(fileName.c_str());
        if(!infile.fail()) break;
        infile.clear();
        cout << "Unable to open that file. Try again." << endl;
    }
}

/* Function: fleschKincaidTest
 * Usage: fleschKincaidTest(infile);
 *
 * This function evaluates the text from a file on the complexity
 * of its reading using a Flesch Kincaid grade level test.
 * At first it splits the text into words and punctuation using TokenScanner class.
 * Then calculates number of the words, syllables and sentences in the text
 * and calculates grade level using a special formula.
 *
 * @param &infile Link to the input file stream with opened file
 */
void fleschKincaidTest(ifstream &infile){
    TokenScanner scanner(infile);
    string token = "";
    int wordCounter = 0;
    int syllableCounter = 0;
    int sentenceCounter = 0;
    double grade = 0.0;

    scanner.ignoreWhitespace();
    scanner.addWordCharacters("'");

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();

        /*consider that it a word if the first character is letter or digit*/
        if (isalpha(token[0])){
            wordCounter++;
            syllableCounter += syllablesIn(token);
            //cout << token << " syllables: " << syllablesIn(token) << endl;
        } else if (token[0] == '.' || token[0] == '!' || token[0] == '?'){
            sentenceCounter++; //count end of sentence symbols
        }
    }

    if (sentenceCounter == 0) sentenceCounter = 1;

    grade = calculateGradeLevel(wordCounter, syllableCounter, sentenceCounter);

    cout << "Words: " << wordCounter << endl;
    cout << "Syllables: " << syllableCounter << endl;
    cout << "Sentences: " << sentenceCounter << endl;
    cout << "Grade level: " << grade << endl;

}

/* Function: syllablesIn
 * Usage: syllableCounter += syllablesIn(token);
 * -----------------------------------------------------------------------------
 *
 * This function calsulates the number of syllables in the word. It count
 * number of vowels in the word except vowels before that are other vowels
 * end 'e' at the end of the word.
 *
 * @param word The word to count number syllables
 * return Number of syllables in word
 */
int syllablesIn(string word){
    int counter = 0;
    word = toLowerCase(word);

    for (int i = 0; i < word.length(); i++){
        if (isVowel(word.at(i))) {

            if (i == 0){ // if first symbol is vowel
                counter++;

            /*If symbol beetwen first and last is wowel and before it not vowel*/
            } else if(i > 0 && i < word.length() - 1 && !isVowel(word.at(i-1))){
                  counter++;

            /* If last symbol is vowel but not 'e'*/
            } else if (word.length() > 1 && i == word.length() - 1 && word.at(i) != 'e' && !isVowel(word.at(i-1))){
                counter++;
            }
         }
    }

    /*any word have minimum one syllable*/
    if (counter == 0){
        return 1;
    }
    return counter;
}

/* Function: isVowel
 * Usage: if(isVowel(word.at(i)))...
 * ----------------------------------------------------
 *
 * Return TRUE if received charecter is vowel
 * and FALSE if no.
 *
 * @param ch Character that will be checked
 * @return Boolean result of the check
 */
bool isVowel(char ch){

    switch (ch) {
    case 'a': case 'e': case 'i': case 'o': case 'u': case 'y':
        return true;


    default:
        return false;
    }
}

/* Function: calculateGradeLevel
 * Usage: grade = calculateGradeLevel(wordCounter, syllableCounter, sentenceCounter);
 * ---------------------------------------------------------------------------------
 *
 * This function calculates the grade level of the text using number of the words,
 * number of the syllables, number of sentences and some strange
 * constants in the special formula.
 *
 * @param word Number of words it text
 * @param syllables Number of syllables in text
 * @param sentences Number of sentences
 * @return Grade level of the text
 */
double calculateGradeLevel(int words, int syllables, int sentences){
    /* Some constants for test*/
    const double c0 = -15.59;
    const double c1 = 0.39;
    const double c2 = 11.8;

    double result = 0.0;

    result = c0 + (c1 * (double(words) / sentences)) + double(c2 * (double(syllables) / words));
    return result;
}
