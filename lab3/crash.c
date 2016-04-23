#include <stdio.h>
#include <error.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ospfs.h"


int main(int argc, char** argv)
{
  int nwrites_to_crash = atoi(argv[1]);
  int file_descriptor = open("./test/crash_helper", O_RDONLY);
  int ret = ioctl(file_descriptor, IOCTLCRASH, nwrites_to_crash);
  close(file_descriptor);
  exit(ret);
}
