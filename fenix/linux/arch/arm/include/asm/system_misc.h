#ifndef __ASM_ARM_SYSTEM_MISC_H
#define __ASM_ARM_SYSTEM_MISC_H

#ifndef __ASSEMBLY__

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/irqflags.h>
#include <linux/reboot.h>
#ifdef CONFIG_AMLOGIC_USER_FAULT
#include <linux/ratelimit.h>
#endif
#include <linux/percpu.h>

extern void cpu_init(void);

void soft_restart(unsigned long);
extern void (*arm_pm_restart)(enum reboot_mode reboot_mode, const char *cmd);
extern void (*arm_pm_idle)(void);

#ifdef CONFIG_AMLOGIC_USER_FAULT
extern void show_all_pfn(struct task_struct *task, struct pt_regs *regs);
#else
static inline void show_all_pfn(struct task_struct *task, struct pt_regs *regs)
{
}
#endif /* CONFIG_AMLOGIC_USER_FAULT */
#ifdef CONFIG_HARDEN_BRANCH_PREDICTOR
typedef void (*harden_branch_predictor_fn_t)(void);
DECLARE_PER_CPU(harden_branch_predictor_fn_t, harden_branch_predictor_fn);
static inline void harden_branch_predictor(void)
{
	harden_branch_predictor_fn_t fn = per_cpu(harden_branch_predictor_fn,
						  smp_processor_id());
	if (fn)
		fn();
}
#else
#define harden_branch_predictor() do { } while (0)
#endif

#define UDBG_UNDEFINED	(1 << 0)
#define UDBG_SYSCALL	(1 << 1)
#define UDBG_BADABORT	(1 << 2)
#define UDBG_SEGV	(1 << 3)
#define UDBG_BUS	(1 << 4)

extern unsigned int user_debug;

#ifdef CONFIG_AMLOGIC_USER_FAULT
#define USR_FAULT_DBG_RATELIMIT_INTERVAL	(5 * HZ)
#define USR_FAULT_DBG_RATELIMIT_BURST		3

#define user_fault_debug_ratelimited()					\
({									\
	static DEFINE_RATELIMIT_STATE(usr_fault_dgb_rs,			\
				      USR_FAULT_DBG_RATELIMIT_INTERVAL,	\
				      USR_FAULT_DBG_RATELIMIT_BURST);	\
	bool __show_ratelimited = false;				\
	if (__ratelimit(&usr_fault_dgb_rs))				\
		__show_ratelimited = true;				\
	__show_ratelimited;						\
})
#endif

#endif /* !__ASSEMBLY__ */

#endif /* __ASM_ARM_SYSTEM_MISC_H */
