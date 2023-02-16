#include "idt.h"
#include "../interrupts/pic.h"

#define IDT_MAX_DESCRIPTORS     256

extern void idt_flush(uint32_t);

static void idt_set_descriptor(uint8_t vector, void* base, uint16_t kenrel_cs ,uint8_t flags);

__attribute__((aligned(0x10))) 
static idt_entry_t idt_entries[IDT_MAX_DESCRIPTORS];
static idt_ptr_t idtr;

extern void* isr_stub_table[];
extern void* irq_stub_table[];

//Set the value of one IDT entry
static void idt_set_descriptor(uint8_t vector, void* base, uint16_t kernel_cs ,uint8_t flags) {
    idt_entry_t* descriptor = &idt_entries[vector];
 
    descriptor->base_low        = (uint32_t)base & 0xFFFF;
    descriptor->kernel_cs       = kernel_cs;                                  // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes      = flags;
    descriptor->base_high       = (uint32_t)base >> 16;
    descriptor->reserved        = 0;
}
 
void init_idt() {
    idtr.base = (uintptr_t)&idt_entries[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
    
    uint8_t vector;
    for (vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x08, 0x8E);
    }

    PIC_remap(0x20, 0x28);

    for(uint8_t i = 0; i < 16; i++)
    {
        idt_set_descriptor(vector, irq_stub_table[i], 0x08, 0x8E);
        vector++;
    }
 
    __asm__ volatile ("lidt %0" : : "m"(idtr));                         // load the new IDT
    __asm__ volatile ("sti");                                           // set the interrupt flag
}