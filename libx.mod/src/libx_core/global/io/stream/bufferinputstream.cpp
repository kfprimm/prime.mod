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
/*!	\file global/io/stream/bufferinputstream.cpp
	\brief Implementation von x::BufferInputStream
	\version 0.1
	\date 1.8.2001
	\author Christian Oberholzer

	Implementation von x::BufferInputStream
*/
//-----------------------------------------------------------------------------------------------------------
#define K_KERNEL
#include "../io.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!	\brief Konstruktor

	Konstruktor
*/
BufferInputStream::BufferInputStream()
:	m_pInput(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Destruktor

	Destruktor
*/
BufferInputStream::~BufferInputStream()
{
	delete m_pInput;
}




//-----------------------------------------------------------------------------------------------------------
void BufferInputStream::SetBuffer(InputBuffer* pBuffer)
{
	delete m_pInput;
	m_pInput = pBuffer;
}




//-----------------------------------------------------------------------------------------------------------
InputBuffer* BufferInputStream::GetBuffer()
{
	return m_pInput;
}




//-----------------------------------------------------------------------------------------------------------
InputBuffer* BufferInputStream::ResetBuffer()
{
	InputBuffer* pBuffer = m_pInput;
	m_pInput = NULL;
	return pBuffer;
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(bool_t& b)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&b), sizeof(bool_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(schar_t& c)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&c), sizeof(schar_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(wchar_t& c)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&c), sizeof(swchar_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(signed_char_t& c)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&c), sizeof(signed_char_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(unsigned_char_t& c)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&c), sizeof(unsigned_char_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(signed_short_t& s)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&s), sizeof(signed_short_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(unsigned_short_t& s)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&s), sizeof(unsigned_short_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(signed_int_t& i)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&i), sizeof(signed_int_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(unsigned_int_t& i)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&i), sizeof(unsigned_int_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(signed_long_t& l)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&l), sizeof(signed_long_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(unsigned_long_t& l)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&l), sizeof(unsigned_long_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(sfloat_t& f)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&f), sizeof(sfloat_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::Read(sdouble_t& d)
{
	_TestBufferAvailable();
	m_pInput->Read(reinterpret_cast<ubyte_t*>(&d), sizeof(sdouble_t) / sizeof(ubyte_t));
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Bytes aus dem Buffer lesen
	\param pBuffer
		Buffer in welchen die Bytes geschrieben werden sollen
	\param uiLength
		Anzahl zu lesende Bytes

	Liest die angegebene Anzahl Bytes aus dem Buffer dieses Streams und schreibt sie in den 
	übergebenen Byte-Buffer.
*/
void BufferInputStream::Read(ubyte_t* const pBuffer, const uint_t uiLength)
{
	_TestBufferAvailable();
	m_pInput->Read(pBuffer, uiLength);
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::SetPosition(const uint_t uiPosition)
{
	_TestBufferAvailable();
	m_pInput->SetPosition(uiPosition);
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
const uint_t BufferInputStream::GetPosition() const
{
	_TestBufferAvailable();
	return m_pInput->GetPosition();
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::PushPosition()
{
	_TestBufferAvailable();
	m_pInput->PushPosition();
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::PopPosition()
{
	_TestBufferAvailable();
	m_pInput->PopPosition();
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::DiscardPosition()
{
	_TestBufferAvailable();
	m_pInput->DiscardPosition();
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
const bool BufferInputStream::Eof() const
{
	_TestBufferAvailable();
	return m_pInput->Eof();
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
const string_t& BufferInputStream::GetDescription() const
{
	static const string_t strDescription(UNICODE_STRING("BufferInputStream"));
	return strDescription;
}




//-----------------------------------------------------------------------------------------------------------
/*!
*/
void BufferInputStream::_TestBufferAvailable() const
{
	if (!m_pInput)
		throw NoBufferException(UNICODE_STRING("m_pInput ist = NULL!"));
}

} // namespace x
