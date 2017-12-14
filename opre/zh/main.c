#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>

void handler(int signum) {
    printf("Kommunikacio megtortent.\n");
}

int main(int argc, char** argv) {

    signal(SIGUSR1, handler);
    pid_t pid;

    struct mq_attr attr;
    struct sigevent notify;
    char* mqname = "/auto1";
    char rcv_buf[64];

    mqd_t mqdes1, mqdes2;

    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 50;
    mq_unlink(mqname);
    mqdes1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);

    if(mqdes1 < 0) {
        printf("mq_open error: %d \n",errno);
        return 1;
    }

    pid = fork();

    if(pid == 0) {
        printf("Felderito: indul\n");
        pid_t cpid = getpid();
        pause();
        int db = mq_receive(mqdes1, rcv_buf, 64, NULL);
        mq_close(mqdes1);
        printf("Felderito: megkaptam a feladatot, indulok keresni [%s] (PID: %d)\n", 
                rcv_buf, cpid);
        sleep(5);
        printf("Felderito: befejeztem a keresest\n");
        kill(getppid(), SIGUSR1);
    } else {
        sleep(1);
        printf("Kapo: indul\n");
        char input[64];
        printf("Kapo: kerem az adatot: ");
        scanf("%[^\n]", input);
        int db = mq_send(mqdes1, input, strlen(input), 5);
        sleep(1);
        kill(pid, SIGUSR1);
        printf("Kapo: felderito ertesitve, most varok\n");
        pause();
        mq_close(mqdes1);
        printf("Kapo: a felderitom visszaert\n");
    }
    mq_close(mqdes1);
    mq_unlink(mqname);
    return 0;
}