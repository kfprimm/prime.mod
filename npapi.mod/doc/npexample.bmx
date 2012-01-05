
Strict

Framework Prime.NPAPI
Import "resource.o"

Type TExampleObject Extends TNPAPIObject
	Field test

	Method Initialize(args:TMap)
		
	End Method
	
	Method InvokeDefault()
		Return NPString("Hello World!")
	End Method
		
	Method Start(name$, purpose$)	
		'SetGraphics GetGraphics()
		Return NPString("Started up by "+name+" in order to "+purpose+"!")
	End Method
	
	Method Update()
	
		Return NPVoid() 
	End Method
	
	Method OnEvent( event:TEvent )	
		Select event.id
		Case EVENT_MOUSEDOWN
			Notify "Mouse down!"
		Case EVENT_MOUSEUP
			Notify "Mouse up!"
		Case EVENT_GADGETPAINT
			Notify "Paint!"
		End Select
	End Method
	
	Method Stop()

	End Method
	
	Method Scriptable()
		Return True
	End Method
End Type

Type TExamplePlugin Extends TNPAPIPlugin
	Method Initialize()
		RegisterMime "application/x-bmx-npapi", New TExampleObject, "Example Object from a BlitzMax generated plugin." 
	End Method 
	
  Method Name$()
		Return "BlitzMax Example Plugin"
	End Method
	
	Method Author$()
		Return "Example, Inc."
	End Method
	
	Method Copyright$()
		Return "Copyright 2011 John Doe"
	End Method
	
	Method Description$()
		Return "An example plugin generated from BlitzMax."
	End Method
End Type 
New TExamplePlugin.Run
