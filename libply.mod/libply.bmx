
Strict

Module sys87.libply
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream

Const PLY_ASCII     = 1
Const PLY_BINARY_BE = 2
Const PLY_BINARY_LE = 3

Const PLY_OKAY  =  0
Const PLY_ERROR = -1

Const PLY_START_TYPE = 0
Const PLY_CHAR       = 1
Const PLY_SHORT      = 2
Const PLY_INT        = 3
Const PLY_UCHAR      = 4
Const PLY_USHORT     = 5
Const PLY_UINT       = 6
Const PLY_FLOAT      = 7
Const PLY_DOUBLE     = 8
Const PLY_END_TYPE   = 9

Const PLY_SCALAR = 0
Const PLY_LIST   = 1

Global PLY_TYPES_ID[] = [PLY_CHAR,PLY_SHORT,PLY_INT,PLY_UCHAR,PLY_USHORT,PLY_UINT,PLY_FLOAT,PLY_DOUBLE]
Global PLY_TYPES_STR$[] = ["char","short","int","uchar","ushort","uint","float","double"]

Function PlyTypeByID$(id)
	For Local i=0 To PLY_TYPES_ID.length-1
		If PLY_TYPES_ID[i]=id Return PLY_TYPES_STR[i]
	Next
End Function

Function PlyTypeByStr(str$)
	For Local i=0 To PLY_TYPES_STR.length-1
		If PLY_TYPES_STR[i]=str Return PLY_TYPES_ID[i]
	Next
End Function

Function PlySplit$[](str$)
	Local res$[],instring=False
	For Local i=0 To str.length-1
		If str[i]=" "[0] Or str[i]="~t"[0] instring=False;Continue
		If Not instring res=res[..res.length+1]
		res[res.length-1]:+Chr(str[i])
		instring=True
	Next
	Return res
End Function

Type TPlyFile
	Field version$
	Field elements:TPlyElement[]
	
	Function Read:TPlyFile(url:Object)
		Local stream:TStream=TStream(url),owns_stream
		If stream=Null stream=ReadStream(url);owns_stream=True
		If stream=Null Return Null
		
		If ReadLine(stream)<>"ply"
			If owns_stream CloseStream stream
			Return Null
		EndIf
		
		Local file:TPlyFile=New TPlyFile
		Local element:TPlyElement
		While True
			Local line$=ReadLine(stream),i$[]=PlySplit(line)
			Select i[0] 
			Case "format"
				If i[1]<>"ascii"
					If owns_stream CloseStream stream
					Return Null
				EndIf
				file.version=" ".Join(i[2..])
			Case "element"
				element=file.AddElement(i[1],Int(i[2]))
			Case "property"
				Local is_list,list_type
				If i[1]="list"
					is_list=True
					list_type=PlyTypeByStr(i[2])
				EndIf
				Local property:TPlyProperty=element.AddProperty(i[i.length-1],PlyTypeByStr(i[i.length-2]),element.count,is_list,list_type)
			Case "end_header"
				Exit
			Case "comment"				
			Default
				DebugLog "Unrecognized header line: ~q"+line+"~q"
			End Select
		Wend
		
		For Local element:TPlyElement=EachIn file.elements
			For Local i=0 To element.count-1
				Local line$=ReadLine(stream),split$[]=PlySplit(line)
				For Local j=0 To element.properties.length-1
					Local prop:TPlyProperty=element.properties[j]
					If prop.is_list
						prop.AddData i,line
					Else
						prop.AddData i,split[j]
					EndIf
				Next
			Next
		Next
		
		If owns_stream CloseStream stream
		
		Return file
	End Function
		
	Method AddElement:TPlyElement(name$,count)
		Local elem:TPlyElement=New TPlyElement
		elem.name=name
		elem.count=count
		elements:+[elem]
		Return elem
	End Method
	
	Method FindElement:TPlyElement(name$)
		For Local elem:TPlyElement=EachIn elements
			If elem.name=name Return elem
		Next
	End Method
End Type

Type TPlyElement
	Field name$,count,type_
	Field properties:TPlyProperty[]
	
	Method AddProperty:TPlyProperty(name$,type_,count,is_list=False,list_type=0)
		Local property:TPlyProperty=New TPlyProperty
		property.name=name
		property.type_=type_
		property.is_list=is_list
		property.list_type=list_type
		property.data=New String[count]
		If property.is_list property.list=New String[][count]
		properties:+[property]
		Return property
	End Method
	
	Method FindProperty:TPlyProperty(name$)
		For Local prop:TPlyProperty=EachIn properties
			If prop.name=name Return prop
		Next
	End Method
End Type

Type TPlyProperty
	Field name$,type_
	Field is_list,list_type
	Field data$[],list$[][]
	
	Method AddData(index,str$)
		data[index]=str
		If is_list
			Local split$[]=PlySplit(str)
			list[index]=split[1..Min(split.length,Int(split[0])+1)]
		EndIf
	End Method
	
	Method ListCount(index)
		Return list[index].length
	End Method
	
	Method ListAsInt[](index)
		Local array[list[index].length]
		For Local i=0 To list[index].length-1
			array[i]=Int(list[index][i])
		Next
		Return array
	End Method
	
	Method AsFloat#(index)
		Return Float(data[index])
	End Method
End Type
