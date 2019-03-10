// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <ModAPI\MainUtilities.h>
#include <Spore\Detouring.h>
#include <Spore\Messaging.h>

#include "Debugging.h"
#include "DetouredSwarmManager.h"
#include "TestMode.h"
#include "EffectEditorMode.h"
#include "EffectEditorCheat.h"
#include "Cheats.h"
#include "ThumbnailCaptureScript.h"

void Initialize()
{
	Debugging::Initialize();
	// Not available in public release
	// TestMode::InitializationFunction();
	Cheats::AddCheats();
	EffectEditorCheat::AddCheat();
	EffectEditorMode::AddGameMode();

	ThumbnailCaptureScript::AddCheat();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	LONG error;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// This line is always necessary
		ModAPI::ModAPIUtils::InitModAPI();

		/* Insert code here */

		/* 
		Note: You cannot use any Spore function inside this method. 
		If you want to add any initialization methods, do it with:

		bool myFunction() {...}

		And then, inside the DllMain function:

		ModAPI::ModAPIUtils::AddInitFunction(&myFunction);

		Things you can do in the DllMain method:
		 - Add init functions
		 - Add UI event listeners
		 - Detour functions
		*/

		ModAPI::ModAPIUtils::AddInitFunction(&Initialize);


		PrepareDetours(hModule);

		Debugging::AttachDetour();
		DetouredSwarmManager::AttachDetour();

		error = SendDetours();

		if (error != NO_ERROR)
		{
			MessageBoxA(NULL, "Error detouring", "Error", MB_OK);
		}


	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


