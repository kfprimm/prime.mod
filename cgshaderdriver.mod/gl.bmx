
Strict

Import BRL.GLGraphics
Import "driver.bmx"

Type TCgGLData
	Field _frag,_vert
	Field _data,_mvp
End Type

Rem
Type TCgGLShaderDataHandler Extends TShaderDataHandler
	Method Apply(shader:Object, data:Object)
		If cgres._data
			cgSetMatrixParameterfr cgres._mvp,data._modelviewproj.ToPtr()
		EndIf
	End Method
End Type
End Rem

Type TCgGLShaderDriver Extends TCgShaderDriver	
	Method Initialize(driver:TGraphicsDriver)
		_vert_profile=cgGLGetLatestProfile(CG_GL_VERTEX)
		_frag_profile=cgGLGetLatestProfile(CG_GL_FRAGMENT)
		If _vert_profile=CG_PROFILE_UNKNOWN Or _frag_profile=CG_PROFILE_UNKNOWN Return False
		Return True
	End Method	
	
	Method Compatible(driver:TGraphicsDriver)
		'Local d:TMaxB3DDriver=TMaxB3DDriver(driver)
		'If d=Null Return False
		Return True
		'Return TGLGraphicsDriver(d._parent._parent)=True
	End Method
	
	Method Compile:TShaderCode(code:TShaderCode)
		Local res:TCgGLData=TCgGLData(code._res)
		If Not code._recompile And res<>Null Return code
		
		If res=Null res=New TCgGLData
		
		Local vert_comp$,frag_comp$
		code.GetComposition vert_comp,frag_comp

		cgGLSetOptimalOptions _vert_profile
		res._vert=cgCreateProgram(_context,CG_SOURCE,vert_comp,_vert_profile,"main",Null)
		cgGLLoadProgram res._vert
		
		res._data=cgGetFirstParameter(res._vert,CG_PROGRAM)
		While res._data>0
			If cgGetTypeString(cgGetParameterNamedType(res._data))="MB3D_DATA" Exit
			res._data=cgGetNextParameter(res._data)
		Wend
		
		If res._data
			res._mvp=cgGetNamedStructParameter(res._data,"modelViewProj")
			If Not res._mvp RuntimeError "De"
		EndIf
		
		DebugLog "Vertex: "+GetLastError()
		
		cgGLSetOptimalOptions _frag_profile
		res._frag=cgCreateProgram(_context,CG_SOURCE,frag_comp,_frag_profile,"main",Null)
		cgGLLoadProgram res._frag
		
		DebugLog "Pixel: "+GetLastError()
		
		code._recompile=False
		code._res=res
		Return code
	End Method
		
	Method Apply(res:Object,data:Object)
		Local cgres:TCgGLData=TCgGLData(res)
		If cgres
			_handler.Apply cgres, data
			cgGLEnableProfile _vert_profile
			cgGLEnableProfile _frag_profile
			
			cgGLBindProgram cgres._vert		
			cgGLBindProgram cgres._frag
		Else
			cgGLDisableProfile _vert_profile
			cgGLDisableProfile _frag_profile			
		EndIf
	End Method	
	
	Method GetLastError$()
		Local error,str$
		str=String.FromCString(cgGetLastErrorString(Varptr error))
		Return str
	End Method
End Type
New TCgGLShaderDriver
