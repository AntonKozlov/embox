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

#define SET_BIT(bit,field) __asm__ __volatile__ ("lock btsl %1,%0":"=m"(field):"Ir"(bit):"memory" );
#define CLEAR_BIT(field, bit) __asm__ __volatile__ ("lock btrl %1,%0":"=m" ((field)):"Ir"((bit)):"memory")

extern shared_info_t xen_shared_info;
extern void do_divide_error(void);

EMBOX_UNIT_INIT(xen_int_init);

static int xen_int_init(void) {
	return 0;
}

void irqctrl_enable(unsigned int irq) {
	//CLEAR_BIT(xen_shared_info.evtchn_mask, irq); // unmask
}

void irqctrl_disable(unsigned int irq) {
	//SET_BIT(irq,xen_shared_info.evtchn_mask[0]); //mask_evtchn(i);
	printk("in disable\n");
}

void irqctrl_force(unsigned int irq) {
    //for (volatile int j = 0; j < 2; ++j){
		//for (volatile int i=0; i < 1000000000; ++i){

		//}
	//}
    //if (irq == 0){
    printk ("IRQ number ======%d\n", irq);
        int x= 0;
        int y = 100 / x; //do_divide_error();
    //int y = x + 100;
		//printk("%p",(void *) &y);
        printk ("%d\n", y);
    //}
}

void interrupt_handle(void) {
	irqctrl_disable(14);
}
