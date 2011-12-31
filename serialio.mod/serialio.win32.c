
#include "serialio.h"

int sio_enum_devices()
{
  HKEY key;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &key) != ERROR_SUCCESS)
    return -1;

  char v[128], d[128];
  int i = 0;
  for (i = 0;;i++)
  {
    int vsize = 128, dsize = 128;
    if (RegEnumValue(key, i, (LPSTR)&v, &vsize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
      break;
  }
  RegCloseKey(&key);
  return i;
}

int sio_device_name(int index, char *buf)
{
  HKEY key;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &key) != ERROR_SUCCESS)
    return 1;

  char v[128];
  int vsize = 128, bsize = 32;
  if (RegEnumValue(key, index, (LPSTR)&v, &vsize, NULL, NULL, (LPSTR)buf, &bsize) != ERROR_SUCCESS)
    return 2;
  RegCloseKey(&key);
  return 0;
}

int sio_open(SIO *sio, const char *device, int baud)
{
  sio->handle = CreateFile(device, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
  if (sio->handle == INVALID_HANDLE_VALUE)
    return 1;

  DCB dcb;
  FillMemory(&dcb, sizeof(DCB), 0);
  dcb.DCBlength = sizeof(DCB);

  if (!BuildCommDCB("9600,n,8,1", &dcb))
    return 2;
  if (!SetCommState(sio->handle, &dcb))
    return 3;
  if (!SetupComm(sio->handle, 1024, 1024))
    return 4;

  COMMTIMEOUTS cmt;
  cmt.ReadIntervalTimeout = MAXDWORD;
  cmt.ReadTotalTimeoutMultiplier = 0;
  cmt.ReadTotalTimeoutConstant = 0;
  cmt.WriteTotalTimeoutConstant = 1000;
  cmt.WriteTotalTimeoutMultiplier = 1000;

  if (!SetCommTimeouts(sio->handle, &cmt))
    return 5;

  return 0;
}

void sio_flush(SIO *sio)
{
  PurgeComm(sio->handle, PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT);
}

int sio_read(SIO *sio, char *ptr, int length, int block)
{
  int read = -1;
  ReadFile(sio->handle, ptr, length, &read, NULL);
  return read;
}

int sio_write(SIO *sio, char *ptr, int length)
{
  int written = -1;
  WriteFile(sio->handle, ptr, length, &written, NULL);
  return written;
}

void sio_close(SIO *sio)
{
  CloseHandle(sio->handle);
}


