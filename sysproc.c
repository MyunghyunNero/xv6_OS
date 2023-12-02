#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
extern pte_t * walkpgdir(pde_t *pgdir, const void *va, int alloc);

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
sys_ssualloc(void)
{
  int n;  // 할당할 메모리 크기

  // 매개변수로 전달된 메모리 크기를 추출
  if(argint(0, &n) < 0 || n <= 0 || n % PGSIZE != 0)
    return -1;  // 매개변수 검증

  // 현재 프로세스의 구조체를 가져옴
  struct proc *curproc = myproc();

  // 새로운 메모리 크기를 계산
  uint new_sz = curproc->sz + n;

  // 실제 메모리 할당 없이 프로세스의 메모리 크기만 증가
  curproc->sz = new_sz;
  return new_sz - n;  // 할당된 가상 메모리 주소 반환
}
int
sys_getvp(void)
{
  struct proc *curproc = myproc();

  // 가상 페이지 개수 반환
  return curproc->sz / PGSIZE;
}

int
sys_getpp(void)
{
  struct proc *curproc = myproc();

  // 물리 페이지 개수 반환
  int num_pages = 0;

  // 프로세스의 페이지 테이블을 확인하여 물리 페이지 개수 계산
  for (uint va = 0; va < curproc->sz; va += PGSIZE) {
    pde_t *pde = &curproc->pgdir[PDX(va)];
    pte_t *pte = walkpgdir(curproc->pgdir, (char*)va, 0);

    if (pde && *pde & PTE_P && pte && *pte & PTE_P) {
      num_pages++;
    }
  }

  return num_pages;
} 