#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 4 // 프로세스 개수

void scheduler_func(void)
{
    int pid, i;
    
    printf(1,"start of scheduler\n");
    for(i = 0; i < PNUM; i++){
        if((pid = fork()) == 0) {// 프로세스 생성
            int pidChild = getpid();
            set_sche_info((i+1)*(i+1),(i+1)*100);  // 시스템 콜 호출
            while(1);        //자식 종료tick까지 죽지 않게 계속 호출 
        
        }   
        else if(pid < 0){
            printf(1, "fork error\n");
        }
    }
    
    for(int i=0;i<PNUM;i++)     //자식 프로세스 죽기 전까지 부모프로세스 죽지 않게 하기 
        wait();
    printf(1,"end of scheduler\n");
    return;
}

int main(void)
{
    scheduler_func();
    exit();
}