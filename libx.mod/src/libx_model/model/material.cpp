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
/*!	\file model/material.cpp
	\brief Implementation von x::model::Material
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Material
*/
//-----------------------------------------------------------------------------------------------------------
#include "./material.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Material::Material()
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Material::~Material()
{
}




//-----------------------------------------------------------------------------------------------------------
void Material::AddTextureSampler(
	const TextureSampler& unit
)
{
	texture_samplers_.push_back(unit);
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Material::GetTextureSamplerCount() const
{
	return (uint_t)texture_samplers_.size();
}




//-----------------------------------------------------------------------------------------------------------
TextureSampler& Material::GetTextureSampler(
	const uint_t index
)
{
	return texture_samplers_[index];
}




//-----------------------------------------------------------------------------------------------------------
const TextureSampler& Material::GetTextureSampler(
	const uint_t index
) const
{
	return texture_samplers_[index];
}




//-----------------------------------------------------------------------------------------------------------
void Material::SetAmbientColor(
	const math::Colorf& color
)
{
	ambient_color_ = color;
}




//-----------------------------------------------------------------------------------------------------------
void Material::SetDiffuseColor(
	const math::Colorf& color
)
{
	diffuse_color_ = color;
}




//-----------------------------------------------------------------------------------------------------------
void Material::SetSpecularColor(
	const math::Colorf& color
)
{
	specular_color_ = color;
}




//-----------------------------------------------------------------------------------------------------------
void Material::SetEmissiveColor(
	const math::Colorf& color
)
{
	emissive_color_ = color;
}




//-----------------------------------------------------------------------------------------------------------
void Material::SetPower(
	const float_t power
)
{
	power_ = power;
}




//-----------------------------------------------------------------------------------------------------------
const math::Colorf& Material::GetAmbientColor() const
{
	return ambient_color_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Colorf& Material::GetDiffuseColor() const
{
	return diffuse_color_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Colorf& Material::GetSpecularColor() const
{
	return specular_color_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Colorf& Material::GetEmissiveColor() const
{
	return emissive_color_;
}




//-----------------------------------------------------------------------------------------------------------
const float_t Material::GetPower() const
{
	return power_;
}

}} // namespace x::model
