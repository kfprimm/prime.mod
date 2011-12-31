
#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#ifdef WIN32

#include <windows.h>

typedef struct SIO 
{
  HANDLE handle;
} SIO;

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

typedef struct SIO 
{
  int fd;
  struct termios oldtio,newtio;
} SIO;

#endif

int sio_enum_devices();
int sio_device_name(int index, char *buf);

int  sio_open(SIO *sio, const char *device, int baud);
void sio_flush(SIO *sio);
int  sio_read(SIO *sio, char *ptr, int length, int block);
void sio_close(SIO *sio);

#endif
