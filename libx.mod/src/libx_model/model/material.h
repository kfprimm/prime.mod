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
/*!	\file model/material.h
	\brief Definition von x::model::Material
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Material
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_MATERIAL_H
#define MODEL_MATERIAL_H

#include "../../libx_core/global/global.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
struct TextureSampler
{
	TextureSampler()
	:	texture()
	{
	}
	TextureSampler(const TextureSampler& unit)
	:	texture(unit.texture)
	{
	}
	~TextureSampler()
	{
	}
	const TextureSampler& operator = (const TextureSampler& unit)
	{
		texture = unit.texture;
		return (*this);
	}
	string_t texture;
};




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Material-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Material-Interface. Definiert das Interface über welches Kaos-Geometryes implementiert 
	werden.
*/
class Material
{
public:
	typedef vector<TextureSampler> texture_samplers_t;
	typedef texture_samplers_t::iterator texture_samplers_it_t;
	typedef texture_samplers_t::const_iterator texture_samplers_const_it_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Material();
	virtual ~Material();

	virtual void AddTextureSampler(const TextureSampler& unit);
	virtual const uint_t GetTextureSamplerCount() const;
	virtual TextureSampler& GetTextureSampler(const uint_t index);
	virtual const TextureSampler& GetTextureSampler(const uint_t index) const;

	virtual void SetAmbientColor(const math::Colorf& color);
	virtual void SetDiffuseColor(const math::Colorf& color);
	virtual void SetSpecularColor(const math::Colorf& color);
	virtual void SetEmissiveColor(const math::Colorf& color);
	virtual void SetPower(const float_t power);

	virtual const math::Colorf& GetAmbientColor() const;
	virtual const math::Colorf& GetDiffuseColor() const;
	virtual const math::Colorf& GetSpecularColor() const;
	virtual const math::Colorf& GetEmissiveColor() const;
	virtual const float_t GetPower() const;

protected:
	texture_samplers_t texture_samplers_;

	math::Colorf ambient_color_;
	math::Colorf diffuse_color_;
	math::Colorf specular_color_;
	math::Colorf emissive_color_;
	float_t power_;
};

}} // namespace x::model

#endif // MODEL_MATERIAL_H
