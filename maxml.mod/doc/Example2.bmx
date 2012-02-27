'MaXML 2 Example 2

'Note: These demos do not demonstrate all of MaXML's features; only very basic features.
'Please refer to the MaXML 2.0 documentation for a complete list of methods and functions.

Import pub.MaXML

Local node:xmlNode, root:xmlNode
Local name:String
Local foundSomething = False


'First, load the XMl file. The xmlDocument.Create() function allows you to specify a filename for quick and easy XML file loading:
Local XMLFile:xmlDocument = xmlDocument.Create("example2.xml")	

'Get the root node
root = XMLFile.Root()

'Sort all of the root's children alphabetically by their attribute values (which in this case is the name of the contacts)
root.SortChildren(SORTBY_ATTR_VALUE)

'Display some data provided by the root node
Print
Print root.Attribute("title").Value
Print
																
'Loop through all the nodes named "person" under the root
node = root.FirstChild()	'First, get the first child of the root...
While node <> Null
	'... process it
	If node.Name = "person" Then
		Print node.Attribute("firstname").Value	'Print out the person's name attribute
	End If
	
	'... and go on to the next node
	node = node.NextSibling()	'Get the next node
Wend

'Ask the user for one of the names
Print
Print "Enter a name shown above:"
name = Input()

'Find the person the user typed by iterating through all the nodes under the root
node = root.FirstChild()	'First, get the first child of the root...
While node <> Null
	'... process it
	If node.Name = "person" Then
		'If this is the person the user typed, show more information
		If node.Attribute("firstname").Value.ToLower() = name.ToLower() Then
			Print
			Print "----- " + node.Attribute("firstname").Value + " " + node.Attribute("lastname").Value + " -----"
			Print "Phone Number: " + node.FindChild("phone").Value
			foundSomething = True
		End If
	End If
	
	'... and go on to the next node
	node = node.NextSibling()	'Get the next node
Wend

If Not foundSomething Then Print "Not Found."

End


