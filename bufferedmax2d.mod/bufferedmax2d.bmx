
Strict

Rem
	bbdoc: Buffering system for Max2D
End Rem
Module sys87.BufferedMax2D
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

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

Type TBufferedMax2DDriver Extends TMax2DDriver
	Global _parent:TMax2DDriver
	
	Field _current:TGraphics
	Field _backbuffer:TBuffer=New TBuffer,_currentbuffer:TBuffer
	
	Method CreateFrameFromPixmap:TImageFrame(pixmap:TPixmap,flags) 
		Local bf:TBufferedImageFrame=New TBufferedImageFrame
		bf._parent=_parent.CreateFrameFromPixmap(pixmap,flags)
		Return bf
	End Method
	
	Method SetBlend( blend )
		Return _parent.SetBlend(blend)
	End Method
	Method SetAlpha( alpha# )
		Return _parent.SetAlpha(alpha)
	End Method
	Method SetColor( red,green,blue )
		Return _parent.SetColor(red,green,blue)
	End Method
	Method SetClsColor( red,green,blue )
		Return _parent.SetClsColor(red,green,blue)
	End Method
	Method SetViewport( x,y,width,height )
		Return _parent.SetViewport(x,y,width,height)
	End Method
	Method SetTransform( xx#,xy#,yx#,yy# )
		Return _parent.SetTransform(xx,xy,yx,yy)
	End Method
	Method SetLineWidth( width# )
		Return _parent.SetLineWidth(width)
	End Method
	
	Method Cls()
		Return _parent.Cls()
	End Method
	Method Plot( x#,y# )
		Return _parent.Plot(x,y)
	End Method
	Method DrawLine( x0#,y0#,x1#,y1#,tx#,ty# )
		Return _parent.DrawLine(x0,y0,x1,y1,tx,ty)
	End Method
	Method DrawRect( x0#,y0#,x1#,y1#,tx#,ty# )
		Return _parent.DrawRect(x0,y0,x1,y1,tx,ty)
	End Method
	Method DrawOval( x0#,y0#,x1#,y1#,tx#,ty# )
		Return _parent.DrawOval(x0,y0,x1,y1,tx,ty)
	End Method
	Method DrawPoly( xy#[],handlex#,handley#,originx#,originy# )
		Return _parent.DrawPoly(xy,handlex,handlex,originx,originy)
	End Method
		
	Method DrawPixmap( pixmap:TPixmap,x,y )
		Return _parent.DrawPixmap(pixmap,x,y)
	End Method
	Method GrabPixmap:TPixmap( x,y,width,height )
		Return _parent.GrabPixmap(x,y,width,height)
	End Method
	
	Method SetResolution( width#,height# )
		Return _parent.SetResolution(width,height)
	End Method
	
	Method GraphicsModes:TGraphicsMode[]()
		Return _parent.GraphicsModes()
	End Method
		
	Method AttachGraphics:TGraphics( widget,flags )
		Return MakeGraphics(_parent.AttachGraphics(widget,flags))		
	End Method
	
	Method CreateGraphics:TGraphics( width,height,depth,hertz,flags )
		Return MakeGraphics(_parent.CreateGraphics(width,height,depth,hertz,flags))
	End Method
	
	Method SetGraphics( g:TGraphics )
		_parent.SetGraphics(g)
		_current=g
		_backbuffer._width=GraphicsWidth()
		_backbuffer._height=GraphicsHeight()
		_currentbuffer=_backbuffer
	End Method
	
	Method Flip( sync )
		Return _parent.Flip(sync)
	End Method
	
	Method MakeGraphics:TGraphics(g:TGraphics)
		TMax2DGraphics(g)._driver=Self
		Return g
	End Method

	Method MakeBuffer:TBuffer(src:Object,width,height,flags) Abstract
	
	Method SetBuffer(buffer:TBuffer) Abstract
	
	Method BackBuffer:TBuffer()
		Return _backbuffer
	End Method	
	
	Method ImageBuffer:TBuffer(image:TImage,frame=0,flags=BUFFER_COLOR)
		Return MakeBuffer(image.Frame(frame),image.width,image.height,flags)		
	End Method
End Type

Rem
	bbdoc: Width of the buffer.
End Rem
Function BufferWidth(buffer:TBuffer)
	Return buffer._width
End Function
Rem
	bbdoc: Height of the buffer.
End Rem
Function BufferHeight(buffer:TBuffer)
	Return buffer._height
End Function
Rem
	bbdoc: Sets the current buffer for drawing.
End Rem
Function SetBuffer(buffer:TBuffer)
	Return TBufferedMax2DDriver(GetGraphicsDriver()).SetBuffer(buffer)
End Function
Rem
	bbdoc: Returns the current drawing buffer.
End Rem
Function CurrentBuffer:TBuffer()
	Return TBufferedMax2DDriver(GetGraphicsDriver())._currentbuffer
End Function
Rem
	bbdoc: Returns the driver's back buffer.
End Rem
Function BackBuffer:TBuffer()
	Return TBufferedMax2DDriver(GetGraphicsDriver()).BackBuffer()
End Function
Rem
	bbdoc: Returns the buffer used to draw to a image and it's frame.
End Rem
Function ImageBuffer:TBuffer(image:TImage,frame=0)
	Return TBufferedMax2DDriver(GetGraphicsDriver()).ImageBuffer(image,frame)
End Function