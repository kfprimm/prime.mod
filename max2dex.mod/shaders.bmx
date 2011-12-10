
Strict

Import BRL.Hook

Const SHADER_VERTEX   = 1
Const SHADER_PIXEL    = 2
Const SHADER_GEOMETRY = 4

Type TShaderFrag
	Field _code$,_type
	Field _recompilehook=AllocHookId()
	
	Function Create:TShaderFrag(code$,typ)
		Local frag:TShaderFrag=New TShaderFrag
		frag.SetCode code
		frag._type=typ
		Return frag
	End Function
	
	Method GetCode$()
		Return _code
	End Method
	Method SetCode(code$)
		_code=code
		RunHooks _recompilehook,Self
	End Method
End Type

Type TShaderCode
	Field _driver$,_frag:TShaderFrag[]
	Field _composition$
	Field _res:Object,_recompile
	
	Method AddFrag(frag:TShaderFrag)
		AddFrags([frag])
	End Method
	Method AddFrags(frags:TShaderFrag[])
		For Local frag:TShaderFrag=EachIn frags
			AddHook frag._recompilehook,RecompileHook,Self
		Next
		_recompile=True
		_frag:+frags
	End Method
	
	Method GetComposition(vert$ Var,frag$ Var)
		vert="";frag=""
		For Local f:TShaderFrag=EachIn _frag
			Select f._type
			Case SHADER_PIXEL
				frag:+f._code
			Case SHADER_VERTEX
				vert:+f._code
			End Select
		Next		
	End Method
	
	Function RecompileHook:Object(id,data:Object,context:Object)
		TShaderCode(context)._recompile=True
		Return data
	End Function
End Type

Type TShader
	Field _name$
	Field _meta_key$[],_meta_value$[]
	Field _code:TShaderCode[]	
	
	Method Create:TShader(name$)
		SetName name
		Return Self
	End Method
	
	Method Load:TShader(url:Object)
		Return Null
	End Method
	
	Method GetName$()
		Return _name
	End Method
	Method SetName(name$)
		_name=name
	End Method
	
	Method GetMetaData$(name$)
		For Local i=0 To _meta_key.length-1
			If _meta_key[i]=name Return _meta_value[i]
		Next
	End Method
	Method SetMetaData$(name$,data$)
		For Local i=0 To _meta_key.length-1
			If _meta_key[i]=name
				_meta_value[i]=data
				Return
			EndIf
		Next
		_meta_key:+[name]
		_meta_value:+[data]
	End Method
	
	Method ImportCode(shader:TShader)
		_code:+shader._code
	End Method
	
	Method AddCode:TShaderCode(driver$="")
		Local code:TShaderCode=GetCode(driver)
		If code<>Null Return code
		code=New TShaderCode
		code._driver=driver
		_code:+[code]
		Return code
	End Method
	Method GetCode:TShaderCode(driver$)
		If _code.length=0 Return Null
		For Local code:TShaderCode=EachIn _code
			If code._driver=driver Return code
		Next
		Return Null
	End Method
End Type

Type TShaderHandler
	Method Apply(shader:Object, data:Object) Abstract
End Type

Type TShaderDriver
	Global _first:TShaderDriver
	Field _next:TShaderDriver
	Field _handler:TShaderHandler
	
	Method New()
		_next=_first
		_first=Self		
	End Method
	
	Method Use(shader:TShader,data:Object)
		Local res:Object
		If shader<>Null
			Local code:TShaderCode=shader.GetCode(Name())
			If code code=Compile(code)
			If code res=code._res
		EndIf
		Apply res,data
		Return True
	End Method
	
	Method Compile:TShaderCode(code:TShaderCode) Abstract
	Method Apply(res:Object, data:Object) Abstract
	Method Name$() Abstract
End Type

' To be ported later
Rem
		Try
			If ReadLine(stream)<>"[shader]" CloseStream(stream);Return Null
			
			Local status,code:TShaderCode
			While Not Eof(stream)
				Local line$=ReadLine(stream).Trim()
				If line="[import]"
					status=1
				ElseIf line=""
				ElseIf line="[/shader]"
					Exit
				Else
					If line[0]="["[0] And line[line.length-1]="]"[0]
						status=2
						code=shader.AddCode(line[1..line.length-1])
					Else
						Select status
						Case 0
							Local split$[]=line.Split(":")
							Local name$=split[0].Trim(),value$=split[1].Trim()
							shader.SetMetaData name,value
						Case 1
							Local imported:TShader
							For Local shader:TShader=EachIn _config.List[WORLDLIST_SHADER]
								If shader._name=line imported=shader;Exit
							Next
							If imported shader.ImportCode(shader)
						Case 2
							Local split$[]=line.Split(":")
							Local typ_str$=split[0].Trim(),url$=split[1].Trim()
							Local typ
							Select typ_str
							Case "pixel"
								typ=SHADER_PIXEL
							Case "vertex"
								typ=SHADER_VERTEX
							End Select
							Local text$
							Try
								text=LoadText(GetStream(url))
							Catch e:Object
								ModuleLog "Failed to load ~q"+url+"~q."
							End Try
							code.AddFrag TShaderFrag.Create(text,typ)
						End Select
					EndIf
				EndIf
			Wend
		Catch e:Object
		End Try
End rem