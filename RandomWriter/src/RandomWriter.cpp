#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"

using namespace std;

string promptUserForFile(ifstream & infile, string prompt); //ask for input file
Map<string, Vector<char>> makeMap(Vector<char> vec, int order);
string arToStr(Vector<char>);
string startOfRes(Map<string, Vector<char>>);
string makeRandomTail(string, Map<string, Vector<char>>, int);

int main() {
    ifstream infile;
    promptUserForFile(infile, "Enter filename: ");
    int order =  getInteger("Enter number of order (from 1 to 10):");
    Vector<char> inputFileRead;
    char ch;
    while (infile.get(ch)){
        if (ch == EOF) break;
        inputFileRead.add(ch);
    }
    Map<string, Vector<char>> map = makeMap(inputFileRead, order);
    string a = startOfRes(map);
    cout << "RANDOM TEXT: " << makeRandomTail(a, map, order) << endl;
    return 0;
}
/*
* function create random text
*
* @param first string from startOfRes,
* map from makeMap,
* oreder - current seed that entered user
* @return random text
*/
string makeRandomTail(string str, Map<string, Vector<char>> map, int order){
    string res;
    res += str;
    for(int i = 2000 - order; i > 0; i--){
        string lastOrder = res.substr(res.length() - order, res.length());
        if(!map.containsKey(lastOrder)){
            return res;
        }
        Vector<char> adding = map.get(lastOrder);
        string addToRes = arToStr(adding);
        int rnd = randomInteger(0, addToRes.length()-1);
        char ch = addToRes[rnd];
        res += ch;
    }
    return res;
}
/*
* choose random first word, To start random text.
*
* @param Map of seeds and next char <seed, char>
* @return first seed of random text
*/
string startOfRes(Map<string, Vector<char>> map){
    Map<string, Vector<char>> bestKey;
    Vector<string> whereGetStartWord;
    string res = "";
    int maxNum = 0;
    for(string j : map){
        Vector<char> tmp = map.get(j);
        int curNum = tmp.size();
        if(curNum > maxNum){
            maxNum = curNum;
        }
    }
    for(string u : map){
        Vector<char> tmp = map.get(u);
        if(tmp.size() == maxNum){
            bestKey.put(u, tmp);
        }

    }
    whereGetStartWord = bestKey.keys();
    int rand = randomInteger(0,(whereGetStartWord.size() - 1));
    res = whereGetStartWord[rand];
    return res;
}

/*
* crate Map like key"re" - vector"a, b, g";
*
* @param vector<char>
* @return string from vector
*/
Map<string, Vector<char>> makeMap(Vector<char> vec, int order){
    Map<string, Vector<char>> map;
    for(int i = 0; i < vec.size() - order - 1; i++){
        Vector<char> sub = vec.subList(i, order);
        string kkey = arToStr(sub);
        Vector<char> vvalue;
        vvalue.add(vec.get(i + order));
        if(map.containsKey(kkey)){
            Vector<char> add = map.get(kkey);
            map.remove(kkey);
            add += vvalue;
            map.put(kkey, add);
            add.clear();
        }
        if(map.containsKey(kkey) == false){
            map.put(kkey, vvalue);
        }
        }
       return map;
}

/*
* transfors array of <char> in one string
*
* @param vector<char>
* @return string from vector
*/
string arToStr(Vector<char> vec){
    string res;
    for(char ch : vec){
        res += ch;
    }
    return res;
}

//ask user to enter file name
string promptUserForFile(ifstream & infile, string prompt ){
    while(true){
        cout << prompt;
        string filename;
        getline(cin, filename);
        infile.open(filename.c_str());
        if(!infile.fail()) return filename;
        infile.clear();
        cout << "UNABLE TO OPEN FILE! Try again." << endl;
        if(prompt == ""){
            prompt = "Input filename: ";
        }
    }
}
