#ifndef __TRACKER__
#define __TRACKER__

#include <string>
#include <vector>
#include <map>
#include <set>

class Tracker {
  private:
    std::set<std::string>               dictWords;

    std::map<std::string, unsigned int> freqCounts;
    std::vector<std::string>            threeWords;
    std::string                         maybeNextWord;

  public:
                Tracker         (std::set<std::string> _dictWords);
    virtual     ~Tracker        ();

    void        add             (std::string _new_word);
    void        clearThreeWords ();
    std::string getTopHundred   ();
};

#endif // __TRACKER__
