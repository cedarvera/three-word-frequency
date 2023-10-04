#include "tracker.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "utils.hpp"

using namespace Utils;

/*
 * Constructor
 */
Tracker::Tracker() { }

/*
 * Destructor.  Nothing to do.
 */
Tracker::~Tracker() { }

/*
 * Add a potential new word to the list.  Once it collects three words
 * it will begin tracking the freqeuency of the sequence.
 */
void Tracker::add(std::string _new_word) {
  // This is a naive approach to handling continuation
  if (!maybeNextWord.empty()) {
    _new_word.insert(0, maybeNextWord);
    maybeNextWord = "";
    // TODO: Verify word in dictionary?
  }
  std::string scrubbed_new_word = scrubUTF8(removeSpaces(_new_word));
  // the removeSpaces is mainly to clear up input that only had
  // non-printable characters
  scrubbed_new_word = scrubPunctuation(scrubbed_new_word);
  if (scrubbed_new_word.empty()) {
    return;
  }

  // Let's check for hyphens to see if we have to split words
  if (hasHyphen(scrubbed_new_word)) {
    std::vector<std::string> splits = splitHyphen(scrubbed_new_word);
    
    // We want to stop at the one before last
    for (unsigned int i = 0; i < splits.size() - 1; ++i) {
      threeWords.push_back(splits[i]);
    }
    // If hyphen is at end then assume end of line will be a continuation and add to next check
    if ('-' == scrubbed_new_word.back()) {
       maybeNextWord = splits.back();
    }
  }
  else {
    threeWords.push_back(scrubbed_new_word);
  }

  // If there more than three words then we want to add it to the frequency counts
  while (threeWords.size() >= 3) {
    std::string seq = threeWords[0] + " " + threeWords[1] + " " + threeWords[2];
    if (freqCounts.find(seq) == freqCounts.end()) {
      freqCounts[seq] = 0;
    }
    ++freqCounts[seq];
    threeWords.erase(threeWords.begin());
  }
}

/*
 * Clear the last three words.  When reading a new file we want to
 * start for scratch.
 */
void Tracker::clearThreeWords() {
  threeWords.clear();
}

/*
 * Get the top hundred in a pre-formatted string
 */
std::string Tracker::getTopHundred() {
  std::vector<std::pair<int, std::string>> sortByVal;
  for (auto& kv: freqCounts) {
    sortByVal.push_back(std::pair<int, std::string>(kv.second, kv.first));
  }
  // More expensive than sorting by descending but works for ease of use
  std::sort(sortByVal.begin(), sortByVal.end());
  std::reverse(sortByVal.begin(), sortByVal.end());

  std::stringstream topHundred;
  // Making a guess of the sequence length will not be too long
  for (unsigned int i = 0; i < 100 && i < sortByVal.size(); ++i) {
    // Style I generally prefer if the line becomes too long
    topHundred
      << std::left
      << std::setw(60) << sortByVal[i].second
      << std::setw(40) << sortByVal[i].first
      << std::endl;
  }
  return topHundred.str();
}