#include <unistd.h>

#include "tracker.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Couple of assumptions
//   - It can read files based on the arguments
//   - It can read text from stdin
//   - It do both simultaneously
int main(int _argc, char* _argv[]) {
  // Let's read in the dictionary for the tracker to help determine
  // if a hyphen is a punctuation or a continuation.
  std::set<std::string> dictWords;
  std::ifstream words;
  words.open("dict/words");
  std::string word_input;
  while (words >> word_input) {
    dictWords.insert(word_input);
  }

  Tracker tracker(dictWords);
  // Deterimine if the input is coming from a pipe
  // NOTE: Had to look it up for a specific implemenatation as I never had to do it before
  //       and useful to have in the future.
  //   - https://stackoverflow.com/a/20330225
  if (!isatty(fileno(stdin))) {
    // Its a pipe.  Read in the text before continuing with checking files
    //   This method is deliminating by non-printable characters so let's take advantage
    std::string piped_input;
    while (std::cin >> piped_input) {
      tracker.add(piped_input);
    }
    tracker.clearThreeWords();
  }

  // Read in the command line arguments to determine the files to read
  std::vector<std::string> paths;
  for (int i = 1; i < _argc; ++i) {
    paths.push_back(_argv[i]);
  }

  // Read through each file and follow the same method of reading from stdin for consistency
  for (auto& path: paths) {
    std::ifstream file;
    file.open(path);
    std::string file_input;
    // Same method as reading from the pipe above
    while(file >> file_input) {
      tracker.add(file_input);
    }
    tracker.clearThreeWords();
  }
  std::cout << tracker.getTopHundred() << std::endl;
  return 0;
}
