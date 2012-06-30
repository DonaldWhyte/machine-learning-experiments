#ifndef BYTE_FREQUENCY_TABLE_H
#define BYTE_FREQUENCY_TABLE_H

#include "Util.h"

/* TODO: comment
 * TODO: TEST */
class ByteFrequencyTable
{

public:
    ByteFrequencyTable();

    /* The rule of three is implemented here. */
    virtual ~ByteFrequencyTable();
    ByteFrequencyTable(const ByteFrequencyTable& other);
    ByteFrequencyTable& operator= (const ByteFrequencyTable& other);

    unsigned int& operator[] (const Byte byte); // for write access
    unsigned int operator[] (const Byte byte) const; // for read-only access

private:
    unsigned int* frequencies;

};

#endif
