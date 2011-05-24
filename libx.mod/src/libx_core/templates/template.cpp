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
/*!	\file templates/template.cpp
	\brief Implementation von x::templates::Template
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Template
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "./templates.h"

namespace x { namespace templates {

using namespace std;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Template::Template(TemplateTypes Type)
:	type_(Type),
	ref_count_(1)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Template::~Template()
{
}




//-----------------------------------------------------------------------------------------------------------
void Template::AddRef()
{
	++ref_count_;
}




//-----------------------------------------------------------------------------------------------------------
void Template::RemRef()
{
	--ref_count_;
	if (ref_count_ == 0)
	{
		OnZeroRef();
		delete this;
	}
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Template::GetRefCount() const
{
	return ref_count_;
}




//-----------------------------------------------------------------------------------------------------------
void Template::OnZeroRef()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Typ des Templates zurückgeben
	\return Typ

	Gibt den Typ dieses Templates zurück.
*/
const TemplateTypes Template::GetType() const
{
	return type_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Einrücken ausgeben
	\param level

	Als Hilfsfunktion für die Ausgabe der Dateistruktur gedacht, gibt die
	Einrückung aus.
*/
void Template::PrintIndent(
	const uint_t level
) const
{
	/*for (uint_t i = 0; i < level; ++i)
		cout << "  ";
	cout << "+-- ";*/
}

}} // namespace x::templates

