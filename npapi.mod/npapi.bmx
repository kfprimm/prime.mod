
Strict

Module Prime.NPAPI
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "LD_OPTS: --exclude-all-symbols --enable-stdcall-fixup"

Import BRL.FileSystem
Import BRL.TextStream
Import BRL.RamStream
Import BRL.System
Import BRL.Reflection
Import BRL.Map
Import BRL.GLMax2D
Import "glue.c"

?Win32
Incbin "npapi.rc"
?

Extern
	Function _npapi_set_string(v:Byte Ptr, text:Byte Ptr, length)
	Function _npapi_get_string:Byte Ptr(v:Byte Ptr)
End Extern

Type TNPAPIObject
	Global _methodmap:TMap = New TMap
	Field _hwnd, _result:Byte Ptr
	Field _graphics:TGraphics
	
	Method Initialize(args:TMap) ; End Method
	Method Scriptable() ; End Method
	Method InvokeDefault() ; End Method
	Method Destroy() ; End Method
	Method OnEvent( event:TEvent ) ; End Method
	
	Method NPVoid()
		Int Ptr(_result + 0)[0] = 0
		Return True
	End Method
	
	Method NPNull()
		Int Ptr(_result + 0)[0] = 1
		Return True
	End Method
	
	Method NPBool(bool)
		Int Ptr(_result + 0)[0] = 2
		Int Ptr(_result + 8)[0] = (bool <> 0)
		Return True
	End Method

	Method NPInt(integer)
		Int Ptr(_result + 0)[0] = 3
		Int Ptr(_result + 8)[0] = integer
		Return True
	End Method
		
	Method NPDouble(d!)
		Int Ptr(_result + 0)[0] = 4
		Double Ptr(_result + 8)[0] = d
		Return True
	End Method
	
	Method NPString(str$)
		Local txt:Byte Ptr = str.ToCString()
		_npapi_set_string _result, txt, str.length
		MemFree txt
		Return True
	End Method
	
	Method Hwnd()
		Return _hwnd
	End Method

	Method NPObject(obj:TNPAPIObject)
		Int Ptr(_result + 0)[0] = 6
		Byte Ptr Ptr(_result + 1)[0] = (Byte Ptr(obj) - 8)
	End Method

	Method RegisterMethods()
		Local typ:TTypeId = TTypeId.ForObject(Self), name$ = typ.Name()
		If _methodmap.ValueForKey(name) = Null
			Local methods:TMap = New TMap
			For Local meth:TMethod = EachIn typ.EnumMethods()
				methods.Insert meth.Name(), meth
			Next
			_methodmap.Insert name, methods
		EndIf
	End Method
	
	Method GetGraphics:TGraphics( flags = GRAPHICS_BACKBUFFER|GRAPHICS_DEPTHBUFFER )
		If _graphics = Null _graphics = brl.Graphics.AttachGraphics(Hwnd(), flags)
		Return _graphics
	End Method
	
	Method FindMethod:TMethod(name$)
		Return TMethod(TMap(_methodmap.ValueForKey(TTypeId.ForObject(Self).Name())).ValueForKey(name))
	End Method
		
	Function GetData(obj:TNPAPIObject, so:Byte Ptr Ptr) "C"
		If Not obj.Scriptable() Return False
		so[0] = obj._scriptobject
		Return True
	End Function	
	
	Function SetData(obj:TNPAPIObject, so:Byte Ptr) "C"
		obj._scriptobject = so
	End Function	
	
	Function OnHasMethod(obj:TNPAPIObject, methodName:Byte Ptr) "C"
		Return obj.FindMethod(String.FromCString(methodName)) <> Null
	End Function
	
	Function OnInvoke(obj:TNPAPIObject, methodName:Byte Ptr, vargs:Byte Ptr, count, result:Byte Ptr) "C"		
		Local name$
		If methodName = Null
			name = "InvokeDefault"
		Else
			 name = String.FromCString(methodName)
		EndIf
		Local meth:TMethod = obj.FindMethod(name), args:Object[meth.ArgTypes().length]
		For Local i = 0 Until Min(args.length, count)
			Local arg:Byte Ptr = vargs + (16 * i)
			Select Int Ptr(arg + 0)[0]
			Case 2, 3
				args[i] = String(Int Ptr(arg + 8)[0])	
			Case 4
				args[i] = String(Double Ptr(arg + 8)[0])
			Case 5
				args[i] = String.FromCString(_npapi_get_string(arg))	
			End Select
		Next
		obj._result = result
		Return Int(String(meth.Invoke(obj, args)))
	End Function
	
	Function OnHandleEvent(obj:TNPAPIObject, event, wParam, lParam)
		?Win32
		bbSystemEmitOSEvent( obj.Hwnd(), event,wParam,lParam,obj )
		?
	End Function
	
	Function EventHook:Object(id, data:Object, context:Object)
		Local event:TEvent = TEvent(data)
		Local obj:TNPAPIObject = TNPAPIObject(event.source)
		If obj obj.OnEvent(event)
		Return data
	End Function
		
	Function OnSetWindow(obj:TNPAPIObject, hwnd)
		obj._hwnd = hwnd
	End Function
	
	Field ded = 4, _scriptobject:Byte Ptr
End Type

Type TNPAPIPlugin
	Global _plugin:TNPAPIPlugin
	Field _objects:TTypeId[]
	Field _mimes$[],_descriptions$[]
	
	Field _instances:TNPAPIObject[]
	
	Method RegisterMime( mime$, obj:TNPAPIObject, description$ = "", exts$[] = Null)
		obj.RegisterMethods
		_mimes :+ [mime]
		_objects :+ [TTypeId.ForObject(obj)]
		_descriptions :+ [description]
	End Method
		
	Method New()
		_plugin = Self
	End Method
	
	Method Run()
		_plugin.Initialize
		
		?Debug
		Local base$ = StripAll(StripAll(AppFile))
		?Win32Debug	
		Local def$ = "LIBRARY ~q"+base+".dll~q~nEXPORTS~n~nNP_GetEntryPoints~nNP_Initialize~nNP_Shutdown~nNP_Shutdown@0~n"
		SaveText def, AppDir+"/"+base+".def"
			
		Local header$=""
		header:+ "#define PLUGIN_COMPANYNAME ~q"+Author()+"~q~n"
		header:+ "#define PLUGIN_DESCRIPTION ~q"+Description()+"~q~n"
		header:+ "#define PLUGIN_DESC ~q"+"|".Join(_descriptions)+"~q~n"
		header:+ "#define PLUGIN_INTERNALNAME ~q"+Name().Replace(" ","")+"~q~n"
		header:+ "#define PLUGIN_COPYRIGHT ~q"+Copyright()+"~q~n"
		header:+ "#define PLUGIN_MIME ~q"+"|".Join(_mimes)+"~q~n"
		header:+ "#define PLUGIN_FILENAME ~q"+base+".dll~q~n"
		header:+ "#define PLUGIN_NAME ~q"+Name()+"~q~n"

		
		Local rc$ = LoadText("incbin::npapi.rc")
		SaveText header+rc, AppDir+"/"+base+".rc"
		system_ "windres ~q"+AppDir+"/"+base+".rc"+"~q ~q"+AppDir+"/resource.o~q"
		?Debug
		Local BMX_PATH$=getenv_("BMX_PATH")
		Local src$=ExtractDir(AppFile)+"/"+base+".bmx", opts$ = ""
		system_ BMX_PATH+"/bin/bmk makelib -a -r "+src
		?
	End Method
	
	Method MIMEDescription$()
		Local desc$
		For Local i = 0 To _objects.length-1
			desc:+ _mimes[i]+"::"+_descriptions[i]
			If i < _objects.length-1 desc:+";"
		Next
	End Method
	
	Method Initialize() Abstract
	Method Name$() Abstract
	Method Description$() Abstract
	Method Author$() Abstract
	Method Copyright$() Abstract
		
	Method Shutdown() ; End Method
	
	Function OnNew:Byte Ptr(mime:Byte Ptr, argc, argn:Byte Ptr Ptr, argv:Byte Ptr Ptr) "C"
		Local m$=String.FromCString(mime)
		For Local i=0 To _plugin._objects.length-1
			If m = _plugin._mimes[i] 
				Local map:TMap = CreateMap()
				For Local i = 0 Until argc
					MapInsert map, String.FromCString(argn[i]), String.FromCString(argv[i])
				Next
				Local obj:TNPAPIObject = TNPAPIObject(_plugin._objects[i].NewObject())
				If obj.Initialize(map) <> 0 Return Null
				_plugin._instances :+ [obj]
				Return Byte Ptr(obj) - 8
			EndIf
		Next
	End Function
	
	Function OnDestroy(obj:TNPAPIObject)
		For Local i = 0 To _plugin._instances.length-1
			If _plugin._instances[i] = obj
				_plugin._instances = _plugin._instances[..i] + _plugin._instances[i+1..]
				obj.Destroy()
				Return
			EndIf
		Next
	End Function
	
	Function OnShutdown() "C"
		_plugin.Shutdown
	End Function

	Function GetMIMEDescription:Byte Ptr() "C"
		Global str:Byte Ptr = Null
		If str = Null str = _plugin.MIMEDescription().ToCString()
		Return str
	End Function
	
	Function GetName:Byte Ptr() "C"
		Global str:Byte Ptr = Null
		If str = Null str = _plugin.Name().ToCString()
		Return str
	End Function
	
	Function GetDescription:Byte Ptr() "C"
		Global str:Byte Ptr = Null
		If str = Null str = _plugin.Description().ToCString()
		Return str
	End Function
End Type

AddHook EmitEventHook, TNPAPIObject.EventHook
