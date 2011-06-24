
Strict

Rem
	bbdoc: Buffered OpenGL driver for Max2D
End Rem
Module sys87.GLBufferedMax2D
ModuleInfo "Author: Kevin Primm"
ModuleInfo "Version: 0.01"
ModuleInfo "License: MIT"

Import sys87.BufferedMax2D
Import BRL.GLMax2D
Import PUB.GLew

Type TGLBuffer Extends TBuffer
	Field _fbo,_name
End Type

Type TGLBufferedMax2DDriver Extends TBufferedMax2DDriver
	Method SetGraphics(g:TGraphics)
		Super.SetGraphics g
		glewInit
	End Method
	
	Method MakeBuffer:TBuffer(src:Object,width,height)
		Local frame:TBufferedImageFrame=TBufferedImageFrame(src)
		If Not frame Return Null
		If frame._buffer Return frame._buffer
		
		Local buffer:TGLBuffer=New TGLBuffer
		buffer._width=width
		buffer._height=height
		buffer._name=TGLImageFrame(frame._parent).name
		
		glGenFramebuffersEXT 1,Varptr buffer._fbo
		glBindRenderbufferEXT GL_RENDERBUFFER_EXT,buffer._fbo
		glFramebufferTexture2DEXT GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,buffer._name,0
		
		Local status=glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)
		
		SetBuffer _currentbuffer
				
		If status<>GL_FRAMEBUFFER_COMPLETE_EXT Return Null		
		frame._buffer=buffer
		Return buffer		
	End Method
	
	Method SetBuffer(buffer:TBuffer)
		_currentbuffer=buffer		
		
		If buffer<>_backbuffer
			glBindFramebufferEXT GL_FRAMEBUFFER_EXT,TGLBuffer(buffer)._fbo
			glFramebufferTexture2DEXT GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,TGLBuffer(buffer)._name,0
			glMatrixMode GL_PROJECTION
			glLoadIdentity
			glScalef 1,-1,1
			glOrtho 0,buffer._width,buffer._width,0,-1,1
			glMatrixMode GL_MODELVIEW
			glLoadIdentity
			glViewport 0,0,buffer._width,buffer._width
		Else
			glBindFramebufferEXT GL_FRAMEBUFFER_EXT,0
			TGLMax2DDriver(_parent).ResetGLContext _current
		EndIf
	End Method	
End Type

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function GLBufferedMax2DDriver:TGLBufferedMax2DDriver()
	If GLMax2DDriver()
		Global driver:TGLBufferedMax2DDriver=New TGLBufferedMax2DDriver
		driver._parent=GLMax2DDriver()
		Return driver
	End If
End Function

SetGraphicsDriver GLBufferedMax2DDriver()
