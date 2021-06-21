#ifndef NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H
#define NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H

#include <stm32f10x_rcc.h>
#include "stm32f10x_gpio.h"
#include "stddef.h"

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

typedef struct HardwareDefinition const * const HardwareDefinitionPtr;
HardwareDefinitionPtr detect_hardware(void);

uint32_t get_clock_for_map(RCC_ClocksTypeDef* status, MapClock clock);
Peripheral_t get_peripheral_for_port(GPIO_TypeDef *port);
void init_BGA_hardware(void);


#endif //NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H
