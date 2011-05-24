//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *
 *	It is part of LibX, which is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 *	
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	Lesser General Public License for more details.
 *	
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
/*!	\file temporalinfo.h
	\brief Definition von x::TemporalInfo
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TemporalInfo
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPORALINFO_H
#define TEMPORALINFO_H

#include "./global/global.h"
#include "./header.h"

namespace x {

class Serializer;

namespace scanner {
	class Scanner;
}
namespace templates {
	class Template;
}
namespace tokens {
	class Token;
}

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Temporäre Ladeinformationen
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Speichert Informationen über ein .x-File welche während dem Ladeprozess benötigt 
	werden, wie z.B. den Fileheader (32- oder 64-Bit Floats, etc)
*/
class TemporalInfo
{
public:
	typedef map<string_t, templates::Template*> templates_map_t;
	typedef vector<templates::Template*> instances_container_t;
	typedef pair<string_t, templates::Template*> templates_value_t;
	typedef vector<pair<uint_t, uint_t> > position_stack_t;

public:
	enum FileModes {
		FileModeText,
		FileModeBinary,
		FileModeCompressedText,
		FileModeCompressedBinary
	};

	enum {
		TemporalBufferSize = 2048
	};

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	TemporalInfo(
		Serializer& serializer_,
		BufferInputStream& input_,
		instances_container_t& instances_
	);
	~TemporalInfo();

	//-------------------------------------------------------------------------------------------------------

public:
	/*!
		\var serializer
		\brief Referenz auf den X-Serializer
		
		Referenz auf den X-Serializer von welchem aus geladen wird
	*//*!
		\var header
		\brief File-Header

		X-File-Header, enthält ein zwei globale Einstellungen
	*//*!
		\var mode
		\brief file-type/mode
		
		Shows if we're reading a text or a binary file.
	*//*!
		\var input
		\brief Input-Buffer

		Referenz auf den Inputstream.
	*//*!
		\var buffer
		\brief temporal input-buffer
		
		buffer to store input while parsing in text-mode.
	*//*!
		\var buffer_index
		\brief position in szBuffer
		
		stores the current position in szBuffer.
	*//*!
		\var line
		\brief actual line-number in input-file
		
		this member contains the number of the line where we're currently parsing. only 
		valid in text-mode.
	*//*!
		\var templates
		\brief Definierte Templates

		Alle für dieses X-File definierten Templates
	*//*!
		\var instances
		\brief Root-Instanzen von Templates

		Root-Instanzen von Templates
	*/
	
	Serializer& serializer;
	Header header;
	scanner::Scanner* scanner;
	FileModes mode;
	BufferInputStream& input;
	schar_t* buffer;
	schar_t lookahead;
	uint_t buffer_index;
	uint_t buffer_size;
	uint_t line;
	templates_map_t templates;
	instances_container_t& instances;
	templates_map_t named_instances;

	/*!
		\var last_token
		\brief Zuletzt gelesenes Token
		
		Das zuletzt gelesene Token welches ev. noch relevante Informationen
		enthält
	*//*!
		\var last_token_index
		\brief Index wenn LastToken eine Liste ist

		Index auf das aktuelle Element in LastToken wenn LastToken eine List evon Integers oder
		eine Liste von Floats ist
	*/

	tokens::Token* last_token;
	udword_t last_token_index;
};

} // namespace x

#endif // TEMPORALINFO_H
