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

#include <Spore\App\IGameMode.h>
#include <Spore\Effects.h>
#include <Spore\Clock.h>
#include <EASTL\string.h>

using namespace eastl;
using namespace Swarm;

class EffectEditorMode : public App::IGameMode
{
public:
	EffectEditorMode();

	virtual int AddRef() override;
	virtual int Release() override;
	virtual ~EffectEditorMode();

	virtual bool func0Ch() override;
	virtual bool Initialize(App::IGameModeManager* pManager) override;
	virtual bool Dispose() override;
	virtual bool OnEnter() override;
	virtual void OnExit() override;
	virtual void* func20h(int) override;

	virtual bool OnKeyDown(int virtualKey, int modifiers) override;
	virtual bool OnKeyUp(int virtualKey, int modifiers) override;
	virtual bool OnMouseDown(int mouseState, float mouseX, float mouseY, int flags) override;
	virtual bool OnMouseUp(int mouseState, float mouseX, float mouseY, int flags) override;
	virtual bool OnMouseMove(float mouseX, float mouseY, int flags) override;
	virtual bool OnMouseWheel(int nWheelDelta, float mouseX, float mouseY, int state) override;
	virtual void Update(float fDelta1, float fDelta2) override;

	// The function to attach the mode to the game mode manager.
	//	Add this in DllMain as an initialization function.
	static bool AddGameMode();

private:
	void createEffect();

	void updateFileTime();
	
protected:
	// For internal use, do not touch this.
	int mnRefCount;

	intrusive_ptr<IEffectWorld> mpEffectWorld;
	intrusive_ptr<IEffect> mpEffect;

	Clock mClock;
	ULARGE_INTEGER mnLastCheckTime;
	wstring mpPath;
};
