#include "../chardev.h"
#include <fcntl.h>	//open
#include <unistd.h>	//exit
#include <sys/ioctl.h>	//ioctl
#include <stdio.h>
#include <stdlib.h>
int main()
{
  int file_desc,retval;

  if( ( file_desc=open(DEVICE_FILE_NAME,0) ) < 0 )
  {
    printf("Can't open device file: %s\n",DEVICE_FILE_NAME);
    exit(-1);
  }


  if ( ioctl(file_desc,IOCTL_CLEAR_MSG) < 0 ) {
    printf("ioctl_clear_msg failed\n");
    exit(-1);
  } 
  else  {
    printf("clear Message success\n");
	
  }	


  close(file_desc);

  return 0;
}
