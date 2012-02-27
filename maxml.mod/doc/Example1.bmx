'MaXML 2 Example 1

'Note: These demos do not demonstrate all of MaXML's features; only very basic features.
'Please refer to the MaXML 2.0 documentation for a complete list of methods and functions.

Import pub.MaXML

Strict

'Misc. variables
Local Count:Int
Local miscinfo:xmlNode
Local infonode:xmlNode
Local title:String
Local author:String
Local description:String

'Create a new XML document object and load data from the example.xml file.
Local doc1:xmlDocument = New xmlDocument

'Load "example1.xml" into doc1
doc1.Load "example1.xml"

'Find the node that contains the information we want.
infonode = doc1.Root().FindChild("info")

'Gat the value of the "title" node.
title = infonode.FindChild("title").Value

'Get the value of the "author" node.
author = infonode.FindChild("author").Value

'Get the value of the "description" node.
description = infonode.FindChild("description").Value
Print "~n________________________________________________"

'Using MaXML's easy object-oriented interface, retrieving data from any node can be done in a single expression:
Print "~nFile type: " + doc1.Root().Name

'Display the values in title, author, and description
Print "~nTitle: "+title+"~nAuthor: "+author+"~nDescription:~n    "+description

'-------------------------------------------

'Now, find a node named "miscinfo"
miscinfo = doc1.Root().FindChild("miscinfo")

'If the node wan't found, then create one
If miscinfo = Null Then miscinfo = doc1.Root().AddNode("miscinfo", AS_CHILD)	'Note: The second argument is optional, and defaults to AS_CHILD

'Get the count attribute from the node
Count = Int( miscinfo.Attribute("count").Value )

'Incriment the count
Count = Count + 1
                               
'Store the new count value in the miscinfo's "count" attribute
miscinfo.Attribute("count").Value = Count

Print "Count is now: " + Count + " (incrimented and saved to file)"

'Write the file out to "example.xml", overwriting the original file.
doc1.Save "example1.xml", FORMAT_XML

'"Close" the xml file by freeing the xml document
doc1 = Null

GCCollect

End
