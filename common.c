#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include "decode.h"



int main(int argc, char *argv[])
{
    int decodethings=check_operation_type(argv);
    switch(decodethings){
        case e_decode :
        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo)==e_success){
            printf(" ## decoding procedure started ##\n");
            if(do_decode(&decInfo)==e_success){
                printf(" ## decoding done successfully ##\n");
                return 0;
            }
            else {
                return -1;
            }
        }
        else {
            return -1;
        }
    break;
    case e_encode :
    EncodeInfo encInfo;
    //checking operation type encoding or decoding
    int opera = check_operation_type(argv);
    if(opera == e_encode){
        int status=read_and_validate_encode_args(argv,&encInfo);
        if(status==e_success){
            if(do_encoding (&encInfo)==e_success) {

            }
            else {
                return -1;
            }

        }
        else{
            printf("ERROR: encoding failed\n");
        }
    }
    else if(opera == e_decode){

printf("decode not implemented\n");
    }
    else{
        printf("Error : unsuported operation type\n");
    }


    return 0;
}
}
/*check operation type*/
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0){              //IF : checking argv[1] is "-e" ==> return e_encode
    return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0){         //ELSE IF: checking argv[1] is "-d" ==>return e_decode
    return e_decode;
}    
else {
    return e_unsupported;                        //ELSE :==>return e_unsupported
}                                     
                                            
                                            

}
