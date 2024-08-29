#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_BUFFER_SIZE 100

typedef struct _DecodeInfo
{
    /* Encoded Image/File Information */
    FILE *fptr_encoded;           // File pointer to the encoded image/file
    char *encoded_file_name;      // Name of the encoded image/file

    /* Magic String Information */
    int d_magic_string_length;      // Length of the decoded magic string
    char d_magic_string[MAX_BUFFER_SIZE]; // Buffer to store the decoded magic string

    /* Decoded File Information */
    char *decoded_file_name;      // Name of the file to save decoded data
    FILE *fptr_decoded;           // File pointer to the decoded file

    /* Secret File Extension Information */
    int d_secret_ext_length;        // Length of the secret file extension
    char d_secret_extension[MAX_BUFFER_SIZE]; // Buffer to store the secret file extension

    /* Secret Message Information */
    int secret_message_length;    // Length of the decoded secret message

} DecodeInfo;

/* Function Prototypes */
// Function to read and store the arguments for decoding
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
// Function to perform the decoding process
Status do_decode(DecodeInfo *decInfo);

// Function to open files for decoding
Status decode_open_files(FILE **files, char *file_name);

// Function to decode the length of some data
Status decode_length(int *length, DecodeInfo *decInfo);
// Function to decode a string
Status decode_string(int *length, char *str, DecodeInfo *decInfo);

// Function to decode the secret message
Status decode_secret_message(DecodeInfo *decInfo);

#endif 
