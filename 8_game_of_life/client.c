#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdlib.h>

int main(){
  struct sockaddr_in addr;
  int sock;
  char *host = "127.0.0.1";

  sock = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(31340);
  inet_aton(host, &addr.sin_addr);

  if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
     perror("connect");
     }

  char buff[1];
  int n, i=0;
    do {
        n=read(sock, buff, 1);
        printf("%c", buff[0]);
    }
    while(n>0);
    close(sock);

  return 0;
}