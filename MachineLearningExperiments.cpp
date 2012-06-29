#include "IrisSupervisedLearning.h"
#include "ShellcodeFeatures.h"
#include "Util.h"

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
        std::cout << i << ": Length = " << dataItems[i].length << std::endl;
    }

    return 0;
}
