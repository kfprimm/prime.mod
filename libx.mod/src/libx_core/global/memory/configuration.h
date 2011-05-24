//-----------------------------------------------------------------------------------------------------------
/*!	\file global/memory/configuration.h
	\brief Definition von x::debugmanager::Configuration
	\version 0.1
	\date 9.11.2003
	\author Christian Oberholzer

	Definition von x::debugmanager::Configuration

	Paul Nettles Memory Manager! (http://www.paulnettle.com/)
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef KAOS_MEMORY_DEBUGMANAGER_CONFIGURATION_H
#define KAOS_MEMORY_DEBUGMANAGER_CONFIGURATION_H

namespace x { namespace debugmanager {

//-----------------------------------------------------------------------------------------------------------
/*
 *	Verfügbare Konfigurationen
 */
struct normal_configuration { };
struct check_all_configuration { };
struct random_failure_configuration { };
struct normal_configuration_with_logging { };
struct check_all_configuration_with_logging { };
struct random_failure_configuration_with_logging { };




//-----------------------------------------------------------------------------------------------------------
/*
 *	Gewünschte Konfiguration wählen!
 */
typedef normal_configuration current_configuration_t;




//-----------------------------------------------------------------------------------------------------------
/*
 *	Definition der verschiedenen Konfigurationen. Die verschiedenen konstanten Felder
 *	der Konfiguration sind:
 *		- HASHBITS: Sämtliche allozierten Speicherstellen werden über ihren Pointer in einer Hashtable
 *					zur schnellen Suche abgelegt. Diese Konstante definiert wieviele Bits der 
 *					verwendete Hashwert enthalten soll.
 *
 *		- RANDOMWIPE: Per Zufall die Under- und Overflow-Paddings mit einem bekannten Muster füllen?
 *		- RANDOMWIPE_FACTOR: Die Prozentzahl an Paddings welche mit einem Muster gefüllt werden. Diese
 *							 Konstante wird nur benötigt wenn RANDOMWIPE = true
 *		- ALWAYS_WIPE_ALL: Soll der Memory-Manager immer alle Paddings mit einem bekannten Muster füllen?
 *		- PADDING_SIZE:	Grösse der Buffer welche vor und hinter der allozierten Stelle alloziert werden
 *						um Over- und Underflows entdecken zu können.
 *
 *		- RANDOM_FAILURE: Soll ab und zu der Allocation-Befehl fehlschlagen?
 *		- RANDOM_FAILURE_FACTOR: Prozentzahl zu welcher die Allocation-Befehle fehlschlagen
 *
 *		- ALWAYS_VALIDATE_ALL:  Soll der Memory-Manager immer wenn er aufgerufen wird sämtliche allozierten
 *								Felder auf Gültigkeit prüfen. (D.h. z.B. ob ein Over- oder Underflow 
 *								aufgetreten ist.
 *
 *		- ALWAYS_LOG_ALL: Soll der Memory-Manager alle Events loggen?
 *		- CLEANUP_LOG_ON_FIRST_RUN: Soll der Memory-Manager beim Start die Logfiles leeren?
 *
 */
template <
	typename ConfigurationT
>
struct Configuration;

template<>
struct Configuration <normal_configuration>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= false,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 4,

		RANDOM_FAILURE			= false,
		RANDOM_FAILURE_FACTOR	= 0,

		ALWAYS_VALIDATE_ALL		= false,
		ALWAYS_LOG_ALL			= false,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

template<>
struct Configuration <check_all_configuration>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= true,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 1024,

		RANDOM_FAILURE			= false,
		RANDOM_FAILURE_FACTOR	= 0,

		ALWAYS_VALIDATE_ALL		= true,
		ALWAYS_LOG_ALL			= false,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

template<>
struct Configuration <random_failure_configuration>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= false,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 4,

		RANDOM_FAILURE			= true,
		RANDOM_FAILURE_FACTOR	= 50,

		ALWAYS_VALIDATE_ALL		= false,
		ALWAYS_LOG_ALL			= false,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

template<>
struct Configuration <normal_configuration_with_logging>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= false,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 4,

		ALWAYS_VALIDATE_ALL		= false,
		ALWAYS_LOG_ALL			= true,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

template<>
struct Configuration <check_all_configuration_with_logging>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= true,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 1024,

		RANDOM_FAILURE			= false,
		RANDOM_FAILURE_FACTOR	= 0,

		ALWAYS_VALIDATE_ALL		= true,
		ALWAYS_LOG_ALL			= true,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

template<>
struct Configuration <random_failure_configuration_with_logging>
{
	enum 
	{
		HASHBITS				= 12,
		RANDOMWIPE				= false,
		RANDOMWIPE_FACTOR		= 0,
		ALWAYS_WIPE_ALL			= true,
		PADDING_SIZE			= 4,

		RANDOM_FAILURE			= true,
		RANDOM_FAILURE_FACTOR	= 50,

		ALWAYS_VALIDATE_ALL		= false,
		ALWAYS_LOG_ALL			= true,
		CLEANUP_LOG_ON_FIRST_RUN= true,

		PATTERN_PREFIX			= 0xbaadf00d,
		PATTERN_POSTFIX			= 0xdeadc0de,
		PATTERN_UNUSED			= 0xfeedface,
		PATTERN_RELEASED		= 0xdeadbeef
	};
};

}} // namespace x::debugmanager

#endif // KAOS_MEMORY_DEBUGMANAGER_CONFIGURATION_H
