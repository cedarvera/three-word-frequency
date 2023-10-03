# three-word-frequency

## Problem
Executable from the command line when given text(s) will return the top 100 most common three-word sequences in descending order of frequency.

For example, if the code is run on War And Peace (https://gutenburg.org/cache/epub/2600/pg2600.txt), the first lines of the result would (probably) be:

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
