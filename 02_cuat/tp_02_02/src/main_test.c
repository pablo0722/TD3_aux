#include <stdio.h>      // printf()
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <stdlib.h>     // exit()
#include "i2c_td3.h"    // close()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>





int main()
{
    int fd;
    int buf;
    int cant_reads;


    printf ("Abriendo dispositivo...\n");
    
    while (1)
    {
    fd = open("/dev/i2c_td3_dev", O_RDONLY);
    printf ("Dispositivo abierto\n");

    if (fd < 0)
    {
        perror("/dev/i2c_td3");

        exit (-1);
    }



        ioctl(fd, I2C_TD3_CMD_READ_NUM_READS, &cant_reads);
        read(fd, &buf, sizeof(int));
        printf("Leido(%d): \"%d\"\n", cant_reads, buf);
        sleep(1);


        

    close (fd);
    }

    printf ("fin\n");

    return 0;
}