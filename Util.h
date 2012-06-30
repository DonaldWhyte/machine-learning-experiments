#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <sstream>

typedef unsigned char Byte; // TODO: maybe move this to "Types.h" or something like that
typedef std::vector<Byte> ByteArray;

/* Converts a string into any desired type, providing that the standard
 * library's stringstream class can perform the conversion.
 * NOTE: T must also have a default constructor, that way if the
 * conversion fails the return value is some default value. */
 // TODO: make this throw exception
template<typename T>
T FromString(const std::string& str)
{
    T result;

    // Checks if the string is empty before trying to convert
    if (!str.empty())
    {
        std::stringstream iss(str);
        iss >> result;
    }

    return result;
} /* defined in header because it's a templated function */

/* Reads the file with the given filename as a binary file, returning
 * an array of bytes. */
ByteArray ReadBinaryFile(const std::string& filename);
/* Returns how many numbers of bytes are in the given file.
 * NOTE: 'file' must be a valid, open stream. */
size_t GetFilesize(std::ifstream& file);

#endif
