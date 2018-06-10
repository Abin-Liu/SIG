///////////////////////////////////////////////////////////////////////
// LoaderEx.h
//
// Loading/unloading D2Hackit using specified d2 hwnd, d2 executable
// name and d2hackit full path name from a dll.
//
// Most code were copied from Thohell's Loader.exe, just that it has
// been made more convenient for being used by SIG.
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////////////////

#ifndef __LOADEREX_H__
#define __LOADEREX_H__

#include <windows.h>

// Check whether D2hackit was loaded against "hD2Wnd"
BOOL __declspec(dllexport) IsD2HackitLoaded(HWND hD2Wnd, LPCSTR lpszD2ExeName);

// Load D2Hackit, it will return TRUE if D2hackit is loaded, or was already loaded
BOOL __declspec(dllexport) LoadD2Hackit(HWND hD2Wnd, LPCSTR lpszD2ExeName, LPCSTR lpszD2HackitPathName);

// Unload D2Hackit, it will return TRUE if D2hackit is unloaded, or was not loaded
BOOL __declspec(dllexport) UnloadD2Hackit(HWND hD2Wnd, LPCSTR lpszD2ExeName, LPCSTR lpszD2HackitPathName);

#endif // __LOADEREX_H__