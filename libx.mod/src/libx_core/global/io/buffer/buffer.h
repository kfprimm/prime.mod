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
/*!	\file global/io/buffer/buffer.h
	\brief Definition von x::Buffer
	\version 0.1
	\date 23.4.2003
	\author Christian Oberholzer

	Definition von x::Buffer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_BUFFER_BUFFER_H
#define GLOBAL_IO_BUFFER_BUFFER_H

#include "../../global.h"

namespace x {

/*!	\brief Buffer zum Lesen und Schreiben von Elementen
	\date 28.10.2003 implementiert
	\version 0.2
	\author Christian Oberholzer

	Diese Klasse ist die zusammenfassende Klasse der Input/Output-Datenstrukturen der Kaos Engine.
*/
class Buffer
{
public:
	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		
		Konstruktor
	*/
	Buffer() 
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Destruktor

		Destruktor
	*/
	virtual ~Buffer()
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Beschreibung 
		\return Beschreibung des Buffers

		Gibt eine Beschreibung f?r den Benutzer sinnvolle Beschreibung (und wenns nur der Klassenname) des
		abgeleiteten Buffers zur?ck.
	*/
	virtual const string_t& GetDescription() const = 0;
};

} // namespace x

#endif // GLOBAL_IO_BUFFER_BUFFER_H
