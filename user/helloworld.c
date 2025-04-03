#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main()
{
  write(1, "Hello World xv6\n", 17);
  exit(0 , "");
}
