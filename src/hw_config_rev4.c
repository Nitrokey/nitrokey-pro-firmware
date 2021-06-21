#include "hw_config.h"
#include "hw_config_rev4.h"
#include "platform_config.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

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


typedef uint32_t Peripheral_t;
typedef uint32_t Remap_t;
typedef uint16_t Pin_t;
typedef void (*ClockCmd_t)(uint32_t, FunctionalState);

typedef struct PinDefinition{
    GPIO_TypeDef * port;
    Pin_t pin_number;
    GPIOMode_TypeDef mode;
} PinDefinition;

typedef enum MapClock{
    MAP_CLK_UNSET = 0,
    MAP_CLK_PCLK1 = 1,
    MAP_CLK_PCLK2 = 2
} MapClock;

typedef uint8_t PrescalerValue;
typedef uint8_t InterruptValue;

struct HardwareDefinition{
    struct {
        USART_TypeDef* USART;
        ClockCmd_t clock_cmd;
        Peripheral_t usart_peripheral;
        Peripheral_t afio_peripheral;
        Remap_t remap_mapping;
        Remap_t remap_mapping_value;
    } usart;
    struct {
        PinDefinition power_port_1;
        PinDefinition power_port_2;
        PinDefinition sc_clk;
        PinDefinition sc_sda;
        PinDefinition sc_rst;
    } pins;
    struct {
        MapClock map_clock;
        PrescalerValue prescaler_value;
    } clock;
    struct {
        InterruptValue usart;
        InterruptValue exti;
    } interrupts;

    struct {
        PinDefinition smartcard;
        PinDefinition oath;
    } led;
};

const struct HardwareDefinition HW3 = {
        .usart = {
                .USART = USART1,
                .clock_cmd = RCC_APB2PeriphClockCmd,
                .usart_peripheral = RCC_APB2Periph_USART1,
                .afio_peripheral = RCC_APB2Periph_AFIO,
                .remap_mapping = AFIO_MAPR_USART1_REMAP,
                .remap_mapping_value = ENABLE
        },
        .pins = {
                .power_port_1 = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_4,
                        .mode = GPIO_Mode_Out_PP
                },
                .power_port_2 = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_5,
                        .mode = GPIO_Mode_Out_PP
                },
                .sc_clk = {
                        .port = GPIOA,
                        .pin_number = GPIO_Pin_8,
                        .mode = GPIO_Mode_AF_PP
                },
                .sc_sda = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_6,
                        .mode = GPIO_Mode_AF_OD
                },
                .sc_rst = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_3,
                        .mode = GPIO_Mode_Out_PP
                },
        },
        .clock = {
                .map_clock = MAP_CLK_PCLK2,
                .prescaler_value = 20/2
        },
        .interrupts = {
                .usart = USART1_IRQChannel,
                .exti = EXTI9_5_IRQChannel
        },
        .led = {
                .smartcard = {
                        .port = GPIOA,
                        .pin_number = GPIO_Pin_7,
                        .mode = GPIO_Mode_Out_PP
                },
                .oath = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_0,
                        .mode = GPIO_Mode_Out_PP
                }
        }
};

const struct HardwareDefinition HW4 = {
        .usart = {
                .USART = USART3,
                .clock_cmd = RCC_APB1PeriphClockCmd,
                .usart_peripheral = RCC_APB1Periph_USART3,
                .afio_peripheral = RCC_APB2Periph_AFIO,
                .remap_mapping = AFIO_MAPR_USART3_REMAP,
                .remap_mapping_value = AFIO_MAPR_USART3_REMAP_NOREMAP
        },
        .pins = {
                .power_port_1 = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_4,
                        .mode = GPIO_Mode_Out_PP
                },
                .power_port_2 = {
                        .port = GPIOD,
                        .pin_number = GPIO_Pin_2,
                        .mode = GPIO_Mode_Out_PP
                },
                .sc_clk = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_12,
                        .mode = GPIO_Mode_AF_PP
                },
                .sc_sda = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_10,
                        .mode = GPIO_Mode_AF_OD
                },
                .sc_rst = {
                        .port = GPIOB,
                        .pin_number = GPIO_Pin_3,
                        .mode = GPIO_Mode_Out_PP
                },
        },
        .clock = {
                .map_clock = MAP_CLK_PCLK1,
                .prescaler_value = 10/2
        },
        .interrupts = {
                .usart = USART3_IRQChannel,
                .exti = EXTI15_10_IRQChannel
        },
        .led = {
                .smartcard = {
                        .port = GPIOA,
                        .pin_number = GPIO_Pin_4,
                        .mode = GPIO_Mode_Out_PP
                },
                .oath = {
                        .port = GPIOA,
                        .pin_number = GPIO_Pin_7,
                        .mode = GPIO_Mode_Out_PP
                }
        }
};

