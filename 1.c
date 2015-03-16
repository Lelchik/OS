//fork_waitpid.c
//Запускаем несколько дочерных процессов и следим за их завершением

#include <stdio.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAXPROC 10
#define N 80
pid_t pid_list[MAXPROC];
char list_rw[MAXPROC];
char* list_name[MAXPROC];
char* list_args[MAXPROC];
int pid_count=0;

void create_pid_file(pid_t pid, char* name) {   // TODO: add errorhandlers
    char* chr = strrchr(name, '/');
    char filename_dest[50];
    strcpy(filename_dest, "/tmp");        //TODO: don`t forget to change
    strcat(filename_dest, chr);
    strcat(filename_dest, ".pid");
    FILE* pid_file = fopen (filename_dest, "w");
    fprintf(pid_file, "%d\n", pid);
    fclose(pid_file);
    return;
}

int main () { 
    char name[N];
    char argv[N];
    char todo[N];
    int count=0;
    int i, p=0; 
    pid_t cpid;
    FILE * file = fopen("Init.conf", "r");
    while (fscanf (file, "%s %s %s\n", name,argv,todo) != EOF) {  
      cpid = fork(); 
      switch (cpid) { 
        case -1:
            printf("Fork failed; cpid == -1\n");
            break; 
        case 0: 
            cpid = getpid();  
            execl(name," ",argv,NULL);
            exit(0); 
        default:
            create_pid_file(cpid, name);
            pid_list[p]=cpid;
            pid_count++;
            list_rw[p]=todo[0];
            if (list_rw[p]==114) count++;
            list_name[p]=name;
            list_args[p]=argv;
      }
      p++;
    }
    count+=p;
    printf("%d\n",count);
    while (count) {
        cpid=waitpid(-1, NULL, 0);   //ждем любого завершенного потомка
        for (p=0; p<MAXPROC; p++) {
            if(pid_list[p]==cpid){
        //делаем что-то по завершении дочернего процесса
               if (list_rw[p] == 114) { 
                  list_rw[p]=119;
                  cpid = fork();
                  if (cpid==0) { 
                    execl(list_name[p]," ",list_args[p], NULL);
                    exit(0);
                  } else { 
                     create_pid_file(cpid, list_name[p]);
                    pid_list[p]=cpid; 
                    printf("%d\n",cpid );
                    count--;
                  }; 
                  
                } else {
                  pid_list[p]=0;
                  count--;
                }
                break;
              }
        } 
    }
    fclose(file);
    return 0;
}