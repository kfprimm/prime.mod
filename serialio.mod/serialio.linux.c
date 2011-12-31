
#include "serial_io.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int sio_enum_devices()
{
  return 0;
}

int sio_device_name(int index, char *buf)
{
  return 0;
}

int sio_open(SIO *sio, const char *device, int baud)
{
  sio->fd = open(device, O_RDWR | O_NOCTTY ); 
  if (sio->fd < 0) {
    perror(device);
    return 1;
  }
  
  int baudrate = B9600;
  
  tcgetattr(sio->fd,&sio->oldtio);
  
  bzero(&sio->newtio, sizeof(sio->newtio));
  sio->newtio.c_cflag = baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
  sio->newtio.c_iflag = IGNPAR;
  sio->newtio.c_oflag = 0;
  
  sio->newtio.c_lflag = 0;
   
  sio->newtio.c_cc[VTIME]    = 0;
  sio->newtio.c_cc[VMIN]     = 0;
  
  sio_flush(sio);
  tcsetattr(sio->fd,TCSANOW,&sio->newtio);
  
  return 0;
}

void sio_flush(SIO *sio)
{
  tcflush(sio->fd, TCIFLUSH);
}

int sio_read(SIO *sio, char *ptr, int length, int block)
{
  int pos = 0;
  while (pos < length)
  {
    pos += read(sio->fd, &ptr[pos], length - pos);
    if (pos == 0 && block == 0)
      return 0;
  }
  return pos;
}

void sio_close(SIO *sio)
{
  if (sio->fd != 0)
  {
    tcsetattr(sio->fd,TCSANOW,&sio->oldtio);
    close(sio->fd);
  }
}


