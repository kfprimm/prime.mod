
Strict

Rem
	bbdoc: DirectX (.x) model file library
End Rem
Module Prime.libx
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream
Import BRL.EndianStream

Const X_TOKEN_NAME         = 1
Const X_TOKEN_STRING       = 2
Const X_TOKEN_INTEGER      = 3
Const X_TOKEN_GUID         = 5
Const X_TOKEN_INTEGER_LIST = 6
Const X_TOKEN_FLOAT_LIST   = 7

Const X_TOKEN_OBRACE       = 10
Const X_TOKEN_CBRACE       = 11
Const X_TOKEN_OPAREN       = 12
Const X_TOKEN_CPAREN       = 13
Const X_TOKEN_OBRACKET     = 14
Const X_TOKEN_CBRACKET     = 15
Const X_TOKEN_OANGLE       = 16
Const X_TOKEN_CANGLE       = 17
Const X_TOKEN_DOT          = 18
Const X_TOKEN_COMMA        = 19
Const X_TOKEN_SEMICOLON    = 20
Const X_TOKEN_TEMPLATE     = 31
Const X_TOKEN_WORD         = 40
Const X_TOKEN_DWORD        = 41
Const X_TOKEN_FLOAT        = 42
Const X_TOKEN_DOUBLE       = 43
Const X_TOKEN_CHAR         = 44
Const X_TOKEN_UCHAR        = 45
Const X_TOKEN_SWORD        = 46
Const X_TOKEN_SDWORD       = 47
Const X_TOKEN_VOID         = 48
Const X_TOKEN_LPSTR        = 49
Const X_TOKEN_UNICODE      = 50
Const X_TOKEN_CSTRING      = 51
Const X_TOKEN_ARRAY        = 52

Type TXFile
	Field Templates:TXTemplate[]
	Field Entries:TXEntry[]
	
	Field VersionMajor,VersionMinor
	Field Precision
	
	Function Read:TXFile(url:Object)
		Local stream:TStream=LittleEndianStream(ReadStream(url))
		If ReadString(stream,4)<>"xof " Return Null
		Local file:TXFile=New TXFile
		file.VersionMajor=Int(ReadString(stream,2))	
		file.VersionMinor=Int(ReadString(stream,2))
		Local encoding$=ReadString(stream,4)
		file.Precision=(ReadString(stream,4)="0064")
		Select encoding
		Case "txt "
			DebugLog "text format not supported yet!"
			file=Null
		Case "bin "
			If Not file.ParseBinary(stream) file=Null
		End Select
		CloseStream stream
		Return file
	End Function
	
	Method ParseBinary(stream:TStream)
		While Not Eof(stream)
			Select ParseToken(stream)
			Case X_TOKEN_NAME
				Entries:+[ParseEntry(stream)]
			Case X_TOKEN_TEMPLATE
				Templates:+[ParseTemplate(stream)]
			Default
				Error
			End Select
		Wend
		Return True
	End Method
	
	Method ParseToken(stream:TStream)
		Local id=ReadShort(stream)
		Select id
		Case X_TOKEN_NAME
'			DebugLog "X_TOKEN_NAME"
		Case X_TOKEN_STRING
'			DebugLog "X_TOKEN_STRING"
		Case X_TOKEN_INTEGER
'			DebugLog "X_TOKEN_INTEGER"
		Case X_TOKEN_GUID
'			DebugLog "X_TOKEN_GUID"
		Case X_TOKEN_INTEGER_LIST
'			DebugLog "X_TOKEN_INTEGER_LIST"
		Case X_TOKEN_FLOAT_LIST	
'			DebugLog "X_TOKEN_FLOAT_LIST"
		Case X_TOKEN_OBRACE
'			DebugLog "X_TOKEN_OBRACE"
		Case X_TOKEN_CBRACE
'			DebugLog "X_TOKEN_CBRACE"
		Case X_TOKEN_OPAREN
'			DebugLog "X_TOKEN_OPAREN"
		Case X_TOKEN_CPAREN
'			DebugLog "X_TOKEN_CPAREN"
		Case X_TOKEN_OBRACKET
'			DebugLog "X_TOKEN_OBRACKET"
		Case X_TOKEN_CBRACKET
'			DebugLog "X_TOKEN_CBRACKET"
		Case X_TOKEN_OANGLE
'			DebugLog "X_TOKEN_OANGLE"
		Case X_TOKEN_CANGLE
'			DebugLog "X_TOKEN_CANGLE"
		Case X_TOKEN_DOT
'			DebugLog "X_TOKEN_DOT"
		Case X_TOKEN_COMMA
'			DebugLog "X_TOKEN_COMMA"
		Case X_TOKEN_SEMICOLON
'			DebugLog "X_TOKEN_SEMICOLON"
		Case X_TOKEN_TEMPLATE
'			DebugLog "X_TOKEN_TEMPLATE"
		Case X_TOKEN_WORD
'			DebugLog "X_TOKEN_WORD"
		Case X_TOKEN_DWORD
'			DebugLog "X_TOKEN_DWORD"
		Case X_TOKEN_FLOAT
'			DebugLog "X_TOKEN_FLOAT"
		Case X_TOKEN_DOUBLE
'			DebugLog "X_TOKEN_DOUBLE"
		Case X_TOKEN_CHAR
'			DebugLog "X_TOKEN_CHAR"
		Case X_TOKEN_UCHAR
'			DebugLog "X_TOKEN_UCHAR"
		Case X_TOKEN_SWORD
'			DebugLog "X_TOKEN_SWORD"
		Case X_TOKEN_SDWORD
'			DebugLog "X_TOKEN_SDWORD"
		Case X_TOKEN_VOID
'			DebugLog "X_TOKEN_VOID"
		Case X_TOKEN_LPSTR
'			DebugLog "X_TOKEN_LPSTR"
		Case X_TOKEN_UNICODE
'			DebugLog "X_TOKEN_UNICODE"
		Case X_TOKEN_CSTRING
'			DebugLog "X_TOKEN_CSTRING"
		Case X_TOKEN_ARRAY
'			DebugLog "X_TOKEN_ARRAY"
		Default
			id=0
			DebugLog "Unrecognized token."
		End Select
		Return id
	End Method
	
	Method ParseTemplate:TXTemplate(stream:TStream)
		Local template:TXTemplate=New TXTemplate
		 
		While True
			Select ParseToken(stream)
			Case X_TOKEN_NAME
				template.Name=TXToken.GetName(stream)
			Case X_TOKEN_GUID
				template.GUID=TXToken.GetGUID(stream)
				ParseTemplateParts(stream, template)
				Return
			Case X_TOKEN_OBRACE
			Default
				Error
			End Select
		Wend		
		
		Return template
	End Method
	
	Method ParseTemplateParts(stream:TStream, template:TXTemplate)
		ParseTemplateMembersPart(stream, template)
	End Method
	
	Method ParseTemplateMembersPart(stream:TStream, template:TXTemplate)
		ParseTemplateMembersList(stream, template)
	End Method
	
	Method ParseTemplateMembersList(stream:TStream, template:TXTemplate)
		ParseTemplateMembers(stream, template)
	End Method	
	
	Method ParseTemplateMembers(stream:TStream, template:TXTemplate)
		Local member:TXMember = new TXMember
		While True
			Local token = ParseToken(stream)
			Select token
			Case X_TOKEN_WORD, X_TOKEN_DWORD, X_TOKEN_FLOAT, X_TOKEN_DOUBLE, X_TOKEN_CHAR, X_TOKEN_UCHAR, X_TOKEN_SWORD, X_TOKEN_SDWORD, X_TOKEN_LPSTR, X_TOKEN_UNICODE, X_TOKEN_CSTRING
				ParsePrimitive(stream, member, token)
			Case X_TOKEN_ARRAY
				ParseArray(stream, member)
			Case X_TOKEN_NAME
				ParseTemplateReference(stream, member)				
			Case X_TOKEN_CBRACE
				Exit
			Default
				Error
			End Select
			template.Members:+[member]
			member = New TXMember
		Wend
	End Method	
	
	Method ParsePrimitive(stream:TStream, member:TXMember, datatype)
		member.DataType = datatype
		While True
			Select ParseToken(stream)
			Case X_TOKEN_NAME
				member.Name = TXToken.GetName(stream)
			Case X_TOKEN_SEMICOLON
				Exit
			End Select
		Wend
	End Method
	
	Method ParseArray(stream:TStream, member:TXMember)
		member.IsArray = True
		While True
			Select ParseToken(stream)
			Case X_TOKEN_NAME
				member.Name = TXToken.GetName(stream)
			Case X_TOKEN_OBRACKET
				ParseDimension(stream, member)
			Case X_TOKEN_SEMICOLON
				Exit
			End Select
		Wend
	End Method
	
	Method ParseDimension(stream:TStream, member:TXMember)
		While True
			Select ParseToken(stream)
			Case X_TOKEN_INTEGER
				member.ArraySize = TXToken.GetInt(stream)
			Case X_TOKEN_NAME
				member.Name = TXToken.GetName(stream)
			Case X_TOKEN_CBRACKET
				Exit
			Default
				Error
			End Select
		Wend
	End Method
		
	Method ParseTemplateReference(stream:TStream, member:TXMember)
		member.Name = TXToken.GetName(stream)
		While True
			Select ParseToken(stream)
			Case X_TOKEN_SEMICOLON
				Exit
			End Select
		Wend		
	End Method
	
	Method ParseEntry:TXEntry(stream:TStream)
		Local entry:TXEntry=New TXEntry
		entry.TemplateName=TXToken.GetName(stream)
	
		While True
			Select ParseToken(stream)
			Case X_TOKEN_NAME
				entry.Name=TXToken.GetName(stream)
			Case X_TOKEN_OBRACE
				Exit
			Default
				Error
			End Select
		Wend		
				
		While True
			Local data:TXData
			Select ParseToken(stream)
			Case X_TOKEN_INTEGER
				data=TXData.FromInt(ReadInt(stream))		
			Case X_TOKEN_INTEGER_LIST
				data=TXData.FromInts(TXToken.GetInts(stream))		
			Case X_TOKEN_FLOAT_LIST
				If Precision
					data=TXData.FromDoubles(TXToken.GetDoubles(stream))		
				Else
					data=TXData.FromFloats(TXToken.GetFloats(stream))		
				EndIf
			Case X_TOKEN_STRING
				data=TXData.FromString(TXToken.GetString(stream))
			Case X_TOKEN_NAME
				entry.AddChild ParseEntry(stream)
			Case X_TOKEN_OBRACE
				ParseToken(stream)
				data=TXData.FromString(TXToken.GetString(stream))
			Case X_TOKEN_CBRACE
				Exit
			Default
				Error
			End Select			
			If data
				entry.AddData data			
				data = Null
			EndIf
		Wend
		Return entry
	End Method

	Method DumpInfo()
		DebugLog "Templates:"
		For Local template:TXTemplate=EachIn Templates
			template.DumpInfo()
		Next
		Debuglog "Entries: "
		For Local entry:TXEntry=EachIn Entries
			entry.DumpInfo()
		Next
	End Method
	
	Method Error(msg$ = "Error")
		Throw msg
	End Method
End Type

Type TXGUID
	Field data0
	Field data1:Short
	Field data2:Short
	Field data3:Byte[8]
End Type

Type TXToken
	Function GetName$(stream:TStream)
		Local count=ReadInt(stream)
		Return ReadString(stream,count)
	End Function
	
	Function GetString$(stream:TStream)
		Local count=ReadInt(stream)
		Local s$=ReadString(stream,count)
		Local term=ReadShort(stream)
		Return s
	End Function
	
	Function GetInt(stream:TStream)
		Return ReadInt(stream)
	End Function
	
	Function GetGUID:TXGUID(stream:TStream)
		Local guid:TXGUID=New TXGUID
		stream.ReadBytes Varptr guid.data0,4*2
		stream.ReadBytes guid.data3,8
		Return guid
	End Function
	
	Function GetFloats#[](stream:TStream)
		Local count=ReadInt(stream),f#[count]
		stream.ReadBytes f,4*count
		Return f
	End Function
	
	Function GetDoubles![](stream:TStream)
		Local count=ReadInt(stream),d![count]
		stream.ReadBytes d,8*count
		Return d
	End Function
	
	Function GetInts[](stream:TStream)
		Local count=ReadInt(stream),i[count]
		stream.ReadBytes i,4*count
		Return i
	End Function
End Type

Type TXTemplate
	Field Name$, GUID:TXGUID
	Field Members:TXMember[]
	
	Method DumpInfo(level=0)
		DebugLog Name
		For Local member:TXMember=EachIn Members
			DebugLog " "+member.Name
		Next
	End Method
End Type

Type TXMember
	Field Name$,DataType
	Field IsArray,ArraySize
End Type

Const X_DATA_INT = 1
Const X_DATA_INT_LIST = 2
Const X_DATA_FLOAT = 3
Const X_DATA_FLOAT_LIST = 4
Const X_DATA_DOUBLE = 5
Const X_DATA_DOUBLE_LIST = 6
Const X_DATA_STRING = 7

Type TXData
	Field ContentType,ContentCount
	Field Contents:Byte[]
	
	Method Init(typ,size,count,p:Byte Ptr)
		ContentType=typ
		ContentCount=count
		Contents=New Byte[ContentCount*size]
		MemCopy Contents,p,ContentCount*size
	End Method

	Function FromInt:TXData(i)
		Local data:TXData=New TXData
		data.Init X_DATA_INT,4,1,Varptr i
		Return data
	End Function
	
	Function FromInts:TXData(i[])
		Local data:TXData=New TXData
		data.Init X_DATA_INT_LIST,4,i.length,i
		Return data
	End Function
	
	Function FromFloat:TXData(f#)
		Local data:TXData=New TXData
		data.Init X_DATA_FLOAT,4,1,Varptr f
		Return data
	End Function
	
	Function FromFloats:TXData(f#[])
		Local data:TXData=New TXData
		data.Init X_DATA_FLOAT_LIST,4,f.length,f
		Return data
	End Function
	
	Function FromDouble:TXData(d!)
		Local data:TXData=New TXData
		data.Init X_DATA_DOUBLE,8,1,Varptr d
		Return data
	End Function
	
	Function FromDoubles:TXData(d![])
		Local data:TXData=New TXData
		data.Init X_DATA_DOUBLE_LIST,8,d.length,d
		Return data
	End Function
		
	Function FromString:TXData(s$)
		Local data:TXData=New TXData
		Local d:Byte Ptr=s.ToCString()
		data.Init X_DATA_STRING,4,s.length,d
		MemFree d
		Return data
	End Function		
	
	Method ToInt()
		Return Int Ptr(Contents[0])[0]
	End Method

	Method ToIntArray[]()
		Local array[ContentCount]
		MemCopy array,Contents,ContentCount*4
		Return array
	End Method
	
	Method ToFloat#()
		Return Float Ptr(Contents[0])[0]
	End Method

	Method ToFloatArray#[]()
		Local array#[ContentCount]
		MemCopy array,Contents,ContentCount*4
		Return array
	End Method
	
	Method ToDouble!()
		Return Double Ptr(Contents[0])[0]
	End Method
	
	Method ToString$()
		Return String.FromCString(Contents)
	End Method
End Type

Type TXEntry
	Field Name$,TemplateName$
	Field Template:TXTemplate
	Field Data:TXData[]
	
	Field Children:TXEntry[]
	
	Method AddData(dat:TXData)
		Data:+[dat]
	End Method
	
	Method AddChild(child:TXEntry)
		Children:+[child]
	End Method
	
	Method DumpInfo(level=0)
		Local n$=Name,buf$=""[..level]
		If n="" n="<unnamed>"
		DebugLog buf+n+" ("+TemplateName+")"
		For Local child:TXEntry=EachIn Children
			child.DumpInfo level+1
		Next
	End Method
End Type
