/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdio.h>      // printf()
#include <pthread.h>    // pthread_create(), pthread_self()

// Common headers
#include "thread.h"     // enum E_RET_THREAD










/************************************
 *          FUNCTIONS               *
 ************************************/
enum E_RET_THREAD create_thread(pthread_t *thread_id, void * (*thread_func)(void *arg), void *arg)
{
    pthread_t thread_aux;
    pthread_attr_t attr;

    if (thread_id == NULL)
    {
        thread_id = &thread_aux;
    }

    if (pthread_attr_init(&attr) != 0)
    {
        printf ("Error on pthread_attr_init()\n");

        return (RET_THREAD_ERROR_THREAD_ATTR_INIT);
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        printf ("Error on pthread_attr_setdetachstate()\n");

        return (RET_THREAD_ERROR_THREAD_ATTR_DETACH);
    }

    if (pthread_create (thread_id, &attr, thread_func, arg) != 0)
    {
        printf ("Error on pthread_create()\n");

        return (RET_THREAD_ERROR_THREAD_CREATE);
    }

    pthread_attr_destroy(&attr);

    return RET_THREAD_OK;
}