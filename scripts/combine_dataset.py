#-------------------------------------------------------------------------------
# Name: combine_dataset.py
# Author: Donald Whyte
# Date last modified: 29/06/12
# Description:
# Reads the directory containing a shellcode training/test dataset and combines
# ALL of the data items (individual files) into a single file.
#-------------------------------------------------------------------------------

import sys
import os
import struct

def Int2Bytes(integer):
    """Converts an Int32 into a 4 byte 'bytes' object."""
    return struct.pack('i', integer)

def read_binary_file(filename):
    """Reads the entire contents of the binary file called 'filename' and
    returns a bytes object."""
    with open(filename, 'rb') as f:
        data = f.read()
    return data

def get_num_data_items(dataset_directory):
    """Returns the number of identified data items inside the given directory.
    A data item is defined as a file that has the 'bin' extension."""
    num_data_items = 0
    for root, sub_folders, files in os.walk(dataset_directory):
        for filename in files:
            name, extension = os.path.splitext(filename)
            if extension == ".bin":
                num_data_items += 1
    return num_data_items

def combine_dataitems(filename, dataset_directory):
    """Searches through all the files in 'dataset_directory' RECURSIVELY,
    adding all the ones that end with .bin to the final archive, which is
    saved at 'filename'."""
    with open(filename, 'wb') as f:
        # First write how many data items have been added to the archive
        num_data_items = get_num_data_items(dataset_directory)
        f.write(Int2Bytes(num_data_items))

        # Walk through all files recursively, checking if each file's extension is 'bin'
        for root, sub_folders, files in os.walk(dataset_directory):
            for filename in files:
                name, extension = os.path.splitext(filename)
                if extension == ".bin":
                    # Read the file's data
                    data = read_binary_file(os.path.join(root, filename))
                    # Now write the LENGTH of the file, THEN the actual data to
                    # the archive file
                    length = len(data)
                    f.write(Int2Bytes(length))
                    f.write(data)

                    num_data_items += 1

    return True

def parse_commandline_arguments():
    """Parses the command line arguments and returns a dictionary containing
    the arguments' values."""
    if len(sys.argv) < 3:
        sys.exit("Usage: python {} <archive_filename> <dataset_directory>".format(sys.argv[0]))

    args = { 'archive_filename' : sys.argv[1], 'dataset_directory' : sys.argv[2] }
    return args


if __name__ == "__main__":
    args = parse_commandline_arguments()
    if not combine_dataitems(args['archive_filename'], args['dataset_directory']):
        print("Attempt to combine directory {} into archive file {} failed.".
            format(args['archive_filename'], args['dataset_directory']))