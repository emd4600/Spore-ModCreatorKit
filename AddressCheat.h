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

#include <Spore\ArgScript\ICommand.h>

class AddressCheat : public ArgScript::ICommand
{
public:
	AddressCheat();
	virtual ~AddressCheat();

	virtual void ParseLine(const ArgScript::Line& line) override;
	
	///
	/// Returns a basic or complete description of what this parser does. This usually includes information about
	/// how to use it, i.e. the arguments required, possible options, etc
	/// @param mode Whether the basic or complete description must be returned, in the DescriptionMode enum.
	///
	virtual const char* GetDescription(ArgScript::DescriptionMode mode = ArgScript::kDescriptionBasic) const override;
	
	// Add this in DllMain as an initialization function.
	static bool AddCheat(long address, const char* pName);
};

