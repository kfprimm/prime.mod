
Strict

Rem
	bbdoc: Buffered OpenGL driver for Max2D
End Rem
Module Prime.GLMax2DEx
ModuleInfo "Author: Kevin Primm"
ModuleInfo "Version: 0.01"
ModuleInfo "License: MIT"

Import Prime.Max2DEx
Import BRL.GLMax2D
Import PUB.GLew

Type TGLBuffer Extends TBuffer
	Field _fbo,_tex
	Field _depth
End Type

Type TGLBufferedGraphics Extends TGraphics
	Field _width,_height
	
	Function Make:TGLBufferedGraphics(buffer:TBuffer)
		Global g:TGLBufferedGraphics=New TGLBufferedGraphics
		g._width=buffer._width
		g._height=buffer._height
		Return g
	End Function
	
	Method Driver:TGraphicsDriver()
		Return Null
	End Method
	
	Method GetSettings( width Var,height Var,depth Var,hertz Var,flags Var )
		width=_width
		height=_height
	End Method
	
	Method Close()
		Return False
	End Method
End Type

Type TGLSLData
	Field _prog,_vert,_frag
End Type

Type TGLSLDriver Extends TShaderDriver
	Method Compile:TShaderCode(code:TShaderCode)		
		Local res:TGLSLData=TGLSLData(code._res)
		If Not code._recompile And res<>Null Return code
		
		If res=Null
			res=New TGLSLData
			res._prog=glCreateProgram()
		EndIf
		
		Local vert_comp$,frag_comp$
		code.GetComposition vert_comp,frag_comp
		
		Local vs_error$=MakeShader(res._vert,res._prog,GL_VERTEX_SHADER,vert_comp)		
		Local fs_error$=MakeShader(res._frag,res._prog,GL_FRAGMENT_SHADER,frag_comp)
		
		code._recompile=False
		
		If vs_error Or fs_error
			code._res=Null
			Return Null
		Else
			glLinkProgram res._prog
			code._res=res	
			Return code
		EndIf
	End Method
	
	Method MakeShader$(id Var,prog,typ,src$)
		If src="" Return 
		
		If id=0
			id=glCreateShader(typ)
			glAttachShader prog,id
		EndIf
		
		Local str:Byte Ptr=src.ToCString()
		glShaderSource id,1,Varptr str,Null
		MemFree str
		
		glCompileShader id
		Return GetError(id)
	End Method
	
	Method GetError$(shader)
		Local status
		glGetShaderiv shader,GL_COMPILE_STATUS,Varptr status
		If status<>GL_TRUE
			Local max_size
			glGetShaderiv shader,GL_INFO_LOG_LENGTH,Varptr max_size
			Local str:Byte[max_size],size
			glGetShaderInfoLog shader,max_size,Varptr size,str
			'ModuleLog "Fragment compilation error from ~q"+shader.GetName()+"~q."
			Return String.FromCString(str)
		EndIf
	End Method
		
	Method Apply(res:Object,data:Object)
		Local id
		If res<>Null id=TGLSLData(res)._prog
		glUseProgram id
	End Method
	
	Method Name$()
		Return "glsl"
	End Method
End Type

Type TGLMax2DExDriver Extends TMax2DExDriver
	Method SetGraphics(g:TGraphics)
		Super.SetGraphics g
		glewInit
	End Method
	
	Method MakeBuffer:TBuffer(src:Object,width,height,flags)
		Local frame:TBufferedImageFrame=TBufferedImageFrame(src)
		If Not frame Return Null
		If frame._buffer Return frame._buffer
		frame._buffer=MakeGLBuffer(TGLImageFrame(frame._parent).name,width,height,flags)
		Return frame._buffer		
	End Method
	
	Method MakeGLBuffer:TGLBuffer(tex,width,height,flags)
		Local buffer:TGLBuffer=New TGLBuffer
		buffer._tex=tex
		buffer._width=width
		buffer._height=height
		
		glGenFramebuffersEXT 1,Varptr buffer._fbo
		glBindRenderbufferEXT GL_RENDERBUFFER_EXT,buffer._fbo
		glFramebufferTexture2DEXT GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,buffer._tex,0
		
		'If flags&BUFFER_DEPTH
		'	glGenFramebuffersEXT 1,Varptr buffer._depth
		'	glBindRenderbufferEXT GL_RENDERBUFFER_EXT,buffer._depth
		'	glRenderbufferStorageEXT GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,width,height
		'	glFramebufferRenderbufferEXT GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,buffer._depth
		'EndIf
		
		Local status=glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)
		
		SetBuffer _currentbuffer
		
		If status<>GL_FRAMEBUFFER_COMPLETE_EXT Return Null
		
		Return buffer
	End Method
	
	Method SetBuffer(buffer:TBuffer)
		If _currentbuffer<>_backbuffer
			glBindTexture GL_TEXTURE_2D,TGLBuffer(_currentbuffer)._tex
			glGenerateMipmapEXT GL_TEXTURE_2D
		EndIf
		_currentbuffer=buffer		
		
		If buffer<>_backbuffer
			glBindFramebufferEXT GL_FRAMEBUFFER_EXT,TGLBuffer(buffer)._fbo
			glFramebufferTexture2DEXT GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,TGLBuffer(buffer)._tex,0
			SetGLContext buffer._width,buffer._height
		Else
			glBindFramebufferEXT GL_FRAMEBUFFER_EXT,0
			ResetGLContext _current
		EndIf
	End Method	
	
	Method SetGLContext(width,height)
		glMatrixMode GL_PROJECTION
		glLoadIdentity
		glScalef 1,-1,1
		glOrtho 0,width,height,0,-1,1
		glMatrixMode GL_MODELVIEW
		glLoadIdentity
		glViewport 0,0,width,height
	End Method
	
	Method ResetGLContext(g:TGraphics)
		TGLMax2DDriver(_parent).ResetGLContext g
	End Method
End Type

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function GLMax2DExDriver:TGLMax2DExDriver()
	If GLMax2DDriver()
		Global driver:TGLMax2DExDriver=New TGLMax2DExDriver
		driver._parent=GLMax2DDriver()
		Return driver
	End If
End Function

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function GLSLShaderDriver:TGLSLDriver()
	Global _driver:TGLSLDriver=New TGLSLDriver
	Return _driver
End Function

Local driver:TGLMax2DExDriver=GLMax2DExDriver()
If driver 
	SetGraphicsDriver driver
	SetShaderDriver GLSLShaderDriver()
EndIf
