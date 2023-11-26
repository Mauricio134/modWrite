#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include "chardev.h"
#include <fcntl.h> /* open */
#include <unistd.h> /* exit */
#include <sys/ioctl.h> 

#define KEY_2 0x1112
#define KEY_3 0x1113

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};


Union(int file_desc, char valor){
    int ret_val;
    char message[100];
    
    ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);
    if (ret_val < 0) {
        printf("ioctl_get_msg failed:%d\n", ret_val);
        exit(-1);
    }
    printf("get_msg message:%s\n", message);
    char str[2];
    str[0] = valor;
    str[1] = '\0';
    strcat(message, str);

    printf("set_msg message:%s\n", message);

    ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);
    if (ret_val < 0) {
        printf("ioctl_set_msg failed:%d\n", ret_val);
        exit(-1);
    }
}

main()
{
    char c;
    char valor;
    int file_desc, ret_val;
    char msg[100];
    char file_path[20]; // o un tamaño suficientemente grande
    int sp = semget(KEY_2, 1, 0666);
    int sc = semget(KEY_3, 1, 0666);

    strcpy(file_path, "/dev/");
    strcat(file_path, DEVICE_FILE_NAME);

    for(int i = 2; i <= 29;i+=2){
        semop(sp, &p, 1);
        file_desc = open(file_path, 0);
        if (file_desc < 0) {
            printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
            exit(-1);
        }
        valor = i + '0';
        Union(file_desc, valor);

        close(file_desc);
        semop(sc, &v, 1);
    }
    exit(0);
}