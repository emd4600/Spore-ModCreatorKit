/****************************************************************************
* Copyright (C) 2019 Eric Mor
*
* This file is part of Spore ModCreatorKit.
*
* Spore ModAPI is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "stdafx.h"
#include "DetouredDBPF.h"

#include "Debugging.h"
#include "FilePFRecord.h"

#include <Spore\App\ICheatManager.h>
#include <Spore\Resource\cResourceManager.h>

DetourDeclaration(DetouredDBPF::GetFile);

using namespace Resource;

long DetouredDBPF::AttachDetour()
{
	SetDetourAddress(GetFile, GetMethodAddress(Resource::DatabasePackedFile, GetFile));

	long error = 0;
	error |= AttachDetourFunction(GetFile, DetouredGetFile_redirected);

	return error;
}


// virtual bool Resource::DatabasePackedFile::GetFile(const ResourceKey& name, IPFRecord** ppDst, int nDesiredAccess, int nCreateDisposition, bool arg_10, DBPFItem* pDstInfo)
bool DetouredDBPF::DetouredGetFile(const ResourceKey& name, IPFRecord** ppDst, int nDesiredAccess, int nCreateDisposition, bool arg_10, DBPFItem* pDstInfo)
{

	DebugInformation* pDebugInformation = nullptr;
	string16 filePath;

	if (nDesiredAccess == kAccessFlagRead  // we only accept reading
		&& ppDst != nullptr  // no point on doing this if we are not going to get the file
		&& Debugging::Get().GetDebugInformation(this, &pDebugInformation)  // if we have debug information
		&& pDebugInformation->GetFilePath(name, &filePath))  // if this file has a path to debug
	{
		App::ICheatManager* cm = App::ICheatManager::Get();
		if (cm != nullptr)
		{
			wstring dstName;
			IResourceManager::Get()->GetFileName(name, dstName);
			ArgScript::PrintF(cm->GetArgScript(), "File %ls accessed.", dstName);

			// MessageBoxW(NULL, dstName.c_str(), L"File accessed", MB_OK);
		}


		bool result = false;
		FilePFRecord* pRecord = nullptr;

		mIndexMutex.Lock(Mutex::kDefaultWait);

		DBPFItem* info = mpIndex->GetFileInfo(name);
		if (info != nullptr)
		{
			if (pDstInfo != nullptr)
			{
				*pDstInfo = *info;
			}

			if (nCreateDisposition == kCDDefault)
			{
				nCreateDisposition = kCDOpenExisting;
			}


			auto it = field_2E8.find(name);
			if (it != field_2E8.end())
			{
				if (it->second->GetType() == FilePFRecord::kType)
				{
					pRecord = new (mpAllocator, "FilePFRecord") FilePFRecord((FilePFRecord*)it->second, name, this);
				}
			}
			else
			{
				pRecord = new (mpAllocator, "FilePFRecord") FilePFRecord(*info, name, this);
			}

			if (pRecord != nullptr)
			{
				pRecord->SetPath(filePath.c_str());

				pRecord->AddRef();

				if (mnFileAccess & kAccessFlagWrite)
				{
					field_2E8[name] = pRecord;
				}
			}

			if (ppDst != nullptr)
			{
				*ppDst = pRecord;
			}

		}

		mIndexMutex.Unlock();

		if (pRecord != nullptr)
		{
			pRecord->AddRef();
			result = true;
		}

		return result;
	}
	else
	{
		return GetFile_original(this, name, ppDst, nDesiredAccess, nCreateDisposition, arg_10, pDstInfo);
	}
}

