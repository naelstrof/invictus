Invictus
========
A highly effecient, multi-platform, multi-lingual engine that's meant to follow linux application standards.

Features (So far!)
--------
* **Full unicode support**: From printing to terminal to language in-game. All languages can be supported.
* **Text rendering**: Extremely efficient text rendering using self-sizing texture atlases. The only thing sent through the pipeline is the glyphs themselves and then they're cached immediately after. Texture clearing and rendering is all done on the GPU.
* **Virtualized Filesystem**: Thanks to PHYSFS the filesystem is completely virtualized allowing for files to be loaded from a combination of archives and actual files. Prioritizes user files located in their home folder over actual game files. So not only can modding easily be done on a per-user basis without destroying original game content, but you can also have game clients download content packages from servers without any risk to the user!
* **Lua**: Not only does Lua power the game's configuration, but it also powers what resources are loaded and most game content! So now a mature content filter could easily be created without editing any C++ code. Instead of finding a font that encompasses all the languages you wish to support you could have Lua automatically detect and load the specific font that supports the language. Game content will even be capable of updating real-time while the game is running!

### Current State
This image will update with the current state of the engine.

![Invictus Image](http://farmpolice.com/content/current.png "Invictus running Astrostruct")

Libraries
---------
Invictus was supposed to use as few libraries as possible-- but there's no reason to reinvent the wheel. Invictus uses the following:

* [SFML 2.0](http://www.sfml-dev.org/) -- Will be used simply for the windowing it provides, but currently it's engrained in many ways with Invictus.
* [PHYSFS](http://icculus.org/physfs/) -- PHYSFS is a critical part of Invictus, used for creating sane filesystem access (and realtime archive access as a plus!). It's going to be abstracted though as the C api looks and feels dirty compared to the rest of the application.
* [Lua 5.2.2](http://www.lua.org/) -- Invictus will use Lua for loading resources and coding gameplay elements. Much like garry's mod.
* [GLM](http://glm.g-truc.net/) -- Despite SFML providing a few mathimatical resources it is inadequate for 3D. This is where glm comes in. It will not be abstracted in any way, and it will be engrained into the engine.
* [OpenGL](http://www.opengl.org/) -- Invictus will only use OpenGL for graphics. I have no reason to incorperate DirectX.
* [AssImp](http://assimp.sourceforge.net/) -- Invictus will load model assets using assimp. This will allow for a huge amount of flexibility for modders at the sacrifice of load speeds.
* [Freetype](http://freetype.org/) -- Invictus loads font face assets using Freetype. Invictus currently doesn't allow for itallic or bold fonts though.
* [GLEW](http://glew.sourceforge.net/) -- Since SFML provides no interfaces for checking available extensions glew takes the spotlight.

Compiling
---------
Invictus uses CMake as a compiling medium. Compiling will be different for each operating system, but will follow similar rules:

1. Install each of the listed libraries so that CMake can detect them.
2. Run CMake in the same directory as the CMakeLists.txt file to create a method of compilation on your system (makefile, Visual Studio files, etc).
3. Compile!

For distribution of binaries I plan to cross-compile Windows binaries from Linux. Not sure how I'm going to get binaries for Mac while Linux users can compile it themselves easily.
