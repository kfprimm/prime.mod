
Strict

Module sys87.PixmapAsset
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Pixmap
Import sys87.AssetManager

Type TPixmapAssetLoader Extends TAssetLoader
	Method Run:Object(url:Object)
		If TPixmap(url) Return url
		Return LoadPixmap(GetStream(url))
	End Method
	
	Method GetType$()
		Return "pixmap"
	End Method
End Type
New TPixmapAssetLoader