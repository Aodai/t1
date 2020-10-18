#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#define QUIZ_TIME 30
volatile short time_remaining = QUIZ_TIME;
int should_stop = 0;

void print_time(void) {
    time_remaining = time_remaining - 10;
    if(time_remaining > 0) {
    printf("Time remaining: %d\n", time_remaining);
    return;
    }
    printf("Ran out of time!\n");
    should_stop = 1;
}

void *start_quiz(void *vargp) {
    while(should_stop == 0) {
        const char* question = "Write your name: ";
        write(STDOUT_FILENO, question, strlen(question));
        char* buf[1024];
        int bytes = read(STDIN_FILENO, (void *) &buf, 1024);
        write(STDOUT_FILENO, buf, bytes);
    }
    pthread_exit(NULL);
}


void *timer(void *vargp) {
    struct itimerval it_val;
    
    it_val.it_value.tv_sec = 5;
    it_val.it_value.tv_usec = 0;
    it_val.it_interval = it_val.it_value;

    setitimer(ITIMER_REAL, &it_val, 0);

    if (signal(SIGALRM, (void (*)(int)) print_time) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(1);
    }
    while(!should_stop) {

    }
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t timer_thread, quiz_thread;
    pthread_create(&timer_thread, NULL, timer, NULL);
    pthread_create(&quiz_thread, NULL, start_quiz, NULL);
    pthread_join(quiz_thread, NULL);
    pthread_join(timer_thread, NULL);
    pthread_cancel(timer_thread);
    printf("Bye!\n");
    return 0;
}
