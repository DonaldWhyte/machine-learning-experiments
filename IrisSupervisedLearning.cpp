#include "IrisSupervisedLearning.h"
#include "Util.h"

Table ParseCSVFile(const std::string& filename) {
    // Stores parsed data
	Table data;
	// Temporary storage - used to store value of current
    // field and value(s) of current row being parsed
	Row currentRow;
	currentRow.reserve(50);
	std::string buffer;
	buffer.reserve(300);

    // Open an input stream to the file with the given name
	std::ifstream file(filename.c_str());
	// Keep reading characters from the file until we reach
	// the end or an IO error occurs
	while (file.good()) {
		char ch = file.get();
		if (ch == ',')
		{
			currentRow.push_back(buffer);
			buffer.clear();
		}
		else if (ch == '\n')
		{
			currentRow.push_back(buffer);
			buffer.clear();

			data.push_back(currentRow);
			currentRow.clear();
		}
		else
		{
			buffer.push_back(ch);
		}
	}

	return data;
}

void PrintTable(const Table& data)
{
	for (unsigned int i = 0; (i < data.size()); ++i)
	{
		const Row& row = data[i];
		for (unsigned int j = 0; (j < row.size()); ++j)
		{
			std::cout << row[j] << ",";
		}
		std::cout << std::endl;
	}
}

std::vector<SampleType> GetFeatureVectors(const Table& data)
{
    std::vector<SampleType> features;
    features.reserve(data.size());

    for (unsigned int i = 0; (i < data.size()); ++i)
    {
        // Ignore the current row of data if there aren't enough fields
        // We check for n + 1 fields where n is the number of features,
        // since we also need the CLASS of the data item too ((n + 1)th element)
        if (data[i].size() < 5) continue;

        // Construct feature std::vector for this data item, converting
        // textual data to floats
        SampleType featureVector;
        featureVector(0, 0) = FromString<double>(data[i][0]);
        featureVector(1, 0) = FromString<double>(data[i][1]);
        featureVector(2, 0) = FromString<double>(data[i][2]);
        featureVector(3, 0) = FromString<double>(data[i][3]);

        // Add it to the list containing all the feature std::vectors
        features.push_back(featureVector);
    }

    return features;
}

std::vector<std::string> GetClasses(const Table& data)
{
    std::vector<std::string> classes;
    classes.reserve(data.size());

    for (unsigned int i = 0; (i < data.size()); ++i)
    {
        if (data[i].size() >= 5)
        {
            classes.push_back(data[i][4]);
        }
    }

    return classes;
}

std::vector<LabelType> ConstructLabels(const std::vector<std::string>& classes)
{
	std::vector<LabelType> labels;
	labels.reserve(classes.size());
	// Keep track of currently encountered classes and maintain what
	// numerical value we assigned to become the corresponding label
	// using a map. We then go through every class, generating and
	// assigning labels to each data item appropriately.
	std::map<std::string, double> classToLabel;
	for (unsigned int i = 0; (i < classes.size()); ++i)
	{
	    const std::string& currentClass = classes[i];
	    // If class has been encountered before, generate a label for it
	    if (classToLabel.count(currentClass) == 0)
	    {
            classToLabel[currentClass] = static_cast<double>(classToLabel.size());
	    }
	    labels.push_back(classToLabel[currentClass]);
	}
	return labels;
}

double FindBestGamma(dlib::krr_trainer<KernelType>& trainer,
    const std::vector<SampleType>& samples,
    const std::vector<LabelType>& labels)
{
    // Useful method for calculating gamma value too
    /*const double meanSquaredGamma = 1.0 / compute_mean_squared_distance(
      /  randomly_subsample(trainingSamples, 100));*/

    double bestGammaFound = 0;
    double highestAccuracy = 0;
    // Loop over different gamma values and uses most accurate it can find
    for (double gamma = 0.000001; (gamma <= 1); gamma *= 5)
    {
        trainer.set_kernel(KernelType(gamma));

        std::vector<double> looValues;
        trainer.train(samples, labels, looValues);

        const double classificationAccuracy = dlib::mean_sign_agreement(labels, looValues);
        if (classificationAccuracy > highestAccuracy)
        {
            bestGammaFound = gamma;
            highestAccuracy = classificationAccuracy;
        }
    }
    return bestGammaFound;
}

double CalculateAccuracy(const FunctionType& function,
                         const std::vector<SampleType>& samples,
                         const std::vector<LabelType>& labels,
                         bool notifyIfIncorrect)
{
    // Test accuracy of the learned function using the test dataset!
    unsigned int numCorrect = 0;
    for (unsigned int i = 0; (i < samples.size()); ++i)
    {
        // Using thresholding to turn regression into classification
        double prediction = round(function(samples[i]));

        if (prediction == labels[i])
        {
            numCorrect += 1;
        }
        // If flag is set, print out the incorrectly predicted data item
        else if (notifyIfIncorrect)
        {
            std::cout << "Data item " << i << " is incorrect:\n\tPrediction = "
                << prediction << "\n\tReal = " << labels[i] << std::endl;
        }
    }

    return static_cast<double>(numCorrect) / samples.size();
}

void RunIrisSupervisedLearning()
{
    // Load training/test dataset
	Table data = ParseCSVFile("data/iris.data");
	// Print out the data to ensure we've loaded it correctly
	std::cout << "Loaded Data:" << std::endl;
    PrintTable(data);


	// Extract feature std::vectors and classes from the loaded data
	std::vector<SampleType> allSamples = GetFeatureVectors(data);
	std::vector<std::string> classes = GetClasses(data);


	// Construct labels compatible with SVMs using the class data
	// Each class is made an integer. The integers grow one number
	// apart, so three classes will be assigned the labels 1, 2 and
	// 3 respectively.
	std::vector<LabelType> allLabels = ConstructLabels(classes);


    // Randomise the samples and labels to ensure the normalisation process
    // does affect the performance of cross validation
    randomize_samples(allSamples, allLabels);


	// Split dataset in half - one half being the training set
	// and one half being the test set.
	// Done AFTER randomising so half of the data set isn't one class
	// and half is the other - would result in a very incorrect classifier!
	unsigned int numTraining = round(allSamples.size() / 2);
	unsigned int numTest = allSamples.size() - numTraining;

	std::vector<SampleType> trainingSamples;
	std::vector<LabelType> trainingLabels;
	trainingSamples.reserve(numTraining);
	trainingLabels.reserve(numTraining);
	std::vector<SampleType> testSamples;
	std::vector<LabelType> testLabels;
	testSamples.reserve(numTest);
	testLabels.reserve(numTest);

	for (unsigned int i = 0; (i < numTraining); ++i)
	{
	    trainingSamples.push_back(allSamples[i]);
	    trainingLabels.push_back(allLabels[i]);
	}
	for (unsigned int i = numTraining; (i < allSamples.size()); ++i)
	{
        testSamples.push_back(allSamples[i]);
        testLabels.push_back(allLabels[i]);
	}


    // Construct a trainer for the problem
    dlib::krr_trainer<KernelType> trainer;
    double bestGamma = FindBestGamma(trainer, trainingSamples, trainingLabels);
    trainer.set_kernel(KernelType(bestGamma));


    // Actually TRAIN the classifier using the data, LEARNING the function
    FunctionType learnedFunction;
    learnedFunction = trainer.train(trainingSamples, trainingLabels);


    // NOTE: This should just print out 1 for our training method
    std::cout << "The number of support std::vectors in our learned function is "
        << learnedFunction.basis_vectors.nr() << std::endl;

    double accuracy = CalculateAccuracy(learnedFunction, testSamples, testLabels);
    std::cout << "The accuracy of this classifier is: "
        << (accuracy * 100) << "%." << std::endl;
}
