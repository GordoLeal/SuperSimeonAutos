#pragma warning(push)
#pragma warning(disable : 28159)
#include "InGameMenu.h"
#include "keyboard.h"
#include "enums.h"


void InGameMenu::DrawReactOnTopLeft(float x, float y, float width, float height, int red, int green, int blue, int alpha)
{
	GRAPHICS::DRAW_RECT(x + (width / 2), y + (height / 2), width, height, red, green, blue, alpha);
}

void InGameMenu::DrawBasicText(char* text, float x, float y, float scaleY, int font)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0, scaleY);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	//	FONT_CENTRE = 0
	//	FONT_LEFT,= 1
	//	FONT_RIGHT = 2
	//	FONT_JUSTIFY= 3
	UI::SET_TEXT_JUSTIFICATION(1);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}

void InGameMenu::DrawBasicTextWithColor(char* text, float x, float y, float scaleY, int font, int red, int green, int blue, int alpha)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0, scaleY);
	UI::SET_TEXT_COLOUR(red, green, blue, alpha);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	//	FONT_CENTRE = 0
	//	FONT_LEFT,= 1
	//	FONT_RIGHT = 2
	//	FONT_JUSTIFY= 3
	UI::SET_TEXT_JUSTIFICATION(1);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}

void InGameMenu::DrawBasicTextWithColorDS(char* text, float x, float y, float scaleY, int font, int red, int green, int blue, int alpha)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0, scaleY);
	UI::SET_TEXT_COLOUR(red, green, blue, alpha);
	UI::SET_TEXT_DROPSHADOW(8, 0, 0, 0, 255);
	//	FONT_CENTRE = 0
	//	FONT_LEFT,= 1
	//	FONT_RIGHT = 2
	//	FONT_JUSTIFY= 3
	UI::SET_TEXT_JUSTIFICATION(1);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}

void InGameMenu::OpenMenu()
{
	MenuOpen = true;
	page = InGameMenu::MenuPage::Main;
}

void InGameMenu::CloseMenu()
{
	MenuOpen = false;
	page = InGameMenu::MenuPage::Main;
}

void InGameMenu::SwitchOpenState()
{
	MenuOpen = !MenuOpen;
	page = InGameMenu::MenuPage::Main;
}

bool InGameMenu::IsMenuOpen()
{
	return MenuOpen;
}

void InGameMenu::UpdateVehiclesList()
{
	deliveredVehiclesVector = std::vector<const char*>();
	missingVehiclesVector = std::vector<const char*>();

	for (const char* veh : *fullVehiclesList)
	{
		bool found = false;
		for (char* b : *deliveredVehicles)
		{
			std::string a(veh);
			if (a.compare(b) == 0)
			{
				found = true;
				deliveredVehiclesVector.push_back(veh);
				break;
			}
		}

		if (!found)
		{
			missingVehiclesVector.push_back(veh);
		}
	}
}

void InGameMenu::IncreaseSelection()
{
	switch (page)
	{
	case InGameMenu::Main:
		MainMenuSelection++;
		break;
	case InGameMenu::Missing:
		MissingSelection++;
		break;
	case InGameMenu::Delivered:
		DeliveredSelection++;
		break;
	case InGameMenu::Config:
		ConfigSelection++;
		break;
	}
}
void InGameMenu::DecreaseSelection()
{
	switch (page)
	{
	case InGameMenu::Main:
		MainMenuSelection--;
		break;
	case InGameMenu::Missing:
		MissingSelection--;
		break;
	case InGameMenu::Delivered:
		DeliveredSelection--;
		break;
	case InGameMenu::Config:
		ConfigSelection--;
		break;
	}
}

void InGameMenu::UpdateSettingsFile()
{
	gSettings->CreateSettingsFile();
}



//The Main Update loop
void InGameMenu::DrawMenu()
{
	if (!MenuOpen)
	{
		return;
	}
	CONTROLS::DISABLE_CONTROL_ACTION(0, 27, true); // Pull Phone
	//Phone
	//BUG: for some reason only the UP input on the phone is actually blocked, and after trying a few options i gave up
	// just block the phone from going up.
	// i have a option that is to block all the inputs, but i don't wanna do this way.
	//CONTROLS::DISABLE_CONTROL_ACTION(2,172,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,173,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,174,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,175,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,176,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,177,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,178,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,179,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,180,true);
	//CONTROLS::DISABLE_CONTROL_ACTION(2,181,true);
	// Interaction Menu
	CONTROLS::DISABLE_CONTROL_ACTION(0, 244, true);


	float menuPosX = 0.015f;
	float menuPosY = 0.050f;
	float menuW = 0.2f;
	float mainBannerH = 0.05f;
	float secondaryBannerH = 0.03f;
	float selectionH = 0.035f;


	//Top Banner
	DrawReactOnTopLeft(menuPosX, menuPosY, menuW, mainBannerH, 110, 0, 255, 255);
	DrawBasicText((char*)"SUPER SIMEON AUTOS", menuPosX + 0.005f, menuPosY + 0.015f, 0.5f, 1);

	float anchorSecBanner = menuPosY + mainBannerH;

	if (oldPage != page) {
		oldPage = page;
	}

	if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, eControl::ControlFrontendCancel))
	{
		if (page != InGameMenu::MenuPage::Main)
			page = InGameMenu::MenuPage::Main;
		else
		{
			CloseMenu();
		}
	}

	// Check if the player pressed the button to go UP/DOWN in the menu.
	// if he is holding it, keep spamming the UP/DOWN

	if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, eControl::ControlFrontendUp))
	{
		startHoldTime = GetTickCount();
		DecreaseSelection();
	}
	else if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, eControl::ControlFrontendDown))
	{
		startHoldTime = GetTickCount();
		IncreaseSelection();
	}

	if (CONTROLS::IS_CONTROL_PRESSED(2, eControl::ControlFrontendUp) && startHoldTime + holdRepeatButtonTime < GetTickCount())
	{
		startHoldTime = GetTickCount();
		DecreaseSelection();
	}
	else if (CONTROLS::IS_CONTROL_PRESSED(2, eControl::ControlFrontendDown) && startHoldTime + holdRepeatButtonTime < GetTickCount())
	{
		startHoldTime = GetTickCount();
		IncreaseSelection();
	}

	float anchorListStart = menuPosY + mainBannerH + secondaryBannerH;
	switch (page)
	{
	case InGameMenu::Main:
		if (MainMenuSelection < 0)
			MainMenuSelection = 0;
		if (MainMenuSelection >= 3)
			MainMenuSelection = 2;

		DrawReactOnTopLeft(menuPosX, anchorSecBanner, menuW, secondaryBannerH, 0, 0, 0, 255);
		DrawBasicText((char*)"Main Menu", menuPosX + 0.005f, anchorSecBanner, 0.4f, 0);

		DrawBasicSelection(menuPosX, anchorListStart, (char*)"Missing Vehicles", menuW, selectionH, MainMenuSelection == 0 ? true : false);
		DrawBasicSelection(menuPosX, anchorListStart + selectionH, (char*)"Delivered Vehicles", menuW, selectionH, MainMenuSelection == 1 ? true : false);
		DrawBasicSelection(menuPosX, anchorListStart + (selectionH * 2), (char*)"Settings", menuW, selectionH, MainMenuSelection == 2 ? true : false);
		//help 1
		DrawReactOnTopLeft(menuPosX, anchorListStart + (selectionH * 3), menuW, selectionH, 0, 0, 0, 255);
		DrawBasicText((char*)"Use the UP/DOWN arrow keys to move.", menuPosX + 0.0075f,anchorListStart + (selectionH * 3) + 0.0075f, 0.27f, 0);
		//Help 2
		DrawReactOnTopLeft(menuPosX, anchorListStart + (selectionH * 4), menuW, selectionH, 0, 0, 0, 255);
		DrawBasicText((char*)"Press ENTER to confirm", menuPosX + 0.0075f, anchorListStart + (selectionH * 4) + 0.0075f, 0.27f, 0);
		//Help3
		DrawReactOnTopLeft(menuPosX, anchorListStart + (selectionH * 5), menuW, selectionH, 0, 0, 0, 255);
		DrawBasicText((char*)"Press BACKSPACE to return", menuPosX + 0.0075f, anchorListStart + (selectionH * 5) + 0.0075f, 0.27f, 0);

		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, eControl::ControlFrontendAccept))
		{
			switch (MainMenuSelection)
			{
			case 0:
				page = InGameMenu::Missing;
				break;
			case 1:
				page = InGameMenu::Delivered;
				break;
			case 2:
				page = InGameMenu::Config;
				break;
			}
		}
		break;

	case InGameMenu::Missing:

		DrawReactOnTopLeft(menuPosX, anchorSecBanner, menuW, secondaryBannerH, 0, 0, 0, 255);
		DrawBasicText((char*)"Missing Vehicles", menuPosX + 0.005f, anchorSecBanner, 0.4f, 0);
		if (MissingSelection < 0)
			MissingSelection = 0;
		if (MissingSelection >= missingVehiclesVector.size())
			MissingSelection = missingVehiclesVector.size() - 1;

		DrawMissingVehicleSelection(menuPosX, anchorListStart, MissingSelection - 3, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + selectionH, MissingSelection - 2, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 2), MissingSelection - 1, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 3), MissingSelection, menuW, selectionH, true);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 4), MissingSelection + 1, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 5), MissingSelection + 2, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 6), MissingSelection + 3, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 7), MissingSelection + 4, menuW, selectionH, false);
		DrawMissingVehicleSelection(menuPosX, anchorListStart + (selectionH * 8), MissingSelection + 5, menuW, selectionH, false);
		if (allowEditVehicles)
		{
			DrawReactOnTopLeft(menuPosX, anchorListStart + (selectionH * 9), menuW, selectionH, 0, 0, 0, 255);
			DrawBasicText((char*)"[EDIT MODE] Press ENTER to add this vehicle.",menuPosX+0.0075f, anchorListStart + (selectionH * 9)+0.0075f,0.27f,0);
		}
		break;

	case InGameMenu::Delivered:
		DrawReactOnTopLeft(menuPosX, anchorSecBanner, menuW, secondaryBannerH, 0, 0, 0, 255);
		DrawBasicText((char*)"Delivered Vehicles", menuPosX + 0.005f, anchorSecBanner, 0.4f, 0);

		if (DeliveredSelection < 0)
			DeliveredSelection = 0;
		if (DeliveredSelection >= deliveredVehiclesVector.size())
			DeliveredSelection = deliveredVehiclesVector.size() - 1;

		DrawDeliveredVehicleSelection(menuPosX, anchorListStart, DeliveredSelection - 2, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + selectionH, DeliveredSelection - 1, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 2), DeliveredSelection, menuW, selectionH, true);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 3), DeliveredSelection + 1, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 4), DeliveredSelection + 2, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 5), DeliveredSelection + 3, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 6), DeliveredSelection + 4, menuW, selectionH, false);
		DrawDeliveredVehicleSelection(menuPosX, anchorListStart + (selectionH * 7), DeliveredSelection + 5, menuW, selectionH, false);
		if (allowEditVehicles)
		{
			DrawReactOnTopLeft(menuPosX, anchorListStart + (selectionH * 8), menuW, selectionH, 0, 0, 0, 255);
			DrawBasicText((char*)"[EDIT MODE] Press ENTER to remove this vehicle.", menuPosX + 0.0075f, anchorListStart + (selectionH * 8) + 0.0075f, 0.27f, 0);
		}
		break;

	case InGameMenu::Config:
		float configSelectionH = 0.030f;
		float configOptionH = 0.030f;
		float bothH = configSelectionH + configOptionH;
		DrawReactOnTopLeft(menuPosX, anchorSecBanner, menuW, secondaryBannerH, 0, 0, 0, 255);
		DrawBasicText((char*)"Settings", menuPosX + 0.005f, anchorSecBanner, 0.4f, 0);
		float configsAmount = 5;
		if (ConfigSelection < 0)
			ConfigSelection = 0;
		if (ConfigSelection >= settingsVector.size())
			ConfigSelection = settingsVector.size() - 1;

		DrawConfigBoolSelection(menuPosX, anchorListStart, menuW, configSelectionH, configOptionH, false, ConfigSelection - 2);
		DrawConfigBoolSelection(menuPosX, anchorListStart + bothH, menuW, configSelectionH, configOptionH, false, ConfigSelection - 1);
		DrawConfigBoolSelection(menuPosX, anchorListStart + (bothH * 2), menuW, configSelectionH, configOptionH, true, ConfigSelection);
		DrawConfigBoolSelection(menuPosX, anchorListStart + (bothH * 3), menuW, configSelectionH, configOptionH, false, ConfigSelection + 1);
		DrawConfigBoolSelection(menuPosX, anchorListStart + (bothH * 4), menuW, configSelectionH, configOptionH, false, ConfigSelection + 2);
		DrawReactOnTopLeft(menuPosX, anchorListStart + (bothH * 5), menuW, 0.0625f, 0, 0, 0, 255);
		DrawBasicText((ConfigSelection >= 0 && ConfigSelection < settingsVector.size()) ? (char*)settingsVector[ConfigSelection].tip : (char*)"",
			menuPosX + 0.005f,
			anchorListStart + (bothH * 5),
			0.3f,
			0);

		break;
	}
}

bool InGameMenu::VehicleToBeRemoved(char** veh)
{
	if (vehToBeRemoved != "") {
		*veh = vehToBeRemoved;
		vehToBeRemoved = (char*)"";
		return true;
	}
	return false;
}

bool InGameMenu::VehicleToBeAdded(char** veh)
{
	if (vehToBeAdded != "") {
		*veh = vehToBeAdded;
		vehToBeAdded = (char*)"";
		return true;
	}
	return false;
}

void InGameMenu::RequestVehicleToBeRemoved(char* veh)
{
	vehToBeRemoved = veh;
}

void InGameMenu::RequestVehicleToBeAdded(char* veh)
{
	vehToBeAdded = veh;
}

void InGameMenu::DrawMissingVehicleSelection(float x, float y, int value, float width, float height, bool selected)
{
	if (selected)
	{
		DrawReactOnTopLeft(x, y, width, height, 255, 255, 255, 200);
	}
	else
	{
		DrawReactOnTopLeft(x, y, width, height, 0, 0, 0, 200);
	}
	if (value >= 0 && value < missingVehiclesVector.size())
	{
		std::string totalname = "";
		totalname += "(";
		totalname += missingVehiclesVector[value];
		totalname += ") ";
		totalname += UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(GAMEPLAY::GET_HASH_KEY((char*)missingVehiclesVector[value])));

		if (selected)
		{
			DrawBasicTextWithColor((char*)totalname.c_str(), x + 0.005f, y + 0.0035f, 0.35f, 0, 0, 0, 0, 255);
			// if is selected and allow edit is available.
			// enable the possibility to request an edit
			if (allowEditVehicles && CONTROLS::IS_CONTROL_JUST_PRESSED(0, eControl::ControlFrontendAccept)) 
			{
				RequestVehicleToBeAdded((char*)missingVehiclesVector[value]);
			}
		}
		else
		{
			DrawBasicText((char*)totalname.c_str(), x + 0.005f, y + 0.0035f, 0.35f, 0);
		}
	}
}

void InGameMenu::DrawDeliveredVehicleSelection(float x, float y, int value, float width, float height, bool selected)
{
	if (selected)
	{
		DrawReactOnTopLeft(x, y, width, height, 255, 255, 255, 200);
	}
	else
	{
		DrawReactOnTopLeft(x, y, width, height, 0, 0, 0, 200);
	}
	if (value >= 0 && value < deliveredVehiclesVector.size())
	{
		std::string totalname = "";
		totalname += "(";
		totalname += deliveredVehiclesVector[value];
		totalname += ") ";
		totalname += UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(GAMEPLAY::GET_HASH_KEY((char*)deliveredVehiclesVector[value])));

		if (selected)
		{
			DrawBasicTextWithColor((char*)totalname.c_str(), x + 0.005f, y + 0.0035f, 0.35f, 0, 0, 0, 0, 255);
			// if is selected and allow edit is available.
			// enable the possibility to request an edit
			if (allowEditVehicles && CONTROLS::IS_CONTROL_JUST_PRESSED(0, eControl::ControlFrontendAccept))
			{
				RequestVehicleToBeRemoved((char*)deliveredVehiclesVector[value]);
			}
		}
		else
		{
			DrawBasicText((char*)totalname.c_str(), x + 0.005f, y + 0.0035f, 0.35f, 0);
		}
	}
}

void InGameMenu::DrawConfigBoolSelection(float x, float y, float width, float textHeight, float optionHeight, bool selected, int settingID)
{
	const float textSize = 0.30f;
	const float textOptionSize = 0.30f;
	if (selected)
	{
		DrawReactOnTopLeft(x, y, width, textHeight, 255, 255, 255, 200);
		DrawReactOnTopLeft(x, y + textHeight, width, optionHeight, 200, 200, 200, 200);
	}
	else
	{
		DrawReactOnTopLeft(x, y, width, textHeight, 0, 0, 0, 200);
		DrawReactOnTopLeft(x, y + textHeight, width, optionHeight, 0, 0, 0, 200);
	}

	if (settingID >= 0 && settingID < settingsVector.size())
	{
		if (selected)
		{
			DrawBasicTextWithColor((char*)settingsVector[settingID].name, x + 0.005f, y, textSize, 0, 0, 0, 0, 255);

			if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, eControl::ControlFrontendAccept))
			{
				*(settingsVector[settingID].option) = !*(settingsVector[settingID].option);
				UpdateSettingsFile();
			}
		}
		else
		{
			DrawBasicText((char*)settingsVector[settingID].name, x + 0.005f, y, textSize, 0);
		}

		if (*settingsVector[settingID].option)
			DrawBasicTextWithColorDS((char*)"Enabled", x + 0.005f, y + textHeight, textOptionSize, 0, 100, 200, 100, 255);
		else
			DrawBasicTextWithColorDS((char*)"Disabled", x + 0.005f, y + textHeight, textOptionSize, 0, 200, 100, 100, 255);
	}
}

void InGameMenu::DrawBasicSelection(float x, float y, char* text, float width, float height, bool selected)
{
	if (selected)
	{
		DrawReactOnTopLeft(x, y, width, height, 255, 255, 255, 200);
		DrawBasicTextWithColor(text, x + 0.005f, y + 0.0035f, 0.35f, 0, 0, 0, 0, 255);
	}
	else
	{
		DrawReactOnTopLeft(x, y, width, height, 0, 0, 0, 200);
		DrawBasicText(text, x + 0.005f, y + 0.0035f, 0.35f, 0);
	}
}
