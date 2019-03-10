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
#include "EffectEditorCheat.h"

#include <Spore\App\ICheatManager.h>
#include <Spore\GameModes.h>


EffectEditorCheat::EffectEditorCheat()
{
}


EffectEditorCheat::~EffectEditorCheat()
{
}

// The initialization function that adds the cheat to the game.
bool EffectEditorCheat::AddCheat()
{
	// Replace EffectEditorCheat with the keyword used to call your cheat (such as 'moremoney', 'adddna', etc)
	App::ICheatManager::Get()->AddCheat("effectEditor", new EffectEditorCheat());
	
	return true;
}


void EffectEditorCheat::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	App::IGameModeManager::Get()->SetActiveModeByName("EffectEditorMode");
}

const char* EffectEditorCheat::GetDescription(ArgScript::DescriptionMode mode) const
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
