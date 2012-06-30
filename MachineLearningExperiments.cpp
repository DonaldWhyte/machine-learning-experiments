#include "IrisSupervisedLearning.h"
#include "ShellcodeFeatures.h"
#include "Util.h"

// reutrns true if shellcode
bool Classify(double* classProbabilities, double** wordProbabilities, const ByteFrequencyTable& dataItem)
{
    double finalClassProbabilities[2] = { classProbabilities[0], classProbabilities[1] };

    for (unsigned int j = 0; (j < 2); ++j)
    {
        // TODO
        for (unsigned int i = 0; (i < 256); ++i)
        {
            finalClassProbabilities[i] *= wordProbabilities[j][i];
        }
    }

    if (finalClassProbabilities[0] >= finalClassProbabilities[1]) return true;
    else return false;
}

int main(int argc, char* argv[])
{
    RunIrisSupervisedLearning();

    /* Test code for DatasetReader. */
    DatasetReader reader;
    reader.ReadDataset("scripts/dataset2.bin");
    const std::vector<ShellcodeDataItem> dataItems = reader.GetDataItems();

    std::cout << "The number of data items in the archive is " << dataItems.size() << std::endl;
    for (unsigned int i = 0; (i < dataItems.size()); ++i)
    {
        std::cout << i << ": Class = " << dataItems[i].label << ", Length = "
            << dataItems[i].featureVector.length << std::endl;
    }

    char* classes[2] = { "shellcode", "non-shellcode " };
    double classProbabilities[2] = { 0.5, 0.5 };
    double* wordProbabilites[2][256] = { { 0 } };

    // TODO: calculating word probabilites??

    return 0;
}
