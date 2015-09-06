#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"\
//#include <string>
using namespace std;

string findNextWord(string word, Lexicon &english, Lexicon &alreadyUsed);
void wordLadder(Lexicon &english, string startWord, string endWord);


int main() {
   //Create a vocabulary for checking the validity of the newly created words
   Lexicon english("EnglishWords.txt");

   while(true){
    //Setting the initial word
    string startWord = getLine("Enter source word (RETURN to quit): ");

   //Setting the final word
    string endWord = getLine("Enter destination word: ");

   //The search function of the shortest ladder dictionary
   wordLadder(english,startWord,endWord);
   }
	return 0;
}



/*The search function of the shortest ladder dictionary.
Takes to verify the validity of the lexicon, the word of the initial and final*/
void wordLadder(Lexicon &english, string startWord, string endWord)
{
   // Creating a queue to place ladders dictionary
    Queue<Vector<string> > queueWordLadders;

   //Lexicon to have used words
    Lexicon alreadyUsed;

   //The line for the newly generated words
    string nextWord;

   //Clear the queue
    queueWordLadders.clear();

   //Create a vector and move it to the initial word. We put the vector at the end of the queue.
    Vector<string> addToQueue;
    addToQueue.add(startWord);   
    queueWordLadders.enqueue(addToQueue);

   // We put the initial word in the lexicon already using the word.
    alreadyUsed.add(startWord);


   //*************Cycle find the shortest ladder dictionary************************
    bool start = true;
    while (start)
    {

        /* build up a new list(Vector<string>) of items to search, or stop if empty*/
        if (queueWordLadders.size() == 0)
        {
            cout << "Nope nothing found! no ladder"<< endl;
            break;
        }

        /*Create a list and we set to the first element in the queue.
        In turn, this element is then removed.*/
        Vector<string> firstLadderToQueue = queueWordLadders.dequeue();


        /*The cycle of adding all the vocabulary of stairs to the difference in
        the words of one letter (one-step, two-step ladder and so on
        until you find the shortest ladder final of elements that will be the final word).*/

        /***********The cycle of creation of the dictionary of stairs and find the shortest**********/
        while (true)
        {

            // find the next word to use
            nextWord = findNextWord(firstLadderToQueue[firstLadderToQueue.size()-1], english, alreadyUsed);
            // the end of the search
            if (nextWord == endWord)
            {
                cout << "FOUND ladder !!" << endl;
                for (string list: firstLadderToQueue)
                {
                    cout << list << " -> ";
                }
                cout << endWord << endl << endl;
                start = false;
                break;
            } else if (nextWord != "")
            {
                /*if there is another word to search with add to the end of the list.
                Create a copy of the first list in the queue
                added to his word and put a list of the queue.*/
                Vector<string> addMoreLadder = firstLadderToQueue;
                addMoreLadder.add(nextWord);
                queueWordLadders.enqueue(addMoreLadder);
            }
            // else if nothing left to search for stop!!
            else if (nextWord =="")
                break;
            //Add a word to the list of already used
            alreadyUsed.add(nextWord);

        }
    }
}



/*Creating a word that differs from the input one letter and a test of its validity
The function takes an input word, the vocabulary of English words, the lexicon has used words.
The function returns a word which is different from the input one letter.*/
string findNextWord(string word, Lexicon &english, Lexicon &alreadyUsed)
{
    string newWord;
    for (int i =0; i < word.length(); i++)
    {
        newWord = word;
        for (char ch = 'a'; ch <= 'z'; ch++)
        {
            // create a new word
            newWord = newWord.replace(i,1,1, ch);
            // make sure it is a valid word and also not already used
            if (english.contains(newWord))
                if (!alreadyUsed.contains(newWord))
                    return newWord;
        }
    }
    //If the word is not valid and is already used returns an empty string.
    return "";
}
