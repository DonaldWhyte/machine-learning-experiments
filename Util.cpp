#include <fstream>
#include "Util.h"

ByteArray ReadBinaryFile(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (file.is_open())
    {
        size_t filesize = GetFilesize(file);

        // Now just read in ALL the data from the file at once
        ByteArray data(filesize);
        file.read(&data[0], filesize);
        return data;
    }
    else
    {
        return ByteArray();
    }
}

size_t GetFilesize(std::ifstream& file)
{
    // First, remember where we currently are in the file
    int originalLocation = file.tellg();
    // Now find out how many bytes in the file stream
    file.seekg(0, std::ios_base::end);
    size_t numBytes = file.tellg();
    // Restore the stream's 'pointer' back to its original location
    file.seekg(originalLocation, std::ios_base::beg);

    return numBytes;
}
