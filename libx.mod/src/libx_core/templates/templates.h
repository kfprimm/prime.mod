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
/*!	\file templates/templates.h
	\brief Definition von  x::templates-Konstanten
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von  x::templates-Konstanten
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_TEMPLATES_H
#define TEMPLATES_TEMPLATES_H

#include "../global/global.h"
#include "../scanner/scanner.h"
#include "../temporalinfo.h"
#include "../guid.h"

namespace x { namespace templates {

enum TemplateTypes 
{
	TemplateTypeAnimation,
	TemplateTypeAnimationKey,
	TemplateTypeAnimationOptions,
	TemplateTypeAnimationSet,
	TemplateTypeAnimTicksPerSecond,
	TemplateTypeBoolean,
	TemplateTypeBoolean2D,
	TemplateTypeColorRGB,
	TemplateTypeColorRGBA,
	TemplateTypeCoords2D,
	TemplateTypeDeclData,
	TemplateTypeEffectDWord,
	TemplateTypeEffectFloats,
	TemplateTypeEffectInstance,
	TemplateTypeEffectParamDWord,
	TemplateTypeEffectParamFloats,
	TemplateTypeEffectParamSTring,
	TemplateTypeEffectString,
	TemplateTypeFaceAdjacency,
	TemplateTypeFloatKeys,
	TemplateTypeFrame,
	TemplateTypeFrameTransformMatrix,
	TemplateTypeFVFData,
	TemplateTypeHeader,
	TemplateTypeIndexedColor,
	TemplateTypeMaterial,
	TemplateTypeMaterialWrap,
	TemplateTypeMatrix4x4,
	TemplateTypeMesh,
	TemplateTypeMeshFace,
	TemplateTypeMeshFaceWraps,
	TemplateTypeMeshMaterialList,
	TemplateTypeMeshNormals,
	TemplateTypeMeshTextureCoords,
	TemplateTypeMeshVertexColors,
	TemplateTypePatch,
	TemplateTypePatchMesh,
	TemplateTypePatchMesh9,
	TemplateTypePMAttributeRange,
	TemplateTypePMInfo,
	TemplateTypePMVSplitRecord,
	TemplateTypeSkinWeights,
	TemplateTypeTextureFilename,
	TemplateTypeTimedFloatKeys,
	TemplateTypeVector,
	TemplateTypeVertexDuplicationIndices,
	TemplateTypeVertexElement,
	TemplateTypeXSkinMeshHeader,
	TemplateTypeGeneric // Eigentlich kein Template, d.h. einfach alles was nicht definiert ist...
};

extern const Guid g_guids[];

}} // namespace x::templates

#include "./template.h"
#include "./animation.h"
#include "./animationkey.h"
#include "./animationset.h"
#include "./animtickspersecond.h"
#include "./boolean.h"
#include "./boolean2d.h"
#include "./colorrgb.h"
#include "./colorrgba.h"
#include "./coords2d.h"
#include "./decldata.h"
#include "./effectdword.h"
#include "./effectfloats.h"
#include "./effectinstance.h"
#include "./effectparamdword.h"
#include "./effectparamfloats.h"
#include "./effectparamstring.h"
#include "./effectstring.h"
#include "./floatkeys.h"
#include "./frame.h"
#include "./frametransformmatrix.h"
#include "./fvfdata.h"
#include "./indexedcolor.h"
#include "./material.h"
#include "./materialwrap.h"
#include "./matrix4x4.h"
#include "./mesh.h"
#include "./meshface.h"
#include "./meshfacewraps.h"
#include "./meshmateriallist.h"
#include "./meshnormals.h"
#include "./meshtexturecoords.h"
#include "./meshvertexcolors.h"
#include "./skinweights.h"
#include "./texturefilename.h"
#include "./vector.h"
#include "./vertexduplicationindices.h"
#include "./vertexelement.h"
#include "./xskinmeshheader.h"

// additional Templates
#include "./headertemplate.h"

#endif // TEMPLATES_TEMPLATES_H
