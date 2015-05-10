#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#define BOARD_WIDTH 78
#define BOARD_HEIGHT  24
volatile sig_atomic_t keep_going = 1;
int board[BOARD_WIDTH][BOARD_HEIGHT];


void initialize_board () {
  int i, j;
  for (i=0; i<BOARD_WIDTH; i++) for (j=0; j<BOARD_HEIGHT; j++) 
    board[i][j] = 0;
}

int xadd (int i, int a) {
  i += a;
  while (i < 0) i += BOARD_WIDTH;
  while (i >= BOARD_WIDTH) i -= BOARD_WIDTH;
  return i;
}
int yadd (int i, int a) {
  i += a;
  while (i < 0) i += BOARD_HEIGHT;
  while (i >= BOARD_HEIGHT) i -= BOARD_HEIGHT;
  return i;
}
int adjacent_to ( int i, int j) {
  int k, l, count;
  count = 0;
  for (k=-1; k<=1; k++) for (l=-1; l<=1; l++)
    if (k || l)
      if (board[xadd(i,k)][yadd(j,l)]) count++;
  return count;
}

void play() {
  int i, j, a, newboard[BOARD_WIDTH][BOARD_HEIGHT];
  for (i=0; i<BOARD_WIDTH; i++) for (j=0; j<BOARD_HEIGHT; j++) {
    a = adjacent_to ( i, j);
    if (a == 2) newboard[i][j] = board[i][j];
    if (a == 3) newboard[i][j] = 1;
    if (a < 2) newboard[i][j] = 0;
    if (a > 3) newboard[i][j] = 0;
  }
  for (i=0; i<BOARD_WIDTH; i++) 
    for (j=0; j<BOARD_HEIGHT; j++) {
    board[i][j] = newboard[i][j];
  }
}

void print () {
  int i, j;
  for (j=0; j<BOARD_HEIGHT; j++) {
    for (i=0; i<BOARD_WIDTH; i++) {
      printf ("%c", board[i][j] ? 'x' : ' ');
    }
    printf ("\n");
  }
}
void read_file (char *name) {
  FILE  *f;
  int i, j;
  char s[100];
  f = fopen (name, "r");
  for (j=0; j<BOARD_HEIGHT; j++) {
    if (fgets (s, 100, f)) {
      for (i=0; i<BOARD_WIDTH; i++) {
        if (s[i]=='1') {
          board[i][j] = 'x'; 
        }
      }
    }
  }
  fclose (f);
}
void catch_alarm (int sig){
    alarm(1);
    print ();
    play();
    signal (sig, catch_alarm); 
}


void handle_error(char* err_fun) {
  printf("%s() failed, errno = %s", err_fun, strerror(errno));
  exit(1);
}

void child_proc(int sock) {
  int i, j;
  for (j=0; j<BOARD_HEIGHT; j++) {
    for (i=0; i<BOARD_WIDTH; i++) {
      if (board[i][j]) {
        //printf ("%s",  "x" );
        write(sock,"x",1);
      } else {
        //printf ("%s",  " " );
        write(sock, " " ,1);
      }
    }
    write(sock, "\n" ,1);
    //printf ("\n");
  }
  close(sock);
  exit(0);
}

int main(int argc, char *argv[]) {
  int i, j;
  initialize_board ();
  if (argc<2) {
    printf ("Enter file wuth board\n"); 
    return;
  }
  read_file ( argv[1]);
  pid_t cpid;
  struct sockaddr_in addr;
  int accepted_socket;
  int fork_pid;
  int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(-1 == listen_socket) {
    handle_error("socket");
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(31340);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if(-1 == bind(listen_socket, (struct sockaddr*)&addr, sizeof(addr))){
    handle_error("bind");
  }

  if(-1 == listen(listen_socket, 100)) {
    handle_error("listen");
  }
  signal (SIGALRM, catch_alarm);
  alarm(1);
  keep_going=0;

  // cpid = fork(); 
  //     switch (cpid) { 
  //       case -1:
  //           printf("Fork failed; cpid == -1\n");
  //           break; 
  //       case 0: 
  //           cpid = getpid();  
  //           while(1) {
  //            // printf("%d\n", BOARD_HEIGHT);
  //             if (!keep_going) {
  //               do_stuff(); 
  //             } else {
  //               if (keep_going==2){
  //                 printf("Error\n"); break;
  //               }
  //             }
  //           }
  //           exit(0); 
  //       default:
          for(;;) {
            //printf("%d\n", cqw);
          accepted_socket = accept(listen_socket, 0, 0);
          if(-1 == accepted_socket) {
            handle_error("accept");
          }

          fork_pid = fork();
          if(-1 == fork_pid) {
            handle_error("fork");
          } else if(0 == fork_pid) {
            close(listen_socket);
            child_proc(accepted_socket);
          } else {
            close(accepted_socket);
          }
        }
       // exit(0);
     // }
}