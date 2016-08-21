#ifndef __TL7689_CPU_FREQUENCY_H__
#define __TL7689_CPU_FREQUENCY_H__

struct tl7689_cpufreq_config {
	unsigned int max_frequency;
};

extern struct tl7689_cpufreq_config*  tl7689_cpufreq_cfg;

#endif
