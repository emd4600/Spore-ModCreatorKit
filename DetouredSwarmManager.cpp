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
#include "DetouredSwarmManager.h"
#include <Spore\Resources.h>
#include <Spore\App\ICheatManager.h>
#include <Spore\Swarm\CollectionResource.h>

DetourDeclaration(DetouredSwarmManager::GetDirectoryAndEffectIndex);

long DetouredSwarmManager::AttachDetour()
{
	SetDetourAddress(GetDirectoryAndEffectIndex, GetMethodAddress(Swarm::cSwarmManager, GetDirectoryAndEffectIndex));

	long error = 0;
	error |= AttachDetourFunction(GetDirectoryAndEffectIndex, DetouredGetDirectoryAndEffectIndex_redirected);

	return error;
}

static int directoryIndex = -1;

// int GetDirectoryAndEffectIndex(uint32_t instanceID, uint32_t groupID)
int DetouredSwarmManager::DetouredGetDirectoryAndEffectIndex(uint32_t instanceID, uint32_t groupID)
{
	// _effect_editor
	if (groupID == 0 && instanceID == 0x568EEC19)
	{
		Resource::ResourceObject* pResource = nullptr;
		Swarm::EffectDirectory* pDirectory = nullptr;
		
		if (!Resource::IResourceManager::Get()->GetResource(
			ResourceKey(Hash::FNV("main"), 0xEA5118B0, Hash::FNV("_SporeModder_EffectEditor")),
			&pResource))
		{
			App::ConsolePrintF("Effect editor won't work: there were errors reading main.effdir!");
			return -1;
		}

		pDirectory = ((Swarm::CollectionResource*)pResource)->GetCollection();

		if (directoryIndex == -1) 
		{
			this->mDirectories.push_back(pDirectory);
			directoryIndex = this->mDirectories.size() - 1;
		}
		else
		{
			this->mDirectories[directoryIndex] = pDirectory;
		}

		int effectIndex = pDirectory->GetEffectIndex(instanceID, groupID);
		if (effectIndex == -1)
		{
			App::ConsolePrintF("Effect editor won't work: main.effdir does not contain an effect called '_effect_editor'!");
			return -1;
		}

		return directoryIndex << 24 | (effectIndex & 0xFFFFFF);
	}
	else
	{
		// By default, this just calls the original Spore function.
		int original_result = GetDirectoryAndEffectIndex_original(this, instanceID, groupID);
		return original_result;
	}
}

