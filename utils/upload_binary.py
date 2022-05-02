""" The script upload the firmware files to firbase storage.

Usage:
    it run by timheuer/base64-to-file@v1 action after encoding the the key stored in the github secret.
    for debug, run it with 'python ./upload_binary.py -d <path to json key>' (from the script folder).
"""
import firebase_admin
from firebase_admin import credentials, storage
# from google.cloud.storage.blob import Blob
import os
import sys
import argparse


# base64 decoded key file will be stored in temporary directory on runner machine
# https://github.com/marketplace/actions/base64-to-file
GITHUB_TEMP_PATH = '/Users/runner/work/_temp' 

# google cloud's service account key file absolute path on github's machine directory
# note that the file name must be matched with the file name created from timheuer/base64-to-file@v1 action on the workflow
KEY_FILE_PATH = GITHUB_TEMP_PATH + '/ota_key.json'

# bucket name is is the part of path gs://<bucket_name>.appspot.com at firebase storage
BUCKET_NAME = 'esp-ota-712ba'
BUCKET_URL = BUCKET_NAME + '.appspot.com'
STORAGE_BUCKET = 'storageBucket'

# set the upload file's path in repository's directory
FILE_NAME = 'firmware.bin'
UPLOAD_FILE_PATH = os.path.join(os.getcwd(), 'utils', FILE_NAME)


def upload(key_file_path):
    # apply the bucket domain to the credentials
    cred = credentials.Certificate(key_file_path)
    firebase_admin.initialize_app(cred, {STORAGE_BUCKET : BUCKET_URL})

    # refer to the storage bucket
    bucket = storage.bucket()

    # if the file name contains file path, the bucket will create folders corresponding to the path.
    blob  = bucket.blob(FILE_NAME)

    #upload to firebase storage
    blob.upload_from_filename(UPLOAD_FILE_PATH)

    print("your file url ", blob.public_url)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(add_help=True)
    parser.add_argument('-d', metavar='KEY_FILE_PATH' , default=f'{KEY_FILE_PATH}', help='debug mode, argument is JSON key file path')
    args = parser.parse_args()
    upload(args.d)

    # try:
    #     opts, args = getopt.getopt(sys.argv,"hd:")
    # except getopt.GetoptError:
    #     print('error: unrecognized option. debug mode usage: python ./upload_binary.py -d <path to json key>')
    #     sys.exit(2)
    # if len(args == 0):
    #     upload(KEY_FILE_PATH)
    # else:
    #      # only one option in this case for arg

