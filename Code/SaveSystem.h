#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <list>

class SaveSystem
{
public:
	enum ErrSave {
		SaveDone = 0,
		DocumentsFolderNotFound,
		EnhancedFolderNotFound,
		EnhancedFolderEmpty,
		OldPatchFolderNotFound,
		OldPatchFolderEmpty,
		FileDoesNotExist,
		FolderNotFound,
		FileDoesNotExistOrNotBellowBuffer,
		CouldNotGetGTAVModule,
		AddressToPaternNotFound,
		ModuleHandleWasNull,
		SaveFileDontHaveData,
		UNK // This should only be used during development as placeholder.
	};

	/// <summary>
	/// Reads the last written time for every save file in the path and tried to save the progress in the last written save.
	/// </summary>
	/// <param name="vehicles"></param>
	/// <param name="collectedAmount"></param>
	/// <param name="isCurrentPatch"></param>
	/// <param name="SaveFolderPath"></param>
	/// <returns></returns>
	static ErrSave SaveProgress(std::list<char*> vehicles, bool isCurrentPatch, std::wstring SaveFolderPath);
	static ErrSave SaveProgressForReplay(std::list<char*> vehicles, bool isCurrentPatch, std::wstring SaveFolderPath);
	static ErrSave GetPointerToBeLoadedSaveFile(intptr_t* pointerBuffer);
	static ErrSave LoadProgressForFirstTime(std::wstring saveFolderPath, std::list<char*>& deliveredVehiclesFromSave);
	/// <summary>
	/// 
	/// </summary>
	static ErrSave LoadProgress(std::wstring saveFolderPath, int saveSlotNumber, std::list<char*>& deliveredVehiclesFromSave);
	static ErrSave LoadProgressFromReplay(std::wstring saveFolderPath, std::list<char*>& deliveredVehiclesFromSave);
	/// <summary>
	/// Quick helper to get the value in the address of global and insert into the pointer for the int.
	/// </summary>
	static ErrSave GetLastReadSlotNumber(int* lastSavedSlotBuffer, intptr_t* pointerToLastSavedSlotNumber);
	/// <summary>
	/// Quick helper to get the Char 
	/// </summary>
	/// <param name="lastSavedSlotBuffer"></param>
	/// <param name="pointerToLastSavedSlotNumber"></param>
	/// <returns></returns>
	static ErrSave GetToBeReadSaveFile(char** ToBeReadSaveFile, intptr_t* pointerToBeReadSaveFile);
	/// <summary>
	/// Try get the pointer to the address of the global variable that holds the int value for the last loaded slot.
	/// </summary>
	static ErrSave GetPointerToLastLoadedSlotNumber(intptr_t* pointerBuffer);

	static ErrSave GetPointerToIsSaveHappening(intptr_t* pointerBuffer);
	static ErrSave GetIntPointerFromPointer(int** inB, intptr_t* pointer);
	/// <summary>
	/// returns the path to the save file folder
	/// </summary>
	static ErrSave GetSaveFilePath(bool isCurrentPatch,bool isEnhanced, std::wstring* pathToSaveFolder);
	//static ErrSave FillArrayWithSaveFileData(std::wstring saveFolderPath, std::wstring saveFileName, std::list<char*>& deliveredVehiclesFromSave);
};

