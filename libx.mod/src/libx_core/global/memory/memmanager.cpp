//-----------------------------------------------------------------------------------------------------------
/*!	\file global/memory/memmanager.cpp
	\brief Implementation von x::debugmanager::DebugMemoryManager
	\version 0.1
	\date 9.11.2003
	\author Christian Oberholzer

	Implementation von x::debugmanager::DebugMemoryManager

	Paul Nettles Memory Manager! (http://www.paulnettle.com/)
*/
//-----------------------------------------------------------------------------------------------------------
#define K_KERNEL

#include <new>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include "../global.h"

#pragma warning(disable:4996)

namespace x { 
	
MemoryManager& get_memory_manager()
{
	static debugmanager::DebugMemoryManager myMemoryManager;
	return myMemoryManager;
}
	
namespace debugmanager {

using namespace std;

string_t DebugMemoryManager::m_strAllocationTypes[] = {
	UNICODE_STRING("unknown"),
	UNICODE_STRING("new"),
	UNICODE_STRING("new[]"),
	UNICODE_STRING("malloc"),
	UNICODE_STRING("calloc"),
	UNICODE_STRING("realloc"),
	UNICODE_STRING("delete"),
	UNICODE_STRING("delete[]"),
	UNICODE_STRING("free"),
};

//-----------------------------------------------------------------------------------------------------------
/*!	\brief Konstruktor

	Setzt alle Variabeln auf ihre Default-Werte.
*/
DebugMemoryManager::DebugMemoryManager()
:	m_pReservoir(NULL),
	m_ppReservoirBuffer(NULL),
	m_uiReservoirBufferSize(0),
	m_Statistics(),
	m_uiHashSize(0),
	m_HashTable(NULL),
	m_uiAllocationCount(0),
	m_uiOwner(0),
	m_bStacktraceFeature(false)
{
	strcpy(m_szOwner, "");

	/*
	 *	Hashtable initialisieren
	 */
	m_uiHashSize = 1 << configuration_t::HASHBITS;
	m_HashTable = (AllocationUnit**)malloc(sizeof(AllocationUnit*) * m_uiHashSize);
	memset(m_HashTable, 0, sizeof(AllocationUnit*) * m_uiHashSize);

	/*
	 *	Eventuell Logfiles leeren
	 */
	if (configuration_t::CLEANUP_LOG_ON_FIRST_RUN)
		_ClearLogfiles();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Destruktor
	
	Gibt den Speicher wieder frei + schreibt Leak- und Memoryreport in eine Datei.
*/
DebugMemoryManager::~DebugMemoryManager()
{
	DumpLeakReport();
	DumpMemoryReport("kaos__memory__debugmanager__report.log");

	/*
	 *	Memory freigeben
	 */
	_Cleanup();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief "Owner" setzen
	\param szFile
	\param uiLine

	Setzt den Ursprung des letzten new-Befehls. Dies ist n?tig, da die Angaben ?ber Ursprungsdatei und
	Zeile nicht direkt an den operator new ?bergeben werden k?nnen. (Dies w?re auf bequeme Art und Weise
	nur mit einem Makro m?glich, was den Placement-new-Operator unm?glich machen w?rde.
*/
void DebugMemoryManager::SetOwner(const char* szFile, const uint_t uiLine)
{
	strcpy(m_szOwner, szFile);
	m_uiOwner = uiLine;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief "Owner"-Datei zur?ckgeben
	\return Datei

	Gibt die Owner-Datei zur?ck.
*/
const char* DebugMemoryManager::GetOwnerFile() const
{
	return m_szOwner;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief "Owner"-Zeile zur?ckgeben
	\return Zeile

	Gibt die Zeile in der Owner-Datei zur?ck.
*/
const uint_t DebugMemoryManager::GetOwnerLine() const
{
	return m_uiOwner;
}



//-----------------------------------------------------------------------------------------------------------
/*!	\brief "Owner"-Angaben zur?cksetzen
	
	Setzt die "Owner"-Angaben auf ??:0 zur?ck.
*/
void DebugMemoryManager::ResetOwner()
{
	strcpy(m_szOwner, "");
	m_uiOwner = 0;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief new-Befehl
	\param uiSize Zu allozierende Gr?sse
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde
	\return Pointer auf den allozierten Speicherblock

	Alloziert einen Block von der Gr?sse iSize und gibt einen Pointer darauf zur?ck.
*/
void* DebugMemoryManager::DoNew(uint_t uiSize, const char* szFile, const uint_t uiLine)
{
	using namespace std;
	
	// ANSI says: allocation requests of 0 bytes will still return a valid value
	if (uiSize == 0)
		uiSize = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	while(true)
	{
		void* p = NULL;

		/*
			*	Versuchen zu allozieren
			*/
		p  = Allocator(szFile, uiLine, ALLOC_NEW, uiSize);
		
		/*
			*	Ordnungsgem?ss das allozierte Memory zur?ckgeben
			*/
		if(p)
		{
			return p;
		}

		/*
			*	Wenn es einen new-Handler gibt so verwenden wir den new-Handler um den Client-Code davon zu 
			*	benachrichtigen, dass zu wenig Memory vorhanden ist und etwas freigegeben werden soll.
			*
			*	Wenn kein new-Handler gesetzt ist wird eine bad_alloc-Exception geworfen.
			*
			*	Siehe auch C++-Standard.
			*/
		new_handler nh = set_new_handler(NULL);
		set_new_handler(nh);

		if (nh)
		{
			(*nh)();
		}
		else
		{
			throw bad_alloc();
		}
	}

	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief new[]
	\param uiSize Zu allozierende Gr?sse
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde
	\return Pointer auf den allozierten Speicherblock

	Alloziert einen Block von der Gr?sse iSize und gibt einen Pointer darauf zur?ck.
*/
void* DebugMemoryManager::DoNewArray(uint_t uiSize, const char* szFile, const uint_t uiLine)
{
	using namespace std;
	
	// ANSI says: allocation requests of 0 bytes will still return a valid value
	if (uiSize == 0)
		uiSize = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	while(true)
	{
		void* p = NULL;

		/*
			*	Versuchen zu allozieren
			*/
		p  = Allocator(szFile, uiLine, ALLOC_NEW_ARRAY, uiSize);

		/*
			*	Ordnungsgem?ss das allozierte Memory zur?ckgeben
			*/
		if(p)
		{
			return p;
		}

		/*
			*	Wenn es einen new-Handler gibt so verwenden wir den new-Handler um den Client-Code davon zu 
			*	benachrichtigen, dass zu wenig Memory vorhanden ist und etwas freigegeben werden soll.
			*
			*	Wenn kein new-Handler gesetzt ist wird eine bad_alloc-Exception geworfen.
			*
			*	Siehe auch C++-Standard.
			*/
		new_handler nh = set_new_handler(NULL);
		set_new_handler(nh);

		if (nh)
		{
			(*nh)();
		}
		else
		{
			throw bad_alloc();
		}
	}

	return NULL;
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief delete
	\param p Pointer auf den Block der freigegeben werden soll
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde

	Gibt den ?bergebenen Block Speicher wieder frei.
*/
void  DebugMemoryManager::DoDelete(void* p, const char* szFile, const uint_t uiLine)
{
	// ANSI says: delete & delete[] allow NULL pointers (they do nothing)
	if (p)
	{
		Deallocator(szFile, uiLine, ALLOC_DELETE, p);
	}
//	else if (DebugMemoryManager::ALWAYSLOGALL)
//	{
//		k_log("DebugMemoryManager::Delete", "-- %8s of NULL by %s: %05d", m_strAllocationTypes[DebugMemoryManager::ALLOC_DELETE], m_strFile, m_iLine);
//	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief delete[]
	\param p Pointer auf den Block der freigegeben werden soll
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde

	Gibt den ?bergebenen Block Speicher wieder frei.
*/
void  DebugMemoryManager::DoDeleteArray(void* p, const char* szFile, const uint_t uiLine)
{
	// ANSI says: delete & delete[] allow NULL pointers (they do nothing)
	if (p)
	{
		Deallocator(szFile, uiLine, ALLOC_DELETE_ARRAY, p);
	}
//	else if (DebugMemoryManager::ALWAYSLOGALL)
//	{
//		k_log("DebugMemoryManager::Delete", "-- %8s of NULL by %s: %05d", m_strAllocationTypes[DebugMemoryManager::ALLOC_DELETE], m_strFile, m_iLine);
//	}
}




//-----------------------------------------------------------------------------------------------------------
/*! \brief malloc
	\param uiSize Gr?sse welche alloziert werden soll
	\param szFile Sourcedatei aus welcher der Aufruf kommt
	\param uiLine Linie in der Sourcedatei von welcher der Aufruf kommt
	\return Pointer auf den allozierten Speicherblock

	Alloziert Speicher von der Gr?sse iSize und gibt einen Pointer darauf zur?ck.
*/
void* DebugMemoryManager::DoMalloc(uint_t uiSize, const char* szFile, const uint_t uiLine)
{
	void* pRet = NULL;
	pRet = Allocator(szFile, uiLine, ALLOC_MALLOC, uiSize);
	return pRet;
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief calloc
	\param uiItemSize Gr?sse eines Elements
	\param uiNumItems Anzahl zu allozierender Elemente
	\param szFile Sourcedatei aus welcher der Aufruf kommt
	\param uiLine Linie in der Sourcedatei von welcher der Aufruf kommt
	\return Pointer auf den allozierten Speicherblock

	Alloziert iNum-Elemente von der Gr?sse iSize und gibt einen Pointer darauf zur?ck.
*/
void* DebugMemoryManager::DoCalloc(uint_t uiItemSize, uint_t uiNumItems, const char* szFile, const uint_t uiLine)
{
	void* pRet = NULL;
	pRet = Allocator(szFile, uiLine, ALLOC_CALLOC, uiItemSize * uiNumItems);
	return pRet;
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief realloc
	\param p Pointer auf den alten Speicherblock
	\param uiNewSize Neue Gr?sse des Blocks
	\param szFile Sourcedatei aus welcher der Aufruf kommt
	\param uiLine Linie in der Sourcedatei von welcher der Aufruf kommt
	\return Pointer auf den allozierten Speicherblock

	Vergr?ssert bzw. Verkleinert den ?bergebenen Speicherblock p auf die Gr?sse uiReportedSize
	und gibt einen Pointer auf den neuen Speicherblock zur?ck. 
*/
void* DebugMemoryManager::DoRealloc(void* p, uint_t uiNewSize, const char* szFile, const uint_t uiLine)
{
	void* pRet = NULL;
	pRet = Reallocator(szFile, uiLine, ALLOC_REALLOC, uiNewSize, p);
	return pRet;
}




//-----------------------------------------------------------------------------------------------------------
/*! \brief free
	\param p Pointer auf den Speicherblock
	\param szFile Sourcedatei aus welcher der Aufruf kommt
	\param uiLine Linie in der Sourcedatei von welcher der Aufruf kommt

	Gibt den Speicher von Block p frei. Leitet den Aufruf weiter an g_MemManager.Deallocator().
*/
void  DebugMemoryManager::DoFree(void* p, const char* szFile, const uint_t uiLine)
{
	Deallocator(szFile, uiLine, ALLOC_FREE, p);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Stacktrace-On-Allocation-Feature einschalten
	
	Schaltet das Feature mit welchem bei jeder Allocation ein Stacktrace erstellt wird ein.
*/
void DebugMemoryManager::EnableStacktraceFeature()
{
	m_bStacktraceFeature = true;
}



//-----------------------------------------------------------------------------------------------------------
/*!	\brief Stacktrace-On-Allocation-Feature einschalten
	
	Schaltet das Feature aus mit welchem bei jeder Allocation ein Stacktrace erstellt wird ein.
*/
void DebugMemoryManager::DisableStacktraceFeature()
{
	m_bStacktraceFeature = false;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Validate-Methode
	\param p Pointer p
	\return flag welches anzeigt ob der Pointer auf alloziertes Memory zeigt

	Diese Methode pr?ft ob der ?bergebene Pointer auf vorhandenes Memory zeigt, und dass das Memory
	an der ?bergebenen Stelle noch i.O. ist, d.h. keine Over- und Underflows aufgetreten sind und
	auch das Memory noch nicht freigegeben worden ist.
*/
bool DebugMemoryManager::ValidateAddress(void* p)
{
	AllocationUnit* pAllocUnit = _FindAllocUnit(p);
	if (!pAllocUnit)
		return false;
	else
		return ValidateAllocUnit(pAllocUnit);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Validate-Methode
	\param pAllocUnit Allocation Unit 
	\return Flag

	Diese Methode pr?ft ob die ?bergebene Allocation Unit noch in Ordnung ist, das heisst dass keine
	Buffer Over- und Underflows aufgetreten sind und dass es auch noch nicht freigegeben worden ist.
*/
bool DebugMemoryManager::ValidateAllocUnit(AllocationUnit* pAllocUnit)
{
	uint_t i;
	ubyte_t* pActualAddress = reinterpret_cast<ubyte_t*>(pAllocUnit->pActualAddress);
	ubyte_t* pReportedAddress = reinterpret_cast<ubyte_t*>(pAllocUnit->pReportedAddress);
	ubyte_t* pPostfixAddress = pReportedAddress + (pAllocUnit->uiReportedSize);

	udword_t udPatternPrefix = configuration_t::PATTERN_PREFIX;
	udword_t udPatternReleased = configuration_t::PATTERN_RELEASED;
	udword_t udPatternPostfix = configuration_t::PATTERN_POSTFIX;
	const ubyte_t* pPrefixPattern = reinterpret_cast<const ubyte_t*>(&udPatternPrefix);
	const ubyte_t* pReleasedPattern = reinterpret_cast<const ubyte_t*>(&udPatternReleased);
	const ubyte_t* pPostfixPattern = reinterpret_cast<const ubyte_t*>(&udPatternPostfix);

	/*
	 *	Pr?fen auf Buffer Under- und Overflow
	 */
	uint_t uiNumBytesCorruptedFromPrefix = 0;
	uint_t uiNumBytesCorruptedFromPostfix = 0;

	for (i = 0; i < (sizeof(udword_t) * configuration_t::PADDING_SIZE); ++i)
	{
		if (pActualAddress[i] != pPrefixPattern[i % 4])
			++uiNumBytesCorruptedFromPrefix;
		if (pPostfixAddress[i] != pPostfixPattern[i % 4])
			++uiNumBytesCorruptedFromPostfix;
	}
	if (uiNumBytesCorruptedFromPrefix > 0)
	{
		size_t uiActualAddress = (size_t)pAllocUnit->pActualAddress;
		size_t uiReportedAddress = (size_t)pAllocUnit->pReportedAddress;
		cout 
			<< "Buffer underflow in Alloc Unit '" << pAllocUnit->uiAllocationNumber
			<< "' on Actual Position '" << uiActualAddress
			<< "' (reported '" << uiReportedAddress << "'), "
			<< uiNumBytesCorruptedFromPrefix << " bytes corrupted"
			<< endl
		;

		return false;
	}
	if (uiNumBytesCorruptedFromPostfix > 0)
	{
		size_t uiActualAddress = (size_t)pAllocUnit->pActualAddress;
		size_t uiReportedAddress = (size_t)pAllocUnit->pReportedAddress;
		cout
			<< "Buffer overflow in Alloc Unit '" << pAllocUnit->uiAllocationNumber
			<< "' on Actual Position '" << uiActualAddress
			<< "' (reported '" << uiReportedAddress << "'), "
			<< uiNumBytesCorruptedFromPostfix << " bytes corrupted"
			<< endl
		;

		return false;
	}

	/*
	 *	Pr?fen ob die Adresse bereits freigegeben worden ist
	 */
	bool bAllReleased = true;
	for (i = 0; i < pAllocUnit->uiReportedSize; ++i)
	{
		if (pReportedAddress[i] != pReleasedPattern[i % 4])
			bAllReleased = false;
	}
	if (bAllReleased)
	{
		size_t uiActualAddress = (size_t)pAllocUnit->pActualAddress;
		size_t uiReportedAddress = (size_t)pAllocUnit->pReportedAddress;
		cout
			<< "Allocation Unit wurde bereits freigegeben! Unit '" << pAllocUnit->uiAllocationNumber
			<< "' on Actual Position '" << uiActualAddress
			<< "' (reported '" << uiReportedAddress
			<< "')"
			<< endl
		;

		return false;
	}

	return true;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Validate-Methode
	\return Flag

	Diese Methode validiert s?mtliche Allokationen ob sie noch g?ltig sind oder nicht.
*/
bool DebugMemoryManager::ValidateAllAllocations()
{
	bool bRetVal = true;
	for (uint_t i = 0; i < m_uiHashSize; ++i)
	{
		AllocationUnit* pCurrentUnit = m_HashTable[i];
		while (pCurrentUnit)
		{
			if (!ValidateAllocUnit(pCurrentUnit))
				bRetVal = false;

			pCurrentUnit = pCurrentUnit->pNext;
		}
	}

	return bRetVal;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Derzeit allozierte Gr?sse zur?ckgeben
	\return Gr?sse

	Gibt die Anzahl Bytes welche derzeit im gesamten alloziert worden ist zur?ck.
*/
uint_t DebugMemoryManager::GetAllocatedSize()
{
	return m_Statistics.uiTotalActualMemory + m_uiReservoirBufferSize * (256 * sizeof(AllocationUnit*));
}



//-----------------------------------------------------------------------------------------------------------
/*!	\brief Vom Client allozierte Gr?sse zur?ckgeben
	\return Gr?sse

	Gibt die Anzahl Byte zur?ck welche derzeit vom Client alloziert worden sind.
*/
uint_t DebugMemoryManager::GetReportedSize()
{
	return m_Statistics.uiTotalReportedMemory;
}



//-----------------------------------------------------------------------------------------------------------
/*!	\brief Break-Point
	\param p 
	\return Flag

	Schaltet einen Breakpoint ein oder aus. Dieser breakpoint stoppt die Ausf?hrung des Programmes sofort, wenn 
	realloc auf die ?bergebene Speicherstelle aufgerufen wird. 

	Zur?ckgegeben wird ein Flag welches den aktuellen Wert des Breakpointes dieser Speicherstelle zur?ckgibt.
*/
bool DebugMemoryManager::BreakOnRealloc(void* p)
{
	AllocationUnit* pAllocUnit = _FindAllocUnit(p);
	if (pAllocUnit)
	{
		pAllocUnit->bBreakOnRealloc = !pAllocUnit->bBreakOnRealloc;
		return pAllocUnit->bBreakOnRealloc;
	}
	else
	{
		return false;
	}
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief Break-Point
	\param p
	\return Flag

	Schaltet einen Breakpoint ein oder aus. Dieser breakpoint stoppt die Ausf?hrung des Programmes sofort, wenn 
	der Speicher auf welchen der ?bergebene Pointer zeigt freigegeben wird.

	Zur?ckgegeben wird ein Flag welches den aktuellen Wert des Breakpointes dieser Speicherstelle zur?ckgibt.
*/
bool DebugMemoryManager::BreakOnDealloc(void* p)
{
	AllocationUnit* pAllocUnit = _FindAllocUnit(p);
	if (pAllocUnit)
	{
		pAllocUnit->bBreakOnDealloc = !pAllocUnit->bBreakOnDealloc;
		return pAllocUnit->bBreakOnDealloc;
	}
	else
	{
		return false;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Allocator
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde
	\param uiAllocType Typ des Operators/der Funktion von welcher aus diese Methode aufgerufen wurde.
	\param uiReportedSize Gr?sse in Bytes des Speichers der alloziert werden soll
	\return Pointer auf den allozierten Block Speicher

	Alloziert Endg?ltig den gew?nschten Speicher und gibt einen Pointer darauf zur?ck.
*/
void* DebugMemoryManager::Allocator(const char* szFile, uint_t uiLine, uint_t uiAllocType, uint_t uiReportedSize)
{
	m_uiAllocationCount++;

/*	TODO!!
	if (configuration_t::ALWAYS_LOG_ALL)
	{
	}*/

	AllocationUnit* pAllocUnit = _GetEmptyAllocUnit();
	pAllocUnit->uiActualSize = _CalculateActualSize(uiReportedSize);

	pAllocUnit->pActualAddress = malloc(pAllocUnit->uiActualSize);
	pAllocUnit->uiReportedSize = _CalculateReportedSize(pAllocUnit->uiActualSize);
	pAllocUnit->pReportedAddress = _CalculateReportedAddress(pAllocUnit->pActualAddress);
	pAllocUnit->uiAllocType = uiAllocType;
	strcpy(pAllocUnit->szSourceFile, szFile);
	pAllocUnit->uiSourceLine = uiLine;
	pAllocUnit->uiAllocationNumber = m_uiAllocationCount;
	if (pAllocUnit->uiAllocationNumber == 304)
		pAllocUnit->uiAllocationNumber = pAllocUnit->uiAllocationNumber;
	/*
	 *	Wenn das Stacktrace-Feature eingeschaltet ist, an dieser Stelle den Stacktrace machen.
	 */
	if (m_bStacktraceFeature)
	{
		string_t strStacktrace;
		//stacktrace(strStacktrace);
		_LogStacktrace(szFile, uiLine, strStacktrace, m_uiAllocationCount);
	}

	/*
	 *	Fehler-Check
	 */ 
	if (!pAllocUnit->pActualAddress)
	{
		// Handle as described in ANSI-C++
		_PutBackAllocUnit(pAllocUnit);
		return NULL;// Exception(UNICODE_STRING("Request for allocation failed. Out of memory."));
	}
	if (pAllocUnit->uiAllocType == ALLOC_UNKNOWN) 
	{
		free(pAllocUnit->pActualAddress);
		_PutBackAllocUnit(pAllocUnit);
		throw Exception(UNICODE_STRING("Der Allocation-Type muss bekannt sein!"));
	}

	/*
	 *	Diverse Operationen
	 */ 
	_InsertIntoHashtable(pAllocUnit);
	_AddToStatistics(pAllocUnit);
	// TODO RANDOM WHIPE
	_WhipeWithPattern(pAllocUnit, configuration_t::PATTERN_UNUSED);

	/*
	 *	calloc() expects the reported memory address range to be filled with 0's
	 */
	if (pAllocUnit->uiAllocType == ALLOC_CALLOC)
		memset(pAllocUnit->pReportedAddress, 0, pAllocUnit->uiReportedSize);

	/*
	 *	Eventuell Validieren
	 */
	if (configuration_t::ALWAYS_VALIDATE_ALL)
		ValidateAllAllocations();

	/*
	 *	Log the result
	 */ 
	//if (alwaysLogAll) log("[+] ---->             addr 0x%08X", (unsigned int) au->reportedAddress);

	ResetOwner();
	return pAllocUnit->pReportedAddress;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Reallocator
	\param szFile Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde
	\param uiAllocType Typ des Operators/der Funktion von welcher aus diese Methode aufgerufen wurde.
	\param uiReportedSize Gr?sse in Bytes des Speichers der alloziert werden soll
	\param p Pointer auf den alten Block Speicher
	\return Pointer auf den allozierten Block Speicher
	\todo Implementieren

	Vergr?ssert bzw. verkleinert p auf uiReportedSize-Bytes. Danach wird das Ergebnis zur?ckgegeben.
*/
void* DebugMemoryManager::Reallocator(const char* szFile, uint_t uiLine, uint_t uiAllocType, uint_t uiReportedSize, void* p)
{
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Deallocator
	\param strFileName Datei von welcher aus diese Methode aufgerufen wurde
	\param uiLine Linie von welcher aus diese Methode aufgerufen wurde
	\param iDeallocType Typ des Operators/der Funktion von welcher aus diese Methode aufgerufen wurde.
	\param p Pointer auf den freizugebenden Speicherblock

	Gibt den ?bergebenen Speicherblock frei.
*/
void DebugMemoryManager::Deallocator(const char* strFileName, uint_t uiLine, uint_t iDeallocType, void* p)
{
//	if (DebugMemoryManager::ALWAYSLOGALL)
//		k_log("DebugMemoryManager::Deallocator", "%8s of addr 0x%08X by %s: %05d\n", m_strAllocationTypes[iDeallocType], p, strFileName, uiLine);

	AllocationUnit* pAllocUnit = _FindAllocUnit(p);
	if (!pAllocUnit) 
		throw Exception(UNICODE_STRING("Request to deallocate RAM that was never allocated"));

	/*
	 *	If you hit this assert, then the allocation unit that is about to be deallocated is damaged. But you probably
	 *	already know that from a previous assert you should have seen in validateAllocUnit()
	 */
	assert(pAllocUnit);//kaos_assert(ValidateAllocUnit(pAllocUnit), UNICODE_STRING("allocation-unit besch?digt!"));

	/*
	 *	If you hit this assert, then this deallocation was made from a source that isn't setup to use this memory
	 *	tracking software, use the stack frame to locate the source and include our H file.
	 */
	assert(iDeallocType != ALLOC_UNKNOWN); // kaos_assert((iDeallocType != ALLOC_UNKNOWN), UNICODE_STRING("ram wurde von anderer stelle aus alloziert!"));

	/*
	 *	If you hit this assert, you were trying to deallocate RAM that was not allocated in a way that is compatible with
	 *	the deallocation method requested. In other words, you have a allocation/deallocation mismatch.
	 */
	assert(
		(iDeallocType == ALLOC_DELETE       && pAllocUnit->uiAllocType == ALLOC_NEW      ) ||
		(iDeallocType == ALLOC_DELETE_ARRAY && pAllocUnit->uiAllocType == ALLOC_NEW_ARRAY) ||
		(iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_MALLOC   ) ||
		(iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_CALLOC   ) ||
		(iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_REALLOC  )
	);
/*	kaos_assert(((iDeallocType == ALLOC_DELETE       && pAllocUnit->uiAllocType == ALLOC_NEW      ) ||
				 (iDeallocType == ALLOC_DELETE_ARRAY && pAllocUnit->uiAllocType == ALLOC_NEW_ARRAY) ||
				 (iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_MALLOC   ) ||
				 (iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_CALLOC   ) ||
				 (iDeallocType == ALLOC_FREE         && pAllocUnit->uiAllocType == ALLOC_REALLOC  )), 
				UNICODE_STRING("die methode zum freigeben passt nicht zur methode mit welcher das memory alloziert wurde!"));*/

	/* 
	 *	If you hit this assert, then the "break on dealloc" flag for this allocation unit is set. Interrogate the 'au'
	 *	variable to determine information about this allocation unit.
	 */
	assert(pAllocUnit->bBreakOnDealloc == false);//kaos_assert(pAllocUnit->bBreakOnDealloc == false, UNICODE_STRING("breakpoint"));

	/* 
	 *	Wipe the deallocated RAM with a new pattern. This doen't actually do us much good in debug mode under WIN32,
	 *	because Microsoft's memory debugging & tracking utilities will wipe it right after we do. Oh well.
	 */
	_WhipeWithPattern(pAllocUnit, configuration_t::PATTERN_RELEASED);

	/*
	 *	Alles freigeben
	 */ 
	free(pAllocUnit->pActualAddress);
	_RemoveFromHashtable(pAllocUnit);
	_RemoveFromStatistics(pAllocUnit);
	_PutBackAllocUnit(pAllocUnit);

	// Validate every single allocated unit in memory
	if (configuration_t::ALWAYS_VALIDATE_ALL)
		ValidateAllAllocations();

	ResetOwner();
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Debug-Methode

	Gibt alle im Moment noch allozierten Bl?cke Speicher aus. Diese Methode sollte bevorzugt am
	Ende im Destruktor aufgerufen werden, da zu diesem Zeitpunkt eigentlich keine Speicherbl?cke
	mehr alloziert sein d?rfen.
*/
void DebugMemoryManager::DumpLeakReport()
{
	// Report-File ?ffnen
	FILE* fpReport = fopen("kaos__memory__debugmanager__leaks.log", "a");
	if (!fpReport) return;

	time_t  t = time(NULL);
	tm *pTime = localtime(&t);
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "|                                          Memory leak report for:  %02d.%02d.%04d %02d:%02d:%02d                                            |\n", pTime->tm_mday, pTime->tm_mon + 1, pTime->tm_year + 1900, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "\n");
	fprintf(fpReport, "\n");

	if (m_Statistics.uiTotalAllocUnitCount)
	{
		fprintf(fpReport, "%d memory leak%s found:\n", m_Statistics.uiTotalAllocUnitCount, m_Statistics.uiTotalAllocUnitCount == 1 ? "" : "s");
	}
	else
	{
		fprintf(fpReport, "No memory leaks found!\n");
	}
	fprintf(fpReport, "\n");

	if (m_Statistics.uiTotalAllocUnitCount)
	{
		DumpAllocations(fpReport);
	}

	fclose(fpReport);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Debug-Methode
	\param fpReport Datei in welche der Report ausgegeben werden soll

	Gibt alle aktuellen Allozierten Bl?cke Speicher in die ?bergebene Datei aus.
*/
void DebugMemoryManager::DumpAllocations(FILE* fpReport)
{
	fprintf(fpReport, "Alloc.   Addr       Size       Addr       Size              BreakOn BreakOn              \n");
	fprintf(fpReport, "Number Reported   Reported    Actual     Actual    Method   Dealloc Realloc Allocated by \n");
	fprintf(fpReport, "------ ---------- ---------- ---------- ---------- -------- ------- ------- -------------------------------------------- \n");
	for (uint_t i = 0; i < m_uiHashSize; i++)
	{
		AllocationUnit* pUnit = m_HashTable[i];
		while (pUnit)
		{
			fprintf(fpReport, "%06d 0x%08X 0x%08X 0x%08X 0x%08X %-8s    %c       %c    %s:%05d\n",
					pUnit->uiAllocationNumber,
					(size_t)pUnit->pReportedAddress, pUnit->uiReportedSize,
					(size_t)pUnit->pActualAddress, pUnit->uiActualSize,
					m_strAllocationTypes[pUnit->uiAllocType].c_str(),
					pUnit->bBreakOnDealloc ? 'Y' : 'N',
					pUnit->bBreakOnRealloc ? 'Y' : 'N',
					pUnit->szSourceFile,
					pUnit->uiSourceLine);
//			fprintf(fpReport, "%s\n\n", pUnit->strStackTrace);
			pUnit = pUnit->pNext;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Debug-Methode
	\param pUnit
	\param strPrefix
	\todo Implementieren und Dokumentieren
*/
void DebugMemoryManager::DumpAllocUnit(AllocationUnit*, const char*)
{

}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Debug-Methode
	\param szFilename Name der Datei in welche der Report ausgegeben werden soll

	Gibt einen Memoryreport in die Datei strFilename aus.
*/
void DebugMemoryManager::DumpMemoryReport(const char* szFilename)
{
	// Open the report file
	FILE* fpReport = fopen(szFilename, "a");
	if (!fpReport) return;

	// Header
	time_t  t = time(NULL);
	tm *pTime = localtime(&t);
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "|                                          Memory report for:  %02d.%02d.%04d %02d:%02d:%02d                                                 |\n", pTime->tm_mday, pTime->tm_mon + 1, pTime->tm_year + 1900, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "\n");
	fprintf(fpReport, "\n");

	// Report summary
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "|                                                           T O T A L S                                                            |\n");
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "              Allocation unit count: %s\n", _InsertHochkommas(m_Statistics.uiTotalAllocUnitCount).c_str());
	fprintf(fpReport, "            Reported to application: %s\n", _MemorySizeString(m_Statistics.uiTotalReportedMemory).c_str());
	fprintf(fpReport, "         Actual total memory in use: %s\n", _MemorySizeString(m_Statistics.uiTotalActualMemory).c_str());
	fprintf(fpReport, "           Memory tracking overhead: %s\n", _MemorySizeString(m_Statistics.uiTotalActualMemory - m_Statistics.uiTotalReportedMemory).c_str());
	fprintf(fpReport, "\n");

	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "|                                                            P E A K S                                                             |\n");
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "              Allocation unit count: %s\n", _InsertHochkommas(m_Statistics.uiPeakAllocUnitCount).c_str());
	fprintf(fpReport, "            Reported to application: %s\n", _MemorySizeString(m_Statistics.uiPeakReportedMemory).c_str());
	fprintf(fpReport, "                             Actual: %s\n", _MemorySizeString(m_Statistics.uiPeakActualMemory).c_str());
	fprintf(fpReport, "           Memory tracking overhead: %s\n", _MemorySizeString(m_Statistics.uiPeakActualMemory - m_Statistics.uiPeakReportedMemory).c_str());
	fprintf(fpReport, "\n");

	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "|                                                      A C C U M U L A T E D                                                       |\n");
	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
	fprintf(fpReport, "              Allocation unit count: %s\n", _InsertHochkommas(m_Statistics.uiAccumulatedAllocUnitCount).c_str());
	fprintf(fpReport, "            Reported to application: %s\n", _MemorySizeString(m_Statistics.uiAccumulatedReportedMemory).c_str());
	fprintf(fpReport, "                             Actual: %s\n", _MemorySizeString(m_Statistics.uiAccumulatedActualMemory).c_str());
	fprintf(fpReport, "\n");

//	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
//	fprintf(fpReport, "|                                                           U N U S E D                                                            |\n");
//	fprintf(fpReport, " ---------------------------------------------------------------------------------------------------------------------------------- \n");
//	fprintf(fpReport, "    Memory allocated but not in use: %s\n", _MemorySizeString(CalcAllUnused()));
//	fprintf(fpReport, "\n");

	DumpAllocations(fpReport);

	fclose(fpReport);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Get-Methode
	\return Statistiken

	Gibt die aktuellen Statistiken des MemManagers zur?ck.
*/
Statistics& DebugMemoryManager::GetMemoryStatistics()
{
	return m_Statistics;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Leere Allocation Unit zur?ckgeben
	\return Allocation Unit

	Gibt eine leere Allocation Unit zur?ck. Falls keine mehr vorhanden sind wird eine weitere alloziert,
	andernfalls wird eine der bereits vorhandenen Allocation Units zur?ckgegeben.
*/
AllocationUnit* DebugMemoryManager::_GetEmptyAllocUnit()
{
	/*
	 *	Im Reservoir liegen die freien Allocation Units. Wenn das Reservoir leer ist sollten
	 *	256 neue Allocation Units alloziert werden.
	 */
	if (!m_pReservoir)
	{
		m_pReservoir = static_cast<AllocationUnit*>(malloc(sizeof(AllocationUnit) * 256));
		memset(m_pReservoir, 0, sizeof(AllocationUnit)*256);

		// Danger Will Robinson!
		assert(m_pReservoir);//kaos_assert(m_pReservoir, UNICODE_STRING("fehler beim allozieren f?r RAM f?r internes Memory-Tracking"));
		if (!m_pReservoir) 
			throw Exception(UNICODE_STRING("Unable to allocate RAM for internal memory tracking data"));

		//memset(m_pReservoir, 0, sizeof(AllocationUnit) * 256);
		for (uint_t i = 0; i < 256 - 1; i++)
			m_pReservoir[i].pNext = &m_pReservoir[i + 1];
		m_pReservoir[255].pNext = NULL;

		AllocationUnit** ppTemp = static_cast<AllocationUnit**>(malloc(sizeof(AllocationUnit*) * (m_uiReservoirBufferSize + 1)));
		memcpy(ppTemp, m_ppReservoirBuffer, sizeof(AllocationUnit*) * m_uiReservoirBufferSize);
		free(m_ppReservoirBuffer);
		m_ppReservoirBuffer = ppTemp;
		m_ppReservoirBuffer[m_uiReservoirBufferSize] = m_pReservoir;
		++m_uiReservoirBufferSize;
	}

	assert(m_pReservoir);//kaos_assert(m_pReservoir, UNICODE_STRING("Fehler! m_pReservoir darf nicht mehr NULL sein"));

	/*
	 *	Allocation Unit holen
	 */
	AllocationUnit* pAllocUnit = m_pReservoir;
	m_pReservoir = m_pReservoir->pNext;

	/*
	 *	Allocation Unit leeren
	 */
	pAllocUnit->uiActualSize = 0;
	pAllocUnit->uiReportedSize = 0;
	pAllocUnit->pActualAddress = NULL;
	pAllocUnit->pReportedAddress = NULL;
	strcpy(pAllocUnit->szSourceFile, "");
	pAllocUnit->uiAllocType = 0;
	pAllocUnit->uiSourceLine = 0;
	pAllocUnit->bBreakOnDealloc = false;
	pAllocUnit->bBreakOnRealloc = false;
	pAllocUnit->uiAllocationNumber = 0;
	pAllocUnit->pNext = NULL;
	pAllocUnit->pPrev = NULL;
	return pAllocUnit;
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief AllocationUnit zur?cklegen
	\param pAllocUnit

	Legt die ?bergebene Allocation Unit zur?ck in den Pool der frei benutzbaren Allocation Units.
*/
void DebugMemoryManager::_PutBackAllocUnit(AllocationUnit* pAllocUnit)
{
	pAllocUnit->pNext = m_pReservoir;
	m_pReservoir = pAllocUnit;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param p Pointer auf die ReportedAddress
	\return Die gefundene AllocUnit oder NULL

	Sucht die MemoryAllocationUnit zu welcher p geh?rt. Falls keine gefunden werden kann
	wird NULL zur?ckgegeben.
*/
AllocationUnit* DebugMemoryManager::_FindAllocUnit(void* p)
{
	assert(p);//kaos_assert(p, UNICODE_STRING("ung?ltiger Wert f?r p (NULL) wurde ?bergeben"));

	uint_t uiHashIndex = _ComputeHashFromAddress(p);
	AllocationUnit* pAllocUnit = m_HashTable[uiHashIndex];
	while (pAllocUnit)
	{
		if (pAllocUnit->pReportedAddress == p)
			return pAllocUnit;

		pAllocUnit = pAllocUnit->pNext;
	}

	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Mit Pattern f?llen
	\param pAllocUnit Allocation Unit welche mit den Patterns gef?llt werden soll
	\param udSpecialPattern Das Pattern welches f?r den Teil der Daten verwendet werden soll, welche
			danach der User benutzt, d.h. entweder UNUSED oder RELEASED-Pattern

	F?llt die ?bergebene Allocation Unit mit den definierten Pattern, damit das 
	Memory besser klassifiziert und Buffer-Over- und Underflows gefunden werden k?nnen.
*/
void DebugMemoryManager::_WhipeWithPattern(AllocationUnit* pAllocUnit, const udword_t udSpecialPattern)
{
	ubyte_t* pActualAddress = (ubyte_t*)pAllocUnit->pActualAddress;
	ubyte_t* pReportedAddress = (ubyte_t*)pAllocUnit->pReportedAddress;

	udword_t udPatternPrefix = configuration_t::PATTERN_PREFIX;
	udword_t udPatternPostfix = configuration_t::PATTERN_POSTFIX;
	const ubyte_t* pPrefixPattern = reinterpret_cast<const ubyte_t*>(&udPatternPrefix);
	const ubyte_t* pUnusedPattern = reinterpret_cast<const ubyte_t*>(&udSpecialPattern);
	const ubyte_t* pPostfixPattern = reinterpret_cast<const ubyte_t*>(&udPatternPostfix);
	uint_t i;

	/*
	 *	Buffer f?llen (Schutz vor Under- und Overflows)
	 */
	for (i = 0; i < (sizeof(udword_t) * configuration_t::PADDING_SIZE); ++i) 
	{
		pActualAddress[i] = pPrefixPattern[i % 4];
		pReportedAddress[i + pAllocUnit->uiReportedSize] = pPostfixPattern[i % 4];
	}

	/*
	 *	Mit unused-Byte-Pattern f?llen
	 */
	for (i = 0; i < pAllocUnit->uiReportedSize; ++i)
		pReportedAddress[i] = pUnusedPattern[i % 4];
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Alles Memory freigeben
	
	Gibt alles noch allozierte Memory frei.
*/
void DebugMemoryManager::_Cleanup()
{
	free(m_HashTable);

	if (m_ppReservoirBuffer)
	{
		for (uint_t i = 0; i < m_uiReservoirBufferSize; ++i)
		{
			AllocationUnit* pUnits = m_ppReservoirBuffer[i];
			free(pUnits);
			m_ppReservoirBuffer[i] = NULL;
		}
		free(m_ppReservoirBuffer);
		m_ppReservoirBuffer = NULL;
		m_uiReservoirBufferSize = 0;
		m_pReservoir = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param uiValue Wert welcher bearbeitet werden soll.
	\return String von uiValue

	Wandelt uiValue in einen String um und f?gt Hochkommas an den n?tigen Stellen ein.
*/
string_t DebugMemoryManager::_InsertHochkommas(uint_t uiValue)
{
	char szTemp[512];
	sprintf(szTemp, "%d", uiValue);
	string_t strNumber = szTemp;

	uint_t uiIndex = 3;
	while (true)
	{
		if (strNumber.length() > uiIndex)
		{
			string_t strCopy = strNumber.substr(0, strNumber.length() - uiIndex);
			strCopy += UNICODE_STRING("\'");
			strCopy += strNumber.substr(strNumber.length() - uiIndex);
			strNumber = strCopy;

			uiIndex += 4;
		}
		else
		{
			return strNumber;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param uiValue Wert welcher bearbeitet werden soll
	\return String von uiValue

	Wandelt uiValue in einen String um und erg?nzt ihn mit Gr?ssenabgaben.
*/
string_t DebugMemoryManager::_MemorySizeString(uint_t uiValue)
{
	if (uiValue > (1024 * 1024))
	{
		/*
		 *	float_t fValue = kaos_convert<float_t>(uiValue);
		 *
		 *	Keine KAOS-Conversion an dieser Stelle, da wir Wissen, dass eventuell ein Genauigkeitsverlust
		 *	auftreten könnte! -> kaos_convert würde sonst eine Exception werfen was hier nicht so viel 
		 *	Sinn macht
		 */
		float_t fValue = (float_t)uiValue; 
		float fMegaBytes = fValue / (1024.0f * 1024.0f);
		/*
		 *	String bearbeiten
		 */
		char szTemp[512];
		sprintf(szTemp, "%f", fMegaBytes);
		string_t strMegaBytes = szTemp;
		size_t uiIndex = strMegaBytes.find_first_of(UNICODE_STRING('.'));
		strMegaBytes = strMegaBytes.substr(0, uiIndex + 3);

		/*
		 *	Hochkomma-String und zur?ckgeben
		 */
		string_t strWithHochkommas = _InsertHochkommas(uiValue);
		return strWithHochkommas + 
			   UNICODE_STRING("(") + 
			   strMegaBytes + 
			   UNICODE_STRING("MB )");
	}

	if (uiValue > 1024)
	{
		/*
		 *	float_t fValue = kaos_convert<float_t>(uiValue);
		 *
		 *	Keine KAOS-Conversion an dieser Stelle, da wir Wissen, dass eventuell ein Genauigkeitsverlust
		 *	auftreten könnte! -> kaos_convert würde sonst eine Exception werfen was hier nicht so viel 
		 *	Sinn macht
		 */
		float_t fValue = (float_t)uiValue;
		float fKiloBytes = fValue / 1024.0f;
		
		/*
		 *	String bearbeiten
		 */
		char szTemp[512];
		sprintf(szTemp, "%f", fKiloBytes);
		string_t strKiloBytes = szTemp;
		size_t uiIndex = strKiloBytes.find_first_of(UNICODE_STRING('.'));
		strKiloBytes = strKiloBytes.substr(0, uiIndex + 3);

		/*
		 *	Hochkomma-String und zur?ckgeben
		 */
		string_t strWithHochkommas = _InsertHochkommas(uiValue);
		return strWithHochkommas + 
			   UNICODE_STRING("(") + 
			   strKiloBytes + 
			   UNICODE_STRING("KB)");
	}

	string_t strWithHochkommas = _InsertHochkommas(uiValue);
	return strWithHochkommas + 
			UNICODE_STRING(" Bytes");
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Hashwert berechnen
	\param p
	\return Der berechnete Wert

	Berechnet aus der ?bergebenen Adresse den Hashwert f?r die interne HashTable.
*/
uint_t DebugMemoryManager::_ComputeHashFromAddress(void* p)
{
	return ((uint_t)p >> 4) & (m_uiHashSize - 1);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief In die Hashtable einf?gen
	\param pAllocUnit

	F?gt die ?bergebene Allocation Unit in die Hashtable ein.
*/
void DebugMemoryManager::_InsertIntoHashtable(AllocationUnit* pAllocUnit)
{
	uint_t uiHashIndex = _ComputeHashFromAddress(pAllocUnit->pReportedAddress);
	
	if (m_HashTable[uiHashIndex]) 
		m_HashTable[uiHashIndex]->pPrev = pAllocUnit;
	pAllocUnit->pNext = m_HashTable[uiHashIndex];
	pAllocUnit->pPrev = NULL;
	m_HashTable[uiHashIndex] = pAllocUnit;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Aus der Hashtable entfernen
	\param pAllocUnit

	Entfernt die ?bergebene Allocation Unit aus der Hashtable.
*/
void DebugMemoryManager::_RemoveFromHashtable(AllocationUnit* pAllocUnit)
{
	uint_t uiHashIndex = _ComputeHashFromAddress(pAllocUnit->pReportedAddress);
	if (m_HashTable[uiHashIndex] == pAllocUnit)
	{
		m_HashTable[uiHashIndex] = pAllocUnit->pNext;
		if(m_HashTable[uiHashIndex]) 
			m_HashTable[uiHashIndex]->pPrev = NULL;
	}
	else
	{
		if (pAllocUnit->pPrev) 
			pAllocUnit->pPrev->pNext = pAllocUnit->pNext;
		if (pAllocUnit->pNext) 
			pAllocUnit->pNext->pPrev = pAllocUnit->pPrev;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param uiReportedSize Die ReportedSize des Memoryblocks
	\return Die wirkliche Gr?sse des Memoryblocks

	Berechnet die wirkliche Gr?sse eines Blocks aus dessen ReportedSize.
*/
const uint_t DebugMemoryManager::_CalculateActualSize(const uint_t uiReportedSize) const
{
	return (uiReportedSize + 2 * sizeof(udword_t) * configuration_t::PADDING_SIZE);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param uiActualSize Die gesamte Gr?sse des Blocks
	\return Die ReportedSize des Blocks

	Berechnet die ReportedSize aus der gesamten Gr?sse eines Blocks
*/
const uint_t DebugMemoryManager::_CalculateReportedSize(const uint_t uiActualSize) const
{
	return (uiActualSize - 2 * sizeof(udword_t) * configuration_t::PADDING_SIZE);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Helper-Methode
	\param pActualAddress Wirliche Adresse des Beginns eines Blocks
	\return pReportedAddress

	Berechnet pReportedAddress aus pActualAddress.
*/
void* DebugMemoryManager::_CalculateReportedAddress(void* pActualAddress)
{
	if (!pActualAddress) 
		return NULL;

	ubyte_t* p = static_cast<ubyte_t*>(pActualAddress);
	return (p + sizeof(udword_t) * configuration_t::PADDING_SIZE);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Allocation Unit zur Statistik hinzuf?gen
	\param pAllocUnit

	F?gt die Eigenschaften/Gr?ssen zur Statistik hinzu (aufgerufen aus der Allocation-Methode).
*/
void DebugMemoryManager::_AddToStatistics(AllocationUnit* pAllocUnit)
{
	m_Statistics.uiAccumulatedActualMemory += pAllocUnit->uiActualSize;
	m_Statistics.uiAccumulatedReportedMemory += pAllocUnit->uiReportedSize;
	m_Statistics.uiAccumulatedAllocUnitCount++;
	m_Statistics.uiTotalActualMemory += pAllocUnit->uiActualSize;
	m_Statistics.uiTotalReportedMemory += pAllocUnit->uiReportedSize;
	m_Statistics.uiTotalAllocUnitCount++;

	if (m_Statistics.uiTotalActualMemory > m_Statistics.uiPeakActualMemory) 
		m_Statistics.uiPeakActualMemory = m_Statistics.uiTotalActualMemory;
	if (m_Statistics.uiTotalReportedMemory > m_Statistics.uiPeakReportedMemory) 
		m_Statistics.uiPeakReportedMemory = m_Statistics.uiTotalReportedMemory;
	if (m_Statistics.uiTotalAllocUnitCount > m_Statistics.uiPeakAllocUnitCount)
		m_Statistics.uiPeakAllocUnitCount = m_Statistics.uiTotalAllocUnitCount;
}





//-----------------------------------------------------------------------------------------------------------
/*!	\brief Allocation Unit von der Statistik entfernen
	\param pAllocUnit

	Zieht die Gr?ssen der ?bergebenen Unit von den entsprechenden Membern der Statistik wieder ab.
*/
void DebugMemoryManager::_RemoveFromStatistics(AllocationUnit* pAllocUnit)
{
	m_Statistics.uiTotalActualMemory -= pAllocUnit->uiActualSize;
	m_Statistics.uiTotalReportedMemory -= pAllocUnit->uiReportedSize;
	m_Statistics.uiTotalAllocUnitCount--;
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Alle Logfiles leeren

	Leert s?mtliche Logfiles des Memory-Managers.
*/
void DebugMemoryManager::_ClearLogfiles()
{
	FILE* fp = NULL;

	string_t strDebug = "kaos__memory__debugmanager__debug.log";
	string_t strLeaks = "kaos__memory__debugmanager__leaks.log";
	string_t strReport = "kaos__memory__debugmanager__report.log";
	string_t strStacktraces = "kaos__memory__debugmanager__stacktraces.log";

	fp = fopen(strDebug.c_str(), "w");
	fclose(fp);
	fp = fopen(strLeaks.c_str(), "w");
	fclose(fp);
	fp = fopen(strReport.c_str(), "w");
	fclose(fp);
	fp = fopen(strStacktraces.c_str(), "w");
	fclose(fp);
}




//-----------------------------------------------------------------------------------------------------------
/*!	\brief Stacktrace ausgeben
	\param strFile Datei von welcher aus alloziert wurde
	\param uiLine Zeile von welcher aus alloziert wurde
	\param strStacktrace der aktuelle Stacktrace
	\param uiAllocationNumber die Allocation-Number mit welcher danach bei den Mem-Leaks zum Beispiel 
			stacktraces identifiziert werden k?nnen

	Loggt den Stacktrace der Allocation mit der ?bergeben Nummer.
*/
void DebugMemoryManager::_LogStacktrace(const string_t& strFile,
										const uint_t uiLine, 
										const string_t& strStacktrace,
										const uint_t uiAllocationNumber)
{
	FILE* fpOut = fopen("kaos__memory__debugmanager__stacktraces.log", "a");
	if (!fpOut)
		return;

	fprintf(fpOut, "--------------------------------------------------------------------------------------\n"
				   "Allocation (Number '%d') in '%s:%d':\n"
				   "Callstack: \n"
				   "%s\n"
				   "\n",
				   uiAllocationNumber, strFile.c_str(), uiLine, strStacktrace.c_str());

	fclose(fpOut);
}

}} // x::debugmanager
