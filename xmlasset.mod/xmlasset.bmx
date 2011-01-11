
Strict

Module sys87.XMLAsset
ModuleInfo "Author: Kevin Primm"

Import PUB.MaXML
Import sys87.AssetManager

Type TXMLAssetLoader Extends TAssetLoader
	Method Run:Object(url:Object)
		Local doc:xmlDocument=New xmlDocument
		doc.Load url
		Return doc
	End Method
	
	Method GetType$()
		Return "xml"
	End Method
End Type
New TXMLAssetLoader