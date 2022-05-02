"""The script encodes a file to a base64 string and print it out to file.

Usage:
    'python ./encode_key.py <path to json key>' (from the script folder)

Output:
    file which contain the base64 string. the file will be locate at the same directory as the input file. if the file exist it will be override.
"""


import base64
import os
import sys

def encode(file: os.PathLike):
    # get the input file path and name and create path for output file
    file_path = os.path.realpath(os.path.join(file)) 
    file_name = os.path.basename(file_path)
    output_dir_name = os.path.dirname(file_path)
    output_file_name = file_name.split('.', maxsplit=1)[0] + '-base64'
    output_file_path = os.path.join(output_dir_name, output_file_name)

    print('encodeing..')
    # r is read mode, b is binary type
    with open(file_path, 'rb') as json_file:
        encoded_file = base64.b64encode(json_file.read())
        base64_string = encoded_file.decode() # decode() to convert the binary to text

    # w is write mode. override file if exist
    with open(output_file_path, 'w') as base64_file:
        base64_file.write(base64_string)
    
    print(f'done. output file: {output_file_path}') 


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('error: a path to file must be provided.')
        exit(2)
    encode(sys.argv[1])
