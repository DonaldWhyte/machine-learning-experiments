#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H

/* Everything implemented in header since it's a templated class. */

/* Generic frequency table, which can be adopted to store different TYPES
 * of frequencies and have different (fixed) sizes. Note that T must have
 * implemented the default constructor and addition operators. */
template<typename T, unsigned int SIZE>
class FrequencyTable
{

public:

    FrequencyTable() : frequencies(new T[SIZE])
    {
        for (unsigned int i = 0; (i < SIZE); ++i)
        {
            frequencies[i] = T();
        }
    }

    /* The rule of three is implemented here. */
    virtual ~FrequencyTable()
    {
        delete [] frequencies;
    }
    FrequencyTable(const FrequencyTable<T, SIZE>& other) : frequencies(new T[SIZE])
    {
        Copy(other);
    }
    FrequencyTable& operator= (const FrequencyTable<T, SIZE>& other)
    {
        Copy(other);
        return *this;
    }

    T& operator[] (const unsigned int index) // for write access
    {
        return frequencies[index];
    }
    const T& operator[] (const unsigned int index) const // for read-only access
    {
        return frequencies[index];
    }

    /* Reutnrs the total frequency of EVERY byte in the table. */
    T TotalFrequency() const
    {
        T totalFrequency = 0;
        for (unsigned int i = 0; (i < 256); ++i)
            totalFrequency += frequencies[i];
        return totalFrequency;
    }

protected: /* so it can be accessed by subclasses */
    T* frequencies;

private:
    /* Wrapped in function to prevent repetition in assignment operator and
     * copy constructor. */
    void Copy(const FrequencyTable<T, SIZE>& other)
    {
        for (unsigned int i = 0; (i < SIZE); ++i)
        {
            frequencies[i] = other.frequencies[i];
        }
    }


};

#endif
