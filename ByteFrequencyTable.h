#ifndef BYTE_FREQUENCY_TABLE_H
#define BYTE_FREQUENCY_TABLE_H

#include "FrequencyTable.h"

typedef FrequencyTable<double, 256> RelativeByteFrequencyTable;

/* TODO: comment
 * TODO: TEST */
class ByteFrequencyTable : public FrequencyTable<unsigned int, 256>
{

public:
    /* Returns a table containing the RELATIVE frequency of each byte. */
    RelativeByteFrequencyTable RelativeFrequencies() const;

};

#endif
