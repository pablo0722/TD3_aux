#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // open()
#include <unistd.h> // read(), write()
#include <stdio.h>  // printf()
#include <string.h> // strlen()










int main()
{
    int fd;
    char buf[200] = {0};

    fd = open("/dev/cind_dev", O_RDWR);

    printf("fd: %d\n", fd);

    if(fd < 0)
    {
        perror("mal abierto");

        return -1;
    }

    scanf("%s", buf);

    printf("escribiendo...\n");
    write(fd, buf, strlen(buf) + 1);

    buf[0] = '\0';

    printf("leyendo...\n");
    read(fd, buf, 20);

    printf("cerrando...\n");
    close(fd);

    printf("buf: \"%s\"\n",buf);


    return 0;
}