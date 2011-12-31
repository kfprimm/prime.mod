
#include <windows.h>

#define PRINT_BUFFER_SIZE 300
int _printer_count = 0;
PRINTER_INFO_2 _printers[PRINT_BUFFER_SIZE];

int prtr_enum_devices()
{
	int size = 0;
	if (EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, _printers, sizeof(PRINTER_INFO_2)*PRINT_BUFFER_SIZE, &size, &_printer_count) == 0)
		return -1;
	return _printer_count;
}

int prtr_get_device(int index, char **printername, char **drivername, char **portname)
{
	*printername = _printers[index].pPrinterName;
	*drivername= _printers[index].pDriverName;
	*portname= _printers[index].pPortName;
}

