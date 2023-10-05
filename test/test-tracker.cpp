#include "../src/tracker.hpp"

#include <string>
#include <vector>
#include <iomanip>

#include <CppUTest/TestHarness.h>
#include <CppUTest/TestOutput.h>

TEST_GROUP(TEST_TRACKER) { };

TEST(TEST_TRACKER, constructionAndDestruction) {
  // Make sure there is no exceptions or memory leaks
  std::set<std::string> emptyset;
  Tracker tracker(emptyset);
}

TEST(TEST_TRACKER, AddSequences) {
  std::set<std::string> dict = { "solid", "liquid", "gas", "plasma", "one", "two", "three", "four", "five" };
  Tracker tracker(dict);
  // Add words without clearing
  std::vector<std::string> words = { "solid", "liquid", "gas", "plasma" };
  for (auto& word: words) {
    tracker.add(word);
  }
  std::vector<std::string> next_words = { "one", "two", "three", "four", "five" };
  for (auto& word: next_words) {
    tracker.add(word);
  }
  // lets verify the output. Reverse order!
  std::vector<std::string> expectedSeqs = {
    "two three four",
    "three four five",
    "solid liquid gas",
    "plasma one two",
    "one two three",
    "liquid gas plasma",
    "gas plasma one"
  };
  std::stringstream expected;
  for (auto& expectedSeq: expectedSeqs) {
    expected
      << std::left
      << std::setw(60) << expectedSeq
      << std::setw(40) << 1
      << std::endl;
  }
  CHECK_EQUAL(expected.str(), tracker.getTopHundred());
}

TEST(TEST_TRACKER, AddSequencesWithClear) {
  std::set<std::string> dict = { "solid", "liquid", "gas", "plasma", "one", "two", "three", "four", "five" };
  Tracker tracker(dict);
  // Add words
  std::vector<std::string> words = { "solid", "liquid", "gas", "plasma" };
  for (auto& word: words) {
    tracker.add(word);
  }
  tracker.clearThreeWords();
  std::vector<std::string> next_words = { "one", "two", "three", "four", "five" };
  for (auto& word: next_words) {
    tracker.add(word);
  }
  // lets verify the output. Reverse order!
  std::vector<std::string> expectedSeqs = {
    "two three four",
    "three four five",
    "solid liquid gas",
    "one two three",
    "liquid gas plasma"
  };
  std::stringstream expected;
  for (auto& expectedSeq: expectedSeqs) {
    expected
      << std::left
      << std::setw(60) << expectedSeq
      << std::setw(40) << 1
      << std::endl;
  }
  CHECK_EQUAL(expected.str(), tracker.getTopHundred());
}

TEST(TEST_TRACKER, AddSequencesWithPunctuation) {
  std::set<std::string> dict = { "solid", "liquid", "gas", "plasma", "one", "two", "three", "four", "five" };
  Tracker tracker(dict);
  // Add words
  std::vector<std::string> words = { "solid's", "liquid,", ".gas", "plasma" };
  for (auto& word: words) {
    tracker.add(word);
  }
  tracker.clearThreeWords();
  std::vector<std::string> next_words = { "one", "two!", "three-", "four?", "five" };
  for (auto& word: next_words) {
    tracker.add(word);
  }
  // lets verify the output. Reverse order!
  std::vector<std::string> expectedSeqs = {
    "two three four",
    "three four five",
    "solid's liquid gas",
    "one two three",
    "liquid gas plasma"
  };
  std::stringstream expected;
  for (auto& expectedSeq: expectedSeqs) {
    expected
      << std::left
      << std::setw(60) << expectedSeq
      << std::setw(40) << 1
      << std::endl;
  }
  CHECK_EQUAL(expected.str(), tracker.getTopHundred());
}

TEST(TEST_TRACKER, AddSequencesWithHyphen) {
  std::set<std::string> dict = { "solid", "liquid", "gas", "plasma", "one", "two", "three", "four", "five" };
  Tracker tracker(dict);
  // Add words
  std::vector<std::string> words = { "solid's", "liquid,", "gas-plasma" };
  for (auto& word: words) {
    tracker.add(word);
  }
  tracker.clearThreeWords();
  std::vector<std::string> next_words = { "on-", "e", "two!", "three-", "four?", "five" };
  for (auto& word: next_words) {
    tracker.add(word);
  }
  // lets verify the output. Reverse order!
  std::vector<std::string> expectedSeqs = {
    "two three four",
    "three four five",
    "solid's liquid gas",
    "one two three",
    "liquid gas plasma"
  };
  std::stringstream expected;
  for (auto& expectedSeq: expectedSeqs) {
    expected
      << std::left
      << std::setw(60) << expectedSeq
      << std::setw(40) << 1
      << std::endl;
  }
  CHECK_EQUAL(expected.str(), tracker.getTopHundred());
}
