
Strict

Module sys87.ZipArchive
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream
Import sys87.Archive
Import sys87.libzip

Private
Function zip_source(state:Byte Ptr,data:Byte Ptr,length,cmd)
	DebugLog "de"
End Function

Public

Type TZipStream Extends TStream
	Field _file,_pos
	
	Method Seek(pos)
		_pos=pos
	End Method
	
	Method Pos()
		Return _pos
	End Method
		
	Method ReadBytes( buf:Byte Ptr,count )
		zip_fread _file,buf,count
		_pos:+count
		Return count
	End Method
End Type

Type TZipArchiveFactory Extends TArchiveFactory
	Method OpenArchive:TArchive(url:Object,readable,writeable)
		Local arc:TZipArchive=New TZipArchive
		arc._stream=OpenStream(url,readable,writeable)
		Local flags=ZIP_CREATE
		If writeable=False flags=0
		arc._handle=zip_open(String(url),flags)
		If Not arc._handle Return Null
		
		arc._files=New TArchiveFile[zip_get_num_files(arc._handle)]
		Local sb:zip_stats=New zip_stats
		
		For Local i=0 To arc._files.length-1
			zip_stat_init sb
			zip_stat_index arc._handle,i,0,sb
			'arc._files[i]=String.FromCString(sb.name)
		Next
		
		Return arc
	End Method
End Type
New TZipArchiveFactory

Type TZipArchive Extends TArchive
	Field _stream:TStream
	Field _handle
	
	Field _indices[]
	
	Method Delete()
		zip_close _handle
	End Method	
End Type

Type TZipArchiveFile Extends TArchiveFile
	Field _file
	
	Method Open:TStream()
		'_file=zip_fopen(_archive._handle,index,ZIP_FL_UNCHANGED)
	End Method
End Type
