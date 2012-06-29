#ifndef IRIS_SUPERVISED_H
#define IRIS_SUPERVISED_H

#include <sstream>
#include <vector>
#include <map>

#include <dlib/svm.h>

#include "CSVLoader.h"


/* Setting up types to use for machine learning
   Data item has four dobules as a feature vector */
typedef dlib::matrix<double, 4, 1> SampleType;
/* Using support vector machine, use a double for the class */
typedef double LabelType;
/* We're going to use a radial basis kernel for the machine learning */
typedef dlib::radial_basis_kernel<SampleType> KernelType;
/* The type of decision function AND normalised decision function to use */
typedef dlib::decision_function<KernelType> FunctionType;
typedef dlib::normalized_function<FunctionType> NormalisedFunctionType;

/* Extracts feature and class data from a table of data. */
std::vector<SampleType> GetFeatureVectors(const Table& data);
std::vector<std::string> GetClasses(const Table& data);
std::vector<LabelType> ConstructLabels(const std::vector<std::string>& classes);

/* Finds the best gamma value to use for the radial basis function. */
double FindBestGamma(dlib::krr_trainer<KernelType>& trainer,
                     const std::vector<SampleType>& samples,
                     const std::vector<LabelType>& labels);
/* Returns a value from 0 to 1 which determines the accuracy of a
 * learned function given some test data. */
double CalculateAccuracy(const FunctionType& function,
                         const std::vector<SampleType>& samples,
                         const std::vector<LabelType>& labels,
                         bool notifyIfIncorrect = false);

/* Call this method to run this experiment. */
void RunIrisSupervisedLearning();

#endif
