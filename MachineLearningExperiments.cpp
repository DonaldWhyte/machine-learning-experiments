#include "IrisSupervisedLearning.h"
#include "DatasetReader.h"
#include "Util.h"

int main(int argc, char* argv[])
{
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

    return 0;
}
