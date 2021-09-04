#DSMirror

This is a mirror mode for Dark Souls 1 PTDE. It works by flipping the screen (excluding the UI), then inverting controls and 3D positional audio to match.

##Features

- Mirror the world of Dark Souls to experience it in a different way
- Swap the attack buttons and weapon switch buttons/icons
  - Off by default but can be enabled in dsmirror.ini
- Press the '+' key to toggle mirror mode (key configurable in dsmirror.ini)

##Installation

If you're using DSfix:

- Move dinput8.dll into the dsmirror folder
- Move the dsmirror folder into your Dark Souls folder: `C:\Program Files (x86)\Steam\steamapps\common\Dark Souls Prepare to Die Edition\DATA`
- In DSfix.ini, set dinput8dllWrapper to `dsmirror\dinput8.dll`

If you're not using DSfix:

- Move dinput8.dll and dsmirror into your Dark Souls folder

##Compatibility With Other Mods

If you want to use another dinput8.dll mod with this mod, you can set `load_dll` in dsmirror.ini to the path of the other DLL (relative to DARKSOULS.exe). Note that some mods may be incompatible.