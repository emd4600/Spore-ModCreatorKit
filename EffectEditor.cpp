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
#include "EffectEditor.h"
#include "ExtendedSwarmEffect.h"

#include <Spore\App\IGameModeManager.h>
#include <Spore\Hash.h>
// for diagnosing
#include <Spore\App\ICheatManager.h>

#include <Spore\Swarm\ISwarmManager.h>
#include <Spore\Swarm\IEffect.h>
#include <Spore\Swarm\Components\ParticleEffect.h>

#include <Spore\UTFWin\TreeNode.h>

#include <Spore\Graphics\IModelManager.h>
#include <Spore\Graphics\IRenderManager.h>

EffectEditor::EffectEditor()
	: mnRefCount(0)
	, mpUI(new EffectEditorUI(this))
	, mpEffect(nullptr)
	, mpGridEffect(nullptr)
	, mpDirectory(nullptr)
	, mpVisualEffect(nullptr)
	, mnDirectoryIndex(0xFF)

	, mpModelWorld(nullptr)
	, mpModel(nullptr)
{
	
}

EffectEditor::~EffectEditor()
{

}

int EffectEditor::AddRef()
{
	return ++mnRefCount;
}

int EffectEditor::Release()
{
	if (--mnRefCount == 0)
	{
		delete this;
	}
	return mnRefCount;
}

bool EffectEditor::func0Ch()
{
	return false;
}

bool EffectEditor::Initialize(App::IGameModeManager* pManager)
{
	// just for testing
	mpModelWorld = GetModelManager()->CreateWorld(0x12345678);

	// effect test
	char* pCommandData = new char[0x100];
	memset(pCommandData, 0, 0x100);
	*(int*)pCommandData = 0x145AF18;

	// CALL(GetAddress(0xA74070, PLACEHOLDER, PLACEHOLDER), void, PARAMS(SwarmManager*), PARAMS(SwarmManager::Get()));

	// App::ICheatManager::Get()->AddCheat("effectsManager", (ArgScript::IParser*)pCommandData, 0);

	this->SetEffectDirectory();
	return false;
}

bool EffectEditor::Dispose()
{
	// just for testing
	if (mpModelWorld)
	{
		GetModelManager()->DisposeWorld(0x12345678);
		mpModelWorld = nullptr;
	}

	mpDirectory = nullptr;
	return false;
}

bool EffectEditor::OnEnter()
{
	using namespace Swarm;

	// Load the user interface
	mpUI->Load();

	// Set the camera
	//TODO: Use our own camera?

	App::ICameraManager* pCameraMgr = App::IGameModeManager::Get()->GetCameraManager();
	if (pCameraMgr != nullptr)
	{
		//pCameraMgr->SetActiveCameraByID(Hash::FNV("EditorCameraCreatureUI"));
		pCameraMgr->SetActiveCameraByID(Hash::FNV("EffectEditorCamera"));
	}

	// Load the grid effect
	mpGridEffect = nullptr;
	if (ISwarmManager::Get()->CreateEffect(Hash::FNV("EffectEditorGrid"), 0, mpGridEffect))
	{
		mpGridEffect->Show();
	}


	// trg_fire_campfire2
	SetEffect(ResourceID(Hash::FNV("TestParticles"), 0));


	// just for testing
	mpModel = mpModelWorld->LoadModel(Hash::FNV("ep1_altarreligious"), Hash::FNV("civicobjects"));
	// mpModelWorld->SetVisible(mpModel.get(), true);
	mpModelWorld->ShowModel(mpModel.get());

	IRenderManager::Get()->AddRenderable(this, 0x0F);

	return true;
}

void EffectEditor::OnExit()
{
	mpUI->Unload();

	if (mpEffect)
	{
		mpEffect->Kill();
		mpEffect = nullptr;
	}
	if (mpGridEffect)
	{
		mpGridEffect->Kill();
		mpGridEffect = nullptr;
	}

	mpVisualEffect = nullptr;

	// just for testing
	if (mpModel)
	{
		mpModelWorld->SetVisible(mpModel.get(), false);
		mpModel = nullptr;

		IRenderManager::Get()->RemoveRenderable(0x0F);
	}
}

void* EffectEditor::func20h(int) 
{
	return nullptr;
}

//// INPUT LISTENERS ////

bool EffectEditor::OnKeyDown(int, int)
{
	return false;
}

bool EffectEditor::OnKeyUp(int, int)
{
	return false;
}

bool EffectEditor::OnMouseDown(int mouseState, float mouseX, float mouseY, int flags)
{
	return false;
}

bool EffectEditor::OnMouseUp(int mouseState, float mouseX, float mouseY, int flags)
{
	return false;
}

bool EffectEditor::OnMouseMove(float mouseX, float mouseY, int flags)
{
	return false;
}

bool EffectEditor::OnMouseWheel(int, float mouseX, float mouseY, int)
{
	return false;
}


void EffectEditor::Update(float fDeltaTime, float fDeltaTime2)
{
	static float fDirection = 1.0f;

	// ArgScript::PrintF(CheatManager::GetConsoleStream(), "Update %f %f", arg_0, arg_4);

	Transform t = mpEffect->GetTransform();
	const Vector3& offset = t.GetOffset();


	if (offset.z >= 4.0f)
	{
		fDirection = -1.0f;
	}
	else if (offset.z <= 0.0f)
	{
		fDirection = 1.0f;
	}

	t.SetOffset(Vector3(offset.x, offset.y, offset.z + fDeltaTime * fDirection));

	mpEffect->SetTransform(t);
}

void EffectEditor::EffectTest()
{
	uint32_t nDirectoryIndex = 0;
	uint32_t nEffectIndex = 0;
	mpEffect->GetIndex(nDirectoryIndex, nEffectIndex);

	EffectDirectory* pDirectory = ISwarmManager::Get()->GetDirectory(nDirectoryIndex);

	if (pDirectory != nullptr)
	{
		Components::ParticleEffect* pParticles = (Components::ParticleEffect*)pDirectory->GetComponent(nEffectIndex, Swarm::kEffectTypeParticles);

		if (pParticles != nullptr)
		{
			pParticles->color[0] = Math::ColorRGB(0.0f, 1.0f, 0.0f);
		}
	}
}


bool EffectEditor::InitializationFunction()
{
	App::IGameModeManager::Get()->AddGameMode(new EffectEditor(), 0x548f19d9, "EffectEditor");
	// ContextManager::Get()->SetActiveContext(0x548f19d9);

	return true;
}


void EffectEditor::SetEffect(const ResourceID& name)
{
	mpEffect = nullptr;
	if (ISwarmManager::Get()->CreateEffect(name.mnInstanceID, name.mnGroupID, mpEffect))
	{
		DuplicateEffect();
		FillTreeView(mpUI->GetTreeView());

		// We show the effect after we have done all the duplications required
		mpEffect->Show();
	}

}


void EffectEditor::SetEffectDirectory()
{
	intrusive_ptr<Swarm::IEffect> pEffect = nullptr;
	if (ISwarmManager::Get()->CreateEffect(Hash::FNV("EffectEditorIdentifier"), 0, pEffect))
	{
		size_t nEffectIndex;
		((cEffect*)pEffect.get())->GetIndex(mnDirectoryIndex, nEffectIndex);

		mpDirectory = ISwarmManager::Get()->GetDirectory(mnDirectoryIndex);
	}
}

void EffectEditor::DuplicateEffect()
{

	// mpVisualEffect = mpEffect->GetVisualEffect();

	// This is the directory that contains the real effect, we will copy the effect to our own directory.
	size_t nDirectoryIndex = 0;
	size_t nEffectIndex = 0;
	mpEffect->GetIndex(nDirectoryIndex, nEffectIndex);
	EffectDirectory* pRealDirectory = SwarmManager::Get()->GetDirectory(nDirectoryIndex);

	nEffectIndex = mpDirectory->AddVisualEffect(mpEffect->GetVisualEffect());

	mpVisualEffect = mpDirectory->GetVisualEffect(nEffectIndex);
	((ExtendedSwarmEffect*)mpEffect.get())->SetIndex(mnDirectoryIndex, nEffectIndex);
	((ExtendedSwarmEffect*)mpEffect.get())->SetVisualEffect(mpVisualEffect);

	// TODO: Duplicate subcomponents as well!
	for (VisualEffectBlock& block : mpVisualEffect->blocks)
	{
		switch (block.blockType)
		{
		case Swarm::kEffectTypeParticles: 
			//TODO: Use our own extended component?
			block.blockIndex = mpDirectory->AddComponent(block.blockType, 
				new ParticleEffect(*(ParticleEffect*) pRealDirectory->GetComponent(block.blockIndex, block.blockType)));
		}
	}
}


void EffectEditor::FillTreeView(WinTreeView* pTreeView)
{
	if (!pTreeView) return;

	TreeNode& rootNode = pTreeView->GetRootNode();

	rootNode.SetCaption(L"Effect Editor");

	if (mpEffect)
	{

		TreeNode* pNode;

		TreeNode* pEffectsNode = new TreeNode(pTreeView);
		pEffectsNode->SetCaption(GetText(kStringTableComponents, kStringInstanceEffects).c_str());
		rootNode.AddNode(pEffectsNode);

		const int kNumBlockTypes = 0x30;
		vector<VisualEffectBlock*> blocks[kNumBlockTypes];

		//// Fill Effects ////
		for (size_t i = 0; i < mpVisualEffect->blocks.size(); i++)
		{
			VisualEffectBlock& block = mpVisualEffect->blocks[i];

			pNode = new TreeNode(pTreeView);

			wstring name = wstring(wstring::CtorSprintf(), L"%ls-%d", 
				GetText(kStringTableComponents, kStringFlagKeywords | block.blockType).c_str(), i);

			pNode->SetCaption(name.c_str());
			pEffectsNode->AddNode(pNode);

			blocks[block.blockType].push_back(&block);
		}


		for (int i = 0; i < kNumBlockTypes; i++)
		{
			if (!blocks[i].empty())
			{
				pNode = new TreeNode(pTreeView);
				pNode->SetCaption(GetText(kStringTableComponents, i).c_str());
				rootNode.AddNode(pNode);

				for (const VisualEffectBlock* block : blocks[i])
				{
					wstring name = wstring(wstring::CtorSprintf(), L"%ls-%d",
						GetText(kStringTableComponents, kStringFlagKeywords | block->blockType).c_str(), block->blockIndex);

					TreeNode* pChildNode = new TreeNode(pTreeView);
					pChildNode->SetCaption(name.c_str());
					pNode->AddNode(pChildNode);
				}
			}
		}

	}
}

wstring EffectEditor::GetText(uint32_t tableID, uint32_t instanceID, const wchar_t* pText) const
{
	LocalizedString string = LocalizedString(tableID, instanceID, pText);
	return string.GetText();
}


void EffectEditor::Render(int arg_0, int arg_4, int arg_8, int arg_C)
{
	mpModelWorld->ToRenderable()->Render(arg_0, arg_4, arg_8, arg_C);
}