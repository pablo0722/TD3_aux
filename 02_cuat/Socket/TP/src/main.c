#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <sys/ioctl.h>

#include "i2c_td3.h"    // close()





int main()
{
    int fd = open("/dev/i2c_td3_dev", O_RDONLY);
    int buf;
    int cant_reads;



    if (fd < 0)
    {
        perror("/dev/i2c_td3");

        return -1;
    }
    
    while (1)
    {
        ioctl(fd, I2C_TD3_CMD_READ_NUM_READS, &cant_reads);
        read(fd, &buf, sizeof(int));
        printf("Leido(%d): \"%d\"\n", cant_reads, buf);
    }

    close (fd);

    printf ("fin\n");

    return 0;
}