#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
/* read and validate encode args from argv*/
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
//step1:checking argv[2] is contains sub string as ".bmp"
if(strstr(argv[2],".bmp")){                 //substring present or not
//if( contains store file name into encInfo->src_image_fname)
encInfo->src_image_fname=argv[2];
}
//ELSE return e_failure
else{
    printf("Error:source message image file should contain .bmp file\n");
    return e_failure;
}
//step2:checking argv[3] is contains sub string as "."
char *dot=strchr(argv[3],'.');
if(dot){
//if( contains store file name into encInfo->secret_fname)
encInfo->secret_fname=argv[3];
strcpy(encInfo->extn_secret_file,dot);                           //save that extension
encInfo->exten_size=strlen(encInfo->extn_secret_file);            //save the size of the extension
}
else {
    printf("Error:secret file should contain extenstion file\n");      //ELSE return e_failure
    return e_failure;
}
//step3:checking argv[4]!=NULL
if(argv[4]!=NULL){
if(strstr(argv[4],".bmp")!=NULL)                              //if( contains store file name into encInfo->stego_image_fname)
encInfo->stego_image_fname=argv[4];
else {
    printf("ERROR: the .bmp file is not available\n");         //ELSE return e_failure

    return e_failure;
}
}
else {                                                        //ELSE

    static char  outputfile[]="outputfile.bmp";
     encInfo->stego_image_fname=outputfile;   
     printf("INFO: Output file not mentioned. Creating %s as defualt\n",encInfo->stego_image_fname);                                          //take defualt file name,store file name into encInfo->stego_image_fname

}
return e_success;
}
Status do_encoding(EncodeInfo *encInfo){
    if(open_files(encInfo)==e_success){
        printf("INFO: Done\n");
        printf("ALERT: ENter the magic string it should be less than 20 characters:");
         static char magic_str[20];
        scanf("%[^\n]",magic_str);    
        getchar();
        encInfo->magic_string = magic_str;
        encInfo->magic_strlen=(int)strlen(magic_str);
        if(check_capacity(encInfo)==e_success){
            printf("INFO : Done. Found ok\n");
                if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)== e_success){
                    printf("INFO: ## Validations are done encode procedure started ##\n");
                    printf("INFO: Started to ENcode magic string length \n");
                    if(encode_integer_sec_file(encInfo->magic_strlen,encInfo)== e_success){//magic string integer
                        printf("INFO: Process done\n ");
                        printf("INFO: started to encode magic string\n");
                        if(encode_char_sec_file(encInfo->magic_strlen,encInfo->magic_string,encInfo)==e_success){//magic string
                            printf("INFO: Process done\n");
                            if(encode_integer_sec_file(encInfo->exten_size,encInfo)==e_success){
                                printf("INFO: Started to encode secret file Extention\n");
                                if(encode_char_sec_file(encInfo->exten_size,encInfo->extn_secret_file,encInfo)==e_success){
                                    printf("INFO: Process Done\n");
        
                            if(encode_integer_sec_file(encInfo->size_secret_file,encInfo)==e_success){//secret exten length integer
                                printf("INFO: Started to encode secret data\n");
                                if(encode_char_sec_file(encInfo->size_secret_file,encInfo->secret_data,encInfo)==e_success){//secret file extention string
                                    printf("INFO: Process done\n");
                                    printf("INFO: Required things encoded\n");
                                    
                                    if(copy_remaining_img_data(encInfo)==e_success){
                                        printf("INFO: process done\n");
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
                        else {
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
        else {
    return e_failure;
   }
}



Status check_capacity(EncodeInfo *encInfo){
    int image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("image capacity: %d\n",image_capacity);
    encInfo->image_capacity=image_capacity;
    if(read_and_Validate_secret_file(encInfo) == e_success){
    printf("INFO : Done. Not empty\n");
		printf("INFO : Checking for %s capacity to handle %s \n",encInfo->src_image_fname,encInfo->secret_fname);
    uint encoding_things=(uint)54+(uint)(4+(encInfo->magic_strlen)+4+(encInfo->exten_size)+4+(encInfo->size_secret_file))*(uint)8;
    printf("encoding things: %d\n",encoding_things);
    printf("INFO : checking %s for encoding %s file secret message\n",encInfo->src_image_fname,encInfo->stego_image_fname);
    if(image_capacity>encoding_things)
    {
        return e_success;
    }
    else{
        return e_failure;
    }
}
}
Status read_and_Validate_secret_file(EncodeInfo *encInfo){// store secret datas for encode 
	
	fseek(encInfo->fptr_secret,0,SEEK_END);
	encInfo->size_secret_file=(int)ftell(encInfo->fptr_secret);
	rewind(encInfo->fptr_secret);
    static char secret[MAX_SECRET_BUF_SIZE];
	if(fgets(secret,MAX_SECRET_BUF_SIZE,encInfo->fptr_secret) == NULL ){
		return e_failure;
	}
    strcpy(encInfo->secret_data,secret);
	return e_success;
}


uint get_image_size_for_bmp(FILE *fptr_src_image){// get Img size for BMP file
	fseek(fptr_src_image,0,SEEK_END);
	return (uint)ftell(fptr_src_image);
    rewind(fptr_src_image);
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    rewind (fptr_src_image);
int fetch_mem=54;
unsigned char image_buffer[fetch_mem];
if(fread(image_buffer,sizeof(char),fetch_mem,fptr_src_image)==fetch_mem) {
printf("INFO: copying image Header\n");
}
else{
    printf("ERROR: cant copy source image due to error\n");
return e_failure;
}
printf("%s\n",image_buffer);
    if(fwrite(image_buffer,sizeof(char),fetch_mem,fptr_dest_image)==fetch_mem){
        printf("INFO: Done\n ");
    }
    else{
        printf("ERROR: error occured while writing image source to output file\n");
        return e_failure;
    }

    return e_success;
}

Status encode_integer_sec_file(int length, EncodeInfo *encInfo)
{
unsigned char RGB[32];
if(fread(RGB, sizeof(char),32,encInfo->fptr_src_image)!=32)
{
    printf("ERROR: failed to fetch data from image\n");
    return e_failure;
}
for(int i=0;i<32;i++){
    int fetch=(length >>i)&1;
    RGB[i]=(RGB[i] & ~1) | fetch;
}
if(fwrite(RGB,sizeof(char),32,encInfo->fptr_stego_image)!=32){
printf("ERROR: failed to write the modified data to stego image\n");
return e_failure;
}
return e_success;

 }
Status encode_char_sec_file(int length,char *string, EncodeInfo *encInfo)
{
int count = 0;
int fetch = length*8;
unsigned char image_buffer[fetch];
if(fread(image_buffer,sizeof(char),fetch,encInfo->fptr_src_image)!=fetch){
    printf("ERROR: failed to fetch data from image\n");
    return e_failure;
}
for(int i=0;i<length;i++){
    for(int j=0;j<8;j++){
        char mask=(*(string+i) >> j) & 1;
        image_buffer[count]=(image_buffer[count]& ~1 ) | mask;
        count++;
    }
}
if(fwrite(image_buffer,sizeof(char),fetch,encInfo->fptr_stego_image)!=fetch){
    printf("ERROR: failed to write the modified data to stego image\n");
    return e_failure;
}
return e_success;
   }
    
Status copy_remaining_img_data(EncodeInfo *encInfo){

    long int fetch_mem=(encInfo->image_capacity)-(long int)ftell(encInfo->fptr_stego_image);
    printf("INFO: Copying left over data\n");
    char image_buffer[fetch_mem];
    if(fread(image_buffer,sizeof(char),fetch_mem,encInfo->fptr_src_image)!=fetch_mem){
        printf("ERROR: while remaining data into %s \n",encInfo->src_image_fname);
        return e_failure;
    }
    if(fwrite(image_buffer,sizeof(char),fetch_mem,encInfo->fptr_stego_image)!=fetch_mem){
        printf("ERROR: while writing remaining data into %s \n",encInfo->stego_image_fname);
        return e_failure;
}
return e_success;
}
/*void sec_encode(EncodeInfo *encInfo){
    int fetch=(int)encInfo->size_secret_file * 8;
    char RGB_buff[fetch];
    if(fread(RGB_buff,sizeof(char),fetch,encInfo->fptr_src_image) != fetch){
       printf("Error : cant RGB fetch data \n");
       return e_failure;
    }
    for(int i=0;i<(int)decInfo->size_secret_file;i++){
        for(int j=0;j<fetch;j++){
            char mask = (encInfo->secret_data + i)
        }
    }

}

*/