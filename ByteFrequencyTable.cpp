#include <string.h>
#include "ByteFrequencyTable.h"

ByteFrequencyTable::ByteFrequencyTable() : frequencies(new unsigned int[256])
{
    // Initialises each element of the table to 0
    for (unsigned int i = 0; (i < 256); ++i)
    {
        frequencies[i] = 0;
    }
}

ByteFrequencyTable::~ByteFrequencyTable()
{
    delete [] frequencies;
}

ByteFrequencyTable::ByteFrequencyTable(const ByteFrequencyTable& other) : frequencies(new unsigned int[256])
{
    memcpy(frequencies, other.frequencies, 256);
}

ByteFrequencyTable& ByteFrequencyTable::operator= (const ByteFrequencyTable& other)
{
    memcpy(frequencies, other.frequencies, 256);
    return *this;
}

unsigned int& ByteFrequencyTable::operator[] (const Byte byte)
{
    // static_cast<int> used to prevent warnings from the compiler complaining
    // about using bytes
    // I can always just get rid of this if it turns out to be a bottleneck.
    return frequencies[static_cast<int>(byte)];
}
unsigned int ByteFrequencyTable::operator[] (const Byte byte) const
{
    return frequencies[static_cast<int>(byte)];
}
