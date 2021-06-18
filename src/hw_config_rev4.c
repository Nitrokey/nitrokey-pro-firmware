#include "hw_config.h"
#include "hw_config_rev4.h"


// TODO "compress" the structure by using local ids instead of the full 32-bit GPIO pointers, e.g. map A => GPIOA separately
struct PortPin {
    GPIO_TypeDef * port;
    uint16_t pin;
};

uint32_t get_peripheral_for_port(GPIO_TypeDef *port) {
    if (port == GPIOA) return RCC_APB2Periph_GPIOA;
    if (port == GPIOB) return RCC_APB2Periph_GPIOB;
    if (port == GPIOC) return RCC_APB2Periph_GPIOC;
    if (port == GPIOD) return RCC_APB2Periph_GPIOD;
    // FIXME report error for invalid argument
    return RCC_APB2Periph_GPIOA;
}

#define ARR_LEN(x)      (sizeof(x)/sizeof((x)[0]))
void init_BGA_hardware(void) {

    struct PortPin pins_to_input[] = {
            {GPIOB, GPIO_Pin_7},
            {GPIOB, GPIO_Pin_9},
            {GPIOB, GPIO_Pin_13},
            {GPIOC, GPIO_Pin_0},
            {GPIOC, GPIO_Pin_5},
            {GPIOC, GPIO_Pin_7},
            {GPIOC, GPIO_Pin_8},
            {GPIOC, GPIO_Pin_15},
    };

    for (int i = 0; i < ARR_LEN(pins_to_input); ++i) {
        // enable port clock
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd (get_peripheral_for_port(pins_to_input[i].port), ENABLE);
        GPIO_InitStructure.GPIO_Pin = pins_to_input[i].pin;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // TODO use analog to decrease power consumption
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        // set pin modes
        GPIO_Init ( pins_to_input[i].port, &GPIO_InitStructure);
    }
}
