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
            // Read the next four bytes as an integer which determines the LENGTH
            // (i.e. the number of bytes) contained in the next data item
            int dataItemLength = 0;
            datasetFile.read(reinterpret_cast<char*>(&dataItemLength), sizeof(dataItemLength));
            // Read the raw data of the data item
            ByteArray data(dataItemLength);
            datasetFile.read(&data[0], dataItemLength);

            // Extract features from said data and add the feature vector to the list
            ShellcodeDataItem dataItem = ExtractFeatures(data);
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

ShellcodeDataItem DatasetReader::ExtractFeatures(const ByteArray& dataItem)
{
    /* TODO */
    ShellcodeDataItem featureVector = { dataItem.size() };
    return featureVector;
}
