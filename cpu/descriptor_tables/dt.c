#include "dt.h"

#include "gdt.h"
#include "idt.h"

// initialises the GDT and IDT.
void init_descriptor_tables()
{
    init_gdt();

    init_idt();
}