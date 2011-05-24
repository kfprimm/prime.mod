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
/*!	\file global/io/buffer/inputbuffer.cpp
	\brief Implementation von x::InputBuffer
	\version 0.1
	\date 30.8.2003
	\author Christian Oberholzer

	Implementation von x::InputBuffer
*/
//-----------------------------------------------------------------------------------------------------------
#define K_KERNEL
#include "../io.h"

namespace x { 

//-----------------------------------------------------------------------------------------------------------
/*!	\brief Konstruktor

	Einfacher Konstruktor
*/
InputBuffer::InputBuffer()
:	Buffer(),
	m_uiPosition(0),
	m_PositionStack()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Destruktor

	Destruktor
*/
InputBuffer::~InputBuffer()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Aktuelle Position zurückgeben
	\return Position

	Gibt die aktuelle Position von welcher aus im Stream gelesen wird zurück.
*/
const uint_t InputBuffer::GetPosition() const
{
	return m_uiPosition;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Position setzen
	\param uiNewPosition Neue Position

	Setzt die Position von welcher aus gelesen wird neu. Falls die Position hinter das Ende des Streams
	gesetzt werden soll, so wird eine io::OutOfBoundsException geworfen.
*/
void InputBuffer::SetPosition(const uint_t uiNewPosition)
{
	m_uiPosition = uiNewPosition;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Aktuelle Position auf dem Stack speichern

	Diese Methode speichert die aktuelle Position auf dem Stack, damit sie später wieder verwendet 
	werden kann.
*/
void InputBuffer::PushPosition()
{
	m_PositionStack.push_back(m_uiPosition);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Oberste Position auf dem Stack wiederherstellen
	
	Diese Methode stellt die zuoberst auf dem Stack gespeicherte Position (d.h. die zuletzt über PushPosition()
	gespeicherte Position wieder her. 
*/
void InputBuffer::PopPosition()
{
	try 
	{
		uint_t uiPosition = m_PositionStack.back();
		m_PositionStack.pop_back();
		SetPosition(uiPosition);
	}
	catch (EndOfBufferException&)
	{
		/*
		 *	Hierhin sollten wir nie gelangen, da eh nur validierte Positionen (d.h. solche im gültigen
		 *	Rahmen) auf den Stack gelangen können. Und solche gültigen Positionen können keine Exception
		 *	auslösen, falls der Buffer seine Grösse nicht ändert, was ebenfalls nicht geschehen dürfte.
		 */
		throw;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Oberste Position auf dem Stack verwerfen

	Diese Methode verwirft die zuoberst auf dem Stack gespeicherte Position
*/
void InputBuffer::DiscardPosition()
{
	m_PositionStack.pop_back();
}

} // namespace x
