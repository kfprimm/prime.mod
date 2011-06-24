
Strict

Module sys87.Archive
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream

Type TArchive
	Field _readable,_writeable
	Field _files:TArchiveFile[]
	
	Method Files:TArchiveFile[]()
		Return _files[..]
	End Method
End Type

Type TArchiveFile
	Field name$,size,_archive:TArchive
	
	Method Stream:TStream() Abstract
End Type

Type TArchiveFileStreamFactory Extends TStreamFactory
	Method CreateStream:TStream( url:Object,proto$,path$,readable,writeable )
			
	End Method
End Type
New TArchiveFileStreamFactory

Type TArchiveFactory 
	Global _first:TArchiveFactory
	Field _next:TArchiveFactory
	
	Method New()
		_next=_first
		_first=Self
	End Method
	
	Function Open:TArchive(url:Object,readable=True,writeable=True)
		Local factory:TArchiveFactory=_first
		While factory
			Local arc:TArchive=factory.OpenArchive(url,readable,writeable)
			If arc
				arc._readable=readable
				arc._writeable=writeable
				Return arc
			EndIf
			factory=factory._next
		Wend
	End Function
	
	Method OpenArchive:TArchive(url:Object,readable,writeable) Abstract
End Type

Function OpenArchive:TArchive(url:Object)
	Return TArchiveFactory.Open(url,True,True)
End Function
Function WriteArchive:TArchive(url:Object)
	Return TArchiveFactory.Open(url,False,True)
End Function
Function ReadArchive:TArchive(url:Object)
	Return TArchiveFactory.Open(url,True,False)
End Function