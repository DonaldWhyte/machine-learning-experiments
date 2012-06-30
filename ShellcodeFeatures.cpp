#include "ShellcodeFeatures.h"
#include <fstream>

bool DatasetReader::ReadDataset(const std::string& datasetFilename)
{
    std::ifstream datasetFile(datasetFilename.c_str(), std::ios::binary);
    if (datasetFile.is_open())
    {
        // Read the first four bytes as an integer, which determines the number
        // of data items inside the dataset archive
        int numDataItems = 0;
        datasetFile.read(reinterpret_cast<char*>(&numDataItems), sizeof(numDataItems));

        for (int i = 0; (i < numDataItems && !datasetFile.eof()); ++i)
        {
            // Read the next byte as the CLASS the data item belongs to
            int label = 0;
            datasetFile.read(reinterpret_cast<char*>(&label), sizeof(label));
            // Read the next four bytes as an integer which determines the LENGTH
            // (i.e. the number of bytes) contained in the next data item
            int dataItemLength = 0;
            datasetFile.read(reinterpret_cast<char*>(&dataItemLength), sizeof(dataItemLength));
            // Read the raw data of the data item
            ByteArray data(dataItemLength);
            datasetFile.read(reinterpret_cast<char*>(&data[0]), dataItemLength);

            // Construct the data item using the read label and binary data
            ShellcodeDataItem dataItem;
            dataItem.label = static_cast<ShellcodeClass>(label);
            dataItem.featureVector = ExtractFeatures(data);
            dataItems.push_back(dataItem);
        }

        return true;
    }
    else
    {
        return false;
    }
}

const std::vector<ShellcodeDataItem>& DatasetReader::GetDataItems() const
{
    return dataItems;
}

ShellcodeFeatureVector DatasetReader::ExtractFeatures(const ByteArray& dataItem)
{
    ShellcodeFeatureVector featureVector = {
        dataItem.size(),
        GetByteFrequencies(dataItem)
    };
    return featureVector;
}

ByteFrequencyTable DatasetReader::GetByteFrequencies(const ByteArray& dataItem)
{
    ByteFrequencyTable table;
    for (unsigned int i = 0; (i < dataItem.size()); ++i)
    {
        table[dataItem[i]] += 1;
    }
    return table;
}
