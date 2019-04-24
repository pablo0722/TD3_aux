#ifndef BASICS_H_
#define BASICS_H_










/************************************
 *			INCLUDES				*
 ************************************/
#include <stdio.h>  // perror()
#include <errno.h>  // errno










/************************************
 *			DEFINE					*
 ************************************/
#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); } while (0) // pthread_sigmask()










/************************************
 *          TYPEDEF                 *
 ************************************/
typedef enum E_BOOL
{
    false,
    true
} bool;










#endif // BASICS_H_