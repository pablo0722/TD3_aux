#define DEBUG_LEVEL_NONE    0
#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_INFO    2
#define DEBUG_LEVEL_LOG     3

#define DEBUG               DEBUG_LEVEL_LOG


#if DEBUG >= DEBUG_LEVEL_ERROR
    #define DEBUG_ERROR(fmt, ...)   printk(KERN_ERR "[ERROR] i2c_td3: (func: %s, line: %d)\n", __FUNCTION__, __LINE__); \
                                    printk(KERN_ERR  fmt, ##__VA_ARGS__)
#else
    #define DEBUG_ERROR(fmt, ...)
#endif 

#if DEBUG >= DEBUG_LEVEL_INFO
    #define DEBUG_INFO(fmt, ...)    printk(KERN_INFO "[INFO] i2c_td3: (func: %s, line: %d)\n", __FUNCTION__, __LINE__); \
                                    printk(KERN_INFO fmt, ##__VA_ARGS__)
#else
    #define DEBUG_INFO(fmt, ...)
#endif 

#if DEBUG >= DEBUG_LEVEL_LOG
    #define DEBUG_LOG(fmt, ...)     printk(KERN_DEBUG "[LOG] i2c_td3: (func: %s, line: %d)\n", __FUNCTION__, __LINE__); \
                                    printk(KERN_DEBUG fmt, ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...)
#endif 