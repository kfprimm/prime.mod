
Strict

Module Prime.PixmapAsset
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Pixmap
Import Prime.AssetManager

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
