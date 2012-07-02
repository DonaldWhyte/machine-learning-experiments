#include "ByteFrequencyTable.h"

RelativeByteFrequencyTable ByteFrequencyTable::RelativeFrequencies() const
{
    RelativeByteFrequencyTable relativeFrequencies;
    unsigned int total = TotalFrequency();
    for (unsigned int i = 0; (i < 256); ++i)
    {
        relativeFrequencies[i] = static_cast<double>(frequencies[i]) / total;
    }
    return relativeFrequencies;
}
