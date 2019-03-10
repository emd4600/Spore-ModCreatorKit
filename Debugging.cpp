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
#include "Debugging.h"
#include "DetouredDBPF.h"
#include "DetouredResourceManager.h"

#include <Spore\Hash.h>
#include <Spore\App\IPropManager.h>
#include <Spore\Resource\IResourceManager.h>
#include <Spore\Resource\DatabasePackedFile.h>
#include <Spore\Resource\PFIndexModifiable.h>

#include <EASTL\string.h>

using namespace IO;

Debugging Debugging::sMain;

Debugging::Debugging()
	: mPackageMap()
{

}
Debugging::~Debugging()
{
	try
	{
		for (auto it = mPackageMap.cbegin(); it != mPackageMap.cend(); ++it)
		{
			if (it->second != nullptr)
			{
				delete it->second;
			}
		}
	}
	catch(int) {};
}

long Debugging::AttachDetour()
{
	long result = 0;

	result |= DetouredDBPF::AttachDetour();
	result |= DetouredResourceManager::AttachDetour();

	return result;
}

bool Debugging::GetDebugInformation(const Resource::DBPF* pDBPF, DebugInformation** ppDst) const
{
	auto it = mPackageMap.find(pDBPF);
	if (it != mPackageMap.end())
	{
		if (ppDst != nullptr) {
			*ppDst = it->second;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Debugging::Initialize()
{
	sMain.DetectDBPFs();

	return true;
}

void Debugging::DetectDBPFs()
{

	cResourceManager::DBPFList dbpfList = cResourceManager::DBPFList(ICoreAllocatorAdapter(nullptr, IO::GetAllocator()));

	size_t numDBPFs = IResourceManager::Get()->GetAllDBPFs(dbpfList);

	eastl::wstring resultString = L"";

	for (DBPF* pDBPF : dbpfList)
	{
		resultString += pDBPF->GetPath();
		resultString += L"\n";
	}

	// MessageBoxW(NULL, resultString.c_str(), L"Debugging Test", MB_OK);

	vector<uint32_t> propNames;

	uint32_t nGroupID = Hash::FNV("_SporeModder");
	uint32_t nPropertyID = Hash::FNV("modDebugPath");

	if (App::GetPropManager()->GetAllListIDs(nGroupID, propNames))
	{
		for (size_t i = 0; i < propNames.size(); i++)
		{

			DebugInformation* pInfo = new DebugInformation();
			if (pInfo->Read(propNames[i], nGroupID))
			{
				DBPF* pDBPF = IResourceManager::Get()->GetDBPF(ResourceKey(propNames[i], App::PropertyList::kPropTypeID, nGroupID));
				mPackageMap[pDBPF] = pInfo;
			}
		}
	}
}
