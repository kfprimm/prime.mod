
Strict

Rem
	bbdoc: Gile[s] File Format Library
End Rem
Module Prime.LibGLS
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"
ModuleInfo "Spec: http://www.frecle.net/download/glsspecs.htm"

Const GLS_AUTHOR = $F000
Const GLS_MODELS = $1000
Const GLS_MODEL  = $1001

Type TGLSScene
	Field version#
	Field author$
End Type


