/**
 * @file
 * @brief
 *
 * @date
 * @author
 */

#include <kernel/irq.h>
#include <embox/unit.h>
#include <kernel/printk.h>

#include <drivers/irqctrl.h>
#include <xen/event.h>

extern shared_info_t xen_shared_info;
extern void do_divide_error(void);

EMBOX_UNIT_INIT(xen_int_init);

static int xen_int_init(void) {
	return 0;
}

void irqctrl_enable(unsigned int irq) {
}

void irqctrl_disable(unsigned int irq) {
	printk("in disable\n");
}

void irqctrl_force(unsigned int irq) {
    //for (volatile int j = 0; j < 2; ++j){
		//for (volatile int i=0; i < 1000000000; ++i){

		//}
	//}
    printk ("IRQ number ======%d\n", irq);
    int x= 0;
    int y = 100 / x; //do_divide_error();
    printk ("%d\n", y);
}

void interrupt_handle(void) {
}
