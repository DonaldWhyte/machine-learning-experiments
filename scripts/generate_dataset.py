#-------------------------------------------------------------------------------
# Name: generate_dataset.py
# Author: Donald Whyte
# Date last modified: 29/06/12
# Description:
# Tries to generate a training/test dataset for supervised learning of the
# shellcode classification problem.
#
# It does this by taking the SIZE of the dataset as input and a destination
# DIRECTORY to hold all the data. It attepts to search the entire computer for
# executable files to extract shellcode from.
#
# If there aren't enough executables found to fill HALF of the dataset size
# requested, then lower the size of the final dataset so there aren't more
# RANDOM binary files than SHELLCODE. This is to prevent bias when using the
# dataset for training and testing.
#
# NOTE: This script is dependant on the random_binary_file and extract_shellcode
# modules!
#-------------------------------------------------------------------------------

import sys
import os
import random

import random_binary_file
import extract_shellcode

# The minimun and maximum sizes (in bytes) of the randomly generated binary files
MIN_RANDOM_BINARY_SIZE = 256000 # 250 KB
MAX_RANDOM_NINARY_SIZE = 512000 # 500 KB

def parse_commandline_arguments():
    """Parses the command line arguments and returns"""
    args = {}
    # Parsing mandatory arguments
    try:
        args['directory'] = sys.argv[1]
        args['size'] = int(sys.argv[2])
    except ValueError:
        sys.exit("Invalid size '{}' given.".format(sys.argv[2]))
    return args


if __name__ == "__main__":
    if len(sys.argv) < 3:
        sys.exit("Usage: python {} <dataset_didrectory> <size>".format(sys.argv[0]))

    args = parse_commandline_arguments()
    # Calculate hte directories for data items in each class
    shellcode_directory = os.path.join(args['directory'], "shellcode")
    non_shellcode_directory = os.path.join(args['directory'], "non-shellcode")
    # Make sure these directories exist!
    if not os.path.isdir(shellcode_directory):
        os.makedirs(shellcode_directory)
    if not os.path.isdir(non_shellcode_directory):
        os.makedirs(non_shellcode_directory)

    # Max number of each class allowed in the dataset. Since there are two
    # class, this is HALF of the required dataset
    max_amount_per_class = round(args['size'] / 2)

    print("Generating shellcode data items...")
    # Search in the root of the main drive (C drive) for executables
    shellcode_generated = extract_shellcode.extract_shellcode(
        "C:/", shellcode_directory, max_amount_per_class, False)
    print("...done generating shellcode data items! Found {:d} executables to extract shellcode from.\n"
        .format(shellcode_generated))

    # Calculate mount of random binary files to generate, notifying user if
    # we found less executables than required
    files_to_generate = max_amount_per_class
    if shellcode_generated < max_amount_per_class:
        files_to_generate = shellcode_generated
        print("Could not find {:d} executables to extract shellcode from. Only generating {:d} non-shellcode data items.\n"
            .format(max_amount_per_class, shellcode_generated))

    print("Generating non-shellcode data items...")
    for i in range(files_to_generate):
        filename = "{:d}.bin".format(i)
        filepath = os.path.join(non_shellcode_directory, filename)
        # Size of random binary file is also random, within a specific range
        size = random.randint(MIN_RANDOM_BINARY_SIZE, MAX_RANDOM_NINARY_SIZE)
        random_binary_file.write_random_binary_file(filepath, size)
    print("...done generating non-shellcode data items!")

    print("Dataset fully generated. Final number of data items is {:d}.".format(shellcode_generated * 2))
