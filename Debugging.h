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

#include <Spore\Resource\DatabasePackedFile.h>
#include <Spore\Hash.h>

#include <EASTL\intrusive_ptr.h>
#include <EASTL\map.h>

#include "DebugInformation.h"

using namespace eastl;

class Debugging
{
public:
	Debugging();
	~Debugging();

	/// Gets the debug information for the given DBPF.
	/// Returns true if the DBPF has debug information, false otherwise.
	bool GetDebugInformation(const Resource::DBPF* pDBPF, DebugInformation** pDst = nullptr) const;

	///
	/// Detours all the functions related with file debugging.
	///
	static long AttachDetour();

	///
	/// A ModAPI Init function that detects all the .package files that must be debugged.
	///
	static bool Initialize();

	static Debugging& Get();

protected:
	void DetectDBPFs();

protected:
	/// Maps every package to the debug information
	map<const Resource::DBPF*, DebugInformation*> mPackageMap;


private:
	static Debugging sMain;
};

inline Debugging& Debugging::Get()
{
	return sMain;
}
