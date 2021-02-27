#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void function(int sig){
    printf("Signal Number %d \n",sig);
}

int main(){

  signal(SIGINT,function);
  
  int i=0;
  while(1) {
    printf("I am working: %d\n",++i);
    sleep(1);
  }

  return 0;
}
