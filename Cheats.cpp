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
#include "Cheats.h"

#include "ContextCheat.h"
#include "AddressCheat.h"

#include <Spore\App\ICheatManager.h>

bool Cheats::AddCheats()
{
	App::ICheatManager* pCM = App::ICheatManager::Get();

	pCM->AddCheat(ContextCheat::kKeyword, new ContextCheat(), 0);
	AddressCheat::AddCheat(GetAddress(0x1498444, 0, 0x1493E5C), "raid");
	AddressCheat::AddCheat(GetAddress(0x149845C, 0, 0x1493E74), "space");
	AddressCheat::AddCheat(GetAddress(0x1498474, 0, 0x1493E8C), "debugBadge");
	AddressCheat::AddCheat(GetAddress(0x149848C, 0, 0x1493EA4), "mission");
	AddressCheat::AddCheat(GetAddress(0x14984BC, 0, 0x1493ED4), "debugAri");

	return true;
}