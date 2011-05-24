//-----------------------------------------------------------------------------------------------------------
/*!	\file global/memory/memmanager.h
	\brief Definition von x::debugmanager::DebugMemoryManager
	\version 0.1
	\date 9.11.2003
	\author Christian Oberholzer

	Definition von x::debugmanager::DebugMemoryManager

	Paul Nettles Memory Manager! (http://www.paulnettle.com/)
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef KAOS_MEMORY_DEBUGMEMORYMANAGER_H
#define KAOS_MEMORY_DEBUGMEMORYMANAGER_H

#include <stdio.h>
#include "./configuration.h"

namespace x { 
	
//-----------------------------------------------------------------------------------------------------------
/*!	\brief Interface für alle verwendeten Memory-Manager
	\version 0.1
	\date 8.11.2003
	\author Christian Oberholzer

	Jeder der verfügbaren Memory-Manage rmuss dieses Interface implementieren.
*/
class MemoryManager
{
public:
	virtual void SetOwner(const char* szFile, const uint_t uiLine) = 0;
	virtual const char* GetOwnerFile() const = 0;
	virtual const uint_t GetOwnerLine() const = 0;

	virtual void* DoNew(uint_t uiSize, const char* szFile, const uint_t uiLine) = 0;
	virtual void* DoNewArray(uint_t uiSize, const char* szFile, const uint_t uiLine) = 0;
	virtual void  DoDelete(void* p, const char* szFile, const uint_t uiLine) = 0;
	virtual void  DoDeleteArray(void* p, const char* szFile, const uint_t uiLine) = 0;
	virtual void* DoMalloc(uint_t uiSize, const char* szFile, const uint_t uiLine) = 0;
	virtual void* DoCalloc(uint_t uiItemSize, uint_t uiNumItems, const char* szFile, const uint_t uiLine) = 0;
	virtual void* DoRealloc(void* p, uint_t uiNewSize, const char* szFile, const uint_t uiLine) = 0;
	virtual void  DoFree(void* p, const char* szFile, const uint_t uiLine) = 0;

	// Unterhalt 
	virtual void EnableStacktraceFeature() = 0;
	virtual void DisableStacktraceFeature() = 0;
	virtual bool ValidateAddress(void* p) = 0;
	virtual bool ValidateAllAllocations() = 0;

	// Status
	virtual uint_t GetAllocatedSize() = 0;
	virtual uint_t GetReportedSize() = 0;
};




namespace debugmanager {

//-----------------------------------------------------------------------------------------------------------
/*! \brief Statistikelement
	\version 0.1
	\date 13.4.2002
	\author Christian Oberholzer

	Statistikstruktur für den Memory-Manager

	Changes: 
		- 20.06.2002 Christian Oberholzer Dokumentation
*/
struct Statistics 
{
	/*!
		\var uiTotalReportedMemory
		\brief Reportet Mem
	
		Total des für das Programm allozierten RAMS	
	*//*!
		\var uiTotalActualMemory
		\brief Total alloziertes Mem

		Total alloziertes RAM	
	*//*!	
		\var uiTotalAllocUnitCount
		\brief Total allozierte Blöcke

		Anzahl aller allozierte Blöcke	
	*//*!
		\var uiPeakReportedMemory
		\brief Maximal Reported Mem
	
		Maximum für das Programm zu einem bestimmten Zeitpunkt allozierten RAMS	
	*//*!
		\var uiPeakActualMemory
		\brief Maximal alloziertes Mem

		Maximum des zu einem bestimmten Zeitpunkt allozierten RAMS	
	*//*!
		\var uiPeakAllocUnitCount
		\brief Maximale Anzahl allozierter Blöcke

		Höchste Anzahl an gleichzeitig allozierten Blöcken	
	*//*!	
		\var uiAccumulatedReportedMemory
		\brief Gesamthaft Reported Mem

		Total allen jemals allozierten für das Programm RAMS	
	*//*!
		\var uiAccumulatedActualMemory
		\brief Gesamthaft alloziertes Mem

		Total allen jemals allozierten RAMS	
	*//*!
		\var uiAccumulatedAllocUnitCount
		\brief Gesamtzahl aller allozierter Blöcke

		Total aller allozierten Blöcke	= Anzahl Aufrufe von operator new während 
		des gesamten Programmablaufs.
	*/

	uint_t uiTotalReportedMemory;			
	uint_t uiTotalActualMemory;				
	uint_t uiTotalAllocUnitCount;			
	uint_t uiPeakReportedMemory;				
	uint_t uiPeakActualMemory;				
	uint_t uiPeakAllocUnitCount;				
	uint_t uiAccumulatedReportedMemory;		
	uint_t uiAccumulatedActualMemory;		
	uint_t uiAccumulatedAllocUnitCount;		
};





//-----------------------------------------------------------------------------------------------------------
/*!	\brief Memory Block
	\version 0.1
	\date 13.4.2002
	\author Christian Oberholzer

	Diese Struktur speichert alle nötigen Informationen für einen Speicherblock.

	Changes: 
	- 20.06.2002 Christian Oberholzer Dokumentation
*/
struct AllocationUnit 
{
	/*!
		\var uiActualSize
		\brief Grösse

		Grösse dieses Blocks	
	*//*!
		\var uiReportedSize
		\brief Reported Size

		Für das Programm benötigte Grösse	
	*//*!
		\var pActualAddress
		\brief Beginn des Blocks

		Adresse an welcher dieser Block liegt	
	*//*!
		\var pReportedAddress
		\brief Zurückgegebener Pointer

		Adresse welche an das Programm zurückgegeben wurde	
	*//*!
		\var szSourceFile
		\brief Sourcefile

		Sourcefile in welchem der Block alloziert wurde	
	*//*!
		\var uiAllocType
		\brief AllocationType

		AllocationType, z.B. new oder malloc	
	*//*!
		\var uiSourceLine
		\brief Sourcezeile

		Zeile im entsprechenden Sourcefile auf welcher der Block alloziert wurde	
	*//*!
		\var bBreakOnDealloc
		\brief Breakpoint

		Breakpoint beim freigeben des Blocks	
	*//*!
		\var bBreakOnRealloc
		\brief Breakpoint

		Breakpoint bei Änderungen der Grösse des Blocks	
	*//*!
		\var uiAllocationNumber
		\brief ID

		ID	
	*//*!
		\var pNext
		\brief Nächster Block

		Nächster Block in einer doppelt gelinkten Liste	
	*//*!
		\var pPrev
		\brief Vorghergehender Block

		Vorhergehender Block in einer doppelt gelinkten Liste	
	*/

	uint_t	uiActualSize;		
	uint_t	uiReportedSize;		
	void*	pActualAddress;		
	void*	pReportedAddress;
	char szSourceFile[256];
	uint_t	uiAllocType;			
	uint_t	uiSourceLine;		
	bool	bBreakOnDealloc;	
	bool	bBreakOnRealloc;	
	uint_t	uiAllocationNumber;	
	AllocationUnit* pNext;
	AllocationUnit* pPrev;
};




//-----------------------------------------------------------------------------------------------------------
//! AllocationTypes
/*! Mögliche Methoden mit welchen Speicher alloziert 
	werden kann. Da das allozieren und freigeben von Speicher zwischen
	C-Funktionen wie malloc/free nicht mit den C++-Operatoren wie new und
	delete gemixt werden dürfen muss gespeichert werden wie das RAM alloziert
	wurde, damit unterschiedliche freigabetypen unterschieden werden können.
*/
enum ALLOCATION_TYPES 
{
	//! Unbekannt
	/*! Der Speicher wurde von einer unbekannten Funktion aus
		alloziert.
	*/
	ALLOC_UNKNOWN		= 0,

	//! new
	/*! Der Speicher wurde mit dem C++-Operator new alloziert.
		Beispiel: new char;
	*/
	ALLOC_NEW			= 1,

	//! new[]
	/*! Der Speicher wurde mit dem C++-Operator new[] alloziert. 
		Beispiel: new char[256];
	*/
	ALLOC_NEW_ARRAY		= 2,

	//! malloc()
	/*! Der Speicher wurde mit der C-Funktion malloc alloziert. 
		Beispiel: char* pcItem = malloc(sizeof(char) * 5);
	*/
	ALLOC_MALLOC		= 3,

	//! calloc()
	/*! Der Speicher wurde mit der C-Funktion calloc alloziert.
		Beispiel: char* pcItem = calloc(5, sizeof(char));
	*/
	ALLOC_CALLOC		= 4,

	//! realloc()
	/*! Der Speicher wurde mit der C-Funktion realloc alloziert.
		Beispiel: pcItem = realloc(pcItem, sizeof(char) * 10);
	*/
	ALLOC_REALLOC		= 5,

	//! delete
	/*! Der C++-Operator zum freigeben von Speicher welcher mit new alloziert wurde.
	*/
	ALLOC_DELETE		= 6,

	//! delete[]
	/*! Der C++-Operator zum freigeben von Speicher welcher mit new[] alloziert wurde.
	*/
	ALLOC_DELETE_ARRAY	= 7,

	//! free()
	/*! Die C-Funktion zum freigeben von Speicher welcher mit einer anderen C-Funktion
		alloziert worden ist.
	*/
	ALLOC_FREE			= 8,
};




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Memory-Manager
	\version 0.2
	\date 13.4.2002
	\author Christian Oberholzer

	Für Debugzwecke werden vor und nach dem zu allozierenden Block noch zusätzliche Blöcke 
	(von der Grösse DebugMemoryManager::PADDINGSIZE) alloziert und mit einem bekannten Bytemuster 
	gefüllt, so dass Underflows und Overflows leicht erkannt werden können.
	-> Alle Variabeln welche "reported" im Namen beinhalten beschreiben Werte, welche zurückgeben,
	also z.B. uiReportedSize enhält die Grösse welche von new alloziert werden soll, während alle
	Variabeln welche "actual" enthalten die tatsächlich benutzte Grösse angeben, also z.B. in jenem 
	Fall uiActualSize = uiReportedSize + 2 * PADDINGSIZE.

	Changes:
		- 09.11.2003 Christian Oberholzer In der Ornderstruktur verschoben und umbenannt
		- 20.06.2002 Christian Oberholzer Dokumentation begonnen
*/
class DebugMemoryManager : public MemoryManager
{
public:
	/*!	
		\typedef configuration_t
		\brief Aktuelle Konfiguration

		Die vom Benutzer gewählte Konfiguration.
	*/
	typedef debugmanager::Configuration<debugmanager::current_configuration_t> configuration_t;

public:
	/*
	 *	Konstruktor und Destruktor
	 */
	DebugMemoryManager();
	~DebugMemoryManager();

	/*
	 *	"Besitzer", d.h. Ursprung des letzten Allocation-Befehls zurückgeben
	 */
	virtual void SetOwner(const char* szFile, const uint_t uiLine);
	virtual const char* GetOwnerFile() const;
	virtual const uint_t GetOwnerLine() const;
	virtual void ResetOwner();

	/*
	 *	Befehle entsprechend den üblichen Allocation-Arten
	 */
	virtual void* DoNew(uint_t uiSize, const char* szFile, const uint_t uiLine);
	virtual void* DoNewArray(uint_t uiSize, const char* szFile, const uint_t uiLine);
	virtual void  DoDelete(void* p, const char* szFile, const uint_t uiLine);
	virtual void  DoDeleteArray(void* p, const char* szFile, const uint_t uiLine);
	virtual void* DoMalloc(uint_t uiSize, const char* szFile, const uint_t uiLine);
	virtual void* DoCalloc(uint_t uiItemSize, uint_t uiNumItems, const char* szFile, const uint_t uiLine);
	virtual void* DoRealloc(void* p, uint_t uiNewSize, const char* szFile, const uint_t uiLine);
	virtual void  DoFree(void* p, const char* szFile, const uint_t uiLine);

	/*
	 *	Unterhalt 
	 */
	virtual void EnableStacktraceFeature();
	virtual void DisableStacktraceFeature();
	virtual bool ValidateAddress(void* p);
	virtual bool ValidateAllocUnit(AllocationUnit* pUnit);
	virtual bool ValidateAllAllocations();

	/*
	 *	Status
	 */
	virtual uint_t GetAllocatedSize();
	virtual uint_t GetReportedSize();

	/*
	 *	Allocation und Dealocatoin
	 */
	virtual void* Allocator(const char* szFile, uint_t iLine, uint_t uiAllocType, uint_t uiReportedSize);
	virtual void* Reallocator(const char* szFile, uint_t iLine, uint_t uiAllocType, uint_t uiReportedSize, void* p);
	virtual void  Deallocator(const char* szFile, uint_t iLine, uint_t iDeallocType, void* p);

	/*
	 *	Breakpoints
	 */
	virtual bool BreakOnRealloc(void* p);
	virtual bool BreakOnDealloc(void* p);

	// Logging 
	virtual void DumpLeakReport();
	virtual void DumpAllocations(FILE* fpReport);
	virtual void DumpAllocUnit(AllocationUnit* pUnit, const char* strPrefix = "");
	virtual void DumpMemoryReport(const char* szFilename);
	virtual Statistics& GetMemoryStatistics();

protected:
	/*
	 *	Hilfsmethoden für Allocation Units
	 */
	AllocationUnit* _GetEmptyAllocUnit();
	void _PutBackAllocUnit(AllocationUnit* pAllocUnit);
	AllocationUnit* _FindAllocUnit(void* p);
	void _WhipeWithPattern(AllocationUnit* pAllocUnit, const udword_t udSpecialPattern);
	void _Cleanup();

	/*
	 *	Formatierung von Strings
	 */
	string_t _InsertHochkommas(uint_t uiValue); 
	string_t _MemorySizeString(uint_t uiValue);

	/*
	 *	Hilfsmethoden für die Hashtable
	 */
	uint_t _ComputeHashFromAddress(void* p);
	void _InsertIntoHashtable(AllocationUnit* pAllocUnit);
	void _RemoveFromHashtable(AllocationUnit* pAllocUnit);

	/*
	 *	Hilfsmethoden zu Adressen
	 */
	const uint_t _CalculateActualSize(const uint_t uiReportedSize) const;
	const uint_t _CalculateReportedSize(const uint_t uiActualSize) const;
	void* _CalculateReportedAddress(void* pActualAddress);

	/*
	 *	Hilfsmethoden zur Statistik
	 */
	void _AddToStatistics(AllocationUnit* _AddToStatistics);
	void _RemoveFromStatistics(AllocationUnit* _AddToStatistics);

	/*
	 *	Logging-Funktionen
	 */
	void _ClearLogfiles();
	void _LogStacktrace(const string_t& strFile, const uint_t uiLine,
						const string_t& strStacktrace, 
						const uint_t uiAllocationNumber);

protected:
	//! AllocTypes
	/*! Stringbezeichnungen für die verschiedenen Alloc-Types.
	*/
	static string_t m_strAllocationTypes[];

protected:
	/*!
		\var m_pReservoir
		\brief Unbenutzte Infoblock-Strukturen

		Das Reservoir enthält einen Zeiger auf eine Single-Linked List von info-Strukturen
		welche noch nicht benutzt sind.
	*//*!
		\var m_ppReservoirBuffer
		\brief Buffer an allozierten AllocationUnit-Blöcken

		Die AllocationUnits werden in Blöcken von je 256 Strukturen alloziert. Danach werden diese
		Strukturen in Linked Lists verwendet, der Pointer auf den Beginn des allozierten Memorys
		also verloren. Der ReservoirBuffer speichert die Adressen welche vom new-Befehl zurückgegeben
		wurden, um sie am Schluss ordnungsgemäss freigeben zu können.
	*//*!
		\var m_uiReservoirBufferSize
		\brief Anzahl gespeicherte Pointer in m_ppReservoirBuffer

		Speichert die Anzahl der Adressen welche in m_ppReservoirBuffer abgelegt
		worden sind.
	*/

	AllocationUnit*	 m_pReservoir;
	AllocationUnit** m_ppReservoirBuffer;
	uint_t			 m_uiReservoirBufferSize;

	/*!
		\var m_Statistics
		\brief Statistik des Memory-Managers

		Der Memory-Manager führt eine Statistik über den Memoryverbrauch des Programms. Die Statistik
		wird in dieser Variabel gespeichert.
	*/

	Statistics	m_Statistics;

	/*!
		\var m_uiHashSize
		\brief Grösse der Hashtabelle

		In dieser Variabel wird die Grösse der Hashtabelle gespeichert.
	*//*!
		\var m_HashTable
		\brief Hashtable aller Allocations

		Die Hasthtable ein Array von Linked List mit Allocation-Informationen.
	*//*!
		\var m_uiAllocationCount
		\brief Anzahl Allocations

		Dieser Counter wird mit jeder Allocation inkrementiert und kann so einzelne Allocations mit
		einem eindeutigen Key versehen.
	*/

	uint_t			 m_uiHashSize;
	AllocationUnit** m_HashTable;
	uint_t			 m_uiAllocationCount;

	/*!
		\var m_szOwner
		\brief Zuletzt verwendete Datei
		
		Die Datei von welcher das letzte new aufgerufen wurde
	*//*!
		\var m_uiOwner
		\brief Zeilennummer der zuletzt verwendeten Datei

		Die Zeilennummer der Stelle von welcher aus das letzte new aufgerufen wurde.
	*/

	char m_szOwner[1024];
	uint_t	 m_uiOwner;

	/*!
		\var m_bStacktraceFeature
		\brief Stacktrace-Feature-Flag

		Zeigt an, ob das Stacktrace-Feature ein- oder ausgeschaltet ist. D.h. bei jedem
		Allocation-Befehl wird der aktuelle Stacktrace in eine Logdatei geschrieben.
	*/

	bool m_bStacktraceFeature;
};

} // namespace debugmanager

MemoryManager& get_memory_manager();

} // namespace x

/*inline void* operator new (const x::uint_t uiSize, void* p)
{
	return p;
}*/

inline void* operator new (const x::uint_t uiSize)
{
	return x::get_memory_manager().DoNew(uiSize, x::get_memory_manager().GetOwnerFile(), x::get_memory_manager().GetOwnerLine());
}

inline void* operator new (const x::uint_t uiSize, const char* szFile, const x::uint_t uiLine)
{
	return x::get_memory_manager().DoNew(uiSize, szFile, uiLine);
}

/*inline void* operator new[] (const x::uint_t uiSize, void* p)
{
	return p;
}*/

inline void* operator new[] (const x::uint_t uiSize)
{
	return x::get_memory_manager().DoNewArray(uiSize, x::get_memory_manager().GetOwnerFile(), x::get_memory_manager().GetOwnerLine());
}

inline void* operator new[] (const x::uint_t uiSize, const char* szFile, const x::uint_t uiLine)
{
	return x::get_memory_manager().DoNewArray(uiSize, szFile, uiLine);
}

/*inline void operator delete (void* p, void* p2)
{
}*/

inline void operator delete (void* p)
{
	return x::get_memory_manager().DoDelete(p, x::get_memory_manager().GetOwnerFile(), x::get_memory_manager().GetOwnerLine());
}

inline void operator delete (void* p, const char* szFile, const x::uint_t uiLine)
{
	x::get_memory_manager().DoDelete(p, szFile, uiLine);
}

/*inline void operator delete [] (void* p, void* p2)
{
}*/

inline void operator delete [] (void* p)
{
	return x::get_memory_manager().DoDeleteArray(p, x::get_memory_manager().GetOwnerFile(), x::get_memory_manager().GetOwnerLine());
}

inline void operator delete [] (void* p, const char* szFile, const x::uint_t uiLine)
{
	x::get_memory_manager().DoDeleteArray(p, szFile, uiLine);
}

#define new (x::get_memory_manager().SetOwner(__FILE__, __LINE__), false) ? NULL : new
#define delete (x::get_memory_manager().SetOwner(__FILE__, __LINE__), false) ? NULL : NULL, delete

#define SAFE_DELETE(y) { if (y) { delete y; y = NULL; } }
#define SAFE_DELETE_ARRAY(y) { if (y) { delete[] y; y = NULL; } }
#define SAFE_REMREF(y) { if (y) { y->RemRef(); y = NULL; } }

#endif // KAOS_MEMORY_DEBUGMEMORYMANAGER_H
