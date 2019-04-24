#ifndef THREAD_H_
#define THREAD_H_










/************************************
 *          FUNCTIONS               *
 ************************************/
enum E_RET_THREAD
{
	RET_THREAD_OK,
	RET_THREAD_ERROR_THREAD_ATTR_INIT,
	RET_THREAD_ERROR_THREAD_ATTR_DETACH,
	RET_THREAD_ERROR_THREAD_CREATE
};










/************************************
 *          FUNCTIONS               *
 ************************************/
enum E_RET_THREAD create_thread(pthread_t *thread_id, void * (*thread_func)(void *arg), void *arg);










#endif // THREAD_H_