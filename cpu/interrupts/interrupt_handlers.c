#include "interrupt_handlers.h"
#include "../registers.h"
#include "../../drivers/screen.h"
#include "pic.h"

isr_t interrupt_handlers[256];

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
  kprint("received interrupt: ");
  char str[128];
  itoa(regs.int_no, str);
  kprint(str);
  kprint(", ");
  kprint(exception_messages[regs.int_no]);
  kprint("\n");
} 

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
  // char str[128];
  // kprint("IRQ: ");
  // itoa(regs.int_no - 32, str);
  // kprint(str);
  // kprint("\n");

  PIC_sendEOI(regs.int_no - 32);

  if (interrupt_handlers[regs.int_no] != 0)
  {
    isr_t handler = interrupt_handlers[regs.int_no];
    handler(regs);
  }
}

// Register new Interrupt handler
void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
} 

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
 
void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}