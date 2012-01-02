
Strict

Rem
	bbdoc: Direct3D 9 driver for Max2DEx
End Rem
Module Prime.D3D9Max2DEx
ModuleInfo "Author: Kevin Primm"
ModuleInfo "Version: 0.01"
ModuleInfo "License: MIT"

Import Prime.Max2DEx
Import BRL.D3D9Max2D

?Win32

Type TD3D9Buffer Extends TBuffer

End Type

Type TD3D9Max2DExDriver Extends TMax2DExDriver
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
Function D3D9Max2DExDriver:TD3D9Max2DExDriver ()
	If D3D9Max2DDriver()
		Global driver:TD3D9Max2DExDriver = New TD3D9Max2DExDriver 
		driver._parent=D3D9Max2DDriver()
		Return driver
	End If
End Function

Local driver:TD3D9Max2DExDriver = D3D9Max2DExDriver()
If driver SetGraphicsDriver driver

?
