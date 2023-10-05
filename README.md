# three-word-frequency

## Problem
Executable from the command line when given text(s) will return the top 100 most common three-word sequences in descending order of frequency.

For example, if the code is run on War And Peace (https://gutenburg.org/cache/epub/2600/pg2600.txt), the first lines of the result would (probably) be:

| Sequence | Frequency |
| -------- | --------- |
| he did not | 225 |
| one of the | 187 |
| out of the | 178 |
| that he was | 156 |
| as soon as | 146 |

## Requirements
- Accepts a list of one or more file paths (e.g., ``./solution texts/war-and-peace.txt texts/moby-dick.txt``)
- Accepts input via stdin (e.g., ``cat text/*.txt | solution``)
- Outputs the first 100 most common three-word sequences
- Ignores punctuation, line endings, and is case insensitive
  - Contractions shouldn't change into two words (e.g., can't should not become can t)
- Unicode may be handled in any way you like (including replacing it with a space)

## Test Requirements
- Have automated tests

# Usage
Implemented in Gentoo.  It should be able to be compiled and run on most Linux systems.

## Packages Used
    make
    cpputest
    cmp
    wget
    awk
    sort

In Debian/Ubuntu systems, you install the packages ``build-essential``, ``cpputest``, and ``wget``.
    apt install build-essential cpputest wget

## Usage
Everything should be accessible via the Makefile targets.

### Makefile Targets
#### Cleaning Targets
- **clean**: Remove the binary, object files and other generated files from running the test.
- **dist-clean**: Runs clean and removes the downloaded text from Gutenberg.org
#### Build and Setup Targets
- **setup**: Download *War And Peace* and *Moby Dick* from Gutenberg.org
- **build**: Compiles the main binary ``three-word-frequency``.
- **build-test**: Compiles the unit tests.  It is also named ``three-word-frequency``.
#### Run Targets
- **run-stdin**: Build and pipes *War And Peace* into ``three-word-frequency`` and output the top 100 most frequent sequences.
- **run-file**: Build and reads *War And Peace* and *Moby Dick* file with ``three-word-frequency`` and output the top 100 most frequent sequences.
- **run-test-stdin**: Build and pipes a smaller test file and compare with known expected text and output the results.
- **run-test-file**: Builds and reads a smaller test file and compare with known expected text and output the results.
- **run-unit-test**: Builds and runs the unit test.

### Retrospective
#### If I had more time
- Investigate a better way of determining if a hyphen is a punctuation or a continuation.
- Determining contractions less naively.
- Add code coverage with ``lcov`` or ``gcov`` to see much of the code is not tested.
- The ``add`` method on the ``Tracker`` class can be broken up a litter more.
#### Cool try out
- Scanning a character at a time instead of relying on the ``>>`` operator when reading from the streams.
- Figure out how to determine compound word.
