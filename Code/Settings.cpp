#pragma once
#include "Settings.h"
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <Windows.h>

const char* SettingsFileName = "SSA_Settings.txt";

void Settings::CreateSettingsFile()
{
	std::fstream settingsStream;
	settingsStream.open(SettingsFileName, std::ios::in | std::ios::out | std::ios::trunc);
	settingsStream.clear();
	if (settingsStream.is_open()) {
		settingsStream << "# SETTINGS FOR SUPER SIMEON AUTOS\n";
		settingsStream << "# 0 = Disabled | 1 = Enabled\n\n";

		settingsStream << "# Enable lighthouse as a delivery point [enabled (1) by default]\n";
		settingsStream << "LightHouseAsDelivery=";
		settingsStream << LightHouseAsDelivery;
		settingsStream << "\n\n";

		settingsStream << "# Enable the Del Perro Pier as a delivery point [enabled (1) by default]\n";
		settingsStream << "PierAsDelivery=";
		settingsStream << PierAsDelivery;
		settingsStream << "\n\n";

		settingsStream << "# Enable/Disable the Del Perro Pier as a delivery point during Daddy's Little Girl [enabled (1) by default]\n";
		settingsStream << "PierStateDuringDLG=";
		settingsStream << PierStateDuringDLG;
		settingsStream << "\n\n";

		settingsStream << "# Enable or Disable Simeon as a delivery point during Armenian missions (Frankling and Lamar | Complications) [enabled (1) by default]\n";
		settingsStream << "SimeonStateDuringArmenian=";
		settingsStream << SimeonStateDuringArmenian;
		settingsStream << "\n\n";

		settingsStream << "# Enable the lifeguard tower (Vespucci Beach) as a delivery point [Disabled (0) by default]\n";
		settingsStream << "BeachAsDelivery=";
		settingsStream << BeachAsDelivery;
		settingsStream << "\n\n";

		settingsStream << "# Protection for the player to not abuse the parking lot next to lifeguard tower (anti cheat) [enabled (1) by default]\n";
		settingsStream << "AntiParkingLotBeach=";
		settingsStream << AntiParkingLotBeach;
		settingsStream << "\n\n";

		settingsStream << "# Enable Simeon as a delivery point [enabled (1) by default]\n";
		settingsStream << "SimeonAsDelivery=";
		settingsStream << SimeonAsDelivery;
		settingsStream << "\n\n";

		settingsStream << "# Enable Trailers as vehicles that can be delivered [enabled (1) by default]\n";
		settingsStream << "EnableTrailers=";
		settingsStream << EnableTrailers;
		settingsStream << "\n\n";

		settingsStream << "# Enable Heli/Planes to be delivered [enabled (1) by default]\n";
		settingsStream << "EnableFlyingVehicles=";
		settingsStream << EnableFlyingVehicles;
		settingsStream <<"\n\n";

		settingsStream << "# Enable boats to be delivered [enabled (1) by default]\n";
		settingsStream << "EnableWaterVehicles="; 
		settingsStream << EnableWaterVehicles;
		settingsStream << "\n\n";

		settingsStream << "# Enable Bury the Hatchet ( North Yankton ) vehicles [Disabled (0) by default]\n";
		settingsStream << "EnableBuryNYVehicles=";
		settingsStream << EnableBuryNYVehicles;
		settingsStream << "\n\n";
		
		settingsStream << "# Enable On mission 0 vehicles ( only clown car at the moment ) [Disabled (0) by default]\n";
		settingsStream << "OnMissionZeroVehicles=";
		settingsStream << OnMissionZeroVehicles;
		settingsStream << "\n\n";

		settingsStream << "# Display Max Amount of vehicles to be collected [enabled (1) by default]\n";
		settingsStream << "DisplayMaxAmount=";
		settingsStream << DisplayMaxAmount;
		settingsStream << "\n\n";

		settingsStream << "# Show help text about missing vehicles list every 30 minutes [enabled (1) by default]\n";
		settingsStream << "ShowHelpText=";
		settingsStream << ShowHelpText;
		settingsStream << "\n\n";

		settingsStream <<  "# Tip: If for some reason you lose your progress (maybe a bug or lost save file)\n";
		settingsStream <<  "# load a save in the game and generate the SSA_MissingVehicles by holding your \"Get in cover\" and \"Reload Weapon\" buttons (Q and R by default) for 5 seconds\n";
		settingsStream <<  "# Inside the SSA_MissingVehicles.txt, add a + or ! at the end of the line for every vehicle you want to Add or Remove from the list.\n";
		settingsStream <<  "# like this: \"Asea (ASEA2)+\" or \"Asea (ASEA2)!\" \n";
		settingsStream <<  "# Inside the game hold the \"Get in Cover\" and \"Jump\" buttons (Q and Spacebar by default), this will load all the vehicles with + at the end\n";

		settingsStream << "# Mod Made by GordoLeal\n";
		settingsStream << "# Twitch.tv/GordoLeal";
	}
	settingsStream.close();
}

bool Settings::DoesSettingsFileExists()
{
	std::ifstream settingsStream;
	settingsStream.open(SettingsFileName);
	bool exists = false;
	if (settingsStream.is_open())
	{
		exists = true;
	}
	settingsStream.close();
	return exists;
}

bool stringToBool(std::string inS) {
	if (inS.at(0) == '0') {
		return false;
	}
	else if (inS.at(0) == '1') {
		return true;
	}
	else
	{
		return false;
	}
}

void Settings::ReadValuesFromSettingsFile()
{
	if (!DoesSettingsFileExists()) {

		LightHouseAsDelivery = true;
		BeachAsDelivery = false;
		SimeonAsDelivery = true;
		SimeonStateDuringArmenian = true;
		PierAsDelivery = true;
		PierStateDuringDLG = true;
		EnableTrailers = true;
		EnableFlyingVehicles = true;
		EnableWaterVehicles = true;
		DisplayMaxAmount = true;
		ShowHelpText = true;
		AntiParkingLotBeach = true;
		EnableBuryNYVehicles = false;
		OnMissionZeroVehicles = false;
		CreateSettingsFile();
		return;
	}

	std::fstream settFileStream;
	settFileStream.open(SettingsFileName, std::ios::out | std::ios::in);
	std::string line;
	while (std::getline(settFileStream, line))
	{
		bool End = false;
		bool readingCmd = true;
		bool foundCmd = false;
		std::string command;
		std::string value;

		for (int x = 0; x < line.size(); x++)
		{
			if (line.at(0) == '#' && line.at(0) == '\n')
			{
				break;
			}

			switch (line.at(x))
			{
			case '\n':
				End = true;
				break;
			case '=':
				readingCmd = false;
				foundCmd = true;
				break;
			default:
			{
				if (readingCmd) {
					command += line.at(x);
				}
				else
				{
					value += line.at(x);
					End = true;
				}
				break;
			}
			}

			if (End)
				break;
		}

		if (foundCmd)
		{
			bool found = false;
			if (!found && command.find("LightHouseAsDelivery") == 0)
			{
				LightHouseAsDelivery = stringToBool(value);
				found = true;
			}
			if (!found && command.find("BeachAsDelivery") == 0)
			{
				BeachAsDelivery = stringToBool(value);
				found = true;
			}
			if (!found && command.find("SimeonAsDelivery") == 0)
			{
				SimeonAsDelivery = stringToBool(value);
				found = true;
			}
			if (!found && command.find("SimeonStateDuringArmenian") == 0)
			{
				SimeonStateDuringArmenian = stringToBool(value);
				found = true;
			}
			if (!found && command.find("PierAsDelivery") == 0)
			{
				PierAsDelivery = stringToBool(value);
				found = true;
			}
			if (!found && command.find("EnableTrailers") == 0)
			{
				EnableTrailers = stringToBool(value);
				found = true;
			}
			if (!found && command.find("PierStateDuringDLG") == 0)
			{
				PierStateDuringDLG = stringToBool(value);
				found = true;
			}
			if (!found && command.find("EnableFlyingVehicles") == 0)
			{
				EnableFlyingVehicles = stringToBool(value);
				found = true;
			}
			if (!found && command.find("EnableWaterVehicles") == 0)
			{
				EnableWaterVehicles = stringToBool(value);
				found = true;
			}
			if (!found && command.find("DisplayMaxAmount") == 0)
			{
				DisplayMaxAmount = stringToBool(value);
				found = true;
			}
			if (!found && command.find("ShowHelpText") == 0)
			{
				ShowHelpText = stringToBool(value);
				found = true;
			}
			if (!found && command.find("AntiParkingLotBeach") == 0)
			{
				AntiParkingLotBeach = stringToBool(value);
				found = true;
			}
			if (!found && command.find("EnableBuryNYVehicles") == 0)
			{
				EnableBuryNYVehicles = stringToBool(value);
				found = true;
			}
			if (!found && command.find("OnMissionZeroVehicles") == 0)
			{
				OnMissionZeroVehicles = stringToBool(value);
				found = true;
			}

		}
	}
}


