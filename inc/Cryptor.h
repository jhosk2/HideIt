/*
 * Cryptor.h
 *
 *  Created on: Dec 10, 2014
 *      Author: jowonbin
 */

#ifndef CRYPTOR_H_
#define CRYPTOR_H_

#include <FApp.h>
#include <FBase.h>
#include <FIo.h>

using namespace Tizen::Base;
using namespace Tizen::Io;

const int secretKeyLen = 8;
static const byte secretKey[secretKeyLen] = {
    0x62, 0x5C, 0xC7, 0x7E, 0xEA, 0x7B, 0xA5, 0x4D
};

const int ivectorLen = 8;
static const byte ivector[ivectorLen] = {
	0x3E, 0xB5, 0x01, 0x45, 0xE4, 0xF8, 0x75, 0x3F
};

const int blocksize = 16;

class Cryptor {
public:
	Cryptor();
	virtual ~Cryptor();

	void Encrypt( const String& filePath );
	void Decrypt( const String& encryptedFilePath, bool isTemp = false );

private :
	bool InitDatabase();
	void GetSrcPath(const String& encryptedPath, String& out);
	void GetSrcSize(const String& encryptedPath, long long int& out);
	bool AddRow(const String& src, const String& dest);
	bool RemoveRow(const String& src);

	const String dbName;
    Database* pDatabase;
};

#endif /* CRYPTOR_H_ */
