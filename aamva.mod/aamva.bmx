
Strict

Rem
	bbdoc: AAMVA license data parser
End Rem
Module Prime.AAMVA
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import BRL.Stream
Import BRL.Map

Type TAAMVAHeader
	Field des$, rs$, st$
	Field jurisdiction$ 
	Field version 
	
	Field data:TAAMVAData
	
	Method Process:TAAMVAHeader(stream:TStream)
		If ReadString(stream, 1) <> "@" Return Null
		des = ReadString(stream, 1)
		rs = ReadString(stream, 1)
		st = ReadString(stream, 1)
		If ReadString(stream, 5) <> "ANSI " Return Null
		
		jurisdiction = ReadString(stream, 6)
		version = Int(ReadString(stream, 2))	
		
		Select version
		Case 1
			data = New TAAMVAData01.Process(Self, stream)
		Case 3
			data = New TAAMVAData03.Process(Self, stream)
		Default
			Return Null
		End Select	
			
		Return Self	
	End Method
End Type

Type TAAMVASubFile
	Field designator$, offset, length
	Field fields:TMap = New TMap
End Type

Type TAAMVAData
	Field subfiles:TMap = New TMap
	Field fields:TMap = New TMap
	
	Method Process:TAAMVAData(header:TAAMVAHeader, stream:TStream) Abstract

	Method ReadSubFiles(header:TAAMVAHeader, stream:TStream, count)
		For Local i = 0 To count - 1
			Local subfile:TAAMVASubFile = New TAAMVASubFile
			subfile.designator = ReadString(stream, 2)
			subfile.offset = Int(ReadString(stream, 4))
			subfile.length = Int(ReadString(stream, 4))
			
			subfiles.Insert subfile.designator, subfile
		Next
	End Method

	Method ReadElements(header:TAAMVAHeader, stream:TStream)
		Local sf$ = ReadString(stream, 2)		
		Try
			Repeat
				Local subfile:TAAMVASubFile = TAAMVASubFile(subfiles.ValueForKey(sf))
				
				Local tag$ = ReadString(stream, 3)
				While tag[0] <> header.st[0]
					
					Local value$ = "", c = 0
					While c <> header.des[0]
						If c <> 0 value :+ Chr(c)
						c = ReadByte(stream)
					Wend
					
					subfile.fields.Insert tag, value.Trim()
					fields.Insert tag, value.Trim()
					
					tag = ReadString(stream, 3)
				Wend
				sf$ = tag[1..]
			Forever 
		Catch ex:TStreamReadException
		End Try
	End Method	
	
	Method ValueFor$(key$)
		Return String(fields.ValueForKey(key))
	End Method
	
	Method Value$(key$) Abstract
End Type

Type TAAMVAData01 Extends TAAMVAData 	
	Method Process:TAAMVAData01(header:TAAMVAHeader, stream:TStream)
		ReadSubFiles(header, stream, Int(ReadString(stream, 2)))
		ReadElements(header, stream)
		Return Self
	End Method	
	
	Method Value$(key$)
		Select key
		Case "first_name"	
			Return ValueFor("DAA").Split(",")[1].Trim().Split(" ")[0].Trim()
		Case "middle_name"
			Return " ".Join(ValueFor("DAA").Split(",")[1].Trim().Split(" ")[1..]).Trim()
		Case "last_name"
			Return ValueFor("DAA").Split(",")[0]
		Case "address"
			Return ValueFor("DAG")
		Case "locality"
			Return ValueFor("DAI")
		Case "state"
			Return ValueFor("DAJ")
		Case "zip"
			Return ValueFor("DAK")
		Case "date_of_birth"
			Return ValueFor("DBB")
		Case "sex"
			If ValueFor("DBC") = "M" Return "MALE"
			If ValueFor("DBC") = "F" Return "FEMALE"
		End Select
	End Method
End Type

Type TAAMVAData03 Extends TAAMVAData
	Field jurisdiction_version
	
	Method Process:TAAMVAData03(header:TAAMVAHeader, stream:TStream)
		jurisdiction_version = Int(ReadString(stream, 2))
		ReadSubFiles(header, stream, Int(ReadString(stream, 2)))
		ReadElements(header, stream)
		Return Self
	End Method
	
	Method Value$(key$)
		Select key
		Case "first_name"	
			Return ValueFor("DCT").Split(",")[0].Trim()
		Case "middle_name"
			Return ValueFor("DCT").Split(",")[1].Trim()
		Case "last_name"
			Return ValueFor("DCS")
		Case "address"
			Return ValueFor("DAG")
		Case "locality"
			Return ValueFor("DAI")
		Case "state"
			Return ValueFor("DAJ")
		Case "zip"
			Return ValueFor("DAK")
		Case "date_of_birth"
			Return ValueFor("DBB")
		Case "sex"
			If ValueFor("DBC") = "1" Return "MALE"
			If ValueFor("DBC") = "0" Return "FEMALE"
		End Select
	End Method
End Type

