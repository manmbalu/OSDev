#include "util.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../cpu/descriptor_tables/dt.h"
#include "../cpu/timer.h"

void k_main()
{
    init_descriptor_tables();

    init_keyboard();

    init_timer(50000);

    //asm volatile("int $33");
}

