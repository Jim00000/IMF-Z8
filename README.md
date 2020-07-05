# IMF-Z8

This project aims for automating the process of buying cards in the game (As you know that it is quite slow and boring to buy cards in a regular way). I also provide the speed hack to accelerate the whole process.

## Build

Before you start building IMF-Z8 by yourself, please make sure that you have all of necessary tools and libraries :
- Visual Studio 2019 (Be sure to Visual C++, Microsoft SDKs and .Net framework are installed)

and then `$ git clone https://github.com/Jim00000/IMF-Z8.git` to clone this repository.

After the download is complete, click IMF-Z8/IMF-Z8.sln to open Visual Studio IDE.

Build *BuildBatchGenerator* subproject at first and it is recommended to configure x64 Release build.

Go to IMF-Z8\x64\Release directory and run BuildBatchGenerator.exe program and you should get a *build.bat* file.

Copy or move that batch file (build.bat) to IMF-Z8\Third_Party directory (in the same directory with *Detours-4.0.1* folder) and run that batch.

It takes some time to build detours library.

While detours library is ready, return back to Visual Studio IDE and build the other subprojects (BuyCardBot, SpeedHack and SpeedHackDllInjector).

All of the executables are inside IMF-Z8\x64\Release directory and you should see following at least :
- BuyCardBot.exe
- SpeedHackDllInjector.exe
- SpeedHack.dll

## User guide

1. Run the game (YuGiOh.exe)
2. Run SpeedHackDllInjector.exe to speedhack (SpeedHackDllInjector.exe and SpeedHack.dll must be in the same directory) at first and do not close the cmd window.
3. Be sure to stay in the *Card Shop* in the game, run BuyCardBot.exe and configure which card set to buy with an index input to the cmd window of BuyCardBot.
4. Enjoy the convenience and do not move your mouse.

If you wanna disable the speedhack, find the cmd window of SpeedHackDllInjector and press any key to disable the speedhack effect.
If you wanna stop the buying cards process, just move your mouse and the cmd window of BuyCardBot will close automatically.

## Reference

- [Visual Studio](https://visualstudio.microsoft.com/)
- [Cheat Engine](https://github.com/cheat-engine/cheat-engine)
- [Detours library of Microsoft](https://github.com/microsoft/Detours)
- [Speedhack by absoIute (Github)](https://github.com/absoIute/Speedhack)
- [API Hooking with MS Detours (Code Project)](https://www.codeproject.com/Articles/30140/API-Hooking-with-MS-Detours)
- [SendInput documentation of Microsoft](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput)

## License

This project is under GPLv3 license
