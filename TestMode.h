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

#include <Spore\App\DefaultGameMode.h>

#include <Spore\Swarm\IEffect.h>
#include <Spore\Graphics\IModelWorld.h>
#include <Spore\Graphics\ILightingWorld.h>

class TestMode : public App::DefaultGameMode
{
public:
	TestMode();

	virtual bool OnEnter() override;
	virtual void OnExit() override;

	static bool InitializationFunction();

protected:
	intrusive_ptr<Swarm::IEffect>			mpEffect;

	intrusive_ptr<Graphics::ILightingWorld>	mpLightingWorld;
	intrusive_ptr<Graphics::IModelWorld>	mpModelWorld;
	intrusive_ptr<Graphics::Model>			mpModel;

protected:
	static TestMode* sInstance;
};