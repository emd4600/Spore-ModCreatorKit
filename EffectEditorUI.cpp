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
#include "EffectEditorUI.h"
#include "CustomDrawable.h"
#include "EffectEditor.h"

#include <Spore\UTFWin\Window.h>
#include <Spore\UTFWin\WindowManager.h>
#include <Spore\UTFWin\cSPUIMessageBox.h>
#include <Spore\UTFWin\TreeNode.h>

#include <Spore\App\IGameModeManager.h>
#include <Spore\Hash.h>

// for testing
#include <Spore\App\ICheatManager.h>


ResourceKey EffectEditorUI::kKeyExitDialog;


EffectEditorUI::EffectEditorUI(EffectEditor* pEditor)
	: mpMainWindow(nullptr)
	, mpTreeView(nullptr)
	, mLayout()
	, mOpenEffectDialog()
	, mpEditor(pEditor)
	, mpSelectedNode(nullptr)
{

	ResourceKey::Parse(kKeyExitDialog, L"EffectEditorDialogs!ExitDialog.prop");
}

EffectEditorUI::~EffectEditorUI()
{

}

int EffectEditorUI::AddRef()
{
	return DefaultObject::AddRef();
}
int EffectEditorUI::Release()
{
	return DefaultObject::Release();
}
void* EffectEditorUI::Cast(uint32_t type)
{
	if (type == IWinProc::TYPE) {
		return (IWinProc*) this;
	}
	else {
		return DefaultObject::Cast(type);
	}
}


void EffectEditorUI::Load()
{

	mLayout.LoadByName(L"EffectEditorUI");

	mpMainWindow = mLayout.FindWindowByID(Hash::FNV("MainWindow"));
	mpMainWindow->AddWinProc(this);

	mpTreeView = (WinTreeView*)mLayout.FindWindowByID(kControlComponentsTreeView);
	mpTreeView->AddWinProc(this);

	////// Just a window for testing ////
	//mpTestWindow = new Window();
	//mpTestWindow->AddWinProc(this);

	//mpTestWindow->SetSize(400, 400);
	//mpTestWindow->SetLocation(200, 200);

	//mpTestWindow->SetFillColor(Color(255, 67, 128, 95));

	//mpTestWindow->SetDrawable(new CustomDrawable());


	//// Add window to the main frame

	//WindowManager::Get()->GetMainWindow()->AddWindow(mpTestWindow.get());
}

void EffectEditorUI::Unload()
{
	mOpenEffectDialog.Hide();

	if (mpMainWindow != nullptr)
	{
		mpMainWindow->GetParent()->DisposeWindowFamily(mpMainWindow.get());
		mpMainWindow = nullptr;
	}

}

WinTreeView* EffectEditorUI::GetTreeView()
{
	return mpTreeView.get();
}

void EffectEditorUI::UpdateSelectedComponent()
{
	
}


int EffectEditorUI::GetEventFlags() const
{
	return kEventFlagAdvanced | kEventFlagBasicInput;
}

static EffectEditorDialogCallback callback = EffectEditorDialogCallback();

bool EffectEditorUI::HandleMessage(IWindow* pWindow, const Message& msg)
{
	// ignore those types, otherwise they cause crashes (they don't set the mpSource)
	if (msg.mEventType == UTFWin::kMsgWinProcAdded || msg.mEventType == UTFWin::kMsgWinProcRemoved)
	{
		return true;
	}

	if (msg.mEventType == UTFWin::kMsgComponentActivated)
	{
		uint32_t controlID = msg.mpSource->GetControlID();

		if (controlID == kControlExitButton)
		{
			App::IGameModeManager::Get()->SetActiveModeByName("GalaxyGameEntry");

			return true;
		}
		else if (controlID == kControlEditButton)
		{
			// cSPUIMessageBox::ShowDialog(&callback, kKeyExitDialog);

			mOpenEffectDialog.Show();

			return true;
		}
		else if (controlID == kControlSaveButton)
		{
			mpEditor->EffectTest();

			return true;
		}
	}

	if (msg.mpSource != nullptr && msg.mpSource->GetControlID() == kControlComponentsTreeView)
	{
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "TreeView event: %d", msg.mEventType);
	}

	return false;
}


void EffectEditorDialogCallback::OnButtonPress(IWindow* pWindow, uint32_t buttonID)
{
	int button = buttonID;
}
