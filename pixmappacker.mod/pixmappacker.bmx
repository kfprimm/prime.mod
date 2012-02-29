
Strict

Rem
	bbdoc: Pixmap Packer
End Rem
Module Prime.PixmapPacker
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Pixmap
Import Prime.BlockPacker

Type TPixmapPacker
	Field packer:TBlockPacker = New TBlockPacker
	Field pixmaps:TPixmap[], format
	
	Method Add(pixmap:TPixmap)
		packer.Add PixmapWidth(pixmap), PixmapHeight(pixmap)
		If pixmap.format > format format = pixmap.format 
		pixmaps :+ [pixmap]
	End Method
	
	Method Reset()
		packer.Reset
		pixmaps = Null
		format = 0
	End Method
	
  Method Run:TPixmap(flags = PACKER_NONE, minsize = -1)
  	Local width, height
  	If packer.Run(width,height,flags,minsize)
  		Local pixmap:TPixmap = CreatePixmap(width,height,format)
  		ClearPixels pixmap,0
  		For Local i = 0 Until pixmaps.length
  			Local sub:TPixmap = pixmaps[i]
  			Local x,y,width,height
  			If packer.Get(i,x,y,width,height)
					sub=CreatePixmap(sub.height,sub.width,pixmap.format)
					For Local y=0 To PixmapHeight(pixmaps[i])-1
						For Local x=0 To PixmapWidth(pixmaps[i])-1
							WritePixel sub,((sub.width-1)-y),x,ReadPixel(pixmaps[i],x,y)
						Next
					Next
  			EndIf
			pixmap.Paste sub,x,y
  		Next
  		Return pixmap
  	EndIf
  	Return Null
  End Method
  
  Method Get(index, x Var, y Var, wid Var, hit Var)
  	Return packer.Get(index,x,y,wid,hit)
  End Method
End Type

