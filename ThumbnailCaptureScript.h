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

#include <Spore\UTFWin\IWinProc.h>
#include <Spore\Cheats.h>
#include <Spore\Palettes\ItemViewer.h>
#include <Spore\Graphics\IRenderable.h>
#include <Spore\Messaging.h>
#include <EASTL\hash_map.h>

using namespace UTFWin;

class ThumbnailCaptureScript
	: public IWinProc
	, public ArgScript::ICommand
	, public DefaultObject
	, public Graphics::IRenderable
	, public App::IMessageListener
{
public:
	ThumbnailCaptureScript();
	virtual ~ThumbnailCaptureScript();

	virtual int AddRef() override;
	virtual int Release() override;
	virtual void* Cast(uint32_t) const override;
	
	virtual int GetEventFlags() const override;
	virtual bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	virtual void ParseLine(const ArgScript::Line& line) override;
	virtual const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	virtual void Render(int flags, int layerIndex, void*, void*) override;

	virtual bool HandleMessage(uint32_t messageID, void* pMessage) override;

	static void AddCheat();

private:

	void CaptureImage();
	Vector3 CalculateOffset(const Graphics::Model* model);

	void InjectListeners();
	void RemoveListeners();

	// Theorically we should use intrusive_ptrs, but this script will delete when the game closes so better not delay those

	hash_map<IWindow*, Palettes::ItemViewer*> mItemViewers;

	Palettes::ItemViewer* mpItemViewer;
	bool mOldColorSet;
	ColorRGB mOldColor;
	ColorRGB mIdentityColor;
	string16 mFolderPath;
	bool mIsEnabled;
};

