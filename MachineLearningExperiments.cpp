#include "IrisSupervisedLearning.h"
#include "ShellcodeFeatures.h"
#include "Util.h"

// reutrns true if shellcode
ShellcodeClass Classify(const std::vector<double>& classProbabilities,
    const std::vector<std::vector<double> >& wordProbabilities,
    const ByteFrequencyTable& dataItemFrequencies)
{
    double finalClassProbabilities[2] = { classProbabilities[0], classProbabilities[1] };
    unsigned int totalFrequency = dataItemFrequencies.TotalFrequency();

    for (unsigned int j = 0; (j < 2); ++j)
        for (unsigned int i = 0; (i < 256); ++i)
        {
            // Calculate relative frequency of word in data item
            double relativeFrequency = dataItemFrequencies[j] / totalFrequency;
            if (dataItemFrequencies[j] > 0)
                finalClassProbabilities[j] *= wordProbabilities[j][i] * relativeFrequency;
        }

    if (finalClassProbabilities[0] <= finalClassProbabilities[1]) return CLASS_SHELLCODE;
    else return CLASS_NON_SHELLCODE;
}

int main(int argc, char* argv[])
{
    RunIrisSupervisedLearning();

    /* Test code for DatasetReader. */
    DatasetReader reader;
    reader.ReadDataset("scripts/dataset.bin");
    const std::vector<ShellcodeDataItem> dataItems = reader.GetDataItems();

    std::cout << "The number of data items in the archive is " << dataItems.size() << std::endl;
    for (unsigned int i = 0; (i < dataItems.size()); ++i)
    {
        std::cout << i << ": Class = " << dataItems[i].label << ", Length = "
            << dataItems[i].featureVector.length << std::endl;
    }

    char* classes[2] = { "non-shellcode", "shellcode " };
    std::vector<double> classProbabilities(2);
    classProbabilities[0] = 0.5; classProbabilities[1] = 0.5;
    // wordProbabilities[i][j] is the probability of word j occuring if dat item is class i
    std::vector<std::vector<double> > wordProbabilites(2);

    // Calculate the probability of each word occurring in EACH class!
    for (unsigned int i = 0; (i < 2); ++i)
    {
        wordProbabilites[i].resize(256);

        // Calculate total frequenct of ALL WORDS for data items in this class
        unsigned int totalFrequencyInClass = 0;
        for (unsigned int j = 0; (j < 256); ++j)
            for (unsigned int k = 0; (k < dataItems.size()); ++k)
                if (dataItems[k].label == static_cast<int>(i))
                    totalFrequencyInClass += dataItems[k].featureVector.byteFrequencies[j];

        for (unsigned int j = 0; (j < 256); ++j)
        {
            // Calculate total frequency of INDIVIDUAL word over ALL data items with current class
            unsigned int totalWordFrequency = 0;
            for (unsigned int k = 0; (k < dataItems.size()); ++k)
                if (dataItems[k].label == static_cast<int>(i))
                    totalWordFrequency += dataItems[k].featureVector.byteFrequencies[j];

            // Now use this to calculate the probability of word occuring IF data
            // item is of the given class
            wordProbabilites[i][j] = static_cast<double>(totalWordFrequency) / totalFrequencyInClass;
        }
    }

    // Now try and test the classifier by classifying all data items imported
    unsigned int numCorrect = 0; // keep track of how many were predicted correctly
    for (unsigned int i = 0; (i < dataItems.size()); ++i)
    {
        ShellcodeClass output = Classify(classProbabilities, wordProbabilites,
            dataItems[i].featureVector.byteFrequencies);
        ShellcodeClass real = dataItems[i].label;
        if (output == real)
        {
            numCorrect += 1;
        }
        std::cout << i << ": Prediction = " << output << ", Real = " << real << std::endl;
    }
    double accuracy = static_cast<double>(numCorrect) / dataItems.size();
    std::cout << "Accuracy of the classifier is " << accuracy * 100 << "%" << std::endl;

    return 0;
}
