#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#define QUIZ_TIME 30
volatile short time_remaining;
int should_stop;
pthread_t timer_thread, quiz_thread;

void print_time(void) {
    printf("Time remaining: %d\n", time_remaining);
}

void *start_quiz(void *vargp) {
    for(int i = 0; i < 5; i++) {
        const char* question = "Write your name: ";
        write(STDOUT_FILENO, question, strlen(question));
        char* buf[1024];
        int bytes = read(STDIN_FILENO, (void *) &buf, 1024);
        write(STDOUT_FILENO, buf, bytes);
    }
    pthread_cancel(timer_thread);
    pthread_exit(NULL);
}


void *timer(void *vargp) {
    for(int i = 0; i < QUIZ_TIME/10; i++) {
        sleep(10);
        time_remaining = time_remaining - 10;
        print_time();
    }
    should_stop = 1;
    pthread_cancel(quiz_thread);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int wantQuiz = 1;
    while(wantQuiz == 1)
    {
        time_remaining = QUIZ_TIME;
        should_stop = 0;
        pthread_create(&timer_thread, NULL, timer, NULL);
        pthread_create(&quiz_thread, NULL, start_quiz, NULL);
        pthread_join(quiz_thread, NULL);
        pthread_join(timer_thread, NULL);
        printf("Would you like to take another quiz?\n1. Yes\n2. No\n");
        scanf("%d", &wantQuiz);
    }
    
    printf("Bye!\n");
    return 0;
}
