#ifndef NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H
#define NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H

uint32_t get_peripheral_for_port(GPIO_TypeDef *port);
void init_BGA_hardware(void);

#endif //NITROKEY_PRO_FIRMWARE_HW_CONFIG_REV4_H
