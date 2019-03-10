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
#include "AddressCheat.h"

#include <Spore\App\ICheatManager.h>


AddressCheat::AddressCheat()
{
}


AddressCheat::~AddressCheat()
{
}

// The initialization function that adds the cheat to the game.
bool AddressCheat::AddCheat(long address, const char* pName)
{
	// Replace AddressCheat with the keyword used to call your cheat (such as 'moremoney', 'adddna', etc)
	AddressCheat* pCheat = new AddressCheat();

	// Replace the vtable with the address
	*((long*)pCheat) = address;

	App::ICheatManager::Get()->AddCheat(pName, pCheat);
	
	return true;
}


void AddressCheat::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
}

const char* AddressCheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::kDescriptionBasic)
	{
		return "This cheat does something.";
	}
	else
	{
		return "Elaborate description of what this cheat does.";
	}
}
