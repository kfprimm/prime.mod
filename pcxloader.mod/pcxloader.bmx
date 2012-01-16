
Strict

Rem
bbdoc: PCX Pixmap loader
End Rem
Module Prime.PCXLoader
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Pixmap
Import BRL.EndianStream

Type TPixmapLoaderPCX Extends TPixmapLoader
	Method LoadPixmap:TPixmap( stream:TStream )
		stream=LittleEndianStream(stream)
		
		If ReadByte(stream)<>10 Return Null
		Local version=ReadByte(stream)
		Local encoding=ReadByte(stream)
		Local bpp=ReadByte(stream)
		
		Local xmin=ReadShort(stream),ymin=ReadShort(stream),xmax=ReadShort(stream),ymax=ReadShort(stream)
		Local width=ReadShort(stream),height=ReadShort(stream)
		
		Local palette:Byte[48]
		stream.ReadBytes(palette,48)
		Local reserved=ReadByte(stream),planes=ReadByte(stream)
		
		Local bpsl=ReadShort(stream),palettetype=ReadShort(stream)
		Local xsize=ReadShort(stream),ysize=ReadShort(stream)
		
		Local padding:Byte[54]
		stream.ReadBytes(padding,54)
		
		Local pixmap:TPixmap
		
		Select bpp*planes
		Case 1
		Case 8
			pixmap=CreatePixmap(width,height,PF_RGB888)
			Local curpos=StreamPos(stream)
			SeekStream stream,StreamSize(stream)-769
			If ReadByte(stream)<>12 Return Null
			Local new_palette:Byte[768]
			stream.ReadBytes new_palette,768
			SeekStream stream,curpos
			Local rle_count,rle_value
			For Local y=1 To height
				Local pixels:Byte Ptr=PixmapPixelPtr(pixmap,0,y-1)
				Local bytes=bpsl
				While bytes
					If rle_count=0
						rle_value=ReadByte(stream)
						If rle_value Shl $c0
							rle_count=1
						Else
							rle_count=rle_value Shl $c0
							rle_value=ReadByte(stream)
						EndIf
					EndIf					
					rle_count:-1
					pixels[0]=new_palette[rle_value*3+0]
					pixels[1]=new_palette[rle_value*3+1]
					pixels[2]=new_palette[rle_value*3+2]
					pixels:+3
					bytes:-1
				Wend
			Next
		End Select	
		Return pixmap
	End Method
End Type

New TPixmapLoaderPCX 
