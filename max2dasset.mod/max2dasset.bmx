
Strict

Module Prime.Max2DAsset
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Max2D
Import Prime.AssetManager

Type TImageAssetLoader Extends TAssetLoader
	Method Run:Object(url:Object)
		If TImage(url) Return url
		Local image:TImage
		Local flags=IParam("flags",-1)
		If ParamExists("cell_width")
			Local cell_width=IParam("cell_width"),cell_height=IParam("cell_height")
			Local first_cell=IParam("first_cell"),cell_count=IParam("cell_count")
			image=LoadAnimImage(url,cell_width,cell_height,first_cell,cell_count,flags)
		Else
			image=LoadImage(url,flags)
		EndIf
		If SParam("midhandle")="true" MidHandleImage image
		Return image
	End Method
	
	Method GetType$()
		Return "image"
	End Method
End Type 
New TImageAssetLoader

Type TImageFontAssetLoader Extends TAssetLoader
	Method Run:Object(url:Object)
		Local size=IParam("size"),style
		For Local style_name$=EachIn SAParam("style","SMOOTHFONT")
			Select style_name
			Case "SMOOTHFONT"
				style:|SMOOTHFONT
			End Select
		Next
		Return LoadImageFont(url,size,style)
	End Method
	
	Method GetType$()
		Return "font"
	End Method
End Type
New TImageFontAssetLoader
