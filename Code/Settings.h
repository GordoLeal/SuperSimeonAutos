#pragma once
class Settings
{
public:
	bool LightHouseAsDelivery;
	bool BeachAsDelivery;
	bool SimeonAsDelivery;
	bool SimeonStateDuringArmenian;
	bool PierAsDelivery;
	bool PierStateDuringDLG;
	bool EnableTrailers;
	bool EnableFlyingVehicles;
	bool EnableWaterVehicles;
	bool EnableBuryNYVehicles;
	bool DisplayMaxAmount;
	bool ShowHelpText;
	bool AntiParkingLotBeach;
	bool OnMissionZeroVehicles;
	bool DoesSettingsFileExists();
	void ReadValuesFromSettingsFile();
	void CreateSettingsFile();
};

