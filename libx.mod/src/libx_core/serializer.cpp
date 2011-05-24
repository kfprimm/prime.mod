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
/*!	\file serializer.cpp
	\brief Implementation von x::Serializer
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::Serializer
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./scanner/textscanner.h"
#include "./scanner/binaryscanner.h"
#include "./parseexception.h"
#include "./serializer.h"

#pragma warning(disable:4996)

namespace x {

using namespace x::scanner;
using namespace x::tokens;
using namespace x::templates;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Serializer::Serializer()
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Serializer::~Serializer()
{
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh importieren
	\param input
		Quelldaten
	\param instances

	Importiert den Mesh aus dem übergebenen Stream und speichert ihn in der übergebenen 
	Mesh-Struktur.
*/
void Serializer::Import(
	BufferInputStream& input,
	instances_t& instances
)
{
	TemporalInfo info(*this, input, instances);
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Animation"),					new Animation));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimationKey"),				new AnimationKey));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimationSet"),				new AnimationSet));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("AnimTicksPerSecond"),			new AnimTicksPerSecond));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Boolean"),					new Boolean));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Boolean2d"),					new Boolean2d));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("ColorRGB"),					new ColorRGB));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("ColorRGBA"),					new ColorRGBA));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Coords2d"),					new Coords2d));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("DeclData"),					new DeclData));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectDWord"),				new EffectDWord));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectFloats"),				new EffectFloats));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectInstance"),				new EffectInstance));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamDWord"),			new EffectParamDWord));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamFloats"),			new EffectParamFloats));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectParamString"),			new EffectParamString));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("EffectString"),				new EffectString));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FloatKeys"),					new FloatKeys));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Frame"),						new Frame));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FrameTransformMatrix"),		new FrameTransformMatrix));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("FVFData"),					new FVFData));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("IndexedColor"),				new IndexedColor));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Header"),						new HeaderTemplate));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Material"),					new Material));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MaterialWrap"),				new MaterialWrap));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Matrix4x4"),					new Matrix4x4));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Mesh"),						new Mesh));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshFace"),					new MeshFace));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshFaceWraps"),				new MeshFaceWraps));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshMaterialList"),			new MeshMaterialList));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshNormals"),				new MeshNormals));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshTextureCoords"),			new MeshTextureCoords));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("MeshVertexColors"),			new MeshVertexColors));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("SkinWeights"),				new SkinWeights));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("TextureFilename"),			new TextureFilename));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("TimedFloatKeys"),				new TimedFloatKeys));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("Vector"),						new Vector));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("VertexDuplicationIndices"),	new VertexDuplicationIndices));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("VertexElement"),				new VertexElement));
	info.templates.insert(TemporalInfo::templates_value_t(UNICODE_STRING("XSkinMeshHeader"),			new XSkinMeshHeader));

	info.header.Read(input);
	info.header.CheckMagic();

	try {
		ReadFile(info);
	}
	catch (Exception& e) {
		switch (info.mode)
		{
		case TemporalInfo::FileModeBinary:
			{
				uint_t position = info.input.GetPosition();
				char position_string[512];
				sprintf(position_string, "%d", position);
				throw ParseException(
					string_t(UNICODE_STRING("An error occured during parsing process at position ")) + 
					position_string + 
					UNICODE_STRING(", error description: '") + e.GetDescription() + UNICODE_STRING("'")
				);
			}
			break;

		case TemporalInfo::FileModeText:
			{
				char line_string[512];
				sprintf(line_string, "%d", info.line);
				throw ParseException(
					string_t(UNICODE_STRING("An error occured during parsing process on line ")) + 
					line_string + 
					UNICODE_STRING(", error description: '") + e.GetDescription() + UNICODE_STRING("'")
				);
			}
			break;
		}
		throw;
	}

	PrintFileStructure(info);
	DeleteTemporalInfo(info);
}




//-------------------------------------------------------------------------------------------------------
/*
	\brief Mesh exportieren
	\param pMesh
	\param Output

	Speichert den übergebenen Mesh im Format dieses Serializers in den übergebenen Stream.
/
void Serializer::Export(SharedPtr<Mesh> pMesh, BufferOutputStream& Output)
{
	throw NotSupportedException(UNICODE_STRING("todo!"));
}
*/



//-------------------------------------------------------------------------------------------------------
/*!
	\brief Datei auslesen
	\param info

	Alle Templates und alle Template-instanzen auslesen. Die gelesenen Informationen
	werden in 'info' hierarchisch zwischengespeichert.
*/
void Serializer::ReadFile(
	TemporalInfo& info
)
{
	if (info.header.IsText())
	{
		info.mode = TemporalInfo::FileModeText;
		info.scanner = new TextScanner();
	}
	else if (info.header.IsBinary())
	{
		info.mode = TemporalInfo::FileModeBinary;
		info.scanner = new BinaryScanner();
	}
	else if (info.header.IsTextCompressed())
	{
		throw NotSupportedException(
			UNICODE_STRING("compressed-text-format-reader todo!")
		);
	}
	else if (info.header.IsBinaryCompressed())
	{
		throw NotSupportedException(
			UNICODE_STRING("compressed-binary-format-reader todo!")
		);
	}

	while (!info.scanner->Eof(info))
	{
		Token* token = info.scanner->ReadToken(info);
		if (token == NULL)
			break;

		switch (token->GetType())
		{
		case Token::TypeTemplate: {
			string_t name_string;
			Template* current_template = ReadTemplateDefinition(name_string, info);
			if (info.templates.find(name_string) == info.templates.end())
			{
				info.templates.insert(TemporalInfo::templates_value_t(name_string, current_template));
			}
			else
			{
				current_template->RemRef();
			}
			break;
		}
		case Token::TypeName: {
			Template* instance = ReadTemplateData(token, info);
			info.instances.push_back(instance);
			break;
		}
		}

		/*if (info.mode == TemporalInfo::FileModeText && !info.scanner->Eof(info))
		{
			// just go over whitespaces, read until next character or eof 
			// to terminate the loop
			SkipTextWhitespaces(info);
		}*/

		/*string_t type_string;
		Token* token = info.scanner->ReadToken(info);
		switch (token->GetType())
		{
		case Token::TypeName: type_string = UNICODE_STRING("Token::TypeName"); break;
		case Token::TypeString: type_string = UNICODE_STRING("Token::TypeString"); break;
		case Token::TypeInteger: type_string = UNICODE_STRING("Token::TypeInteger"); break;
		case Token::TypeGuid: type_string = UNICODE_STRING("Token::TypeGuid"); break;
		case Token::TypeIntegerList: type_string = UNICODE_STRING("Token::TypeIntegerList"); break;
		case Token::TypeFloatList: type_string = UNICODE_STRING("Token::TypeFloatList"); break;
		case Token::TypeOBrace: type_string = UNICODE_STRING("Token::TypeOBrace"); break;
		case Token::TypeCBrace: type_string = UNICODE_STRING("Token::TypeCBrace"); break;
		case Token::TypeOParen: type_string = UNICODE_STRING("Token::TypeOParen"); break;
		case Token::TypeCParen: type_string = UNICODE_STRING("Token::TypeCParen"); break;
		case Token::TypeOBracket: type_string = UNICODE_STRING("Token::TypeOBracket"); break;
		case Token::TypeCBracket: type_string = UNICODE_STRING("Token::TypeCBracket"); break;
		case Token::TypeOAngle: type_string = UNICODE_STRING("Token::TypeOAngle"); break;
		case Token::TypeCAngle: type_string = UNICODE_STRING("Token::TypeCAngle"); break;
		case Token::TypeDot: type_string = UNICODE_STRING("Token::TypeDot"); break;
		case Token::TypeComma: type_string = UNICODE_STRING("Token::TypeComma"); break;
		case Token::TypeSemicolon: type_string = UNICODE_STRING("Token::TypeSemicolon"); break;
		case Token::TypeTemplate: type_string = UNICODE_STRING("Token::TypeTemplate"); break;
		case Token::TypeWord: type_string = UNICODE_STRING("Token::TypeWord"); break;
		case Token::TypeDWord: type_string = UNICODE_STRING("Token::TypeDWord"); break;
		case Token::TypeFloat: type_string = UNICODE_STRING("Token::TypeFloat"); break;
		case Token::TypeDouble: type_string = UNICODE_STRING("Token::TypeDouble"); break;
		case Token::TypeChar: type_string = UNICODE_STRING("Token::TypeChar"); break;
		case Token::TypeUChar: type_string = UNICODE_STRING("Token::TypeUChar"); break;
		case Token::TypeSWord: type_string = UNICODE_STRING("Token::TypeSWord"); break;
		case Token::TypeSDWord: type_string = UNICODE_STRING("Token::TypeSDWord"); break;
		case Token::TypeVoid: type_string = UNICODE_STRING("Token::TypeVoid"); break;
		case Token::TypeLpStr: type_string = UNICODE_STRING("Token::TypeLpStr"); break;
		case Token::TypeUnicode: type_string = UNICODE_STRING("Token::TypeUnicode"); break;
		case Token::TypeCString: type_string = UNICODE_STRING("Token::TypeCString"); break;
		case Token::TypeArray: type_string = UNICODE_STRING("Token::TypeArray"); break;
		}
		printf("TOKEN: %s\n", type_string.c_str());*/
	}
}




//-----------------------------------------------------------------------------------------------------------
Template* Serializer::InstanciateTemplateByGuid(
	TemporalInfo& info,
	const Guid& guid
) const
{
	Template* current_template = NULL;
	if (guid == g_guids[TemplateTypeAnimation])
		current_template = new Animation;
	else if (guid == g_guids[TemplateTypeAnimationKey])
		current_template = new AnimationKey;
	else if (guid == g_guids[TemplateTypeAnimationSet])
		current_template = new AnimationSet;
	else if (guid == g_guids[TemplateTypeAnimTicksPerSecond])
		current_template = new AnimTicksPerSecond;
	else if (guid == g_guids[TemplateTypeBoolean])
		current_template = new Boolean;
	else if (guid == g_guids[TemplateTypeBoolean2D])
		current_template = new Boolean2d;
	else if (guid == g_guids[TemplateTypeColorRGB])
		current_template = new ColorRGB;
	else if (guid == g_guids[TemplateTypeColorRGBA])
		current_template = new ColorRGBA;
	else if (guid == g_guids[TemplateTypeCoords2D])
		current_template = new Coords2d;
	else if (guid == g_guids[TemplateTypeDeclData])
		current_template = new DeclData;
	else if (guid == g_guids[TemplateTypeEffectDWord])
		current_template = new EffectDWord;
	else if (guid == g_guids[TemplateTypeEffectFloats])
		current_template = new EffectFloats;
	else if (guid == g_guids[TemplateTypeEffectInstance])
		current_template = new EffectInstance;
	else if (guid == g_guids[TemplateTypeEffectParamDWord])
		current_template = new EffectParamDWord;
	else if (guid == g_guids[TemplateTypeEffectParamFloats])
		current_template = new EffectParamFloats;
	else if (guid == g_guids[TemplateTypeEffectParamSTring])
		current_template = new EffectParamString;
	else if (guid == g_guids[TemplateTypeEffectString])
		current_template = new EffectString;
	else if (guid == g_guids[TemplateTypeFloatKeys])
		current_template = new FloatKeys;
	else if (guid == g_guids[TemplateTypeFrame])
		current_template = new Frame;
	else if (guid == g_guids[TemplateTypeFrameTransformMatrix])
		current_template = new FrameTransformMatrix;
	else if (guid == g_guids[TemplateTypeFVFData])
		current_template = new FVFData;
	else if (guid == g_guids[TemplateTypeHeader])
		current_template = new HeaderTemplate;
	else if (guid == g_guids[TemplateTypeIndexedColor])
		current_template = new IndexedColor;
	else if (guid == g_guids[TemplateTypeMaterial])
		current_template = new Material;
	else if (guid == g_guids[TemplateTypeMaterialWrap])
		current_template = new MaterialWrap;
	else if (guid == g_guids[TemplateTypeMatrix4x4])
		current_template = new Matrix4x4;
	else if (guid == g_guids[TemplateTypeMesh])
		current_template = new Mesh;
	else if (guid == g_guids[TemplateTypeMeshFace])
		current_template = new MeshFace;
	else if (guid == g_guids[TemplateTypeMeshFaceWraps])
		current_template = new MeshFaceWraps;
	else if (guid == g_guids[TemplateTypeMeshMaterialList])
		current_template = new MeshMaterialList;
	else if (guid == g_guids[TemplateTypeMeshNormals])
		current_template = new MeshNormals;
	else if (guid == g_guids[TemplateTypeMeshTextureCoords])
		current_template = new MeshTextureCoords;
	else if (guid == g_guids[TemplateTypeMeshVertexColors])
		current_template = new MeshVertexColors;
	else if (guid == g_guids[TemplateTypeSkinWeights])
		current_template = new SkinWeights;
	else if (guid == g_guids[TemplateTypeTextureFilename])
		current_template = new TextureFilename;
	else if (guid == g_guids[TemplateTypeTimedFloatKeys])
		current_template = new TimedFloatKeys;
	else if (guid == g_guids[TemplateTypeVector])
		current_template = new Vector;
	else if (guid == g_guids[TemplateTypeVertexDuplicationIndices])
		current_template = new VertexDuplicationIndices;
	else if (guid == g_guids[TemplateTypeVertexElement])
		current_template = new VertexElement;
	else if (guid == g_guids[TemplateTypeXSkinMeshHeader])
		current_template = new XSkinMeshHeader;

	return current_template;
}




//-----------------------------------------------------------------------------------------------------------
Template* Serializer::InstanciateTemplateByName(
	TemporalInfo& info,
	const string_t& name
) const
{
	TemporalInfo::templates_map_t::iterator found = info.templates.find(name);
	if (found == info.templates.end())
	{
		throw Exception(
			string_t(UNICODE_STRING("instance of undefined template '")) + 
			name + 
			"' found!"
		);
	}

	return found->second->Clone();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Eine Template-Definition lesen
	\param template_name
	\param info

	Liest ein Template aus dem File.
*/
Template* Serializer::ReadTemplateDefinition(
	string_t& template_name,
	TemporalInfo& info
)
{
	TokenName* name_token = dynamic_cast<TokenName*>(info.scanner->ReadExpectedToken(info, Token::TypeName));
	Token* token_obrace = info.scanner->ReadExpectedToken(info, Token::TypeOBrace);
	TokenGuid* token_guid = dynamic_cast<TokenGuid*>(info.scanner->ReadExpectedToken(info, Token::TypeGuid));
	template_name = name_token->GetName();

	Template* current_template = InstanciateTemplateByGuid(info, token_guid->GetGuid());
	if (!current_template)
		current_template = InstanciateTemplateByName(info, name_token->GetName());
	//if (!current_template)
	//	current_template = new GenericTemplate(name_token, token_guid);
	
	if (!current_template)
	{
		throw Exception(
			string_t(UNICODE_STRING("Template not found! Identifier '")) +
			name_token->GetName() +
			UNICODE_STRING("'")
		);
	}

	current_template->ReadDefinition(info);
	info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	return current_template;
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten auslesen
	\param token
	\param info

	Liest Template-Daten.
*/
Template* Serializer::ReadTemplateData(
	Token* token,
	TemporalInfo& info
)
{
	switch (token->GetType())
	{
	case Token::TypeOBrace: {
		TokenName* reference_name_token = dynamic_cast<TokenName*>(info.scanner->ReadExpectedToken(info, Token::TypeName));
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
		TemporalInfo::templates_map_t::iterator found = info.named_instances.find(reference_name_token->GetName());
		if (found != info.named_instances.end())
		{
			found->second->AddRef();
			return found->second;
		}
		else
		{
			return NULL;
		}
	}
	case Token::TypeName: {
		TokenName* template_name_token = dynamic_cast<TokenName*>(token);
		TokenName* optional_name_token = NULL;
		TokenGuid* optional_guid_token = NULL;

		optional_name_token = dynamic_cast<TokenName*>(info.scanner->ReadOptionalToken(info, Token::TypeName));
		info.scanner->ReadExpectedToken(info, Token::TypeOBrace);
		optional_guid_token = dynamic_cast<TokenGuid*>(info.scanner->ReadOptionalToken(info, Token::TypeGuid));

		// Template suchen und lesen lassen
		Template* current_template = InstanciateTemplateByName(info, template_name_token->GetName());
		current_template->ReadData(optional_name_token, optional_guid_token, info, true);
		if (optional_name_token != NULL)
		{
			//current_template->AddRef();
			info.named_instances.insert(pair<string_t, Template*>(optional_name_token->GetName(), current_template));
		}
		return current_template;
	}
	default: {
		return NULL;
	}
	}
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief delete all contents from a temporal-info structure
*/
void Serializer::DeleteTemporalInfo(
	TemporalInfo& info
)
{
	for (
		TemporalInfo::templates_map_t::iterator begin = info.templates.begin();
		begin != info.templates.end();
		++begin
	)
	{
		Template* current_template = begin->second;
		current_template->RemRef();
	}
	info.templates.clear();
}




//-------------------------------------------------------------------------------------------------------
/*!
	\brief Das nächste DWORD zurückgeben
	\return DWORD

	Gibt das nächste im input zu findende DWORD zurück.
*/
udword_t Serializer::GetNextDword(
	TemporalInfo& info
)
{
	if (info.last_token != NULL && info.last_token->GetType() == Token::TypeIntegerList)
	{
		TokenIntegerList* list = dynamic_cast<TokenIntegerList*>(info.last_token);
		udword_t count = list->GetCount();

		// Im besten Fall hats noch einen Integer im letzten Token
		if (info.last_token_index < count)
		{
			const udword_t* const array = list->GetArray();
			udword_t retval = array[info.last_token_index];
			++info.last_token_index;
			if (info.last_token_index >= count)
			{
				info.last_token = NULL;
				info.last_token_index = 0;
			}
			return retval;
		}
	}

	/*
	 *	Wenn entweder das letzte Token keine Integer-Liste war, oder die Integer-Liste voll
	 *	ausgeschöpft worden ist müssen wir das nächste Token lesen und hoffen, dass sich darin ein 
	 *	Integer befindet. Wenn nicht wird eine Exception geworfen. Achtung: Das nächste Token könnte
	 *	entweder ein einzelner Integer oder eine Integer-Liste sein. Beide Fälle werden akzeptiert.
	 */
	info.scanner->ReadToken(info);
	switch (info.last_token->GetType())
	{
	case Token::TypeInteger:
		{
			TokenInteger* integer_token = dynamic_cast<TokenInteger*>(info.last_token);
			return integer_token->GetDword();
		}
		break;

	case Token::TypeIntegerList:
		// In diesem Fall kann Rekursion zum Ziel führen, da ja jetzt sicher ein 
		// Element der geladenen Liste verfügbar ist!
		return GetNextDword(info);
		break;

	default:
		throw Exception(
			UNICODE_STRING("Token::TypeInteger or Token::TypeIntegerList expected!")
		);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Den nächsten Float zurückgeben
	\return Float

	Gibt den nächsten im input zu findende Float zurück.
*/
float_t Serializer::GetNextFloat32(TemporalInfo& info)
{
	if (info.last_token != NULL && info.last_token->GetType() == Token::TypeFloatList)
	{
		TokenFloatList* list = dynamic_cast<TokenFloatList*>(info.last_token);
		udword_t count = list->GetCount();

		// Im besten Fall hats noch einen Float im letzten Token
		float_t retval = 0.0f;
		if (info.last_token_index < count)
		{
			const float_t* const array = list->GetFloat32Array();
			if (array)
			{
				retval = array[info.last_token_index];
			}
			else
			{
				throw Exception(
					UNICODE_STRING("array invalid ptr!")
				);
			}
		}

		++info.last_token_index;
		if (info.last_token_index >= count)
		{
			info.last_token = NULL;
			info.last_token_index = 0;
		}
		return retval;
	}

	/*
	 *	Wenn entweder das letzte Token keine Float-Liste war, oder die Float-Liste voll
	 *	ausgeschöpft worden ist müssen wir das nächste Token lesen und hoffen, dass sich darin ein 
	 *	Float befindet. Wenn nicht wird eine Exception geworfen. Achtung: Das nächste Token könnte
	 *	entweder ein einzelner Float oder eine Float-Liste sein. Beide Fälle werden akzeptiert.
	 */
	info.scanner->ReadToken(info);
	switch (info.last_token->GetType())
	{
	case Token::TypeFloatList:
		// In diesem Fall kann Rekursion zum Ziel führen, da ja jetzt sicher ein 
		// Element der geladenen Liste verfügbar ist!
		return GetNextFloat32(info);
		break;

	default:
		throw Exception(
			UNICODE_STRING("Token::TypeFloatList expected!")
		);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Die gesamte Dateistruktur ausgeben
	\param info

	Gibt die Struktur der in info gespeicherten Datei auf die Konsole aus.
*/
void Serializer::PrintFileStructure(TemporalInfo& info)
{
	for (
		TemporalInfo::instances_container_t::iterator begin = info.instances.begin();
		begin != info.instances.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(0);
	}
}

} // namespace x
