# Display drivers

ifdef CONFIG_DISPLAY_SUPPORT
display-objs				:= ns115_display-sysfs.o
else
display-objs				:= display-sysfs.o
endif

obj-$(CONFIG_DISPLAY_SUPPORT)		+= display.o
obj-y					+= tve/
