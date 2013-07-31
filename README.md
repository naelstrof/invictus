Invictus
========
A highly effecient, multi-platform, multi-lingual engine that's meant to follow linux application standards.

Current State
-------------
This image will update with the current state of the engine.
![Invictus Image](farmpolice.com/content/current.png "Invictus running Astrostruct")

Libraries
---------
Invictus was supposed to use as few libraries as possible-- but there's no reason to reinvent the wheel. Invictus uses the following:

* [SFML 2.0](http://www.sfml-dev.org/) -- Will be used simply for the windowing it provides, but currently it's engrained in many ways with Invictus.
* [PHYSFS](http://icculus.org/physfs/) -- PHYSFS is a critical part of Invictus, used for creating sane filesystem access (and realtime archive access as a plus!). It's going to be abstracted though as the C api looks and feels dirty compared to the rest of the application.
* [Lua 5.2,2](http://www.lua.org/) -- Invictus will use Lua for loading resources and coding gameplay elements. Much like garry's mod.
* [GLM](http://glm.g-truc.net/) -- Despite SFML providing a few mathimatical resources it is inadequate for 3D. This is where glm comes in, it will not be abstracted in any way and will be engrained into the engine.
* [OpenGL](http://www.opengl.org/) -- Invictus will only use OpenGL for graphics. I have no reason to incorperate DirectX.
* [AssImp](http://assimp.sourceforge.net/) -- Invictus will load model assets using assimp. This will allow for a huge amount of flexibility for modders at the sacrifice of load speeds.
* [Freetype](http://freetype.org/) -- Invictus loads font face assets using Freetype. Invictus currently doesn't allow for itallic or bold fonts though.
* [GLEW](http://glew.sourceforge.net/) -- Since SFML provides no interfaces for checking available extensions glew takes the spotlight.

Compiling
---------
Invictus uses CMake as a compiling medium. Compiling will be different for each operating system, but will follow similar rules:
0. Install each of the listed libraries so that CMake can detect them.
0. Run CMake in the same directory as the CMakeLists.txt file to create a method of compilation on your system (makefile, Visual Studio files, etc).
0. Compile!

For distribution of binaries I plan to cross-compile Windows binaries from Linux. Not sure how I'm going to get binaries for Mac while Linux users can compile it themselves easily.
