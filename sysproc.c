#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_date(void)
{
  struct rtcdate *r;

  argptr(0,(void*)&r,sizeof(r));
  cmostime(r);
  return 0;
}

int
sys_set_sche_info(void)
{
    int n;
    int n2;
    if(argint(0, &n) < 0)    // n 은 우선순위 받기
      return -1;
    if(argint(1, &n2) < 0)   // n2는 종료 tick 받기
      return -1;
    cprintf("set_sche_info() pid =%d\n",myproc()->pid);   //우선순위 할당 출력
    myproc()->priority=n;   //현재 실행하는 프로세스에 우선순위와 종료 tick 저장
    myproc()->cpu_end=n2;
    return 0;
}