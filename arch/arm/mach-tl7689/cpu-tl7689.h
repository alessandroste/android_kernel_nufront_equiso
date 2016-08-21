#ifndef __CPU_TL7689_H__
#define __CPU_TL7689_H__

extern int tl7689_auto_hotplug_init(struct mutex *cpu_lock, unsigned int top, unsigned int bottom);
extern void tl7689_auto_hotplug_governor(unsigned int cpu_freq, bool suspend);
extern void tl7689_auto_hotplug_exit(void);
extern void tl7689_auto_hotplug_enable(bool flag);
extern void tl7689_auto_hotplug_suspend(void);
extern void tl7689_auto_hotplug_resume(void);

#endif
