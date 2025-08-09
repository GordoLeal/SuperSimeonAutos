#pragma once
#include "Script.h"
#include "keyboard.h"
#include "Vehicles.h"
#include "InGameMenu.h"
std::list<char*> deliveredVehicles;
std::list<const char*> fullVehicleList;
//Parking Lot Abuse
Vehicle LastStolenVehicle;
BOOLEAN ParkingAbuseDuringMission;
//Lighthouse generation
UINT64 GTAVBase;
UINT64 EndOfOurModule;
UINT64 OurModuleBase;
Hash lightHouseModel;
Hash lightHouseRoofModel;
Hash lightHouseLightModel;
Hash lightHouseLight2Model;
Entity lightHouseEntity;
Entity lightHouseBase;
Entity lightHouseTop;
Entity lightHouseHat;
Entity lightHouseSignal;
int lighHouseRotationTimer = 0;
Vector3 lightHouseCoords;

enum ScriptStage {
	CheckCurrentVehicle,
	LookingForValidVehicle,
	Delivering, //The Truth - Kifflom
	VehicleAtDelivery,
	DeleteVehicle
};

// Pattern Pointers
intptr_t pSavedSlotNumberPTR;
intptr_t pToBeLoadedSaveFilePTR;
intptr_t pIsSaveMenuOpen;
// Return of the pointers
int LastLoadedSaveSlotNumber;
char* ToBeLoadedSaveFile;
int* IsGameSaving;
// Misc
std::string lastLoadedSaveFile;
std::wstring pathToSaveFolder;
std::string lastValueOfToBeLoadedSaveFile;
bool alreadyManualSaving = false;
bool alreadyAutoSaving = false;
bool alreadyBedSaving = false;
bool wasLoadingScreenActive;
char* lastValidVehicle;
Settings gSettings;
ScriptStage currentStage = ScriptStage::CheckCurrentVehicle;
bool OrtegaTrailerDelivered;
bool IsEnhanced = false;
// Mission specific
bool DisableInArmenian = false;
bool DisableInDLG = false;
// MissionReplay
bool missionReplayCalled;
// Generate Remaining Cars List.
const DWORD genMaxPressingTime = 5000;
DWORD genStartPressingTime;
bool genStartTimer = false;
bool genAlreadyCreatingFile = false;
// Recover Remaining Cars from List.
const DWORD recMaxPressingTime = 8000;
DWORD recStartPressingTime;
bool recStartTimer = false;
bool recAlreadyLookingFile = false;
//Quick Help generate missing vehicles
const DWORD hmvMaxTimer = 1800000; //30 minutes
//DWORD hmvMaxTimer = 30000; //30 seconds for testing
DWORD hmvStartTime;
//Finale Messages
bool FinalReached = false;
bool finSimeonMSGReceived = false;
bool finCashMSGReceived = false;
DWORD finStartTimer;
DWORD finTimeForSimeonMessage = 5000;
DWORD finTimeForMoneyReceivedMSG = 16000;
//InGameMenu
InGameMenu* gameMenu;


static void CreateHelpText(char* text, bool doSound) {
	UI::_SET_TEXT_COMPONENT_FORMAT((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, doSound, -1);
}

void LoadHookPointers() {
	//Save Files

	HMODULE Module = GetModuleHandleA(NULL);
	GTAVBase = (UINT64)Module;

	HMODULE OurModule = GetModuleHandleA("SuperSimeonAutos.asi");
	OurModuleBase = (UINT64)OurModule;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)OurModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)OurModule + dosHeader->e_lfanew);

	DWORD sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	EndOfOurModule = OurModuleBase + sizeOfImage;

	//Saves Folder Path
	std::string gameVersionStr = UNK3::_GET_GAME_VERSION();
	
	//1.27 is only available in legacy, use default.
	if (gameVersionStr == "1.27")
	{
		SaveSystem::GetSaveFilePath(false, false, &pathToSaveFolder);
	}
	else
	{
		SaveSystem::GetSaveFilePath(true, IsEnhanced, &pathToSaveFolder);
	}

	// Pattern Finding
	// Number of last Loaded Save slot
	if (SaveSystem::GetPointerToLastLoadedSlotNumber(&pSavedSlotNumberPTR, IsEnhanced) == SaveSystem::ErrSave::SaveDone)
	{

		if (SaveSystem::GetLastReadSlotNumber(&LastLoadedSaveSlotNumber, &pSavedSlotNumberPTR) != SaveSystem::ErrSave::SaveDone)
		{
			OutputDebugString("something happened address could not be loaded... (getLastReadSlotNumber)");
			CreateHelpText((char*)"something happened address could not be loaded... (getLastReadSlotNumber)", true);
			return;
		}
	}
	else
	{
		OutputDebugString("something happened and pointers could not be loaded... (getPointerToLastLoadedSlot)");
		CreateHelpText((char*)"something happened and pointers could not be loaded... (getPointerToLastLoadedSlot)", true);
		return;
	}

	//char* of to be loaded Save File.
	if (SaveSystem::GetPointerToBeLoadedSaveFile(&pToBeLoadedSaveFilePTR, IsEnhanced) == SaveSystem::ErrSave::SaveDone) {
		if (SaveSystem::GetToBeReadSaveFile(&ToBeLoadedSaveFile, &pToBeLoadedSaveFilePTR) != SaveSystem::ErrSave::SaveDone) {
			OutputDebugString("something happened address could not be loaded... (ToBeLoadedSaveFile)");
			CreateHelpText((char*)"something happened address could not be loaded... (ToBeLoadedSaveFile)", true);
			return;
		}
	}
	else
	{
		OutputDebugString("something happened and pointers could not be loaded...(PointerToBeLoadedSaveFile)");
		CreateHelpText((char*)"something happened and pointers could not be loaded...(PointerToBeLoadedSaveFile)", true);
		return;
	}

	if (SaveSystem::GetPointerToIsSaveHappening(&pIsSaveMenuOpen, IsEnhanced) == SaveSystem::ErrSave::SaveDone) {
		if (SaveSystem::GetIntPointerFromPointer(&IsGameSaving, &pIsSaveMenuOpen) != SaveSystem::ErrSave::SaveDone) {
			OutputDebugString("something happened address could not be loaded...(ToIsSaveHappening)");
			CreateHelpText((char*)"something happened address could not be loaded...(ToIsSaveHappening)", true);
			return;
		}
	}
	else
	{
		OutputDebugString("something happened and pointers could not be loaded...(PointerToIsSaveHappening)");
		CreateHelpText((char*)"something happened and pointers could not be loaded...(PointerToIsSaveHappening)", true);
		return;
	}
}

/// <summary>
/// Fill the vehicle list
/// </summary>
void FillFullVehicleList()
{
	fullVehicleList.clear();
	for (const char* a : VehiclesList)
	{
		fullVehicleList.push_back(a);
	}
	if (IsEnhanced)
	{
		for (const char* aEnh : CurrentPatch_Cars)
		{
			fullVehicleList.push_back(aEnh);
		}
		if (gSettings.EnableFlyingVehicles)
		{
			for (const char* b : FlyingVehicles)
			{
				fullVehicleList.push_back(b);
			}
		}
	}

	if (gSettings.EnableWaterVehicles)
	{
		for (const char* c : WaterVehicles)
		{
			fullVehicleList.push_back(c);
		}
		if (IsEnhanced)
		{
			for (const char* cEnh : CurrentPatch_Water)
			{
				fullVehicleList.push_back(cEnh);
			}
		}
	}


	if (gSettings.EnableTrailers)
	{
		for (const char* d : TrailerVehicles)
		{
			fullVehicleList.push_back(d);
		}
	}

	if (gSettings.EnableBuryNYVehicles)
	{
		for (const char* e : SnowVehicles)
		{
			fullVehicleList.push_back(e);
		}
	}

	if (gSettings.OnMissionZeroVehicles)
	{
		for (const char* f : OmoVehicles)
		{
			fullVehicleList.push_back(f);
		}
	}

}

bool IsInFlyingVehiclesList(char* veh)
{
	for (const char* a : FlyingVehicles)
	{
		std::string sa = a;
		std::string sb = veh;
		if (sa.compare(veh) == 0 && sa.size() == sb.size())
		{
			return true;
		}
	}
	return false;
}

// Save System

void LoadCurrentSave() {

	// Script got reloaded into prologue, this is probably a new game.
	// Mission replay does not cause a script reload.
	if (SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY((char*)"Prologue1")) > 0)
	{
		// Player just loaded a new game.
		// clean everything.
		deliveredVehicles.clear();
	}
	else
	{

		if (missionReplayCalled)
		{
			std::string cmp(ToBeLoadedSaveFile);
			if (cmp.find("MISREP000") != std::string::npos) {
				//Nothing is needed
			}
			else
			{
				// Player loaded a save file while playing replay.
				deliveredVehicles.clear();
				SaveSystem::LoadProgress(pathToSaveFolder, LastLoadedSaveSlotNumber, deliveredVehicles);
			}
			// Player just got out of a mission replay and is loading everything back.
		}
		else
		{

			if (LastLoadedSaveSlotNumber >= 0 && LastLoadedSaveSlotNumber < 15)
			{
				// Player Just loaded a save file.
				// Load user data from that save.
				deliveredVehicles.clear();
				SaveSystem::LoadProgress(pathToSaveFolder, LastLoadedSaveSlotNumber, deliveredVehicles);

			}
			else
			{
				SaveSystem::LoadProgressForFirstTime(pathToSaveFolder, deliveredVehicles);
				// Player Just oppened the game, check the latest saved files.
			}
		}
	}
	missionReplayCalled = false;
}

void SaveCheck()
{
	// GTA and ScriptHookV don't have a option to directly check if the player just saved the game manually, only auto saves.
	// We need to do this manual check via globals.
	// Did a save just happen? Check if any file has been modified, and if it did trigger the save.
	//OutputDebugString(std::to_string(*IsGameSaving).c_str());

	// Check if saving is happening and if is manual save.
	if (*IsGameSaving && SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY((char*)"save_anywhere")) > 0)
	{
		// if save has been called test if we can save and try to save into it.
		if (!alreadyManualSaving && !missionReplayCalled) {
			alreadyManualSaving = true;
			WAIT(200);//GTA needs to finish messing with the file before we do anything.
			if (deliveredVehicles.size() > 0) {
				/*		for (char* dah : deliveredVehicles) {
							OutputDebugString(dah);
						}*/
				SaveSystem::ErrSave err = SaveSystem::SaveProgress(deliveredVehicles, false, pathToSaveFolder);
				if (err == SaveSystem::ErrSave::FileDoesNotExistOrNotBellowBuffer) {
					CreateHelpText((char*)"Saving failed! please try again...", true);
				}
				else if (err == SaveSystem::ErrSave::SaveDone) {
					CreateHelpText((char*)"Collected Vehicles manual saved with success!", true);
				}
				else {
					CreateHelpText((char*)"Saving error! please try again...", true);
				}
			}
		}
	}
	else if (!*IsGameSaving)
	{
		alreadyManualSaving = false;
	}

	// AutoSave
	if (GAMEPLAY::IS_AUTO_SAVE_IN_PROGRESS())
	{
		if (!alreadyAutoSaving && !missionReplayCalled) {
			alreadyAutoSaving = true;
			WAIT(500);//GTA needs to finish messing with the file before we do anything. Auto saves need a bit more because they take some time to update the save file.
			if (deliveredVehicles.size() > 0) {
				//for (char* dah : deliveredVehicles) {
				//	OutputDebugString(dah);
				//}
				SaveSystem::ErrSave err = SaveSystem::SaveProgress(deliveredVehicles, false, pathToSaveFolder);
				if (err == SaveSystem::ErrSave::FileDoesNotExistOrNotBellowBuffer) {
					CreateHelpText((char*)"Saving failed! please try again...", true);
				}
				else if (err == SaveSystem::ErrSave::SaveDone) {
					CreateHelpText((char*)"Collected Vehicles autosaved with success!", true);
				}
				else {
					CreateHelpText((char*)"Error while Auto Saving! please try again...", true);
				}
			}
		}
	}
	else if (!*IsGameSaving)
	{
		alreadyAutoSaving = false;

	}

	// Probably is a Bed save.
	if (*IsGameSaving && !alreadyAutoSaving && !alreadyManualSaving)
	{
		// if save has been called test if we can save and try to save into it.
		if (!alreadyBedSaving && !missionReplayCalled) {
			alreadyBedSaving = true;
			WAIT(200);//GTA needs to finish messing with the file before we do anything.
			if (deliveredVehicles.size() > 0) {
				//for (char* dah : deliveredVehicles) {
				//	OutputDebugString(dah);
				//}
				SaveSystem::ErrSave err = SaveSystem::SaveProgress(deliveredVehicles, false, pathToSaveFolder);
				if (err == SaveSystem::ErrSave::FileDoesNotExistOrNotBellowBuffer) {
					CreateHelpText((char*)"Saving failed! please try again...", true);
				}
				else if (err == SaveSystem::ErrSave::SaveDone) {
					CreateHelpText((char*)"Collected Vehicles saved with success!", true);
				}
				else {
					CreateHelpText((char*)"Saving error! please try again...", true);
				}
			}
		}
	}
	else if (!*IsGameSaving)
	{
		alreadyBedSaving = false;
	}

}
// =-=-=-=-=-=-=-=- FINALE
static void FinaleStart()
{
	//Player started the finale give him 7$ dolars and a message from simeon.
	FinalReached = true;
	finStartTimer = GetTickCount();
}

static void FinaleUpdate()
{
	if (FinalReached && GetTickCount() >= (finStartTimer + finTimeForSimeonMessage) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()) && !finSimeonMSGReceived)
	{
		finSimeonMSGReceived = true;
		UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((char*)"You are my favorite employee!\nMy new employee of the month!\nSoon you will receive your payment!");
		UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", true, 4, (char*)"SIMEON", (char*)"Good Job!");
		UI::_DRAW_NOTIFICATION(1, 1);
	}

	if (FinalReached && GetTickCount() >= (finStartTimer + finTimeForSimeonMessage + finTimeForMoneyReceivedMSG) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()) && !finCashMSGReceived)
	{
		finCashMSGReceived = true;
		UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((char*)"An online transaction was processed for your account in the amount of $7 from Simeon Yetarian");
		UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_BANK_MAZE", (char*)"CHAR_BANK_MAZE", true, 4, (char*)"MAZE BANK", (char*)"");
		UI::_DRAW_NOTIFICATION(1, 1);
		// Pay Player
		int cash0;
		int cash1;
		int cash2;

		STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP0_TOTAL_CASH"), &cash0, -1); // Michael
		STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP1_TOTAL_CASH"), &cash1, -1); // Franklin
		STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP2_TOTAL_CASH"), &cash2, -1); // Trevor
		//Check Status_Enums.sch for more info
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP0_TOTAL_CASH"), cash0 + 7, 1); // Michael
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP1_TOTAL_CASH"), cash1 + 7, 1); // Franklin
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY((char*)"SP2_TOTAL_CASH"), cash2 + 7, 1); // Trevor

	}

}

static void QuickAddToDelivered(char* veh)
{
	deliveredVehicles.push_back(veh);

	if (OrtegaTrailerDelivered)
	{
		//ortega will add +1 to delivered vehicles but not to full vehicle list
		if (deliveredVehicles.size() >= fullVehicleList.size() + 1)
		{
			//Player collected the final car, start finale.
			FinaleStart();
		}
	}
	else if (deliveredVehicles.size() >= fullVehicleList.size())
	{
		//Player collected the final car, start finale.
		FinaleStart();
	}
}

static void QuickRemoveFromDelivered(char* veh) {
	if (std::string(veh).find("PROPTRAILER") != std::string::npos) {
		OrtegaTrailerDelivered = false;
	}
	deliveredVehicles.remove_if([veh](char* v) { return strcmp(v, veh) == 0; });

	if (deliveredVehicles.size() < fullVehicleList.size() + 1)
	{
		FinalReached = false;
	}
}

bool QuickCheckIfDelivered(char* veh)
{
	std::string a(veh);
	for (char* v : deliveredVehicles)
	{
		if (a.compare(v) == 0) {
			return true;
		}
	}
	return false;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=- BLIPS =-=-=-=-=-=-=-=-=-=-
Blip simeonBlip;
Blip countrysideLightHouseBlip;
Blip lifeguardBeachBlip;
Blip PierBlip;

void EnableAllDeliveryBlips()
{

	if (gSettings.BeachAsDelivery) {
		lifeguardBeachBlip = UI::ADD_BLIP_FOR_COORD(-1174, -1773, 3);
		UI::SET_BLIP_FLASHES(lifeguardBeachBlip, true);
		UI::SET_BLIP_FLASH_TIMER(lifeguardBeachBlip, 5000);
		UI::SET_BLIP_COLOUR(lifeguardBeachBlip, BlipColorWhite);
	}

	if (gSettings.LightHouseAsDelivery)
	{
		countrysideLightHouseBlip = UI::ADD_BLIP_FOR_COORD(3426, 5174, 7);
		UI::SET_BLIP_FLASHES(countrysideLightHouseBlip, true);
		UI::SET_BLIP_FLASH_TIMER(countrysideLightHouseBlip, 5000);
		UI::SET_BLIP_COLOUR(countrysideLightHouseBlip, 41);
	}

	if (gSettings.SimeonAsDelivery && !(DisableInArmenian))
	{
		simeonBlip = UI::ADD_BLIP_FOR_COORD(-55, -1112, 26);
		UI::SET_BLIP_FLASHES(simeonBlip, true);
		UI::SET_BLIP_FLASH_TIMER(simeonBlip, 5000);
		UI::SET_BLIP_COLOUR(simeonBlip, BlipColorGreen);

	}
	if (gSettings.PierAsDelivery && !(DisableInDLG)) {
		PierBlip = UI::ADD_BLIP_FOR_COORD(-1813, -1200, 13);
		UI::SET_BLIP_FLASHES(PierBlip, true);
		UI::SET_BLIP_FLASH_TIMER(PierBlip, 5000);
		UI::SET_BLIP_COLOUR(PierBlip, 53);
	}

}

void DisableAllDeliveryBlips() {
	if (UI::DOES_BLIP_EXIST(simeonBlip))
		UI::REMOVE_BLIP(&simeonBlip);
	if (UI::DOES_BLIP_EXIST(countrysideLightHouseBlip))
		UI::REMOVE_BLIP(&countrysideLightHouseBlip);
	if (UI::DOES_BLIP_EXIST(lifeguardBeachBlip))
		UI::REMOVE_BLIP(&lifeguardBeachBlip);
	if (UI::DOES_BLIP_EXIST(PierBlip))
		UI::REMOVE_BLIP(&PierBlip);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=- UI =-=-=-=-=-=-=-=-=-=-

void ShowCollectedAmount() {
	std::string outputAmount;
	int totalDeliveredVehicles = deliveredVehicles.size();
	size_t totalFullVehicles = fullVehicleList.size();

	if (OrtegaTrailerDelivered)
	{
		totalFullVehicles++;
	}
	if (deliveredVehicles.size() >= totalFullVehicles)
	{
		//Player collected the final car, start finale.
		outputAmount += "~y~";
	}
	outputAmount += std::to_string(totalDeliveredVehicles);
	if (gSettings.DisplayMaxAmount) {
		outputAmount += " | ";
		outputAmount += std::to_string(totalFullVehicles);
	}

	//Draw collect amount
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(1);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::SET_TEXT_SCALE(0, 0.8f);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::_ADD_TEXT_COMPONENT_STRING((char*)outputAmount.c_str());
	UI::_DRAW_TEXT(0.25f, 0.85f);

	if (gSettings.ShowHelpText)
	{
		if (GetTickCount() >= hmvStartTime + hmvMaxTimer)
		{
			hmvStartTime = GetTickCount();
			//quick help text
			const char* outputHelp = "Need help?\nPress ~INPUT_RELOAD~ + ~INPUT_COVER~ to check the missing vehicles list.";
			CreateHelpText((char*)outputHelp, true);
		}
	}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-  Testing player in Area =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
typedef struct {
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
}DeliveryArea;

typedef struct
{
	float x;
	float y;
	float z;
} DeliveryTeleportPoint;

enum StatusEntityInArea {
	none,
	Simeon,
	Beach,
	Lighthouse,
	Pier
};

const DeliveryArea SimeonArea = { -38, -1102, -1, -60, -1120, 80 };
const DeliveryTeleportPoint SimeonTPoint = { -62,-1093,26.5f };
const DeliveryArea LighthouseArea = { 3459, 5148, 80, 3356, 5201, -1 };
const DeliveryTeleportPoint LighthouseTPoint = { 3351, 5152, 20 };
const DeliveryArea BeachArea = { -1165, -1807, 50, -1223, -1761, -1 };
const DeliveryTeleportPoint BeachTPoint = { -1187,-1781,9 };
const DeliveryArea PierArea = { -1787, -1180, 80, -1859, -1260, -1 };
const DeliveryTeleportPoint PierTPoint = { -1816,-1192,14.5f };

StatusEntityInArea IsEntityInDeliveryArea(Entity entity) {

	if (ENTITY::IS_ENTITY_IN_AREA(entity, LighthouseArea.x1, LighthouseArea.y1, LighthouseArea.z1, LighthouseArea.x2, LighthouseArea.y2, LighthouseArea.z2, true, true, 0)
		&& gSettings.LightHouseAsDelivery)
	{
		return Lighthouse;
	}
	if (ENTITY::IS_ENTITY_IN_ANGLED_AREA(entity, BeachArea.x1, BeachArea.y1, BeachArea.z1, BeachArea.x2, BeachArea.y2, BeachArea.z2, 45, false, false, 0)
		&& gSettings.BeachAsDelivery)
	{
		return Beach;
	}
	if (ENTITY::IS_ENTITY_IN_AREA(entity, SimeonArea.x1, SimeonArea.y1, SimeonArea.z1, SimeonArea.x2, SimeonArea.y2, SimeonArea.z2, false, false, 0)
		&& gSettings.SimeonAsDelivery)
	{

		if (DisableInArmenian)
		{
			return none;
		}

		return Simeon;
	}
	if (ENTITY::IS_ENTITY_IN_ANGLED_AREA(entity, PierArea.x1, PierArea.y1, PierArea.z1, PierArea.x2, PierArea.y2, PierArea.z2, 45, false, false, 0)
		&& gSettings.PierAsDelivery)
	{
		if (DisableInDLG)
		{
			return none;
		}
		return Pier;
	}
	return none;
}

// =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0= ORTEGA TRAILER =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=

// First check after load to know if ortega was already delivered in this save.
void SetOrtegaTrailerWasDelivered() {
	const std::string modelName = "PROPTRAILER";
	for (char* a : deliveredVehicles) {
		//"PROPTRAILER"
		if (modelName == a)
		{
			OrtegaTrailerDelivered = true;
			return;
		}
	}
	OrtegaTrailerDelivered = false;
}

// =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0= Create Missing Cars =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=

static void CreateMissingCarsTXTFile()
{
	std::fstream genFileStream;
	genFileStream.open("SSA_MissingVehicles.txt", std::ios::in | std::ios::out | std::ios::trunc);
	if (genFileStream.is_open()) {
		std::list<const char*> genMissingVehicles;
		for (const char* x : fullVehicleList)
		{
			bool found = false;
			for (const char* z : deliveredVehicles)
			{

				std::string a(x);
				std::string b(z);
				if (a.find(z) == 0 && a.size() == b.size())
				{
					found = true;
				}
			}
			if (!found)
			{

				genMissingVehicles.push_back(x);
			}

		}

		genFileStream << "Missing Vehicles:\n";
		for (const char* a : genMissingVehicles)
		{
			genFileStream << UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(GAMEPLAY::GET_HASH_KEY((char*)a)));
			genFileStream << " (";
			genFileStream << a;
			genFileStream << ")\n";
		}

		genFileStream << '\n';
		genFileStream << "Delivered Vehicles:\n";

		for (const char* b : deliveredVehicles)
		{
			std::string outLine;
			outLine += UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(GAMEPLAY::GET_HASH_KEY((char*)b)));
			outLine += " (";
			outLine += b;
			outLine += ")";
			genFileStream << outLine;
			genFileStream << '\n';
		}
		genFileStream.close();
	}
}

static void TestInputForMissingVehicles()
{
	bool genList1 = CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlCover);
	bool genList2 = CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlReload);

	if (genList1 && genList2) {

		if (genStartTimer)
		{
			if ((GetTickCount() >= genStartPressingTime + genMaxPressingTime) && !genAlreadyCreatingFile)
			{
				genAlreadyCreatingFile = true;
				CreateMissingCarsTXTFile();
				UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char*)"Missing vehicles list created: \"SSA_MissingVehicles.txt\" (see game files)");
				UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", true, 4, (char*)"SIMEON", (char*)"");
			}
		}
		else
		{
			genStartTimer = true;
			genStartPressingTime = GetTickCount();
		}

	}
	else
	{
		genStartTimer = false;
		genAlreadyCreatingFile = false;
	}
}

static void RecoverVehicleList()
{
	bool recBPress1 = CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlCover);
	bool recBPress2 = CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlJump);
	if (recBPress1 && recBPress2)
	{
		if (recStartTimer)
		{
			if ((GetTickCount() >= recStartPressingTime + recMaxPressingTime) && !recAlreadyLookingFile)
			{
				recAlreadyLookingFile = true;
				UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char*)"Trying to recover delivered cars...");
				UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", false, 4, (char*)"SIMEON", (char*)"");
				std::fstream recoFileStream;
				recoFileStream.open("SSA_MissingVehicles.txt", std::ios::in | std::ios::out);
				if (recoFileStream.is_open())
				{
					int counter = 0;
					int remCounter = 0;
					std::string line;
					//Going Line By Line
					while (std::getline(recoFileStream, line))
					{
						bool readingModel = false;
						std::string vModel;
						if (line.find('+') != std::string::npos)// +\n
						{
							// Going char by char
							for (int x = 0; x < line.size(); x++)
							{
								switch (line[x])
								{
								case '(':
									readingModel = true;
									break;
								case ')':
									// test vehicle and add to the list
									if (!QuickCheckIfDelivered((char*)vModel.c_str()))
									{
										if (vModel.compare("PROPTRAILER") == 0)
										{
											OrtegaTrailerDelivered = true;
											QuickAddToDelivered((char*)"PROPTRAILER");
											counter++;
										}
										else
										{
											//First check if is a valid vehicle.
											for (const char* veh : fullVehicleList)
											{
												std::string b = veh;
												//Check if is the same size for reasons that vehicles can have the same name but with extra stuff after.
												if (std::string(vModel).size() == b.size())
												{
													if (std::string(vModel).find(b) != std::string::npos) //Now verify if is the same name.
													{
														//Now add to the list
														QuickAddToDelivered((char*)veh);
														counter++;
													}
												}
											}
										}
									}
									break;
								case '\n':
									//we are done with this line, go to the next
									break;
								default:
									if (readingModel)
									{
										vModel += line[x];
									}
									break;
								}
							}
						}

						if (line.find('!') != std::string::npos)// !\n
						{
							// Going char by char
							for (int x = 0; x < line.size(); x++)
							{
								switch (line[x])
								{
								case '(':
									readingModel = true;
									break;
								case ')':
									// test vehicle and add to the list
									if (QuickCheckIfDelivered((char*)vModel.c_str()))
									{
										//First check if is a valid vehicle.
										for (const char* veh : fullVehicleList)
										{
											std::string b = veh;
											//Check if is the same size for reasons that vehicles can have the same name but with extra stuff after.
											if (std::string(vModel).size() == b.size())
											{
												if (std::string(vModel).find(b) != std::string::npos) //Now verify if is the same name.
												{
													//Now remove from the list
													QuickRemoveFromDelivered((char*)veh);
													remCounter++;
												}
											}
										}
									}
									break;
								case '\n':
									//we are done with this line, go to the next
									break;
								default:
									if (readingModel)
									{
										vModel += line[x];
									}
									break;
								}
							}
						}

					}
					recoFileStream.close();
					bool z = true;
					if (counter > 0)
					{
						z = false;
						UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
						UI::_ADD_TEXT_COMPONENT_STRING((char*)std::string(std::to_string(counter) + " vehicles added to the delivered list!").c_str());
						UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", false, 4, (char*)"SIMEON", (char*)"");
					}

					if (remCounter > 0)
					{
						z = false;
						UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
						UI::_ADD_TEXT_COMPONENT_STRING((char*)std::string(std::to_string(remCounter) + " vehicles removed from the delivered list!").c_str());
						UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", false, 4, (char*)"SIMEON", (char*)"");
					}

					if (z)
					{
						UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
						UI::_ADD_TEXT_COMPONENT_STRING((char*)"No vehicles changed. Please add \"+\" or \"!\" at the end of the line to add or remove a vehicle.");
						UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", true, 4, (char*)"SIMEON", (char*)"");
					}
					else
					{
						CreateMissingCarsTXTFile();
					}



				}
				else
				{
					UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
					UI::_ADD_TEXT_COMPONENT_STRING((char*)"File \"SSA_MissingVehicles.txt does not exist or was not possible to read.\" ");
					UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", true, 4, (char*)"SIMEON", (char*)"");
				}
			}
		}
		else
		{
			recStartTimer = true;
			recStartPressingTime = GetTickCount();
		}

	}
	else
	{
		recStartTimer = false;
		recAlreadyLookingFile = false;
	}
}

// =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0= LIGHTHOUSE DECORATION =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=

void LighthouseDecoration() {


	if (!lightHouseModel) {
		lightHouseModel = GAMEPLAY::GET_HASH_KEY((char*)"prop_storagetank_07a");
	}

	if (!lightHouseRoofModel) {
		lightHouseRoofModel = GAMEPLAY::GET_HASH_KEY((char*)"prop_watertower01");
	}

	if (!lightHouseLightModel) {
		lightHouseLightModel = GAMEPLAY::GET_HASH_KEY((char*)"prop_warninglight_01");
	}

	if (!lightHouseLight2Model) {
		lightHouseLight2Model = GAMEPLAY::GET_HASH_KEY((char*)"prop_spot_01");
	}

	if (*(UINT64*)(GTAVBase + 0x1624174) == 0x1416CAE828EC8348) //Check if we are on 1.27
	{
		if (*(UINT64*)(GTAVBase + 0xC3FD94) == 0x000244840F01C1F6)	//Check if trainers already patched the pattern
		{
			*(UINT64*)(GTAVBase + 0x1659FDB) = 0x5E5FFD840F01C1F6;
			*(UINT64*)(GTAVBase + 0x1659FE3) = 0xCCCCFF5E5DB4E9FF;
			*(UINT64*)(GTAVBase + 0x16772F0) = OurModuleBase;
			*(UINT64*)(GTAVBase + 0x1678D50) = EndOfOurModule;

			*(UINT64*)(GTAVBase + 0x165A97C) = 0x0001A024848B4850;
			*(UINT64*)(GTAVBase + 0x165A984) = 0xE95800000933E900;
			*(UINT64*)(GTAVBase + 0x165A98C) = 0x244C894CFFFFF64B;
			*(UINT64*)(GTAVBase + 0x165B2BD) = 0x0F0001DA8C053B48;
			*(UINT64*)(GTAVBase + 0x165B2C5) = 0x0AADE9FFFFF6C083;
			*(UINT64*)(GTAVBase + 0x165B2CD) = 0x20244C8944CC0000;
			*(UINT64*)(GTAVBase + 0x165BD7C) = 0x0F0001B56D053B48;
			*(UINT64*)(GTAVBase + 0x165BD84) = 0x13E958FFFFEC0186;
			*(UINT64*)(GTAVBase + 0x165BD8C) = 0x244C894CCCFF5E40;

			*(UINT64*)(GTAVBase + 0xC3FD94) = 0x90909000A1ABE3E9;
			*(UINT8*)(GTAVBase + 0xC3FD9C) = 0x90;
		}

		if (!ENTITY::DOES_ENTITY_EXIST(lightHouseBase)
			|| !ENTITY::DOES_ENTITY_EXIST(lightHouseTop)
			|| !ENTITY::DOES_ENTITY_EXIST(lightHouseHat)
			|| !ENTITY::DOES_ENTITY_EXIST(lightHouseSignal)
			|| !ENTITY::DOES_ENTITY_EXIST(lightHouseEntity)
			)
		{

			if (ENTITY::DOES_ENTITY_EXIST(lightHouseBase))
				OBJECT::DELETE_OBJECT(&lightHouseBase);
			if (ENTITY::DOES_ENTITY_EXIST(lightHouseTop))
				OBJECT::DELETE_OBJECT(&lightHouseTop);
			if (ENTITY::DOES_ENTITY_EXIST(lightHouseHat))
				OBJECT::DELETE_OBJECT(&lightHouseHat);
			if (ENTITY::DOES_ENTITY_EXIST(lightHouseSignal))
				OBJECT::DELETE_OBJECT(&lightHouseSignal);
			if (ENTITY::DOES_ENTITY_EXIST(lightHouseEntity))
				OBJECT::DELETE_OBJECT(&lightHouseEntity);


			STREAMING::REQUEST_MODEL(lightHouseModel);
			STREAMING::REQUEST_MODEL(lightHouseRoofModel);
			STREAMING::REQUEST_MODEL(lightHouseLightModel);
			STREAMING::REQUEST_MODEL(lightHouseLight2Model);
			while (!STREAMING::HAS_MODEL_LOADED(lightHouseModel)
				|| !STREAMING::HAS_MODEL_LOADED(lightHouseRoofModel)
				|| !STREAMING::HAS_MODEL_LOADED(lightHouseLightModel)
				|| !STREAMING::HAS_MODEL_LOADED(lightHouseLight2Model))
			{
				WAIT(0);
			}

			lightHouseBase = OBJECT::CREATE_OBJECT_NO_OFFSET(lightHouseModel, lightHouseCoords.x, lightHouseCoords.y, lightHouseCoords.z, false, true, true);
			ENTITY::FREEZE_ENTITY_POSITION(lightHouseBase, 0x1);
			ENTITY::SET_ENTITY_LOD_DIST(lightHouseBase, 0xFFFF);
			lightHouseTop = OBJECT::CREATE_OBJECT_NO_OFFSET(lightHouseModel, lightHouseCoords.x, lightHouseCoords.y, lightHouseCoords.z + 6.0f, false, true, true);
			ENTITY::FREEZE_ENTITY_POSITION(lightHouseTop, 0x1);
			ENTITY::SET_ENTITY_LOD_DIST(lightHouseTop, 0xFFFF);
			lightHouseHat = OBJECT::CREATE_OBJECT_NO_OFFSET(lightHouseRoofModel, lightHouseCoords.x, lightHouseCoords.y, lightHouseCoords.z + 12.0f, false, true, true);
			ENTITY::FREEZE_ENTITY_POSITION(lightHouseHat, 0x1);
			ENTITY::SET_ENTITY_LOD_DIST(lightHouseHat, 0xFFFF);
			lightHouseSignal = OBJECT::CREATE_OBJECT_NO_OFFSET(lightHouseLightModel, lightHouseCoords.x, lightHouseCoords.y, lightHouseCoords.z + 18.5f, false, true, false);
			ENTITY::FREEZE_ENTITY_POSITION(lightHouseSignal, 0x1);
			ENTITY::SET_ENTITY_LOD_DIST(lightHouseSignal, 0xFFFF);
			lightHouseEntity = OBJECT::CREATE_OBJECT_NO_OFFSET(lightHouseLight2Model, lightHouseCoords.x, lightHouseCoords.y, lightHouseCoords.z + 16.5f, false, true, false);
			ENTITY::FREEZE_ENTITY_POSITION(lightHouseEntity, 0x1);
			ENTITY::SET_ENTITY_LOD_DIST(lightHouseEntity, 0xFFFF);

			lighHouseRotationTimer = GAMEPLAY::GET_GAME_TIMER();
		}
		else
		{
			int newTime = GAMEPLAY::GET_GAME_TIMER();

			float degrees = (newTime / 1000.0f - lighHouseRotationTimer / 1000.0f) * 10.0f;

			Vector3 currentRotation = ENTITY::GET_ENTITY_ROTATION(lightHouseEntity, 2);

			ENTITY::SET_ENTITY_ROTATION(lightHouseEntity, currentRotation.x, currentRotation.y, fmod(currentRotation.z + degrees, 360.0f), 2, true);
			lighHouseRotationTimer = newTime;
		}
	}
}
void WarpPedsInsideVehicleTo(int vehicleID,float coordX, float coordY,float coordZ)
{
	// VS_ANY_PASSENGER = -2, //Any passenger seat
	// VS_DRIVER = -1, // Drivers seat
	// VS_FRONT_RIGHT = 0, // Front Right seat
	// VS_BACK_LEFT, //Back left 	
	// VS_BACK_RIGHT, //Back right
	// VS_EXTRA_LEFT_1,
	// VS_EXTRA_RIGHT_1,
	// VS_EXTRA_LEFT_2,
	// VS_EXTRA_RIGHT_2,
	// VS_EXTRA_LEFT_3,
	// VS_EXTRA_RIGHT_3
	for (int x = -2; x < 9; x++)
	{
		//	if (!VEHICLE::IS_VEHICLE_SEAT_FREE(lastDriven, x)) //this function don't work.
		Ped pedinSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicleID, x); // returns null/0 if seat don't have anyone.
		if (pedinSeat != NULL) {
			if (pedinSeat == PLAYER::PLAYER_PED_ID())
			{
				continue;
			}

			ENTITY::SET_ENTITY_COORDS(pedinSeat, coordX, coordY, coordZ, true, true, true, true);
			break;
		}
	}
}
//  =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=
//  =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0= UPDATE =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0
//  =0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=0=

int daytime = 0;
void Update()
{
	//Funny lighthouse made by Gogsi
	LighthouseDecoration();
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- CONSTANTLY USED VARIABLES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	Ped pPedID = PLAYER::PLAYER_PED_ID();
	Player pID = PLAYER::PLAYER_ID();

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- MISSION REPLAY TEST =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// Detect if we started a Mission Replay.
	if (!missionReplayCalled)
		if (lastValueOfToBeLoadedSaveFile.find("MISREP") == std::string::npos && std::string(ToBeLoadedSaveFile).find("MISREP") != std::string::npos)
		{
			missionReplayCalled = true;
			SaveSystem::SaveProgressForReplay(deliveredVehicles, false, pathToSaveFolder);
		}

	lastValueOfToBeLoadedSaveFile = ToBeLoadedSaveFile;
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- SAVE LOADING TEST=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	SaveCheck();

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- USER INTERFACE =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	ShowCollectedAmount();

	FinaleUpdate();
	gameMenu->DrawMenu();

	if (
		(CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlCover) && CONTROLS::IS_CONTROL_JUST_PRESSED(0, eControl::ControlReload))
		|| (CONTROLS::IS_CONTROL_JUST_PRESSED(0, eControl::ControlCover) && CONTROLS::IS_CONTROL_PRESSED(0, eControl::ControlReload))
		)
	{
		gameMenu->SwitchOpenState();
	}

	char* vehTest;
	if (gameMenu->VehicleToBeRemoved(&vehTest))
	{
		if (QuickCheckIfDelivered(vehTest))
			QuickRemoveFromDelivered(vehTest);
		gameMenu->UpdateVehiclesList();
	}
	if (gameMenu->VehicleToBeAdded(&vehTest))
	{
		if (!QuickCheckIfDelivered(vehTest))
			QuickAddToDelivered(vehTest);
		gameMenu->UpdateVehiclesList();
	}
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- PLAYER INPUT STUFF =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	TestInputForMissingVehicles();
	RecoverVehicleList();

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-  MISSION SPECIFIC STUFF  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 

	DisableInArmenian = false;
	DisableInDLG = false;

	if (!gSettings.SimeonStateDuringArmenian
		&& (SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY((char*)"Armenian1")) > 0
			|| SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY((char*)"Armenian3")) > 0
			)
		)
	{
		DisableInArmenian = true;
	}

	if (!gSettings.PierStateDuringDLG && SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY((char*)"Family2")) > 0)
	{
		DisableInDLG = true;
	}

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- TRAILERS AND ORTEGA TEST =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// For optmization, We want to only call this loop only once per frame, so have it set in a way that can be reused for other parts of the script.
	const int ARR_SIZE = 255;
	Vehicle vehInWorld[ARR_SIZE];
	int vehInWorldCount = worldGetAllVehicles(vehInWorld, ARR_SIZE);
	for (int b = 0; b < vehInWorldCount; b++)
	{
		Vehicle vehTrailerTest = vehInWorld[b];
		if (IsEntityInDeliveryArea(vehTrailerTest) != none)
		{
			if (gSettings.EnableTrailers) {

				for (const char* a : TrailerVehicles)
				{
					if (VEHICLE::IS_VEHICLE_MODEL(vehTrailerTest, GAMEPLAY::GET_HASH_KEY((char*)a)) == TRUE)
					{

						if (!QuickCheckIfDelivered((char*)a))
						{
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehTrailerTest, TRUE, TRUE);
							VEHICLE::DETACH_VEHICLE_FROM_TRAILER(PLAYER::GET_PLAYERS_LAST_VEHICLE());
							QuickAddToDelivered((char*)a);
							VEHICLE::DELETE_VEHICLE(&vehTrailerTest);

							std::string trHelper;
							trHelper += "Trailer Delivered!\n";
							trHelper += "(";
							trHelper += a;
							trHelper += ")";
							CreateMissingCarsTXTFile();
							CreateHelpText((char*)trHelper.c_str(), true);
						}
						break;
					}
				}
			}

			// Using the trailer loop just so we don't have to create a second loop, is a small optmization.
			if (!OrtegaTrailerDelivered) {
				if (VEHICLE::IS_VEHICLE_MODEL(vehTrailerTest, GAMEPLAY::GET_HASH_KEY((char*)"PROPTRAILER")) == TRUE)
				{
					if (!QuickCheckIfDelivered((char*)"PROPTRAILER"))
					{
						VEHICLE::DETACH_VEHICLE_FROM_TRAILER(PLAYER::GET_PLAYERS_LAST_VEHICLE());
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehTrailerTest, TRUE, TRUE);
						VEHICLE::DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(vehTrailerTest);
						QuickAddToDelivered((char*)"PROPTRAILER");
						VEHICLE::DELETE_VEHICLE(&vehTrailerTest);
						std::string trHelper;
						trHelper += "Ortega Delivered???\n";
						trHelper += "(";
						trHelper += "PROPTRAILER";
						trHelper += ")";
						CreateHelpText((char*)trHelper.c_str(), true);
						UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
						UI::_ADD_TEXT_COMPONENT_STRING((char*)"WHY DID YOU BRING A ENTIRE TRAILER? AND WHY THERE'S A GUY INSIDE IT?");
						UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", false, 4, (char*)"SIMEON", (char*)"What is this?");
						UI::_DRAW_NOTIFICATION(0, 1);
						OrtegaTrailerDelivered = true;
						CreateMissingCarsTXTFile();
					}
					break;
				}
			}
		}
	}

	//  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- SCRIPT STAGES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	switch (currentStage)
	{
	case CheckCurrentVehicle:
		// Wait for player to get into a vehicle and check if is valid.
		if (PED::IS_PED_IN_ANY_VEHICLE(pPedID, false))
		{
			// BUG: Entering another vehicle after delivering causes the lookingForValidVehicle to be set
			// even if we are inside a valid vehicle.
			// adding this small delay fixes it. I don't know why...
			WAIT(100);

			Vehicle lastDrivenVehicle = PLAYER::GET_PLAYERS_LAST_VEHICLE();
			bool foundValidVehicle = false;
			bool alreadyHave = false;

			// ANTI PARKING LOT ABUSE
			if (gSettings.AntiParkingLotBeach && gSettings.BeachAsDelivery)
			{
				if (LastStolenVehicle != lastDrivenVehicle)
				{
					if (!VEHICLE::_IS_VEHICLE_ENGINE_ON(lastDrivenVehicle))
					{
						Vector3 CurrentCoords = ENTITY::GET_ENTITY_COORDS(pPedID, 0x1);

						float CheatDistance = SYSTEM::VDIST(CurrentCoords.x, CurrentCoords.y, CurrentCoords.z, -1195, -1788, 0);

						if (CheatDistance < 600.0f)
						{
							if (!GAMEPLAY::GET_MISSION_FLAG())
							{
								ParkingAbuseDuringMission = 0x0;
								ENTITY::SET_ENTITY_COORDS(pPedID, CurrentCoords.x, CurrentCoords.y, CurrentCoords.z + 1, 0x0, 0x0, 0x0, 0x0);
								WAIT(1000);
								VEHICLE::EXPLODE_VEHICLE(lastDrivenVehicle, false, true);
								CreateHelpText((char*)"Parking lot abuse detected!", true);
								break;
							}

							else
							{
								ParkingAbuseDuringMission = 0x1;
							}
						}

						else
						{
							ParkingAbuseDuringMission = 0x0;
							LastStolenVehicle = lastDrivenVehicle;
						}

					}

					else
					{
						ParkingAbuseDuringMission = 0x0;
						LastStolenVehicle = lastDrivenVehicle;
					}
				}
			}

			Hash lastDriveModelHash = ENTITY::GET_ENTITY_MODEL(lastDrivenVehicle);

			for (const char* a : fullVehicleList)
			{

				if (VEHICLE::IS_VEHICLE_MODEL(lastDrivenVehicle, GAMEPLAY::GET_HASH_KEY((char*)a)) == TRUE)
				{
					foundValidVehicle = true;
					// The vehicle we have is valid, check the delivered list.
					if (QuickCheckIfDelivered((char*)a))
					{
						alreadyHave = true;
						std::string alreadyHaveHelpText;
						alreadyHaveHelpText += "This vehicle has already been delivered!\n";
						alreadyHaveHelpText += "(";
						alreadyHaveHelpText += a;
						alreadyHaveHelpText += ")";
						CreateHelpText((char*)alreadyHaveHelpText.c_str(), false);
						break;
					}

					// This vehicle haven't been delivered, tell the player about it and start the script.
					currentStage = ScriptStage::Delivering;
					std::string deliveredHelpText;
					deliveredHelpText += "This vehicle can be delivered!\n";
					deliveredHelpText += "(";
					deliveredHelpText += a;
					deliveredHelpText += ")";
					CreateHelpText((char*)deliveredHelpText.c_str(), true);
					EnableAllDeliveryBlips();
					lastValidVehicle = (char*)a;
					break;
				}
			}

			if (alreadyHave || !foundValidVehicle)
			{
				//Player is in already delivered or vehicle is not in the list, start looking for valid vehicle procedure.
				currentStage = ScriptStage::LookingForValidVehicle;
			}
		}
		else
		{
			// Player is out of vehicle, stop everything.
			DisableAllDeliveryBlips();
		}
		break;

	case LookingForValidVehicle:
		//Player is in invalid vehicle just cycle until he gets out.
		if (!PED::IS_PED_IN_ANY_VEHICLE(pPedID, false)) {
			//Player is not in vehicle, start looking for the next one.
			currentStage = ScriptStage::CheckCurrentVehicle;
		}
		break;

	case Delivering:
		// Player is in a valid vehicle, wait for player to get at the delivery point and do the removing process.
		if (PED::IS_PED_IN_ANY_VEHICLE(pPedID, false))
		{
			// test if player is in delivery area.
			switch (IsEntityInDeliveryArea(pPedID))
			{
			case none:
				break;
			default:
				currentStage = VehicleAtDelivery;
				break;

			}
		}
		else
		{
			// Player got out of vehicle, stop everything.
			DisableAllDeliveryBlips();
			currentStage = ScriptStage::CheckCurrentVehicle;
			lastValidVehicle = (char*)"";
		}
		break;

	case VehicleAtDelivery:
	{ // C2360

		Vehicle lastDriven = PLAYER::GET_PLAYERS_LAST_VEHICLE();
		Hash lastValidHash = GAMEPLAY::GET_HASH_KEY(lastValidVehicle);
		//vehicle id / stopping distance / time to stop the vehicle for / bool: no idea what it does
		//VEHICLE::_TASK_BRING_VEHICLE_TO_HALT(lastDriven, 5, 5, true); // Stop vehicle
		switch (IsEntityInDeliveryArea(pPedID))
		{
		case none:
			//wut?
			break;
		case Simeon:
			if (IsInFlyingVehiclesList(lastValidVehicle)) //Remove delay just to be safe with the player going directly to the lighthouse.
			{
				//Add a small delay just so the player see the car flying
				ENTITY::SET_ENTITY_COORDS(pPedID, SimeonTPoint.x, SimeonTPoint.y, SimeonTPoint.z, false, false, false, false); // warp to safe zone.
			}
			VEHICLE::_TASK_BRING_VEHICLE_TO_HALT(lastDriven, 15, 5, true); // Stop vehicle
			// Remove any peds from the vehicle.
			WarpPedsInsideVehicleTo(lastDriven, SimeonTPoint.x, SimeonTPoint.y, SimeonTPoint.z);
			break;
		case Lighthouse:
			if (!IsInFlyingVehiclesList(lastValidVehicle))
			{
				//Add a small delay just so the player see the car flying
				WAIT(1000);
			}
			ENTITY::SET_ENTITY_COORDS(pPedID, LighthouseTPoint.x, LighthouseTPoint.y, LighthouseTPoint.z, false, false, false, false); // warp to safe zone.
			WarpPedsInsideVehicleTo(lastDriven, LighthouseTPoint.x, LighthouseTPoint.y, LighthouseTPoint.z);
			break;
		case Beach:
			// Parking lot abuse detection
			if (gSettings.AntiParkingLotBeach && ParkingAbuseDuringMission)
			{
				Vector3 CurrentCoords = ENTITY::GET_ENTITY_COORDS(pPedID, 0x1);
				if (IsInFlyingVehiclesList(lastValidVehicle))
				{
					ENTITY::SET_ENTITY_COORDS(pPedID, BeachTPoint.x, BeachTPoint.y, BeachTPoint.z, 0, 0, 0, 0);
					WarpPedsInsideVehicleTo(lastDriven, BeachTPoint.x, BeachTPoint.y, BeachTPoint.z);
				}
				else
				{
					ENTITY::SET_ENTITY_COORDS(pPedID, CurrentCoords.x, CurrentCoords.y, CurrentCoords.z + 1, 0x0, 0x0, 0x0, 0x0);
					WarpPedsInsideVehicleTo(lastDriven, BeachTPoint.x, BeachTPoint.y, BeachTPoint.z);
				}

				WAIT(1000);
				VEHICLE::EXPLODE_VEHICLE(lastDriven, false, true);
				CreateHelpText((char*)"Parking lot abuse detected!", true);

				DisableAllDeliveryBlips();
				currentStage = ScriptStage::CheckCurrentVehicle;
				lastValidVehicle = (char*)"";
				return;
			}
			else
			{
				if (IsInFlyingVehiclesList(lastValidVehicle))
				{
					ENTITY::SET_ENTITY_COORDS(pPedID, BeachTPoint.x, BeachTPoint.y, BeachTPoint.z, 0, 0, 0, 0);
				}
				// Remove any peds from the vehicle.
				WarpPedsInsideVehicleTo(lastDriven, BeachTPoint.x, BeachTPoint.y, BeachTPoint.z);
			}

			VEHICLE::_TASK_BRING_VEHICLE_TO_HALT(lastDriven, 15, 5, true); // Stop vehicle
			
			break;
		case Pier:
			if (IsInFlyingVehiclesList(lastValidVehicle))
			{
				ENTITY::SET_ENTITY_COORDS(pPedID, PierTPoint.x, PierTPoint.y, PierTPoint.z, 0, 0, 0, 0);
			}
			VEHICLE::_TASK_BRING_VEHICLE_TO_HALT(lastDriven, 15, 5, true); // Stop vehicle
			// Remove any peds from the vehicle.
			WarpPedsInsideVehicleTo(lastDriven, PierTPoint.x, PierTPoint.y, PierTPoint.z );
			break;
		}

		// PLAYER IS IN DELIVERY AREA
		AI::TASK_EVERYONE_LEAVE_VEHICLE(lastDriven);
		//ENTITY::SET_ENTITY_AS_MISSION_ENTITY(lastDriven, true, true); // set current vehicle as a mission entity, so we can delete after.
		currentStage = ScriptStage::DeleteVehicle;
		WAIT(500);

		break;
	}
	case DeleteVehicle:
		// BUG: If SET_PLAYER_MAY_NOT_ENTER_ANY_VEHICLE is after GET_PLAYER_LAST_VEHICLE you can't delete the vehicle
		PLAYER::SET_PLAYER_MAY_NOT_ENTER_ANY_VEHICLE(pID);
		VEHICLE::DETACH_VEHICLE_FROM_TRAILER(PLAYER::GET_PLAYERS_LAST_VEHICLE());
		Vehicle lastDriven = PLAYER::GET_PLAYERS_LAST_VEHICLE();
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(lastDriven, true, true);
		// Car is probably free to delete;
		if (!PED::IS_PED_IN_ANY_VEHICLE(pPedID, true)) 
		{
			VEHICLE::DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(lastDriven);
			QuickAddToDelivered(lastValidVehicle);
			gameMenu->UpdateVehiclesList();
			// BUG: if player is in a hangout, for some random reason the script sets the last driven to null and the vehicle never gets deleted.
			// Set position from last driven vehicle really far from the player to force mission fails and force to clean the vehicle from the memory in case is not destroyed.
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::GET_PLAYERS_LAST_VEHICLE(), 1000.0f, -1000.0f, 1000.0f, false, false, true);
			VEHICLE::EXPLODE_VEHICLE(lastDriven, false, true);
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&lastDriven);
			//Last Try to remove the vehicle.
			Vehicle toDelete = PLAYER::GET_PLAYERS_LAST_VEHICLE();
			std::string deliMsg;
			deliMsg += "Vehicle Delivered!\n";
			deliMsg += "(";
			deliMsg += lastValidVehicle;
			deliMsg += ")";
			VEHICLE::DELETE_VEHICLE(&toDelete);
			CreateHelpText((char*)deliMsg.c_str(), true);
			CreateMissingCarsTXTFile();
			currentStage = ScriptStage::CheckCurrentVehicle;
		}

		break;
	}
}

// =-=-=-=-=- MAIN ENTRY FUNCTION =-=-=-=-=-=-=-=-=-=
// Reminder to myself: variables set outside of functions are reserved as globals
// they keep their value between transitions and loading screens IF a default value have not been set.
// the code inside the script is reloaded after transitions and loading screens.
void ScriptMain()
{
	lightHouseCoords.x = -1831.544f;
	lightHouseCoords.y = -1189.259f;
	lightHouseCoords.z = 27.16121f;

	//Check which version of the game the mod is installed
	HMODULE Module = GetModuleHandleA(NULL);
	char modulePath[260];
	GetModuleFileNameA(Module, modulePath, 260);
	if (std::string(modulePath).find("GTA5_Enhanced.exe") != std::string::npos)
	{
		IsEnhanced = true;
	}

	//Just to make sure everything is correctly loaded.
	LoadHookPointers();
	//Settings
	gSettings.ReadValuesFromSettingsFile();
	FillFullVehicleList();
	// Script got reloaded, try read the save files and see if we should do something.
	LoadCurrentSave();

	WAIT(1000);
	//Check if it was possible to create the file.
	if (!gSettings.DoesSettingsFileExists())
	{
		UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
		UI::_ADD_TEXT_COMPONENT_STRING((char*)"SSASettings.txt file couldn't be created.\nPlease start GTAV as ADMINISTRATOR.");
		UI::_SET_NOTIFICATION_MESSAGE((char*)"CHAR_SIMEON", (char*)"CHAR_SIMEON", false, 4, (char*)"WARNING!", (char*)"");
		UI::_DRAW_NOTIFICATION(0, 1);
	}

	//Check if ortega trailer was delivered
	SetOrtegaTrailerWasDelivered();
	currentStage = ScriptStage::CheckCurrentVehicle;

	if (gameMenu == nullptr)
	{
		gameMenu = new InGameMenu(&gSettings, &deliveredVehicles, &fullVehicleList);
	}
	gameMenu->UpdateVehiclesList();

	//The Main Loop
	while (true) {
		Update();
		WAIT(0);
	}
}
