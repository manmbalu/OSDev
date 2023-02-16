#ifndef PIC_H
#define PIC_H

#include "../../kernel/util.h"

#define PIC1_COMMAND    0x20                // IO base address for master PIC
#define PIC1_DATA       (PIC1_COMMAND+1)
#define PIC2_COMMAND    0xA0                // IO base address for slave PIC
#define PIC2_DATA       (PIC2_COMMAND+1)


void PIC_disable();
void PIC_remap(int masterOffset, int slaveOffset);
void PIC_sendEOI(uint8_t irq);

#endif