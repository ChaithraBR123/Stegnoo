#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 100000
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;//it will store source name
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE]; //it will take 8 byte

    /* Secret File Info */
    char *secret_fname;//file pointer for secret name
    FILE *fptr_secret;//to store our secret file extension,while encoding we need to encode secret file extension also
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];//store secret data
    long size_secret_file;
    //char *secret_data;
    long exten_size;//its point to length of the extension

    /* Stego Image Info */
    char *stego_image_fname;//output 
    FILE *fptr_stego_image;//output file image

    //magic string information
    char *magic_string;
    int magic_strlen;
     //secret file information

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);//check if we are checking encoding decoding 

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

Status read_and_Validate_secret_file(EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);//only we need to write function call related to encoding

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

//encode all string length
Status encode_integer_sec_file(int , EncodeInfo*);

//for encode all string
Status encode_char_sec_file(int ,char *, EncodeInfo*);


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(EncodeInfo *encInfo);

#endif
