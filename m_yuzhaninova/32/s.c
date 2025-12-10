#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>


void* handle(void* arg)
{
    int fd = *(int*)arg;
    free(arg);

    char c;
    while (read(fd, &c, 1) > 0)
    {
        printf("[Client %d] %c\n", fd, toupper((unsigned char)c));
    }

    close(fd);
    return NULL;
}

int main()
{
    unlink("/tmp/mysocket");

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd < 0) 
    {
        return 1;
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/mysocket", sizeof(addr.sun_path)-1);

    if (bind(sfd, (void*)&addr, sizeof(addr)) < 0)
    {
        return 1;
    }
    if (listen(sfd, 5) < 0) 
    {
        return 1;
    }

    printf("Сервер запущен\n");

    while (1)
    {
        int cfd = accept(sfd, NULL, NULL);
        if (cfd < 0) 
        {
            continue;
        }
        int* p = malloc(sizeof(int));
        *p = cfd;

        pthread_t t;
        if (!pthread_create(&t, NULL, handle, p))
        {
            pthread_detach(t);
        }
        else 
        {
            close(cfd);
            free(p);
        }
    }

    close(sfd);
    unlink("/tmp/mysocket");
    return 0;
}
