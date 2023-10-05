#include <CppUTest/CommandLineTestRunner.h>

IMPORT_TEST_GROUP(TEST_UTILS);
IMPORT_TEST_GROUP(TEST_TRACKER);

int main(int _argc, char** _argv) {
  return CommandLineTestRunner::RunAllTests(_argc, _argv);
}
