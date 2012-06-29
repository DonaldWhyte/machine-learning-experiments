#ifndef SHELLCODE_FEATURES_H
#define SHELLCODE_FEATURES_H

#include <vector>
#include "Util.h"

/* This structure contains all the features extracted from a single data item. */
struct ShellcodeDataItem
{
    // TODO
    int length;
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
    ShellcodeDataItem ExtractFeatures(const ByteArray& dataItem);
    /* INDIVIDUAL FEATURE EXTRACTION METHODS HERE */

    std::vector<ShellcodeDataItem> dataItems;

};

#endif
