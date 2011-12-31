
Strict

Rem
	bbdoc: Basic COM port access and stream factory.
End Rem
Module Prime.SerialIO
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream
?Win32
Import "serialio.win32.c"
?Not Win32
Import "serialio.linux.c"
?

Extern
	Function sio_enum_devices()
	Function sio_device_name(index, buf:Byte Ptr)
	Function sio_open(sio:Byte Ptr, device:Byte Ptr, baud)
	Function sio_flush(sio:Byte Ptr)
	Function sio_read(sio:Byte Ptr, char:Byte Ptr, length, block)
	Function sio_write(sio:Byte Ptr, char:Byte Ptr, length)
	Function sio_close(sio:Byte Ptr)
End Extern

Type TSerialPort
	Field _readable, _writable
	Field _sio:Byte[128]
	
	Function Open:TSerialPort(device$, baud)
		Local port:TSerialPort = New TSerialPort
		Local dev:Byte Ptr = device.ToCString()
		sio_open(port._sio, dev, baud)
		MemFree dev
		Return port
	End Function
	
	Method Close()
		sio_close(_sio)
	End Method
	
	Method Flush()
		sio_flush(_sio)
	End Method
	
	Method Read(length, buffer:Byte Ptr)
		Return sio_read(_sio, buffer, length, False)
	End Method
	
	Method Write(length, buffer:Byte Ptr)
		Return sio_write(_sio, buffer, length)
	End Method
End Type

Type TSerialPortStream Extends TStream
	Field _port:TSerialPort
	
	Method Create:TSerialPortStream(port:TSerialPort)
		If port = Null Return Null
		_port = port
		Return Self
	End Method
	
	Method Flush()
		Return _port.Flush()
	End Method
	
	Method Close()
		Return _port.Close()
	End Method

	Method Read( buf:Byte Ptr,count )
		Return _port.Read(count, buf)
	End Method

	Method Write( buf:Byte Ptr,count )
		Return _port.Write(count, buf)
	End Method
End Type

Type TSerialPortStreamFactory Extends TStreamFactory
	Method CreateStream:TStream( url:Object,proto$,path$,readable,writeable )
		If proto <> "serial" Return Null
		Local dev$ = path[..path.Find("@")]
		Local baud = Int(path[path.Find("@")+1..])
		Return New TSerialPortStream.Create(TSerialPort.Open(dev, baud))
	End Method
End Type
New TSerialPortStreamFactory 

Function SerialPorts$[]()
	Local count = sio_enum_devices(), ports$[count]
	For Local i = 0 Until count
		Local buf:Byte[256]
		sio_device_name(i, buf)
		ports[i] = String.FromCString(buf)
	Next
	Return ports
End Function
