#include <CppUTest/TestHarness.h>
#include <CppUTest/TestOutput.h>

#include "../src/utils.hpp"

using namespace Utils;

TEST_GROUP(TEST_UTILS) { };

TEST(TEST_UTILS, removeSpaces) {
  CHECK_EQUAL("apple", removeSpaces("a p p l e"));
  CHECK_EQUAL("orange", removeSpaces("orange  "));
  CHECK_EQUAL("peach", removeSpaces("  peach"));
  CHECK_EQUAL("grape", removeSpaces("  grape  "));
  CHECK_EQUAL("", removeSpaces("      "));
}

TEST(TEST_UTILS, hasHyphen) {
  CHECK_EQUAL(true, hasHyphen("car-truck"));
  CHECK_EQUAL(true, hasHyphen("plane-"));
  CHECK_EQUAL(true, hasHyphen("-boat"));

  CHECK_EQUAL(false, hasHyphen("carriage"));
  CHECK_EQUAL(false, hasHyphen("b i k e"));
  CHECK_EQUAL(false, hasHyphen(""));
}

TEST(TEST_UTILS, splitHyphen) {
  std::vector<std::string> splits;
  splits = splitHyphen("fly");
  CHECK_EQUAL(1, splits.size());
  CHECK_EQUAL("fly", splits[0]);

  splits = splitHyphen("butter-fly");
  CHECK_EQUAL(2, splits.size());
  CHECK_EQUAL("butter", splits[0]);
  CHECK_EQUAL("fly",    splits[1]);

  splits = splitHyphen("horse-fly-");
  CHECK_EQUAL(2, splits.size());
  CHECK_EQUAL("horse",  splits[0]);
  CHECK_EQUAL("fly",    splits[1]);
}

TEST(TEST_UTILS, isPunct) {
  CHECK_EQUAL(true, isPunct('\''));
  CHECK_EQUAL(true, isPunct('?'));
  CHECK_EQUAL(true, isPunct('!'));

  CHECK_EQUAL(false, isPunct('a'));
  CHECK_EQUAL(false, isPunct('p'));
  CHECK_EQUAL(false, isPunct('1'));
}

TEST(TEST_UTILS, trimPunctuation) {
  CHECK_EQUAL("sluggers.",  trimPunctuation("sluggers."));
  CHECK_EQUAL(",batters",   trimPunctuation(",batters"));
  CHECK_EQUAL(",pitchers.", trimPunctuation(",pitchers."));
  CHECK_EQUAL("empire,",    trimPunctuation("empire,,"));
  CHECK_EQUAL(",catcher.",  trimPunctuation(",,catcher.?"));
  CHECK_EQUAL("'twas,",     trimPunctuation("'''twas,'..!."));
  CHECK_EQUAL("",           trimPunctuation("'?','..!."));
  CHECK_EQUAL("",           trimPunctuation("'"));
};

TEST(TEST_UTILS, removePunctuation) {
  CHECK_EQUAL("sluggers",  scrubPunctuation("sluggers,"));
  CHECK_EQUAL("batters",   scrubPunctuation(",batters"));
  CHECK_EQUAL("pitchers",  scrubPunctuation(",pitchers."));
  CHECK_EQUAL("empire",    scrubPunctuation("empire',"));
  CHECK_EQUAL("catcher",   scrubPunctuation(",,catcher.."));

  // test contractions
  CHECK_EQUAL("can't",     scrubPunctuation(",,can't.-"));

  // special case contraction
  CHECK_EQUAL("runners'",  scrubPunctuation("'''runners'...."));
  CHECK_EQUAL("'twas",     scrubPunctuation("'''twas,'...."));
};

TEST(TEST_UTILS, scrubUTF8) {
  CHECK_EQUAL("potato$", scrubUTF8("potato$"));
  CHECK_EQUAL("potUato", scrubUTF8("pot£ato")); // two byte utf8 char
  CHECK_EQUAL("pUotato", scrubUTF8("p€otato")); // three byte utf8 char
  CHECK_EQUAL("potatoU", scrubUTF8("potato𐍈")); // four byte utf8 char
}
