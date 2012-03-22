
Strict

Rem
	bbdoc: Extra pixmap functions
End Rem
Module Prime.PixmapEx
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Pixmap

Rem
	bbdoc: Needs documentation. #TODO
End Rem
Function BlurPixmap:TPixmap(pixmap:TPixmap, radius# = 1)
	Local out:TPixmap = CopyPixmap(pixmap)		
	Local w = PixmapWidth(pixmap), h = PixmapHeight(pixmap)

	For Local y = 0 To H-1
		For Local x = 0 To W-1
			Local r, g, b, num	

			For Local y2 = Max(y - radius, 0) To Min(y + radius, h-1)
				For Local x2 = Max(x - radius, 0) To Min(x + radius, w-1)
					Local argb = ReadPixel(pixmap,x2, y2)&$FFFFFF
					
					r :+ (argb Shr 16 & %11111111)
					g :+ (argb Shr 8 & %11111111)
					b :+ (argb & %11111111)
					
					num = num + 1
				Next	
			Next
			
			out.WritePixel x,y,((255&$ff) Shl 24)|((Min(255, r / num)&$ff) Shl 16)|((Min(255, g / num)&$ff) Shl 8)|(Min(255, b / num)&$ff)
		Next
	Next
	Return out
End Function
