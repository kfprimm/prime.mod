
Strict

Module Prime.PrintLib
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "LD_OPTS: -LC:\MinGW\lib"

?Win32
Import "printlib.win32.c"
Import "-lwinspool"
?

Extern 
	Function prtr_enum_devices()
	Function prtr_get_device(index, printername:Byte Ptr Ptr, drivername:Byte Ptr Ptr, portname:Byte Ptr Ptr)
End Extern

Type TPrinter
	Field name$, driver$, port$
	
	Function Enumerate:TPrinter[]()
		Local count = prtr_enum_devices(), printers:TPrinter[]
		For Local i = 0 Until count
			Local pn:Byte Ptr, dn:Byte Ptr, ptn:Byte Ptr
			prtr_get_device(i, Varptr pn, Varptr dn, Varptr ptn)
			Local printer:TPrinter = New TPrinter
			printer.name   = String.FromCString( pn )
			printer.driver = String.FromCString( dn )
			printer.port   = String.FromCString( ptn )
			printers :+ [ printer ]
		Next	
		Return printers 
	End Function	
End Type

Function EnumPrinters:TPrinter[]()
	Return TPrinter.Enumerate()
End Function