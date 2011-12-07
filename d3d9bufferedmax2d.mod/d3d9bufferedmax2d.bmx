
Strict

Rem
	bbdoc: Buffered Direct3D 9 driver for Max2D
End Rem
Module sys87.D3D9BufferedMax2D
ModuleInfo "Author: Kevin Primm"
ModuleInfo "Version: 0.01"
ModuleInfo "License: MIT"

Import sys87.BufferedMax2D
Import BRL.D3D9Max2D

?Win32

Type TD3D9Buffer Extends TBuffer

End Type

Type TD3D9BufferedMax2DDriver Extends TBufferedMax2DDriver
	Method MakeBuffer:TBuffer(src:Object,width,height,flags)
		Return MakeD3D9Buffer(width,height)
	End Method
	
	Method MakeD3D9Buffer:TD3D9Buffer(width,height)
		Local buffer:TD3D9Buffer=New TD3D9Buffer
		buffer._width=width
		buffer._height=height
		Return buffer
	End Method
	
	Method SetBuffer(buffer:TBuffer)
		_currentbuffer=buffer
	End Method
End Type

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function D3D9BufferedMax2DDriver:TD3D9BufferedMax2DDriver()
	If D3D9Max2DDriver()
		Global driver:TD3D9BufferedMax2DDriver=New TD3D9BufferedMax2DDriver 
		driver._parent=D3D9Max2DDriver()
		Return driver
	End If
End Function

SetGraphicsDriver D3D9BufferedMax2DDriver()

?