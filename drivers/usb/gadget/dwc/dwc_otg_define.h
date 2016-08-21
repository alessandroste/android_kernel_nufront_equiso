#ifndef __DWC_OTG_DEFINE__
#define __DWC_OTG_DEFINE__

#define DWC_LINUX
#define DWC_DEVICE_ONLY
//#define DWC_HOST_ONLY

#define EN_DEBUG
//#define DEBUG
//#define DEBUG_SOF

#ifdef EN_DEBUG
void dump_data(char * data, int len, int prntAll);
#define DUMP_DATA		dump_data
#else
#define  DUMP_DATA(...)			do{}while(0)
#endif

#define DRIVER_NAME		"DWC_OTG"
#define  PERR(format,...)       printk(DRIVER_NAME "-[%s#%d:%s]:"format, \
					__FILE__,__LINE__,__func__ , ##__VA_ARGS__ )
#define  PLOG(format,...)		printk(DRIVER_NAME "-[%s#%d:%s]:"format, \
					__FILE__,__LINE__,__func__ , ##__VA_ARGS__ )
#define  PRINT(format,...)		printk(format, ##__VA_ARGS__ )

#ifdef   EN_DEBUG
#define  PDBG(format,...)		printk(DRIVER_NAME "-[%lu#%d:%s]:"format, \
					jiffies,__LINE__,__func__ , ##__VA_ARGS__ )
#define  DUMP_DATA(...)			dump_data(__VA_ARGS__)
#else
#define  PDBG(format,...)		do{}while(0)
#define  DUMP_DATA(...)			do{}while(0)
#endif

#define  ENTER_FUNC()                        PDBG("enter func: %s\n",__func__);
#define  LEAVE_FUNC()                        PDBG("leave func: %s\n",__func__);


#endif  // __DWC_OTG_DEFINE__
