#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo){
    if (strstr(argv[2],".bmp") != NULL) {
        decInfo->encoded_file_name = argv[2];
    } else {
        printf("ERROR: decoded file name is missing\n");
        return e_failure;
    }

    if (argv[3] != NULL) {
        decInfo->decoded_file_name = argv[3];
    } else {
        
        static char  outputfile[]="secretfile";
     decInfo->decoded_file_name = outputfile;   
     printf("INFO: Output file not mentioned. Creating %s as defualt\n",decInfo->decoded_file_name);                                          //take defualt file name,store file name into encInfo->stego_image_fname

    }

    return e_success;
}

Status do_decode(DecodeInfo *decInfo)
 {
    printf("INFO: Opening required files\n");

    if (decode_open_files(&decInfo->fptr_encoded, decInfo->encoded_file_name) == e_success) {

    // Seek to the 54th byte in the encoded file
    if (fseek(decInfo->fptr_encoded, 54, SEEK_SET) == -1) {
        printf("Successfully moved to position 54 in file %s.\n", decInfo->encoded_file_name);
    }
    if(decode_length(&decInfo->d_magic_string_length,decInfo)==e_success){
    if(decode_string(&decInfo->d_magic_string_length,decInfo->d_magic_string,decInfo)==e_success){
        printf("enter magic string :");
        char magic_str[20];
         //printf("%s",decInfo->d_magic_string);
        scanf("%[^\n]",magic_str);
        getchar();
        if(strcmp(decInfo->d_magic_string,magic_str)==0){
            printf("INFO: ## Magic strings are matching validation complete decoding secret file ##\n");
        }
        else{
            printf("ERROR : Magic strings are not matchingn \n");
        }
        if(decode_length(&decInfo->d_secret_ext_length,decInfo)==e_success){
            if(decode_string(&decInfo->d_secret_ext_length,decInfo->d_secret_extension,decInfo)==e_success){
                strcat(decInfo->decoded_file_name,decInfo->d_secret_extension);
                if((decInfo->fptr_decoded=fopen(decInfo->decoded_file_name,"w"))!=NULL){
                 printf("INFO: done opened all required files\n");
                printf("INFO: decoding secret message size\n");   
                if(decode_length(&decInfo->secret_message_length,decInfo)==e_success){
                    printf("INFO: Done\n");
                    printf("INFO: decoding secret message\n");
                    char secret_meee[100]={0};
                    if(decode_string(&decInfo->secret_message_length,secret_meee,decInfo)==e_success){
                        decInfo->fptr_decoded= fopen(decInfo->decoded_file_name,"w");
                        if(fwrite(secret_meee,sizeof(char),decInfo->secret_message_length,decInfo->fptr_decoded) != decInfo->secret_message_length){
                            printf("ERROR : cant write the seecret file\n");
                            return e_failure;
                        }else{
                            return e_success;
                        }

                    return e_success;
                    }
									else{
										return e_failure;
									}
								}
								else{
									return e_failure;
								}
							}
							else{
								return e_failure;
							}
					}
					else{
						return e_failure;
					}				
				}
				else{
					return e_failure;
				}
			}
			else{
				return e_failure;
			}
		}
		else{
			return e_failure;
		}
	}
	else {
		return e_failure;
	}
    }
 
Status decode_open_files(FILE **files, char *file_name)
{
  if((*files=fopen(file_name,"r"))==NULL)  {
printf("ERROR: file not opening %s\n",file_name);
        return e_failure;
    }
    else {
        printf("INFO: file is opened %s\n",file_name);

    return e_success;
}
}

Status decode_length(int *length, DecodeInfo *decInfo)
{
    unsigned char length_buffer[32];  // Assuming the length is stored in 32 bits (4 bytes)
    
    // Read 32 bytes from the encoded file
    if (fread(length_buffer, sizeof(char), 32, decInfo->fptr_encoded) != 32) {
        printf("ERROR: Failed to read length from the encoded file.\n");
        return e_failure;
    }
    
    // Initialize the length to zero
    *length = 0;
    
    // Decode the length from the least significant bit of each byte
    for (int i = 0; i < 32; i++) {
        *length |= (length_buffer[i] & 1) << i;
    }
    
    return e_success;
}

Status decode_string(int *length, char *str, DecodeInfo *decInfo)
{
    int fetch = (*length)*8;
    unsigned char char_buffer[fetch];  // Each character is stored in 8 bits
    
        // Read 8 bits (1 byte) from the encoded file for each character
        if (fread(char_buffer, sizeof(char),fetch, decInfo->fptr_encoded) != fetch) {
            printf("ERROR: Failed to read character from the encoded file.\n");
            return e_failure;
        }
        
        // Initialize the character to zero
        char string_buffer[100] = {0};
        int count=0;
        // Decode the character from the least significant bit of each byte
        for (int i = 0; i < *length; i++) {
        for (int j = 0; j < 8; j++) {
           string_buffer[i] = (string_buffer[i] & ~(1 << j)) | ((char_buffer[count] & 1) << j);           
            count++;
        }
    }
    strcpy(str , string_buffer);
    
    return e_success;
}


























