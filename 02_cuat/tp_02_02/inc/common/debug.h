#ifndef DEBUG_H_
#define DEBUG_H_










/************************************
 *			DEFINE					*
 ************************************/
 #define DEBUG










/************************************
 *			DEBUG					*
 ************************************/
 #ifdef DEBUG
 #define LOG(...) printf(__VA_ARGS__)
 #else
 #define LOG(...)
 #endif










#endif // DEBUG_H_