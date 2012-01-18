
Strict

Rem
	bbdoc: Max2D extensions to support buffering and shaders
End Rem
Module Prime.Max2DEx
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Max2D
Import "buffers.bmx"
Import "shaders.bmx"

Type TMax2DExDriver Extends TMax2DDriver
	Field _parent:TMax2DDriver	
	Field _current:TGraphics
	Field _backbuffer:TBuffer=New TBuffer,_currentbuffer:TBuffer
  Field _shader:TShader, _shaderdriver:TShaderDriver

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
	
	Method SetShaderDriver(driver:TShaderDriver)
		_shaderdriver=driver
	End Method
	
	Method SetShader( shader:TShader )
		_shader = shader
		_shaderdriver.Use _shader, Null
	End Method
End Type

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function DrawRoundRect( x#,y#,width#,height#,radius# )
	radius = Max(Min(radius,Min(width,height)/2.0),0)
	Local segs=Int(Max(Abs(radius)*2,12))&~3,xy#[4*segs*2]
	Function MakeArc(xy#[],index,segs,x#,y#,radius#,ang0#,ang1#)
		Local ang# = ang0, delta#=(ang1-ang0)/segs
		For Local i = index To index+(segs-2)
			xy[i*2+0] = x+(Cos(ang)*radius)
			xy[i*2+1] = y-(Sin(ang)*radius)
			ang:+delta
		Next
		xy[(index+segs-1)*2+0] = x+(Cos(ang1)*radius)
		xy[(index+segs-1)*2+1] = y-(Sin(ang1)*radius)
	End Function
	
	MakeArc xy,0,segs,x+radius,y+radius,radius,180,90
	MakeArc xy,segs*1,segs,(x+width)-radius,y+radius,radius,90,0
	MakeArc xy,segs*2,segs,(x+width)-radius,(y+height)-radius,radius,0,-90
	MakeArc xy,segs*3,segs,x+radius,(y+height)-radius,radius,270,180
	DrawPoly xy
End Function

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
	Return TMax2DExDriver(GetGraphicsDriver()).SetBuffer(buffer)
End Function
Rem
	bbdoc: Returns the current drawing buffer.
End Rem
Function CurrentBuffer:TBuffer()
	Return TMax2DExDriver(GetGraphicsDriver())._currentbuffer
End Function
Rem
	bbdoc: Returns the driver's back buffer.
End Rem
Function BackBuffer:TBuffer()
	Return TMax2DExDriver(GetGraphicsDriver()).BackBuffer()
End Function
Rem
	bbdoc: Returns the buffer used to draw to a image and it's frame.
End Rem
Function ImageBuffer:TBuffer(image:TImage,frame=0)
	Return TMax2DExDriver(GetGraphicsDriver()).ImageBuffer(image,frame)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function SetShaderDriver(driver:TShaderDriver)
	Return TMax2DExDriver(GetGraphicsDriver()).SetShaderDriver(driver)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function SetShader( shader:TShader )
	Return TMax2DExDriver(GetGraphicsDriver()).SetShader(shader)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function CreateShader:TShader(name$)
	Return New TShader.Create(name)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function LoadShader:TShader(url:Object)
	Return New TShader.Load(url)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function CreateShaderFrag:TShaderFrag(code$, typ)
	Return TShaderFrag.Create(code,typ)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function AddShaderCode:TShaderCode(shader:TShader,driver$="")
	Return shader.AddCode(driver)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function AddShaderCodeFrag(code:TShaderCode,frag:TShaderFrag)
	Return code.AddFrag(frag)
End Function
Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function AddShaderCodeFrags(code:TShaderCode,frags:TShaderFrag[])
	Return code.AddFrags(frags)
End Function
