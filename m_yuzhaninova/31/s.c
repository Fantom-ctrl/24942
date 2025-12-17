#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <ctype.h>
#include <poll.h>
#include <sys/socket.h>


int main() 
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd < 0) 
    {
        exit(1);
    }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "./mysocket", sizeof(addr.sun_path) - 1);

    unlink("./mysocket");
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        exit(1);
    }
    if (listen(sfd, 5) < 0) 
    {
        exit(1);
    }
    printf("Сервер запущен\n");

    struct pollfd pfds[6];
    pfds[0].fd = sfd;
    pfds[0].events = POLLIN;

    for (int i = 1; i < 6; i++) 
    {
        pfds[i].fd = -1;
        pfds[i].events = POLLIN;
    }

    while (1)
    {
        if (poll(pfds, 6, -1) < 0) 
        {
            break;
        }

        if (pfds[0].revents & POLLIN) 
        {
            struct sockaddr_un caddr = {0};
            socklen_t clen = sizeof(caddr);
            int cfd = accept(sfd, (struct sockaddr*)&caddr, &clen);
            if (cfd < 0) 
            {
                exit(1);
            }

            int added = 0;
            for (int i = 1; i < 6; i++) 
            {
                if (pfds[i].fd == -1) 
                {
                    pfds[i].fd = cfd;
                    added = 1;
                    break;
                }
            }
            if (!added) 
            {
                close(cfd);
            }
        }

        for (int i = 1; i < 6; i++) 
        {
            int fd = pfds[i].fd;
            if (fd != -1 && (pfds[i].revents & POLLIN)) 
            {
                char c;
                long n = read(fd, &c, 1);
                if (n <= 0) 
                {
                    close(fd);
                    pfds[i].fd = -1;
                    continue;
                }
                c = toupper(c);
                printf("[Client %d] %c\n", fd, c);
                fflush(stdout);
            }
        }
    }

    close(sfd);
    unlink("./mysocket");
    return 0;
}
