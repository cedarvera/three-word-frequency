#ifndef __TRACKER__
#define __TRACKER__

#include <string>
#include <vector>
#include <map>

class Tracker {
  private:
    std::map<std::string, unsigned int> freqCounts;
    std::vector<std::string>            threeWords;
    std::string                         maybeNextWord;

  public:
    Tracker          ();
    virtual ~Tracker ();

    void        add             (std::string _new_word);
    void        clearThreeWords ();
    std::string getTopHundred   ();
};

#endif // __TRACKER__
