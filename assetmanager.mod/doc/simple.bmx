
Strict

Import sys87.AssetManager
Import sys87.ImageAsset

Graphics 800,600

Local AssetMan:TAssetManager=New TAssetManager
AssetMan.Load("example.xml")

Local a:TImage=TImage(AssetMan.Get("a"))
Local b:TImage=TImage(AssetMan.Get("b"))
Local c:TImage=TImage(AssetMan.Get("c"))

While Not KeyDown(KEY_ESCAPE)
	Cls
	DrawImage a,0,0
	DrawImage b,0,32
	DrawImage c,0,64
	Flip
Wend