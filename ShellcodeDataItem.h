#ifndef SHELLCODE_DATAITEM_H
#define SHELLCODE_DATAITEM_H


#include "ByteFrequencyTable.h"
#include <vector>
#include "Util.h"

enum ShellcodeClass
{
    CLASS_NON_SHELLCODE = 0,
    CLASS_SHELLCODE = 1
};

/* IThe feature vector each data item has (i.e. the input to the
 * classification problem). */
struct ShellcodeFeatureVector
{
    // How many bytes does the data item contain
    int length;
    // Store a table which maps a byte to the amount of times it appears
    // in the data item. This is essentially a "bag of words", where the
    // words are simply byte values.
    ByteFrequencyTable byteFrequencies;
};

/* This structure contains all the features extracted from a single data item. */
struct ShellcodeDataItem
{
    ShellcodeClass label; // known output
    ShellcodeFeatureVector featureVector; // input
};

#endif
