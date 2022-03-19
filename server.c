#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>     
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void
store(int sockfd, int outfd)
{
    ssize_t     n;
    char        buf[16384];
again:
    while ( (n = read(sockfd, buf, 16384)) > 0)
        	write(outfd, buf, n);
    if (n < 0 && errno == EINTR)
       	goto again;
	else if (n < 0)
      	fprintf(stderr,"str_echo: read error");
    else if (n==0) {
        fprintf(stderr,"client close\n");
        close(outfd);
    }
}

int
main(int argc, char **argv)
{
    int                 listenfd, connfd,outfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, sizeof(servaddr),0);
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(9999);

    bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    listen(listenfd, 3);

    fprintf(stderr,"listen on 9999\n");
    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr* )&cliaddr, &clilen);

        if ( (childpid = fork()) == 0) {    /* child process */
            close(listenfd);    /* close listening socket */
            outfd=open("./out.so", O_RDWR|O_CREAT);
            if (outfd<0) {
                fprintf(stderr,"openfile failed/n");
                exit(-1);
            }
            store(connfd,outfd);   /* process the request */
            exit(0);
        }
        close(connfd);          /* parent closes connected socket */
    }
}
