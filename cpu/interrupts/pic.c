#include "pic.h"

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
#define ICW1_ICW4	    0x01		                // ICW4 (not) needed
#define ICW1_SINGLE	    0x02		                // Single (cascade) mode
#define ICW1_INTERVAL4	0x04		                // Call address interval 4 (8)
#define ICW1_LEVEL	    0x08		                // Level triggered (edge) mode
#define ICW1_INIT	    0x10		                // Initialization - required!
 
#define ICW4_8086	    0x01		                // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO	    0x02		                // Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08		                // Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C		                // Buffered mode/master
#define ICW4_SFNM	    0x10		                // Special fully nested (not)
#define PIC_EOI			0x20	

// Disable the PIC
void PIC_disable()
{
    outb(PIC2_DATA, 0xff);
    outb(PIC1_DATA, 0xff);
}

// Remap the PIC
void PIC_remap(int masterOffset, int slaveOffset)
{
    uint8_t masterMask, slaveMask;

    // save masks
	masterMask = inb(PIC1_DATA);
	slaveMask = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);      // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, masterOffset);                  // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, slaveOffset);                   // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                             // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                             // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, masterMask);                    // restore saved masks.
	outb(PIC2_DATA, slaveMask);
}

// Send an EOI (end of interrupt) signal to the PICs.
void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}