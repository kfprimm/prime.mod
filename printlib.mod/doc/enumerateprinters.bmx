
Strict

Import Prime.PrintLib

For Local printer:TPrinter = EachIn EnumPrinters()
	Print printer.name+", "+printer.driver+", "+printer.port
Next
