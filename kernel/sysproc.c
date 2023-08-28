#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  uint64 va;
  argaddr(0, &va);
  uint n;
  argint(1, (int*)&n);
  uint64 abitaddr;
  argaddr(2, &abitaddr);
  if (n > 32) {
    panic("pgaccess");
  }
  unsigned int abits = 0;
  pagetable_t pagetable = myproc()->pagetable;
  for(uint i = 0; i < n; ++i ) {
    uint64 a = va + i*PGSIZE;
    pte_t *pte = walk(pagetable, a, 0);
    if (pte == 0) {
      continue;
    }
    if ((*pte & PTE_A) != 0) {
      abits |= (1 << i);
    }
    *pte &= ~PTE_A;
  }
  return copyout(pagetable, abitaddr, (char*)&abits, sizeof(unsigned int));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
