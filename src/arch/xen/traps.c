#include <traps.h>
#include <stdint.h>
#include <xen/xen.h>
#include <kernel/printk.h>
//#include <xen/event.h>

#define DEBUG(fmt,...) printkm2(fmt, ##__VA_ARGS__)
#define SET_BIT(bit,field) __asm__ __volatile__ ("lock btsl %1,%0":"=m"(field):"Ir"(bit):"memory" );
#define CLEAR_BIT(field, bit) __asm__ __volatile__ ("lock btrl %1,%0":"=m" ((field)):"Ir"((bit)):"memory")

extern shared_info_t xen_shared_info;

#if defined(__i386__)
#include <xen_hypercall-x86_32.h>
//#elif defined(__x86_64__)
//#include <hypercall-x86_64.h>
//#elif defined(__arm__) || defined(__aarch64__)
//#include <hypercall-arm.h>
#else
#error "Unsupported architecture"
#endif

/*
 * These are assembler stubs in entry.S.
 * They are the actual entry points for virtual exceptions.
 */
void divide_error(void);
void debug(void);
void int3(void);
void overflow(void);
void bounds(void);
void invalid_op(void);
void device_not_available(void);
void coprocessor_segment_overrun(void);
void invalid_TSS(void);
void segment_not_present(void);
void stack_segment(void);
void general_protection(void);
void page_fault(void);
void coprocessor_error(void);
void simd_coprocessor_error(void);
void alignment_check(void);
void spurious_interrupt_bug(void);
void machine_check(void);

/*void dump_regs(struct pt_regs *regs)
{
    printk("Thread: %s\n", current ? current->name : "*NONE*");
#ifdef __i386__
    printk("EIP: %lx, EFLAGS %lx.\n", regs->eip, regs->eflags);
    printk("EBX: %08lx ECX: %08lx EDX: %08lx\n",
	   regs->ebx, regs->ecx, regs->edx);
    printk("ESI: %08lx EDI: %08lx EBP: %08lx EAX: %08lx\n",
	   regs->esi, regs->edi, regs->ebp, regs->eax);
    printk("DS: %04x ES: %04x orig_eax: %08lx, eip: %08lx\n",
	   regs->xds, regs->xes, regs->orig_eax, regs->eip);
    printk("CS: %04x EFLAGS: %08lx esp: %08lx ss: %04x\n",
	   regs->xcs, regs->eflags, regs->esp, regs->xss);
#else
    printk("RIP: %04lx:[<%016lx>] ", regs->cs & 0xffff, regs->rip);
    printk("\nRSP: %04lx:%016lx  EFLAGS: %08lx\n",
           regs->ss, regs->rsp, regs->eflags);
    printk("RAX: %016lx RBX: %016lx RCX: %016lx\n",
           regs->rax, regs->rbx, regs->rcx);
    printk("RDX: %016lx RSI: %016lx RDI: %016lx\n",
           regs->rdx, regs->rsi, regs->rdi);
    printk("RBP: %016lx R08: %016lx R09: %016lx\n",
           regs->rbp, regs->r8, regs->r9);
    printk("R10: %016lx R11: %016lx R12: %016lx\n",
           regs->r10, regs->r11, regs->r12);
    printk("R13: %016lx R14: %016lx R15: %016lx\n",
           regs->r13, regs->r14, regs->r15);
#endif
}*/

/*static void do_trap(int trapnr, char *str, struct pt_regs * regs, unsigned long error_code)
{
   // printk("FATAL:  Unhandled Trap %d (%s), error code=0x%lx\n", trapnr, str, error_code);
   // printk("Regs address %p\n", regs);
    //dump_regs(regs);
    //do_exit();
}*/
/*#define DO_ERROR(trapnr, str, name) \
void do_##name(struct pt_regs * regs, unsigned long error_code) \
{ \
	do_trap(trapnr, str, regs, error_code); \
}*/

/* Dummy implementation.  Should actually do something */
void do_divide_error(void) {
    printk ("DIVIDE ERROR called\n");
    SET_BIT(0,xen_shared_info.evtchn_mask[0]); // unmask

    //SET_BIT(0,xen_shared_info.evtchn_mask[0]);
}


void do_debug(void) {printk ("ERROR_1\n");}
void do_int3(void) {printk ("ERROR_2\n");}
void do_overflow(void) {printk ("ERROR_3\n");}
void do_bounds(void) {printk ("ERROR_4\n");}
void do_invalid_op(void) {printk ("ERROR_5\n");}
void do_device_not_available(void) {printk ("ERROR_6\n");}
void do_coprocessor_segment_overrun(void) {printk ("ERROR_7\n");}
void do_invalid_TSS(void) {printk ("ERROR_8\n");}
void do_segment_not_present(void) {printk ("ERROR_9\n");}
void do_stack_segment(void) {printk ("ERROR_10\n");}
void do_general_protection(void) {printk ("ERROR_11\n");}
void do_page_fault(void) {printk ("ERROR_12\n");  	CLEAR_BIT(xen_shared_info.evtchn_mask, 14); }// unmask}
void do_coprocessor_error(void) {printk ("ERROR_13\n");}
void do_simd_coprocessor_error(void) {printk ("ERROR_14\n");}
void do_alignment_check(void) {printk ("ERROR_15\n");}
void do_spurious_interrupt_bug(void) {printk ("ERROR_16\n");}
void do_machine_check(void) {}

/*
 * Submit a virtual IDT to teh hypervisor. This consists of tuples
 * (interrupt vector, privilege ring, CS:EIP of handler).
 * The 'privilege ring' field specifies the least-privileged ring that
 * can trap to that vector using a software-interrupt instruction (INT).
 */
static trap_info_t trap_table[] = {
    {  0, 0, FLAT_KERNEL_CS, (unsigned long)divide_error                },
    {  1, 0, FLAT_KERNEL_CS, (unsigned long)debug                       },
    {  3, 3, FLAT_KERNEL_CS, (unsigned long)int3                        },
    {  4, 3, FLAT_KERNEL_CS, (unsigned long)overflow                    },
    {  5, 3, FLAT_KERNEL_CS, (unsigned long)bounds                      },
    {  6, 0, FLAT_KERNEL_CS, (unsigned long)invalid_op                  },
    {  7, 0, FLAT_KERNEL_CS, (unsigned long)device_not_available        },
    {  9, 0, FLAT_KERNEL_CS, (unsigned long)coprocessor_segment_overrun },
    { 10, 0, FLAT_KERNEL_CS, (unsigned long)invalid_TSS                 },
    { 11, 0, FLAT_KERNEL_CS, (unsigned long)segment_not_present         },
    { 12, 0, FLAT_KERNEL_CS, (unsigned long)stack_segment               },
    { 13, 0, FLAT_KERNEL_CS, (unsigned long)general_protection          },
    { 14, 0, FLAT_KERNEL_CS, (unsigned long)page_fault                  },
    { 15, 0, FLAT_KERNEL_CS, (unsigned long)spurious_interrupt_bug      },
    { 16, 0, FLAT_KERNEL_CS, (unsigned long)coprocessor_error           },
    { 17, 0, FLAT_KERNEL_CS, (unsigned long)alignment_check             },
    { 19, 0, FLAT_KERNEL_CS, (unsigned long)simd_coprocessor_error      },
    {  0, 0,           0, 0                           }
};

/* Assembler interface fns in entry.S. */
void hypervisor_callback(void);
void failsafe_callback(void);

void trap_init(void)
{
    HYPERVISOR_set_trap_table(trap_table);
    printk("traps init\n");

#ifdef __i386__
    HYPERVISOR_set_callbacks(
        FLAT_KERNEL_CS, (unsigned long)hypervisor_callback,
        FLAT_KERNEL_CS, (unsigned long)failsafe_callback);
#else
    HYPERVISOR_set_callbacks(
            (unsigned long)hypervisor_callback,
            (unsigned long)failsafe_callback, 0);
#endif
}