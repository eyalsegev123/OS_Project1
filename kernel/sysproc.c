#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64 
sys_exit(void)
{ 
  int n;
  char msg[32];
  
  argint(0, &n);
  
  if(argstr(1, msg, 32) < 0)
    msg[0] = '\0'; // Use empty string if retrieving fails
    
  msg[31] = '\0';
    
  struct proc *p = myproc();
  safestrcpy(p->exit_msg, msg, sizeof(p->exit_msg));
  
  exit(n);
  
  return 0;
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

// uint64
// sys_wait(void)
// {
  
//   uint64 p;
//   argaddr(0, &p);
//   return wait(p);
// }


uint64
sys_wait(void)
{
  uint64 status_addr;
  uint64 msg_addr;

  argaddr(0, &status_addr);
  argaddr(1, &msg_addr);
    
  int status;
  char msg[32];

  int pid = wait(&status, msg);  // This now copies the exit_msg for us

  if (pid < 0)
    return -1;

  struct proc *p = myproc();

  if (copyout(p->pagetable, status_addr, (char *)&status, sizeof(status)) < 0)
    return -1;

  if (copyout(p->pagetable, msg_addr, msg, sizeof(msg)) < 0)
    return -1;

  return pid;
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

uint64
sys_memsize(void)
{
  return myproc()->sz;
}
