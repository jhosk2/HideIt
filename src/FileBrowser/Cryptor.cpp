/*
 * Cryptor.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: jowonbin
 */

#include "Cryptor.h"
#include "FolderBrowser.h"

Cryptor::Cryptor()
	: dbName( Tizen::App::App::GetInstance()->GetAppDataPath() + L"HideIt.db"){
	InitDatabase();
}

Cryptor::~Cryptor() {
    delete pDatabase;
    pDatabase = null;
}

void Cryptor::Encrypt(const String& filePath )
{
   ByteBuffer keyBytes;
   keyBytes.Construct(secretKeyLen);
   keyBytes.SetArray(secretKey, 0, secretKeyLen);
   keyBytes.Flip();

   String encryptedFilePath(HIDEITPATH);
   encryptedFilePath.Append(File::GetFileName(filePath));

   AddRow(filePath, encryptedFilePath);

   File::ConvertToSecureFile(filePath,encryptedFilePath, keyBytes);
}


void Cryptor::Decrypt( const String& encryptedFilePath, bool isTemp )
{
    ByteBuffer keyBytes;
    keyBytes.Construct(secretKeyLen);
    keyBytes.SetArray(secretKey, 0, secretKeyLen);
    keyBytes.Flip();

	String decryptedFilePath;
	if ( isTemp )
	{
		String dest;
		GetSrcPath(encryptedFilePath, dest);
		String ext = File::GetFileExtension(dest);

		decryptedFilePath.Append(TEMPPATH);
		decryptedFilePath.Append(File::GetFileExtension(encryptedFilePath));
	}
	else
	{
		String dest;
		GetSrcPath(encryptedFilePath, dest);
		decryptedFilePath.Append(dest);
	}

	File encrypted;
	encrypted.Construct(encryptedFilePath,"r",keyBytes);

	File output;
	output.Construct(decryptedFilePath, "w");

	long long int size;
	GetSrcSize(encryptedFilePath, size);

	ByteBuffer buffer;
	buffer.Construct((int)size);
	result r = E_SUCCESS;

	buffer.Clear();
	r = encrypted.Read(buffer);
	output.Write(buffer);

	if ( !isTemp )
		RemoveRow(encryptedFilePath);
}

bool Cryptor::InitDatabase()
{
    pDatabase = new Database();
    if (!pDatabase)
    {
        return false;
    }

    result r = pDatabase->Construct(dbName, "a+");
    if (IsFailed(r))
    {
        return false;
    }

    String sql;
    sql.Append(L"CREATE TABLE IF NOT EXISTS HideIt ( src TEXT, encrypted TEXT, size INT64)");
    r = pDatabase->ExecuteSql(sql, true);
	if (IsFailed(r))
	{
		return false;
	}

	return true;
}

bool Cryptor::AddRow(const String& src, const String& dest)
{
	RemoveRow(dest);

	AppLog("Insert rows:");
	pDatabase->BeginTransaction();

	FileAttributes attr;
	File::GetAttributes(src, attr);

    String sql;
    sql.Append(L"INSERT INTO HideIt (src, encrypted, size) VALUES ('");
    sql.Append(src);
    sql.Append("', '");
    sql.Append(dest);
    sql.Append("', ");
    sql.Append(attr.GetFileSize());
    sql.Append(");");

    result r = pDatabase->ExecuteSql(sql, true);

    if (r != E_SUCCESS)
    {
    	pDatabase->RollbackTransaction();
    	switch( r )
    	{
    	case E_SUCCESS:
    	break;
    	case E_INVALID_ARG:
    	break;
    	case E_INVALID_OPERATION:
    	break;
    	case E_OBJECT_LOCKED:
    	break;
    	case E_INVALID_FORMAT:
    	break;
    	case E_STORAGE_FULL:
    	break;
    	case E_IO:
    	break;
    	case E_SYSTEM:
    	break;
    	}
    	return false;
    }

    pDatabase->CommitTransaction();

    return true;
}

void Cryptor::GetSrcPath(const String& encryptedPath, String& out)
{
	String sql = L"SELECT src FROM HideIt WHERE encrypted = '";
	sql.Append(encryptedPath);
	sql.Append("';");

    DbEnumerator* pEnum = pDatabase->QueryN(sql);
	if ( pEnum )
    {
        String strVal;
        while (pEnum->MoveNext() == E_SUCCESS)
        {
        	pEnum->GetStringAt(0, out);
        }
    }
}

void Cryptor::GetSrcSize(const String& encryptedPath, long long int& out)
{
	String sql = L"SELECT size FROM HideIt WHERE encrypted = '";
	sql.Append(encryptedPath);
	sql.Append("';");

    DbEnumerator* pEnum = pDatabase->QueryN(sql);
	if ( pEnum )
    {
        String strVal;
        while (pEnum->MoveNext() == E_SUCCESS)
        {
        	pEnum->GetInt64At(0, out);
        }
    }
}

bool Cryptor::RemoveRow(const String& src)
{
    AppLog("Delete rows:");
    pDatabase->BeginTransaction();

    String sql;
    sql.Append(L"DELETE FROM HideIt WHERE encrypted = '");
    sql.Append(src);
    sql.Append("';");

    result r = pDatabase->ExecuteSql(sql, true);

    if (r != E_SUCCESS)
    {
    	pDatabase->RollbackTransaction();
    	return false;
    }

    pDatabase->CommitTransaction();

    return true;
}
