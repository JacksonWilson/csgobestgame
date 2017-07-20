#pragma once
#include "stdafx.h"

class VMTHook {
	PDWORD* ppClassBase;
	PDWORD pOriginalVMTable;
	PDWORD pNewVMTable;
	DWORD old;
	std::uint32_t dwLength;
	bool replace;

	VMTHook(PDWORD* ppClass, bool bReplace)
	{
		ppClassBase = ppClass;
		replace = bReplace;

		pOriginalVMTable = *ppClassBase;

		if (replace)
		{
			while (IsBadCodePtr((FARPROC)pOriginalVMTable[dwLength]))
			{
				dwLength++;
			}

			pNewVMTable = new DWORD[dwLength];
			std::memcpy(pNewVMTable, pOriginalVMTable, dwLength * sizeof(DWORD));

			VirtualProtect(ppClassBase, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
			*ppClassBase = pNewVMTable;
			VirtualProtect(ppClassBase, sizeof(DWORD), old, &old);
		}
	}

	~VMTHook()
	{
		ppClassBase = &pOriginalVMTable;
		delete[] pNewVMTable;
	}

	template <class T>
	T Hook(uint32_t index, T fnNew) 
	{
		DWORD dwOld = (DWORD)pOriginalVMTable[index];
		pNewVMTable[index] = (DWORD)fnNew;
		
		return (T)dwOld;
	}

	void Unhook(uint32_t index)
	{
		pNewVMTable[index] = pOriginalVMTable[index];
	}
};
