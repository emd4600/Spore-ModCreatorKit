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
#pragma once

#include <Spore\ArgScript\ArgScriptCommand.h>

#include <Spore\Swarm\SwarmManager.h>

#include <EASTL\string.h>
#include <EASTL\vector.h>

using namespace eastl;

namespace Swarm
{
	class EffectsCheat : public ArgScript::Command
	{
	public:

	protected:
		/* 10h */	SwarmManager* mpManager;
		/* 14h */	string field_14;
		/* 24h */	string field_24;
		/* 34h */	string field_34;
		/* 44h */	vector<int> field_44;
		/* 58h */	vector<int> field_58;
		/* 6Ch */	vector<int> field_6C;
		/* 80h */	bool field_70;
		/* 84h */	int field_84;
		/* 88h */	ArgScript::Line field_88;
		/* CCh */	vector<int> field_CC;
	};

	static_assert(sizeof(EffectsCheat) == 0x100, "sizeof(EffectsCheat) != 100h");
}