![simeonautosbanner](https://github.com/user-attachments/assets/39f17048-ae7a-4671-ac17-49fcb32cce04)
# Super Simeon Autos

Super Simeon Autos (SSA) is a mod for Grand Theft Auto V  that lets you collect and deliver (almost) all vehicles to specific locations on the map.
Inspired by the Super Sunshine Autos and similar mods for the 3D era GTA Games.

This mod is aimed at speedrunners and experienced players, but most of the glitch heavy vehicles can be disabled (Bury the Hatchet and Om0 vehicles), so you might be able to have a smooth experience.

## Requirements
This mod currently only works for legacy patch 1.27 (the speedrunners patch) but current patch / Enhanced is on the works.
* Game Version: Grand Theft Auto V [version 1.27](https://github.com/TwosHusbandS/Project-127/) (Legacy).
* ScriptHookV: Install the latest version of [ScriptHookV](https://www.dev-c.com/gtav/scripthookv/) as the mod was developed using it.
* Returning Player Bonus: Required for all vehicles to function properly. The game will still launch without it, but vehicle collection may be impacted. (can be toggled on/off in the Project 1.27 launcher)

## Installation
For Current Patch / Enhanced:
1. Install the latest version of [ScriptHookV](https://www.dev-c.com/gtav/scripthookv/).
2. Download the latest release of Super Simeon Autos from the [Releases](https://github.com/GordoLeal/SuperSimeonAutos/releases) page.
3. Place the downloaded .asi file in the root of your GTA V installation directory.
4. Launch the game and load a save file to generate the <code>SSA_Settings.txt</code> configuration file, you can change it to how you want the gameplay to be or you can use the in game menu by pressing GET IN COVER (Default: Q)+ RELOAD GUN (Default: R).

For 1.27 (Speedrunners Patch):
1. Ensure your game is running GTA V version 1.27 (The Speedrunners Patch). To downgrade follow the [Project-127 Downgrade Guide](https://github.com/TwosHusbandS/Project-127/blob/master/Installer/Info/Help.md).
2. in Project 1.27: Enable "Returning Player Bonus" in Settings -> GTA & Launch Settings -> Tick the "Enable ReturningPlayerBonus" box
3. in Project 1.27: Enable "ScriptHook on Downgraded GTA" in Settings -> GTA & Launch Settings -> Tick the "ScriptHook on Downgraded GTA" box
4. Install the latest version of [ScriptHookV](https://www.dev-c.com/gtav/scripthookv/).
5. Download the latest release of Super Simeon Autos from the [Releases](https://github.com/GordoLeal/SuperSimeonAutos/releases) page.
6. Place the downloaded .asi file in the root of your GTA V installation directory.
7. Launch the game and load a save file to generate the <code>SSA_Settings.txt</code> configuration file, you can change it to how you want the gameplay to be or you can use the in game menu by pressing GET IN COVER (Default: Q)+ RELOAD GUN (Default: R).

## How to play
**Objective**: Locate vehicles across the map ([vehicles guide available](https://docs.google.com/document/d/1HLy1r_IYJAnth6uyUSzunOT0c65c5wLdxrW01GbjFZA/edit?usp=sharing)) and deliver them to highlighted delivery points.

**Customizable Settings**: Upon loading a save for the first time the SSA_Settings.txt file is created, allowing you to enable/disable up to 4 delivery points: Del Perro Pier, Simeon, Lighthouse and Lifeguard Tower. (Lifeguard is disabled by default)

![del perro pier](https://github.com/GordoLeal/SuperSimeonAutos/blob/master/githubpage/delperro.png)
![Simeon](https://github.com/GordoLeal/SuperSimeonAutos/blob/master/githubpage/simeon.png)
![Lighthouse](https://github.com/GordoLeal/SuperSimeonAutos/blob/master/githubpage/lighthouse.png)
![lifeguardtower](https://github.com/GordoLeal/SuperSimeonAutos/blob/master/githubpage/lifeguard.png)

## Best Experience
If you want the same experience as the mods from the 3D era games, enable ONLY the **Del Perro Pier** and **Lighthouse** as delivery points in SSA_Settings.txt.
If you want a chill and fast experience, keep all the options as default.

Other small options inside SSA_Settings.txt or the In Game Menu are also available for you to adjust your gameplay.

**Vehicle Tracking**: You can use the In Game Menu (press GET IN COVER (Default: Q) + RELOAD GUN (Default: R)) or the SSA_MissingVehicles.txt file to see your progress.

We also have a guide that you can access [here](https://docs.google.com/document/d/1HLy1r_IYJAnth6uyUSzunOT0c65c5wLdxrW01GbjFZA/edit?tab=t.0#heading=h.ufh0j67e710b) or you can check the [Wiki](https://github.com/GordoLeal/SuperSimeonAutos/wiki).

## Support
For bugs, questions, or suggestions, send a message in #ssa-mod-discussion in the [GTA V Speedruns Discord](https://discord.com/invite/3qjGGBM) or check the [Project-127 repository](https://github.com/TwosHusbandS/Project-127/) for more information.

## Credits
Mod Created By GordoLeal.

Thanks to Special For and Gogsi for providing help with the code and knowledge about the game.

Thanks to Unnamed, AlexHonix, Twisted and the GTAV speedrun community for helping with the vehicle list.

## Project Tree
This mod is coded and compiled with Visual Studio Community 2022.

Since i use the same inc and lib folders from ScriptHookV SDK to create other mods my folder tree is the one bellow.
This is more of a reminder to myself in case i want to update the mod. but might be usefull if someone want to mess around with this mess of code that i call SSA.

* Folder Setup for the project:
```
Projects Folder
> inc ( ScripthookV )
> lib ( ScripthookV )
> LegitimateBusiness (Mod folder)
  > code
  > githubpage
  > LegitimateBusiness.sln
  > etc..
> Other Mod Projects..
```


