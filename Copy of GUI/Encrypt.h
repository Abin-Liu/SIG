//////////////////////////////////////////////////////////////////////////
//    Encrypt.h
//
// Functions exported by this file are used for text encryption & decryption,
// with optional password protection.
//
// 4/08/2003 Abin
//////////////////////////////////////////////////////////////////////////

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <windows.h>

// The max length of a string that can be encrypted, to guarantee the output
// string not to exceed 32767(32KB) chars (limited by Win9x).
#define MAX_ENCRYPT_LENGTH	16000

// Operation results
enum ERRORCODE { ERR_OK = 0,				// Operation completed successful
				 ERR_NULL_STRING,			// Invalid string source(null pointer)
				 ERR_LENGTH_EXCEED,			// Source string length exceeds the limit
				 ERR_INSUFFICIENT_BUFFER,	// Insufficient buffer size
				 ERR_INVALID_PASSWORD,		// Invalid password
				 ERR_INVALID_SRC,			// Invalid source string
				 ERR_MORE_CHARS,			// There are more characters to be decrypted
				 ERR_INVALID_CHECKSUM };	// Checksum examination failed

///////////////////////////////////////////////////////////////////////////////////////////////
//			GetErrorMessage
//
// Get string description of error codes returned by "GetOpenLength", "Encrypt" and "Decrypt"
///////////////////////////////////////////////////////////////////////////////////////////////
void __declspec(dllexport) GetErrorMessage(int nErrorCode, // The error code
										   LPSTR lpBuffer, // Buffer to receive description
										   DWORD dwMaxBufferSize);

///////////////////////////////////////////////////////////////////////////////////////////////
//			GetEncryptLength
//
// Forecast the string length after encryption.
///////////////////////////////////////////////////////////////////////////////////////////////
WORD __declspec(dllexport) GetEncryptLength(LPCSTR lpSrc); // Source string(normal)

///////////////////////////////////////////////////////////////////////////////////////////////
//			GetDecryptLength
//
// Forecast the string length after decryption.
// Returns ERR_OK if succeeds, an error code if fails.
///////////////////////////////////////////////////////////////////////////////////////////////
int __declspec(dllexport) GetDecryptLength(LPCSTR lpSrc, // Source string(encrypted)
										   LPCSTR lpPassword,
										   WORD& wLength); // Receive the forecast result

///////////////////////////////////////////////////////////////////////////////////////////////
//           IsPasswordProtected
//
// Used before descryption, to determine whether the source string is password protected.
// Returns ERR_OK if succeeds, an error code if fails.
///////////////////////////////////////////////////////////////////////////////////////////////
int __declspec(dllexport) IsPasswordProtected(LPCSTR lpSrc, BOOL& bProtected);

///////////////////////////////////////////////////////////////////////////////////////////////
//			Encrypt
//
// Encrypt a string.
// Returns ERR_OK if succeeds, an error code if fails.
///////////////////////////////////////////////////////////////////////////////////////////////
int __declspec(dllexport) Encrypt(LPCSTR lpSrc, // Source string(normal)
								  LPSTR lpBuffer, // Buffer to receive encrypted string
								  DWORD dwMaxBufferSize,
								  LPCTSTR lpPassword = NULL);

///////////////////////////////////////////////////////////////////////////////////////////////
//			Decrypt
//
// Decrypt a string.
// Returns ERR_OK if succeeds, an error code if fails.
///////////////////////////////////////////////////////////////////////////////////////////////
int __declspec(dllexport) Decrypt(LPCSTR lpSrc, // Source string(encrypted)
								  LPSTR lpBuffer, // Buffer to receive decrypted string
								  DWORD dwMaxBufferSize,
								  LPCTSTR lpPassword = NULL);

#endif // #ifndef __ENCRYPT_H__
