#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

using namespace std;

struct Statistic{
    char letter;
    int quantity;
    double procent;
};

// global variables  section

// main map of seeds and their letter statistics
Map <string, Vector <Statistic>> seedsMap;

// map of seed's frequency
Map <string, int> seedFrequency;

// model level
int modelLevel=0;

// length of output text
const int numLettersToGenerate=2000;
//

/**
 * @brief getValidFileFromUser ask user for a file name and validate it
 * @return validated file name as string
 */
string getValidFileFromUser(){
    string fName="";
    while(true){
        cout << "Enter file name: ";
        cin >> fName;
        ifstream ifs (fName);
        if (ifs.is_open()){
            break;
        } else {
            cout << "That file could not be opened. Try again. \n";
        }
    }
    return fName;
}

/**
 * @brief getValidModelLevelFromUser ask user for a valid model level
 * @return model level in range 1-10
 */
int getValidModelLevelFromUser(){
    int res=0;
    while(true){
        cout << "Enter model level (1-10): ";
        cin >> res;
        if ((res>0)&(res<=10)){
            break;
        } else {
            cout << "That level is incorrect. Try again. \n";
        }
    }
    return res;
}

/**
 * @brief incrementCounterSeeds increment counter of frequecy for current seed
 * @param seed, frequency of which must be increased.
 */
void incrementCounterSeeds(string lastSeed){
    int counter=0;
    if(seedFrequency.containsKey(lastSeed))
        counter=seedFrequency.get(lastSeed);
    counter++;
    seedFrequency.put(lastSeed,counter);
}

/**
 * @brief incrementCounterSeedsLetters increment counter of letter frequency for current seed.
 * @param seed - working seed
 * @param c - letter, frequency of which must be increased.
 */
void incrementCounterSeedsLetters(string seed, char c){
    Vector <Statistic> statVector;

    if(seedsMap.containsKey(seed))
        statVector=seedsMap.get(seed);

    bool isNewLetter=true;

    for(int i=0;i<statVector.size();i++){
        if(statVector[i].letter==c){
            Statistic tmp=statVector[i];
            tmp.quantity++;
            statVector.set(i,tmp);
            isNewLetter=false;
        }
    }

    if(isNewLetter){
        Statistic tmp;
        tmp.letter=c;
        tmp.quantity=1;
        tmp.procent=0;
        statVector.add(tmp);
    }

    seedsMap.put(seed,statVector);
}

/**
 * @brief loadSeedMapsFromFile read and analise text file
 * @param fName - name of file
 */
void loadSeedMapsFromFile(string fName){
    ifstream ifs (fName);

    string seed="";

    while (!ifs.eof()) {
        char c = ifs.get();
        if (((int)(c)==-1)){
            continue;
        }
        // debag
        //cout << c;

        string lastSeed = seed;

        seed+=c;

        if(seed.length() <= modelLevel)
            continue;

        incrementCounterSeeds(lastSeed);
        incrementCounterSeedsLetters(lastSeed,c);

        // cut first letter
        seed=seed.substr(1,modelLevel);
    }
}

/**
 * @brief showSeedsMap proc needed only for debag - show full seeds map with letter's statistic
 */
void showSeedsMap(){
    for(auto elem:seedsMap){
        Vector <Statistic> tmp;
        tmp=seedsMap.get(elem);
        cout << elem << " -> ";
        for(auto elv:tmp){
            cout << " letter "<< elv.letter << "-" << elv.quantity<< "%" << elv.procent << " ";
        }
        cout << endl;
    }
}

/**
 * @brief countStatProcents - count percent of frequency for each letter of each seed.
 */
void countStatProcents(){
    for(auto elem:seedsMap){
        Vector <Statistic> statVector=seedsMap.get(elem);
        int totalLettersInSeedVector=0;
        for(auto letterStat:statVector){
            totalLettersInSeedVector+=letterStat.quantity;
        }
        for(int i=0;i<statVector.size();i++){
            Statistic tmp=statVector[i];
            tmp.procent=(double)tmp.quantity/totalLettersInSeedVector;
            statVector.set(i,tmp);
        }
        seedsMap.put(elem,statVector);
    }
}

/**
 * @brief getMostFrequentlyUsedSeed - seek most frequently used seed
 * @return founded seed
 */
string getMostFrequentlyUsedSeed(){
    int maxSN=0;
    string maxSeed="";
    for(auto el:seedFrequency){
        int timesUsed=seedFrequency.get(el);
        if(timesUsed>maxSN){
            maxSN=timesUsed;
            maxSeed=el;
        }
    }
    return maxSeed;
}

/**
 * @brief findLetterForSeed - randomly choose next letter for current seed, depending to letter's frequency statistic.
 * @param seed - base seed
 * @return founded char
 */
char findLetterForSeed(string seed){
    char res='o';
    Vector<Statistic> lettersStat=seedsMap.get(seed);
    double rnddbl=randomReal(0, 1);
    double currentSum=0;
    for(auto letterStat:lettersStat){
        currentSum+=letterStat.procent;
        if(currentSum>rnddbl){
            res=letterStat.letter;
            break;
        }
    }
    return res;
}

/**
 * @brief generateRandomText - generate and show on screen randomized text
 * @param startSeed - start seed, which begin from.
 */
void generateRandomText(string startSeed){
    cout << startSeed;
    string seed=startSeed;

    for(int i=0;i<numLettersToGenerate;i++){
        char c=findLetterForSeed(seed);
        cout << c;
        seed=seed.substr(1,modelLevel-1)+c;
    }
}

/**
 * main proc.
 */
int main() {

    string fName=getValidFileFromUser();

    modelLevel=getValidModelLevelFromUser();

    loadSeedMapsFromFile(fName);

    countStatProcents();

    // debag
    //showSeedsMap();

    string startSeed=getMostFrequentlyUsedSeed();

    // debag
    //cout <<endl<<"Start seed: "<< startSeed;

    generateRandomText(startSeed);

    return 0;
}
