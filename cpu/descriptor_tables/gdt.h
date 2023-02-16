#ifndef GDT_H
#define GDT_H

#include "../../kernel/util.h"

// Struct of GDT entry.
typedef struct
{
    uint16_t limit_low;           // The lower 16 bits of the limit.
    uint16_t base_low;            // The lower 16 bits of the base.
    uint8_t  base_middle;         // The next 8 bits of the base.
    uint8_t  access;              // Access flags, determine what ring this segment can be used in.
    uint8_t  granularity;
    uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

// Struct for GDT pointer
typedef struct
{
    uint16_t limit;               // The upper 16 bits of all selector limits.
    uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

// Initialise GDT
void init_gdt();

// Get the loaded GDT pointer
void get_gdt_ptr(gdt_ptr_t *ptr);

#endif