#ifndef IPC_H_
#define IPC_H_










/************************************
 *          FUNCIONES               *
 ************************************/
int  create_semaphore           ();
void lock_semaphore             (int sem_id);
void wait_for_locked_semaphore  (int sem_id);
void unlock_semaphore           (int sem_id);










#endif // IPC_H_