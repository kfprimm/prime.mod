
Strict

Module sys87.ImageAsset
ModuleInfo "Author: Kevin Primm"

Import BRL.Max2D
Import sys87.AssetManager

Type TAssetLoaderImage Extends TAssetLoader
	Method Run:Object(url:Object)
		Local flags=IParam("flags",-1)
		If ParamExists("cell_width")
			Local cell_width=IParam("cell_width"),cell_height=IParam("cell_height")
			Local first_cell=IParam("first_cell"),cell_count=IParam("cell_count")
			Return LoadAnimImage(url,cell_width,cell_height,first_cell,cell_count,flags)
		EndIf
		Return LoadImage(url,flags)
	End Method
	
	Method GetType$()
		Return "image"
	End Method
End Type 
New TAssetLoaderImage