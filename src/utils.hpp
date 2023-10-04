#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <vector>

namespace Utils {
  std::string              removeSpaces     (std::string _word);

  bool                     hasHyphen        (std::string _word);
  std::vector<std::string> splitHyphen      (std::string _word);

  bool                     isPunct          (char _char);
  std::string              trimPunctuation  (std::string _word); // Trim the ends of punctuation
  std::string              scrubPunctuation (std::string _word); // Remove any unwanted punctuation

  std::string              scrubUTF8        (std::string _word); // Removing some UTF-8 characters
}

#endif // __UTILS__
