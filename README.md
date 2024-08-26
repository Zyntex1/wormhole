# <img src="resource/icon.png" width="23"/> Photon <!--- [![CI](https://github.com/hero622/photon/actions/workflows/CI.yml/badge.svg)](https://github.com/hero622/photon/actions/workflows/CI.yml) --> [![C++](https://img.shields.io/badge/language-C%2B%2B-f34b7d)](https://en.wikipedia.org/wiki/C%2B%2B) [![Portal 2](https://img.shields.io/badge/game-Portal%202-blue)](https://store.steampowered.com/app/620/Portal_2/) [![Platform](https://img.shields.io/badge/platform-Windows%20%26%20Linux-green)](https://en.wikipedia.org/wiki/Cross-platform_software)

**Photon** is a mod loader for Portal 2.

### Installation
- Download the newest release from [here](https://github.com/hero622/photon/releases).
- Place it next to Portal 2's executable (typically in `SteamLibrary/steamapps/common/Portal 2`).
- Open the game and type `plugin_load photon` in the console.

### Mods
- Install mods by placing them in the `photon` folder inside the Portal 2 directory.
- Enable them from the menu that you can open by pressing <kbd>Insert</kbd>.

## Development

### Building

Requires CMake 3.15.

#### Windows
```
cmake -B build -A Win32
cmake --build build --config Debug/Release
```

#### Linux
```
cmake -B build
cmake --build build --config Debug/Release
```

> [!TIP]
> Configure paths in `cmake.toml` and use `cmake --install build`.

### Pull requests
- Follow the code style.
- Do not stage files that you had to configure.

### Code style
Use the included `.clang-format` file.

## Creating mods
> [!NOTE]
> There's an included [example mod](example-mod) with a few features you can take a look at.

- Download and include the [SDK](src/sdk) in your project.
- Compile your project as a 32-bit shared library using the **C++20** standard.
- Use the `/Zc:preprocessor` compiler flag on **MSVC** only!

#### Barebones mod example
```cpp
/* mod.h */
#pragma once

#include <photon.h>

class c_photon_mod : public photon_api::i_photon_mod {
public:
	virtual bool                   load( photon_api::c_shared* photon );
	virtual void                   unload( );
	virtual void                   on_event( const char* msg );
	virtual photon_api::mod_info_t get_info( );
	virtual void                   paint_menu( );
};

extern c_photon_mod mod;

/* mod.cpp */
#include "mod.h"

c_photon_mod mod;

EXPOSE_PHOTON_MOD( c_photon_mod, mod );

photon_api::c_shared* photon;

bool c_photon_mod::load( photon_api::c_shared* photon ) {
	::photon = photon; // expose photon interface globally.

	photon->common->log( "example mod loaded.\n" );

	return true;
}

void c_photon_mod::unload( ) {
	// unallocate stuff here etc.

	photon->common->log( "example mod unloaded.\n" );
}

void c_photon_mod::on_event( const char* msg ) {
	// handle events here.
}

photon_api::mod_info_t c_photon_mod::get_info( ) {
	photon_api::mod_info_t info;
	info.name    = "Example Mod";
	info.author  = "hero";
	info.version = "0.0.1";
	return info;
}

void c_photon_mod::paint_menu( ) {
	// add controls to the menu here using functions from photon->menu.
}
```
