#include "utils.hpp"

#include <cctype>
#include <sstream>

/*
 * Remove any spaces
 */
std::string Utils::removeSpaces(std::string _word) {
  std::stringstream clean_word;
  for (auto& c: _word) {
    if (isblank(c)) {
      continue;
    }
    // This is usually the first time looking for the word
    // let's lowercase while we are at it.
    clean_word << (char)std::tolower(c);
  }
  return clean_word.str();
}

/*
 * Check if there is hyphen.
 *
 * For so we don't have to check every word if there is hyphen
 */
bool Utils::hasHyphen(std::string _word) {
  return std::string::npos != _word.find('-');
}

/*
 * A hyphen within a "word" could be a joining of words so should be
 * separated.
 *
 * A hyphen at the end can be considered a continuation.
 *
 * Not sure how the best way to determine that it is a continuation without checking
 * if it is a valid word.  In hindsight, due the fact we will have to keep the state
 * of the word.  This should be moved into the Tracker class.
 */
std::vector<std::string> Utils::splitHyphen(std::string _word) {
  std::vector<std::string> splits;
  // Search until hyphen and when found clear and search again
  std::stringstream potential;
  for (auto& c: _word) {
    // found a hyphen add the chunk and look again
    if ('-' == c) {
      splits.push_back(potential.str());
      potential.str("");
      continue;
    }
    potential << c;
  }
  // Add what is left over if not empty
  // probably could have taken advantage of an empty when the last character is a hyphen
  // but makes it harder to debug
  std::string lastword = potential.str();
  if (!lastword.empty()) {
    splits.push_back(lastword);
  }
  return splits;
}

/*
 * In case I encountered cases that ispunct did not consider
 */
bool Utils::isPunct(char _char) {
  if (ispunct(_char)) { return true; }
  if (_char == '\"')  { return true; } // The original issue was a unicode character
				       // probably no longer need
  return false;
}

/*
 * Remove mulitple sequences of punctuation except for if it is next to the
 * characters.
 */
std::string Utils::trimPunctuation(std::string _word) {
  if (_word.empty()) {
    return "";
  }
  // If single character punctuation then simple empty
  if (1 == _word.length() && isPunct(_word.front())) {
    return "";
  } 
  // Trim left
  for (unsigned int i = 0; i < _word.length() - 1; ++i) {
    char& c  = _word[i];
    char& nc = _word[i + 1];
    if (!isPunct(c) || !isPunct(nc)) {
      _word.erase(0, i);
      break;
    }
    // if it reached the end then its all punctuation
    if (ispunct(nc) && (i + 2 == _word.length())) {
      return "";
    }
  }
  // Trim right
  for (unsigned int i = _word.length() - 1; i > 0; --i) {
    char& c  = _word[i];
    char& pc = _word[i - 1];
    if (!isPunct(pc) || !isPunct(c)) {
      _word.erase(i + 1);
      break;
    }
  }
  return _word;
}

/* Assumptions:
 *   The word coming in is already delimited by spaces.
 *   The words are generally valid words and don't contain can'''t
 *
 * Contraction list https://en.wikipedia.org/wiki/Contraction_(grammer)
 *   not             -> n't
 *   let us          -> let's
 *   I'm             -> I'm
 *   are             -> -'re
 *   does, is, has   -> -'s
 *   have            -> -'ve
 *   had, did, would -> -'d
 *   will, shall     -> -'ll
 *   of, of the      -> o'-
 *   is was          -> 'twas
 *   them            -> 'em
 *   you             -> y'-
 *   about           -> 'bout
 *   because         -> 'cause
 *
 *   The only punctuation we have to look out for is apostrophe's
 *   any other punctuation can be naively removed.
 */
std::string Utils::scrubPunctuation(std::string _word) {
  // trim any punctuation
  _word = Utils::trimPunctuation(_word);
  if (_word.empty()) {
    return "";
  }

  char& fc = _word.front();
  // The special cases for first apostrophe
  // it followed by t, e, b and c
  if (fc == '\'') {
    if (_word.length() > 2) {
      char& nc = _word[1];
      if (nc != 't'    // 'twas
       && nc != 'e'    // 'em
       && nc != 'b'    // 'bout
       && nc != 'c') { // 'cause
        _word.erase(0, 1);
      }
    }
  }

  // The special cases for last apostrophe
  // it is preceded by s
  char& bc = _word.back();
  if (bc == '\'') {
    if (_word.length() > 2) {
      char& nc = _word[_word.length() - 2];
      if (nc != 's') {  // -s'
        _word.erase(_word.length() - 1, 1);
      }
    }
  }

  // Remove any non-apostrophes
  std::stringstream scrubbed_word;
  for (unsigned int i = 0; i < _word.length(); ++i) {
    char& c = _word[i];
    if (isPunct(c)) {
      if ('\'' != c) {
        continue;
      }
    }
    scrubbed_word << c;
  }
  return scrubbed_word.str();
}

/*
 * Assumptions:
 *   Only ASCII and UTF-8 encoding.  Assumes code point is not
 *   broken.
 *
 * Simple removal of characters if longer than a byte and replace
 * The original with 'U'.  Keep the original character if it is a
 * 1 byte size code point.
 *
 * https://en.wikipedia.org/wiki/UTF-8
 */
std::string Utils::scrubUTF8 (std::string _word) {
  for (unsigned int i = 0; i < _word.length(); ++i) {
    // If single byte then do nothing
    //   The first bit in the code point is 0
    if (((_word[i] >> 7) & 1) == 0) {
      continue;
    }

    // TODO: Can be done in a for-loop shifting a bit at a time, check, remove
    //       next character if 1.

    // If two bytes then collapse to one char
    //   The first and second bit in the code point is 1
    //   Already know first bit not 0 from previous if statement.
    if (((_word[i] >> 6) & 1) == 1) {
      _word.erase(i + 1, 1);
    }

    // If three bytes then collapse to one char
    //   The first three bits in the code point is 1
    if (((_word[i] >> 5) & 1) == 1) {
      _word.erase(i + 1, 1);
    }
    // If four bytes then collapse to one char
    //   The first four bits in the code point is 1
    if (((_word[i] >> 4) & 1) == 1) {
      _word.erase(i + 1, 1);
    }
    // Replace original location with an non-punctuation
    // but valid single byte character.
    _word[i] = 'U';
  }
  return _word;
}
