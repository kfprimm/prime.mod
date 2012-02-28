
Strict

Rem
	bbdoc: Pixmap Packer
End Rem
Module Prime.PixmapPacker
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Credit: Adapted from John Ratcliff's TexturePacker."
ModuleInfo "Credit: http://code.google.com/p/texture-atlas"

Import BRL.Pixmap

Private 

Function Pow2Size( n )
	Local t=1
	While t<n
		t:*2
	Wend
	Return t
End Function

Type TRect
	Field x,y,width,height
	Field flipped,placed
End Type

Public

Type TRectPacker
	Field area, longestedge
	Field rects:TRect[]
	
	Method Add(width, height)
		Local rect:TRect = New TRect
		rect.width = width
		rect.height = height
		rects :+ [rect]
		area :+ width*height
		longestedge = Max(longestedge, Max(width, height))		
	End Method
	
	Method Run(width Var, height Var, pow2 = False, border = 0)
		If border
			For Local r:TRect = EachIn rects
				r.width :+ border*2
				r.height :+ border*2
			Next
			longestedge :+ border*2
		EndIf
		If pow2 longestedge = Pow2Size(longestedge)
		width = longestedge 
		Local count = area/(longestedge*longestedge)
		height = (count+2)*longestedge
		For Local i = 0 Until rects.length-1
			Local index,mostedge,mostarea
			For Local j = 0 Until rects.length-1
				
			Next
		Next
	End Method
End Type