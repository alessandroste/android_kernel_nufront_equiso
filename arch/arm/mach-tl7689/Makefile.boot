ifeq ($(CONFIG_TL7689_HIGH_PHYS_OFFSET),y)
   zreladdr-y	:= 0x90008000
params_phys-y	:= 0x90000100
initrd_phys-y	:= 0x90800000
else
   zreladdr-y	:= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
endif
