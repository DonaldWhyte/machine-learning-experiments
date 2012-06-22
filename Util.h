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
}
