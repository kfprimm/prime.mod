
Strict

Import BRL.Max2D

Const BUFFER_COLOR = 1
Const BUFFER_DEPTH = 2

Type TBuffer
	Field _width,_height
End Type

Type TBufferedImageFrame Extends TImageFrame
	Field _parent:TImageFrame
	Field _buffer:TBuffer
	
	Method Draw( x0#,y0#,x1#,y1#,tx#,ty#,sx#,sy#,sw#,sh# )
		Return _parent.Draw( x0,y0,x1,y1,tx,ty,sx,sy,sw,sh )
	End Method 
End Type
