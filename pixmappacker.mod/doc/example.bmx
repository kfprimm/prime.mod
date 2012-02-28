
Strict

Import Prime.PixmapPacker

Local exts$[] = ["bmp", "jpg", "png", "tga"]

Local packer:TPixmapPacker = New TPixmapPacker

Local dir$ = RequestDir("Select a directory to pack...")
For Local f$ = EachIn LoadDir(dir)
	Local do = False
	For Local e$ = EachIn exts
		If ExtractExt(f) = e
			do = True
			Exit
		EndIf
	Next
	
	If Not do Continue

	Local pixmap:TPixmap = LoadPixmap(dir+"/"+f)
	If pixmap
		packer.Add pixmap
	Else
		Print "Failed to load ~q"+f+".~q"
	EndIf
Next

Print "Packing..."
Local pixmap:TPixmap = packer.Run(PACKER_POW2)
Print "Created "+PixmapWidth(pixmap)+"x"+PixmapHeight(pixmap)+" pixmap. Saving..."
SavePixmapPNG pixmap, dir+"/textureatlas.png"
Print "Saved as ~q"+dir+"/textureatlas.png~q"
