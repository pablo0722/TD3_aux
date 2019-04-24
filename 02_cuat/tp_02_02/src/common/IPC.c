/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdlib.h>     // NULL, exit()
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

// Common headers
#include "debug.h"      // DEBUG
#include "basics.h"
#include "IPC.h"










/************************************
 *          STRUCTS                 *
 ************************************/
union semun
{
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};










/************************************
 *          DEFINES                 *
 ************************************/
#define SEM_KEY 0x123456










/************************************
 *          FUNCIONES               *
 ************************************/
int create_semaphore()
{
    int sem_id;
    union semun arg;
    int err;


    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id == -1) 
    {
        perror ("semget error");
        exit (-1);
    }
    

    arg.val = 1;

    err = semctl(sem_id, 0, SETVAL, arg);
    if (err == -1) 
    {
        perror ("semctl error");
        exit (-1);
    }


    return sem_id;
}










void lock_semaphore(int sem_id)
{
    struct sembuf sops;
    int ret_val;

    sops.sem_num    = 0;
    sops.sem_op     = -1;
    sops.sem_flg    = 0;

    ret_val = semop(sem_id, &sops, 1);
    if (ret_val == -1)
    {
        perror ("semop lock");
    }
}










void wait_for_locked_semaphore(int sem_id)
{
    struct sembuf sops;
    int ret_val;

    sops.sem_num    = 0;
    sops.sem_op     = 0;
    sops.sem_flg    = 0;

    ret_val = semop(sem_id, &sops, 1);
    if (ret_val == -1)
    {
        perror ("semop wait for locked");
    }
}










void unlock_semaphore(int sem_id)
{
    struct sembuf sops;
    int ret_val;

    sops.sem_num    = 0;
    sops.sem_op     = 1;
    sops.sem_flg    = 0;

    ret_val = semop(sem_id, &sops, 1);
    if (ret_val == -1)
    {
        perror ("semop unlock");
    }
}