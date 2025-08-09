#pragma once
//Scripthook headers
#include "enums.h"
#include "types.h"
#include "natives.h"
#include "main.h"
//Default
#include <string>
#include <iostream>
#include "Settings.h"
#include <list>
#include <vector>

class InGameMenu
{
public:
	static void DrawReactOnTopLeft(float x, float y, float width, float height, int red, int green, int blue, int alpha);
	static void DrawBasicText(char* text, float x, float y, float scaleY, int font, int align);
	static void DrawBasicTextWithColor(char* text, float x, float y, float scaleY, int font, int red, int green, int blue, int alpha);
	static void DrawBasicTextWithColorDS(char* text, float x, float y, float scaleY, int font, int red, int green, int blue, int alpha);
	void OpenMenu();
	void CloseMenu();
	void SwitchOpenState();
	bool IsMenuOpen();
	void UpdateVehiclesList();
	void DrawMenu();
	bool allowEditVehicles = false;
	bool VehicleToBeRemoved(char** veh);
	bool VehicleToBeAdded(char** veh);
	InGameMenu(Settings* settingsPTR, std::list<char*>* deliveredVehiclesListPTR, std::list<const char*>* fullVehiclesListPTR)
	{
		gSettings = settingsPTR;
		deliveredVehicles = deliveredVehiclesListPTR;
		fullVehiclesList = fullVehiclesListPTR;
		settingsVector = std::vector<ConfigOptionStruct>();
		settingsVector.push_back({ "Lighthouse As Delivery",&(gSettings->LightHouseAsDelivery),"Enable the Lighthouse in Monte Gordo\nas a delivery point\n(REQUIRES SAVE LOAD / GAME RESTART)"});
		settingsVector.push_back({ "Pier As Delivery",&(gSettings->PierAsDelivery),"Enable Del Perro Pier as a delivery Point\n(REQUIRES SAVE LOAD / GAME RESTART)"});
		settingsVector.push_back({ "Pier State During DLG",&(gSettings->PierStateDuringDLG), "Should This trigger be active during\nDaddy's Little Girl?"});
		settingsVector.push_back({ "Simeon As Delivery",&(gSettings->SimeonAsDelivery),"Enable Premium Deluxe Motorsport\nas a Delivery Point\n(REQUIRES SAVE LOAD / GAME RESTART)"});
		settingsVector.push_back({ "Simeon State During Armenian",&(gSettings->SimeonStateDuringArmenian),"Should This trigger be active\nduring Armenian missions?"});
		settingsVector.push_back({ "Beach As Delivery",&(gSettings->BeachAsDelivery),"Enable Lifeguard Tower as a Delivery Point\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "Anti Parking Lot Beach",&(gSettings->AntiParkingLotBeach),"Anti-abuse of the parking lot at\nthe Lifeguard Tower"});
		settingsVector.push_back({ "Include Trailers",&(gSettings->EnableTrailers),"Include Trailers\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "Include FlyingVehicles",&(gSettings->EnableFlyingVehicles),"Include Heli, Planes, blimps, etc..\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "Include WaterVehicles",&(gSettings->EnableWaterVehicles),"Boats and Minisubmarines\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "Bury the hatchet Snow Vehicles",&(gSettings->EnableBuryNYVehicles),"North Yankton snow vehicles from\nBury The Hatchet\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "On Mission Zero Vehicles",&(gSettings->OnMissionZeroVehicles),"Only the Clown Van at the moment\n(REQUIRES SAVE LOAD/ GAME RESTART)"});
		settingsVector.push_back({ "Display Max Amount",&(gSettings->DisplayMaxAmount),"Display the amount of vehicles that can be\ndelivered"});
		settingsVector.push_back({ "Show Help Text",&(gSettings->ShowHelpText),"Show the help text every 30 minutes talking\nabout the missing vehicles menu"});
		settingsVector.push_back({ "Edit Mode", &allowEditVehicles,"Press Enter in the Missing and Delivered\nVehicles menu to add or remove a vehicle\nfrom the list"  });
	}
private:
	enum MenuPage {
		Main = 0,
		Missing,
		Delivered,
		Config
	};
	MenuPage page = MenuPage::Main;
	MenuPage oldPage = MenuPage::Main;
	void DrawMissingVehicleSelection(float x, float y, int value, float width, float height, bool selected);
	void DrawDeliveredVehicleSelection(float x, float y, int value, float width, float height, bool selected);
	void DrawBasicSelection(float x, float y, char* text, float width, float height, bool selected);
	void DrawConfigBoolSelection(float x, float y, float width, float textHeight, float optionHeight, bool selected, int settingID);
	void IncreaseSelection();
	void DecreaseSelection();
	bool MenuOpen = false;
	int MainMenuSelection = 0;
	int DeliveredSelection = 0;
	int MissingSelection = 0;
	int ConfigSelection = 0;
	Settings* gSettings;
	std::list<char*>* deliveredVehicles;
	std::list<const char*>* fullVehiclesList;
	std::list<const char*> missingVehicles;
	std::vector<const char*> deliveredVehiclesVector;
	std::vector<const char*> missingVehiclesVector;
	DWORD startHoldTime = 0;
	const DWORD holdRepeatButtonTime = 125;
	struct ConfigOptionStruct {
		const char* name;
		bool* option;
		const char* tip;
	};
	std::vector<ConfigOptionStruct> settingsVector;
	void UpdateSettingsFile();
	void RequestVehicleToBeRemoved(char* veh);
	char* vehToBeRemoved = (char*)"";
	char* vehToBeAdded = (char*)"";
	void RequestVehicleToBeAdded(char* veh);
};

