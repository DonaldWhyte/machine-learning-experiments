#-------------------------------------------------------------------------------
# Name: extract_shellcode.py
# Author: Donald Whyte
# Date last modified: 29/06/12
# Description:
# This script recursively walks through all the subdirectories of the given
# directrory, looking for executable files. It will extract the shellcode
# (assembly instructions in raw binary data) from each one it finds and store
# it in a new binary file.
# Note that this script relies on the 'objdump' tool existing on the system,
# along with the stream redirection operator '>'. Therefore, it can only really
# be used for Windows or Unix-like systems.
#-------------------------------------------------------------------------------

import sys
import os
import subprocess
import io
import re

# Pre-compiled regular expression to save computation later on
# This regular expression will be used a LOT!
HEX_REGEX = re.compile("[0-9A-Fa-f]")

# The name of the temporary file created by disassemble_shellcode()
TEMPORARY_FILENAME = "./__decompiled.tmp"

# NOTE: No asserts for performance reasons.
# TODO: may add them anyway

def is_hex(ch):
    """Returns true if the given single character is a valid hexadecimal digit."""
    return bool(HEX_REGEX.match(ch) != None)


def is_hex_pair(string):
    """Returns true if the given string represents a sequence of two valid
    hexadecimal digits."""
    return (len(string) == 2 and is_hex(string[0]) and is_hex(string[1]))

def is_exe(fpath):
    """Returns true if the given file is executable."""
    # First check the given path is actually a file
    if not os.path.isfile(fpath):
        return False
    # Platform-dependant code!
    # If Windows, just check if the extension is .EXE
    if os.name == "nt":
        name, extension = os.path.splitext(fpath)
        return (extension.lower() == ".exe")
    # If Unix or any other system, check if the file has execution permissions
    else: # UNTESTED
        return os.access(fpath, os.X_OK)

def disassemble_shellcode(executable_path):
    """Disassembles the shellcode from the executable at 'executable_path' and
    returns an array of bytes representing the shellcode."""
    # Perform the initial disassembly
    os.system("objdump -d \"{}\" > \"{}\"".format(executable_path, TEMPORARY_FILENAME))

    # Searches through each line of the disassembled code, extracting the
    # raw memory values, converting them to bytes and storing them in a bytearray
    data = bytearray()
    with open(TEMPORARY_FILENAME, "rt") as f:
        line = f.readline()
        while line != "":
            fields = line.split()

            # 1 to 9 inclusive, don't care about first field!
            for i in range(1, min(len(fields), 10)):
                if (is_hex_pair(fields[i])):
                    data.append( int(fields[i], 16) )
                # If we're not valid hex pair, we've reached end of raw data
                # and hit the text instructions. Break out of loop to prevent
                # redundant processing
                else:
                    break

            line = f.readline()

    # Remove the temporary file created during the process of reading the raw data
    os.remove(TEMPORARY_FILENAME)

    return data

def write_binary_file(output_path, data):
    """Writes the given bytes stored in the bytearray 'data' to a binary file
    at the location pointed to by 'output_path'."""
    with open(output_path, "wb") as f:
        f.write(data)

def extract_shellcode(root_directory, output_directory, limit = 0, preserve_names = True):
    """The highest-level function in the module. Given a root directory to
    start searching for executables from and the directory to write the
    extracted shellcode to, this functionrecursively look through the root
    directory, extracting shellcode as it goes.

    The optional parameter 'limit' determines the maximum amount of executables
    this function will process. If the limit is 0, then the function will
    assume that there is NO max limit and process every executable it finds.

    The optional parameter 'preserve_names' determines if the names/relative
    directories of each found executable is PRESERVED when the shellcode is
    written to the output drectory. For example, if ./folder/code.exe is found,
    then the shellcode will be extracted to output_directory/folder/code.bin.
    If names aren't preserved, then all the shellcode files will simply be
    numbers such as 0.bin, 1.bin and so on.

    This returns the number of executable files successfully processed."""
    # keep track of executable files extracted
    executables_processed = 0
    # Recursively walk through all the subdirectories of the directory
    # provided by the user in the command line arguments
    for root, sub_folders, files in os.walk(root_directory):
        # Look through all the files in each directory
        for filename in files:
            filepath = os.path.join(root, filename)

            # If file is executable, extract its shellcode and write it to
            # another file
            if is_exe(filepath):
                # Compute the full path the new flat binary file containing the
                # EXE's shellcode will be in
                if preserve_names:
                    # Compute the DIRECTORY the file which will store the executable's
                    # instructions is contained in
                    shellcode_directory = os.path.join(output_directory, root)
                    # Make sure that this directory exists
                    if not os.path.isdir(shellcode_directory):
                        os.makedirs(shellcode_directory)
                    # Now use that directory to compute the FULL path of the
                    # new shellcode file
                    name, extension = os.path.splitext(filename)
                    output_filepath = os.path.join(shellcode_directory, (name + ".bin"))
                else:
                    name = "{:d}.bin".format(executables_processed)
                    output_filepath = os.path.join(output_directory, name)

                print("Found {}, trying to extract shellcode...".format(filepath))
                shellcode = disassemble_shellcode(filepath)
                write_binary_file(output_filepath, shellcode)
                print("...extracted shellcode to {}".format(output_filepath))

                executables_processed += 1
                # If we've reached the limit on the amount of executables we
                # can process, just return and stop searching!
                if executables_processed == limit:
                    return executables_processed

    return executables_processed

def parse_commandline_arguments():
    """Returns the arguments from the commandline as a dictionary, inserting
    default values when the argument was not provided."""
    if len(sys.argv) < 2:
        sys.exit("Usage: python {} <root_directory> <output_directory>".format(sys.argv[0]))

    args = { 'root_directory' : sys.argv[1], 'output_directory' : './output', 'limit' : 0 }
    if len(sys.argv) >= 3:
        args['output_directory'] = sys.argv[2]
    if len(sys.argv) >= 4:
        try:
            args['limit'] = int(sys.argv[3])
            if args['limit'] < 0:
                raise ValueError()
        except ValueError:
            sys.exit("Invalid max limit given! Not an integer!")

    return args


if __name__ == "__main__":
    args = parse_commandline_arguments()
    executables_processed = extract_shellcode(args['root_directory'], args['output_directory'], args['limit'])
    print("Processed and extracted shellcode from {:d} executables.".format(executables_processed))

