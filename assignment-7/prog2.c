#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

void function(int sig) {
  //to handle the signal
  printf("Signal number: %d\nnow Parent can print\n", sig);
}

int main() {
  int pid = fork();
  if(pid == -1){
    printf("ERROR OCCURED");
    return 1;
  }

  if(pid == 0) {
    //child process
    sleep(5);
    kill(getppid(), SIGALRM);
    // sending signal to parent
    exit(0);
  }
  else {
    //parent prcoess
    int i=0;
    signal(SIGALRM, function);
    pause();
    while(1){
      printf("Message %d\n",++i);
      sleep(1);
    }
  }
  return 0;
}
