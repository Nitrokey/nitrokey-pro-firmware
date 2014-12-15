#include "stm32f10x.h"
#include "platform_config.h"


uint16_t getu16(uint8_t *array){

    uint16_t result=array[0]+(array[1]<<8);
    return result;
}


uint32_t getu32(uint8_t *array){
    uint32_t result=0;
    int8_t i=0; 


    for(i=3;i>=0;i--){
        result<<=8;
        result+=array[i];
    }

    return result;
}

uint64_t getu64(uint8_t *array){
    uint64_t result=0;
    int8_t i=0; 


    for(i=7;i>=0;i--){
        result<<=8;
        result+=array[i];
    }

    return result;
}
