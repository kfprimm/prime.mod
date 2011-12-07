
Strict

Module Prime.CSV
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream

Type TCSV
	Field _data$[][],_header=-1
	
	Function Read:TCSV(url:Object,delim$=",")
		Local csv:TCSV=New TCSV
		Local stream:TStream=ReadStream(url)
		While Not Eof(stream)
			csv.AddData ParseLine(ReadLine(stream),delim)
		Wend
		Return csv
	End Function
	
	Method SetHeader(index)
		_header=index
	End Method
	
	Method Count()
		Return _data.length-(_header>-1)
	End Method
	
	Method Sort(fld)
		Local swapped
		Repeat
			For Local i=0 To _data.length-1
				'If 
			Next
		Until swapped
	End Method
	
	Method AddData(data$[])
		_data:+[data]
	End Method
	
	Function ParseLine$[](line$,delim$)
		Local str$,p,toke$[],in_str
		While p<line.length
			If line[p]="~q"[0]
				in_str=Not in_str
			Else
				If line[p]=delim[0] And Not in_str
					toke:+[str]
					str=""
				Else
					str:+Chr(line[p])
				EndIf
			EndIf
			p:+1
		Wend
		toke:+[str]
		Return toke
	End Function
	
	Method ObjectEnumerator:TCSVEnumerator()
		Return New TCSVEnumerator.Create(Self)
	End Method
End Type

Type TCSVEnumerator
	Field _data$[][],pos=-1
	Field _header
	
	Method Create:TCSVEnumerator(csv:TCSV)
		_data=csv._data
		_header=csv._header
		Return Self
	End Method
	
	Method HasNext:Int()
		If _header=pos And pos=_data.length-1 Return False
		Return pos<_data.length-1
	End Method
	
	Method NextObject:Object()
		pos:+1
		If pos=_header pos:+1
		Return _data[pos]
	End Method
End Type
