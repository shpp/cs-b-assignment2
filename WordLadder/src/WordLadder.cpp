#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

/**
 * Find and return all "neighbor" words of the current word except the used words.
 *
 * @param word - The current word.
 * @param englishWords - All english words.
 * @param usedWords - The used words.
 * @return - All "neighbor" words.
 */
Vector<string> getNeighborWords(string word, Lexicon &englishWords, Lexicon &usedWords) {
    Vector<string> result;
    // Add current word to the list of used words if it is not there.
    if (!usedWords.contains(word))
        usedWords.add(word);
    // Generate new words from the current word by change one char.
    for (int i = 0; i < word.length(); i ++) {
        string copyWord = word;
        for (char c = 'a'; c <= 'z'; c ++){
            copyWord.replace(i, 1, 1, c);
            // Add new generated word to the list of "neighbor" words and to the list of used words
            // if it is a real english word and it is not used earlier.
            if ((englishWords.contains(copyWord)) && (!usedWords.contains(copyWord))) {
                result.add(copyWord);
                usedWords.add(copyWord);
            }
        }
    }
    return result;
}

/**
 * Put the ladder to the queue.
 *
 * @param ladder - The ladder.
 * @param ladders - The ladders queue.
 */
void putToQueue(Vector<string> ladder, Queue<Vector<string>> &ladders) {
    ladders.enqueue(ladder);
}

/**
 * Put the word to the queue. Create the first ladder.
 *
 * @param word - The first word in the queue.
 * @param ladders - The ladders queue.
 */
void putToQueue(string word, Queue<Vector<string>> &ladders) {
    Vector<string> ladder(1, word);
    ladders.enqueue(ladder);
}

/**
 * Get the ladder from the queue.
 *
 * @param ladders - The ladders queue.
 * @return - The ladder.
 */
Vector<string> getFromQueue(Queue<Vector<string>> &ladders) {
    return ladders.dequeue();
}

/**
 * Find and return the shortest ladder from source word to the destination word.
 *
 * @param sourceWord - The source word.
 * @param destWord - The destination word.
 * @param englishWords - All english words.
 * @return - The found shortest ladder if it exists or empty ladder in other case.
 */
Vector<string> findShortestLadder(string sourceWord, string destWord, Lexicon & englishWords) {
    Queue<Vector<string>> ladders;
    Lexicon usedWords;
    // Put the sourse word to the queue.
    putToQueue(sourceWord, ladders);
    // Repeat search while ladders queue contains ladders.
    while (!ladders.isEmpty()) {
        // Get ladder from queue and get its last word.
        Vector<string> ladder = getFromQueue(ladders);
        string lastWord = ladder[ladder.size() - 1];
        // Return the ladder if the last word of the ladder is the destination word.
        if (lastWord == destWord)
            return ladder;
        // Get "neighbor" words of the last word in the ladder except the used words.
        Vector<string> neighborWords = getNeighborWords(lastWord, englishWords, usedWords);
        // For each "neighbor" word create new ladder by add it word to the end of the ladders copy.
        // The new ladder put to the queue.
        for (string word : neighborWords) {
            Vector<string> copyLadder = ladder;
            copyLadder.add(word);
            putToQueue(copyLadder, ladders);
        }
    }
    // Return the empty ladder if the shortest ladder is not found.
    Vector<string> ladder;
    return ladder;
}

/**
 * Output the shortest ladder if it is found or info message in other case.
 *
 * @param shortestLadder - The ladder.
 */
void showResults(Vector<string> ladder) {
    if (ladder.size() != 0)
        for (int i = 0; i < ladder.size(); i ++)
            if (i != ladder.size() - 1)
                cout << ladder[i] << " -> ";
            else
                cout << ladder[i] << endl;
    else
        cout << "No ladder found." << endl;
}

int main() {
    Lexicon englishWords("EnglishWords.txt");
    while (true) {
        string sourceWord = getLine("Enter source word (RETURN to quit): ");
        if (sourceWord.length() == 0)
            break;
        string destWord = getLine("Enter destination word: ");
        Vector<string> shortestLadder = findShortestLadder(sourceWord, destWord, englishWords);
        showResults(shortestLadder);
    }
	return 0;
}
