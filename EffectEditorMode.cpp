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
#include "EffectEditorMode.h"
#include "DebugInformation.h"
#include "Debugging.h"

#include <Spore\Resources.h>
#include <Spore\App\IGameModeManager.h>
#include <Spore\Hash.h>

EffectEditorMode::EffectEditorMode()
	: mnRefCount(0)
	, mpEffect(nullptr)
	, mClock(Clock::Mode::Milliseconds)
	, mnLastCheckTime()
	, mpPath()
	, mpEffectWorld(nullptr)
{
	
}

EffectEditorMode::~EffectEditorMode()
{

}

// For internal use, do not touch this.
int EffectEditorMode::AddRef()
{
	return ++mnRefCount;
}

// For internal use, do not touch this.
int EffectEditorMode::Release()
{
	if (--mnRefCount == 0)
	{
		delete this;
	}
	return mnRefCount;
}

// The use of this function is unknown.
bool EffectEditorMode::func0Ch()
{
	return false;
}

bool EffectEditorMode::Initialize(App::IGameModeManager* pManager)
{
	// Called when on game startup. Here you should create all your model/effect worlds, etc.

	mpEffectWorld = ISwarmManager::Get()->CreateWorld(0x7856d8f9);
	
	return true;
}

bool EffectEditorMode::Dispose()
{
	// Called when the game exits. Here you should dispose all your model/effect worlds,
	// ensure all objects are released, etc
	
	ISwarmManager::Get()->RemoveWorld(0x7856d8f9);
	
	return true;
}

void EffectEditorMode::createEffect()
{
	if (mpEffect != nullptr)
	{
		mpEffect->Kill(1);
	}

	// We want it to stop and start inmediately, so we will really kill all particles
	ISwarmManager::Get()->GetActiveWorld()->SetState(Swarm::kStateShutdown);
	ISwarmManager::Get()->GetActiveWorld()->SetState(Swarm::kStateActive);

	if (ISwarmManager::Get()->CreateEffect(Hash::FNV("_effect_editor"), 0, mpEffect))
	{
		// This will only be executed if the effect existed and a new instance was created
		mpEffect->Show();
	}

	// Stop and start the clock again
	mClock.Stop();
	mClock.Start();
}

void EffectEditorMode::updateFileTime()
{
	
	if (mpPath.size() != 0)
	{
		HANDLE handle = CreateFile(mpPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		FILETIME lastWriteTime;

		if (GetFileTime(handle, NULL, NULL, &lastWriteTime))
		{
			mnLastCheckTime.LowPart = lastWriteTime.dwLowDateTime;
			mnLastCheckTime.HighPart = lastWriteTime.dwHighDateTime;
		}

		CloseHandle(handle);
	}
}

bool EffectEditorMode::OnEnter()
{
	// Called when the game mode is entered. Here you should load your effects and models,
	// load the UI, add custom renderers, etc.

	// Get the handle of the main.effdir file

	ISwarmManager::Get()->SetActiveWorld(mpEffectWorld.get());

	DebugInformation* pDebugInformation = nullptr;
	Resource::DBPF* pDBPF = nullptr;
	ResourceKey name = ResourceKey(Hash::FNV("main"), 0xEA5118B0, Hash::FNV("_SporeModder_EffectEditor"));
	string16 path;

	pDBPF = Resource::IResourceManager::Get()->GetDBPF(name);

	if (Debugging::Get().GetDebugInformation(pDBPF, &pDebugInformation)
		&& pDebugInformation->GetFilePath(name, &path))
	{
		mpPath = (const wchar_t*)path.c_str();
	}
	else
	{
		mpPath = L"";
	}

	// Load camera and effect

	App::IGameModeManager::Get()->GetCameraManager()->SetActiveCameraByID(Hash::FNV("EffectEditorCamera"));

	App::IGameModeManager::Get()->GetCameraManager()->GetViewer()->SetBackgroundColor(ColorRGBA(0.7f, 0.7f, 0.7f, 1.0f));

	createEffect();
	
	// Return false if there was any error.
	return true;
}

void EffectEditorMode::OnExit()
{
	// Called when the game mode is exited. Here you should kill all effects and models, 
	// stop any renderers, unload the UI, etc.

}

// The use of this function is unknown.
void* EffectEditorMode::func20h(int) 
{
	return nullptr;
}


//// INPUT LISTENERS ////

bool EffectEditorMode::OnKeyDown(int virtualKey, int modifiers)
{
	// Called when a keyboard key button is pressed.
	
	// Return true if the keyboard event has been handled in this method.
	return false;
}

bool EffectEditorMode::OnKeyUp(int virtualKey, int modifiers)
{
	// Called when a keyboard key button is released.
	
	// Return true if the keyboard event has been handled in this method.
	return false;
}

bool EffectEditorMode::OnMouseDown(int mouseState, float mouseX, float mouseY, int flags)
{
	// Called when a mouse button is pressed (this includes the mouse wheel button).
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

bool EffectEditorMode::OnMouseUp(int mouseState, float mouseX, float mouseY, int flags)
{
	// Called when a mouse button is released (this includes the mouse wheel button).
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

bool EffectEditorMode::OnMouseMove(float mouseX, float mouseY, int flags)
{
	// Called when the mouse is moved.
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

bool EffectEditorMode::OnMouseWheel(int nWheelDelta, float mouseX, float mouseY, int state)
{
	// Called when the mouse wheel is scrolled. 
	// This method is not called when the mouse wheel is pressed.
	
	// Return true if the mouse event has been handled in this method.
	return false;
}

#include <Spore\Swarm\cEffectWorld.h>

//// UPDATE FUNCTION ////

void EffectEditorMode::Update(float fDelta1, float fDelta2)
{
	// Called on every frame.
	// fDelta1 and fDelta2 are the ellapsed time in seconds since the last call,
	// the difference between both is uncertain.

	// Update every second
	if (mClock.GetElapsed() >= 1000.0f)
	{

		ULARGE_INTEGER oldTime = mnLastCheckTime;

		updateFileTime();

		if (mnLastCheckTime.QuadPart > oldTime.QuadPart)
		{
			createEffect();
		}
	}
}


//// INITIALIZATION FUNCTION ////

// The initialization function that adds this game mode to the game mode manager.
bool EffectEditorMode::AddGameMode()
{
	// Here, EffectEditorMode is a way to identify your game mode, you can change it.
	// Here, Hash::FNV("EffectEditorMode") is an ID to identify your game mode, you can change it.
	App::IGameModeManager::Get()->AddGameMode(new EffectEditorMode(), Hash::FNV("EffectEditorMode"), "EffectEditorMode");

	return true;
}

