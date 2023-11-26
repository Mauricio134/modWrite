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

#define KEY_1 0x1111
#define KEY_3 0x1113

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};
Union(int file_desc, char valor, int indice){
    int ret_val;
    char message[100];
    
    ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);
    if (ret_val < 0) {
        printf("ioctl_get_msg failed:%d\n", ret_val);
        exit(-1);
    }
    printf("get_msg message:%s\n", message);
    message[indice] = valor;

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
    int si = semget(KEY_1, 1, 0666);
    int sc = semget(KEY_3, 1, 0666);


    strcpy(file_path, "/dev/");
    strcat(file_path, DEVICE_FILE_NAME);

    int j = 2;
    for(int i = 0; j <= 29;j+=3, i++){
        semop(sc, &p, 1);
        file_desc = open(file_path, 0);
        if (file_desc < 0) {
            printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
            exit(-1);
        }
        valor = 'A'+i;
        Union(file_desc, valor, j);

        close(file_desc);
        semop(si, &v, 1);
    }
    exit(0);
}