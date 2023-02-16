#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void io_wait(void);

void memcpy(uint8_t * source, uint8_t * dest, uint32_t len );

#endif