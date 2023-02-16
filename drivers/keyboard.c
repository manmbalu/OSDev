#include "keyboard.h"
#include "../cpu/interrupts/interrupt_handlers.h"
#include "../cpu/interrupts/pic.h"
#include "screen.h" 

// Callback for IRQ1
void KeyboardCallback(registers_t regs)
{
    kprint("Received Key\n");
}

// Initialize keyboard
void init_keyboard()
{
    register_interrupt_handler(IRQ1, &KeyboardCallback);
}