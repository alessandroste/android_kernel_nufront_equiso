#ifndef _TL7689_ARCH_COMMON_H_
#define	_TL7689_ARCH_COMMON_H_


#define S(p0,p1,p2,p3) ((p0)|(p1<<2)|(p2<<4)|(p3<<6))
#define PINMUX(s0,s1,s2,s3) ((s0)|(s1<<8)|(s2<<16)|(s3<<24))

#define TL7689_PINMUX_SIZE 10
#define TL7689_PINMUX(board) unsigned int tl7689_##board##_pinmux[TL7689_PINMUX_SIZE] 
#define TL7689_PINMUX_DECLARE(board) extern unsigned int tl7689_##board##_pinmux[TL7689_PINMUX_SIZE]
#define TL7689_PINMUX_INIT(board) pinmux_init(tl7689_##board##_pinmux, \
					ARRAY_SIZE(tl7689_##board##_pinmux))

extern int pinmux_init(unsigned int * setting, int len);

extern void common_init(void);
extern void ddr_pm_init(void);
extern void scm_init(void);
#endif
