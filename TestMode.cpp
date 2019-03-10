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
#include "TestMode.h"

#include <Spore\App\IGameModeManager.h>
#include <Spore\Swarm\ISwarmManager.h>
#include <Spore\Graphics\IModelManager.h>
#include <Spore\Graphics\IRenderManager.h>
#include <Spore\Graphics\ILightingManager.h>
#include <Spore\Hash.h>

TestMode::TestMode()
	: mpEffect(nullptr)
	, mpModelWorld(nullptr)
	, mpModel(nullptr)
{}

TestMode* TestMode::sInstance;

bool TestMode::InitializationFunction()
{
	sInstance = new TestMode();
	App::IGameModeManager::Get()->AddGameMode(sInstance, 0x7638d92f, "TestMode");

	return true;
}

bool TestMode::OnEnter()
{

	//// CAMERA ////

	App::ICameraManager* pCameraMgr = App::IGameModeManager::Get()->GetCameraManager();
	if (pCameraMgr != nullptr)
	{
		//pCameraMgr->SetActiveCameraByID(Hash::FNV("EditorCameraCreatureUI"));
		pCameraMgr->SetActiveCameraByID(Hash::FNV("EffectEditorCamera"));
	}


	//// EFFECT ////

	if (Swarm::ISwarmManager::Get()->CreateEffect(Hash::FNV("trg_fire_campfire2"), 0, mpEffect))
	{
		mpEffect->Show();

		mpEffect->SetTransform(Transform().SetOffset(Math::Vector3(0, 0, 2.0)));
	}


	//// MODEL ////

	// mpLightingWorld = Graphics::ILightingManager::Get()->CreateWorld(0x95f3d2ab, nullptr, ID("creatureEditor"));

	mpModelWorld = Graphics::IModelManager::Get()->CreateWorld(0x95f3d2ab);
	// mpModelWorld->AddLightingWorld(mpLightingWorld.get(), 0, false);
	mpModelWorld->SetVisible(true);

	Graphics::IRenderManager::Get()->AddRenderable(mpModelWorld->ToRenderable(), Graphics::kRenderQueueMain);


	mpModel = mpModelWorld->LoadModel(Hash::FNV("ep1_altarreligious"), Hash::FNV("civicobjects"));
	//mpModel = mpModelWorld->LoadModel(0x185216C4, 0x40646200);
	mpModelWorld->UpdateModel(mpModel.get());

	mpModel->SetTransform(Transform().SetScale(0.5f));


	return true;
}

void TestMode::OnExit()
{
	if (mpEffect)
	{
		mpEffect->Kill();
		mpEffect = nullptr;
	}

	if (mpModelWorld)
	{
		Graphics::IModelManager::Get()->DisposeWorld(0x95f3d2ab);
	}

	if (mpLightingWorld)
	{
		Graphics::ILightingManager::Get()->DisposeWorld(0x95f3d2ab);
	}
}
