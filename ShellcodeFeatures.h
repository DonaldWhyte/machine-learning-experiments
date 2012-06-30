#ifndef SHELLCODE_FEATURES_H
#define SHELLCODE_FEATURES_H

#include <vector>
#include "ByteFrequencyTable.h"
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


class DatasetReader
{

public:
    /* Reads all of the data items in the given dataset file, extracting their
     * features and storing them internally. After a call to this method, this
     * instance's internal data item store is updated with all the new data items
     * and their features.
     * Returns the success of the read. If false, internal data item store
     *  has not changed. */
    bool ReadDataset(const std::string& datasetFilename);

    const std::vector<ShellcodeDataItem>& GetDataItems() const;

private:
    /* Given the raw binary data of a data item, this function returns a
     * structure containing all the extracted features. */
    ShellcodeFeatureVector ExtractFeatures(const ByteArray& dataItem);
    ByteFrequencyTable GetByteFrequencies(const ByteArray& dataItem);
    /* INDIVIDUAL FEATURE EXTRACTION METHODS HERE */

    std::vector<ShellcodeDataItem> dataItems;

};

#endif
