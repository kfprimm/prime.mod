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
/*!	\file global/io/buffer/inputbuffer.h
	\brief Definition von x::InputBuffer
	\version 0.1
	\date 23.4.2003
	\author Christian Oberholzer

	Definition von x::InputBuffer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_IO_BUFFER_INPUTBUFFER_H
#define GLOBAL_IO_BUFFER_INPUTBUFFER_H

#include "../../global.h"
#include "./buffer.h"

namespace x {

/*!	\brief Buffer zum Lesen von Elementen
	\version 0.1
	\date 28.10.2003 implementiert
	\author Christian Oberholzer

	Von dieser Basisklasse leiten alle Buffer der Kaos-Engine ab, welche zum Lesen von Daten 
	verwendet werden. Diese Klasse stellt bereits einige Grundfunktionalit?ten zur Verf?gung.
*/
class InputBuffer : public Buffer
{
public:
	/*
	 *	Konstruktor / Destruktor
	 */
	InputBuffer();
	virtual ~InputBuffer();

	/*
	 *	Daten lesen
	 */
	virtual void Read(ubyte_t* const pBuffer, const uint_t uiNum) = 0;
	virtual const ubyte_t* GetContent() const = 0;

	/*
	 *	Position ?ndern
	 */
	virtual const uint_t GetPosition() const;
	virtual void SetPosition(const uint_t uiNewPosition);

	/*
	 *	Stack-Operationen
	 */
	virtual void PushPosition();
	virtual void PopPosition();
	virtual void DiscardPosition();

	/*
	 *	Abfragen
	 */
	virtual const bool Eof() const = 0;
	virtual const string_t& GetDescription() const = 0;

protected:
	/*!
		\var m_uiPosition
		\brief Aktuelle Position

		Diese Variabel speichert die Position an welcher sich der Benutzer momentan im Stream befindet.
	*//*!
		\var m_PositionStack
		\brief Stack auf welchem die Positionen abgelegt werden k?nnen

		Die aktuelle Position kann ?ber die Methoden PushPosition/PopPosition/DiscardPosition mit dem
		Stack verwaltet werden, so dass man schnell von einem Ort im Stream zu einem anderen Ort 
		springen kann. Dieser Stack speichert diese Werte
	*/

	uint_t m_uiPosition;
	vector<uint_t> m_PositionStack;
};

} // namespace x

#endif // GLOBAL_IO_BUFFER_INPUTBUFFER_H
