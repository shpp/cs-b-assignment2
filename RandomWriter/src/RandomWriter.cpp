#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"

using namespace std;

string promptUserForFile(ifstream & infile, string prompt); //ask for input file


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

cout << inputFileRead <<endl;
Vector<char> sub = inputFileRead.subList(0, order);
cout << sub << endl;

    return 0;
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
        cout<<"UNABLE TO OPEN FILE! Try again."<<endl;
        if(prompt == ""){
            prompt = "Input filename: ";
        }
    }
}
