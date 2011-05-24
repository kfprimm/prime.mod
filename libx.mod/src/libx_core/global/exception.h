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
/*!	\file global/exception.h
	\brief Definition von x::Exception
	\version 0.15.10.2002
	\author Christian Oberholzer

	Definition von x::Exception
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef GLOBAL_EXCEPTION_H
#define GLOBAL_EXCEPTION_H

#include <typeinfo>

namespace x { 

/*!	\brief Basisklasse aller Exceptions
	\version 0.1
	\date 30.10.2003 implementiert
	\author Christian Oberholzer

	Dies ist die Basisklasse der LibX
*/
class Exception
{
public:
	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Konstruktor
		\param strDescription Beschreibung

		Konstruktor
	*/
	Exception(const string_t& strDescription = UNICODE_STRING(""))
	:	m_strDescription(strDescription)
	{
	}




	//-----------------------------------------------------------------------------------------------------------
	/*!	\brief Copy-Konstruktor

		Konstruktor
	*/
	Exception(const Exception& Rhs)
	:	m_strDescription(Rhs.m_strDescription)
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Destruktor

		Virtueller Destruktor, damit die Klasse per typeid identifizierbar wird.
	*/
	virtual ~Exception()
	{
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Name der Exception zurückgeben
		\return Name

		Gibt den Namen der Exception zurück.
	*/
	string_t GetName() const
	{
		return typeid(*this).name();
	}




	//-------------------------------------------------------------------------------------------------------
	/*!	\brief Beschreibung zurückgeben
		\return Beschreibung

		Gibt die Beschreibung des Fehlers zurück, welche dieser Exception mitgegeben worden ist.
	*/
	const string_t& GetDescription() const
	{
		return m_strDescription;
	}




private:
	Exception& operator = (const Exception&);

protected:
	/*!
		\var m_strDescription
		\brief Beschreibung

		Enthält eine kurze dem Konstruktor übergebene Beschreibung der 
		Exception.
	*/

	string_t m_strDescription;
};

}; // namespace x

#endif // GLOBAL_EXCEPTION_H
