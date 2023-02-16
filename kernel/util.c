#include "util.h"

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("out %%al, %%dx" : : "a"(value), "d" (port));
}

uint8_t inb(uint16_t port)
{
    uint8_t result;
    __asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

uint16_t inw(uint16_t port)
{
    uint16_t result;
    __asm__ volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void io_wait(void)
{
    outb(0x80, 0);
}

void memcpy(uint8_t * source, uint8_t * dest, uint32_t len )
{
    for(uint32_t i = 0; i < len; i++)
    {
        dest[i] = source[i];
    }
}