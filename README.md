<div align="center">
    <img src=".github/media/header.png" style="width: 100%; height: auto;"/>
</div>

[![wakatime](https://wakatime.com/badge/user/3e73d21c-9ccb-4e77-ab4d-6f58f0296cfa/project/d703c117-3f87-4f6f-96f5-e0c40088a6a0.svg)](https://wakatime.com/badge/user/3e73d21c-9ccb-4e77-ab4d-6f58f0296cfa/project/d703c117-3f87-4f6f-96f5-e0c40088a6a0?style=for-the-badge)
![GitHub License](https://img.shields.io/github/license/zoogies/yoyoengine)
![GitHub repo size](https://img.shields.io/github/repo-size/zoogies/yoyoengine)
![GitHub Tag](https://img.shields.io/github/v/tag/zoogies/yoyoengine)

---

<br/>

> [!WARNING]  
> This project is under ***ACTIVE DEVELOPMENT***, so take anything you read in this README with a grain of salt, as its usually one of the last things I update.

# Yoyo Engine

My 2D Game Engine written in C, based off of SDL.

A solid (if I do say so myself) 2D game engine that boasts

- A Project Hub
- Visual Editor
- Multi-platform support (Windows, Linux, Emscripten)
- Native C and lua scripting
- Plugin system

The editor projects are setup in such a way that all of your game logic can be implemented in C, or you can forgo C entirely and use lua scripting for your game logic. There is nothing stopping you from using a combination of both.

The way I have structured this project, all dependancies for the core, editor, and built distributable projects will be downloaded with FetchContent and built automatically by cmake.
**However**, you will need some external packages on your system to use some of the features and programs included within this repo, which are listed [here](#dependencies).

## Documentation

Currently, this readme is serving as the central documentation for the project.

I also provide autogenerated documentation using doxygen [here](https://zoogies.github.io/yoyoengine).

The examples directory in this repository contains minimal examples of usage, which can be opened through adding them as projects in the launcher gui, then opening them. (As of writing this feature is not complete).

## Notices

This section is temporary and will be used to denote any important facts I need to remember about the project.

- The coordinate system is non standard, north is -y, south is +y, east is +x, west is -x
- only text or png images can have their alpha modulated
- The engine is single threaded (except for audio)
- Logging is unavailable until the engine is initialized (SDL needs loaded before we can use it)
- THE LAUNCHER AND EDITOR TOOLCHAIN HAS ONLY BEEN TESTED ON LINUX. OPEN AN ISSUE IF YOU NEED WINDOWS SUPPORT.
- NOTE: make this section limitations and have a seperate notices
- DO NOT USE SPACES IN FILE NAMES, this will break a lot of the file operations especially in the editor, core might be fine though.
- Because of the nature of the editor, it only runs on linux and also accesses data through the loose file directories rather than the pack files which are accessed at runtime
- for buttons, their state is updated on input events, meaning that a "clicked" button would only stop being clicked on the next event NOT the next frame. We could fix this with a iteration and reset at the beginning of each frame, but for now its reset when polled. So a button will only be "clicked" for the first access of the state after the click event.
- There is a limitation in detection of the screen size. Moving games to be fullscreen on your non primary monitor will mess with the mouse position detection, as the size of your primary monitor is used to calculate fullscreen letterboxing
- We can only guarantee one timer executing its callback per frame, which is usually more than good enough unless at extremely low framerates and for time sensitive tasks. There is already a bit of loss due to CPU scheduling, so the timer system wouldnt be ideal for hyper accurate timing anyways.

### editor

- zooming too far out really messes with the SDL line renderer, and you will see weird visual artifacts.
- check out <https://emscripten.org/docs/getting_started/downloads.html> for how to install emscripten, after that assuming you have everything else installed you can build to emscripten outputs

## Dependencies

> [!WARNING]  
> You will need all of the below dependencies installed, besides the **Core & Editor** dependencies, as they come preinstalled.

### Tooling

- A C compiler (gcc, clang, etc) (Mingw for cross compilation)
- [git](https://git-scm.com/)
- [cmake](https://cmake.org/)
- [make](https://www.gnu.org/software/make/)
- [python3](https://www.python.org/)
- [pip](https://pypi.org/project/pip/)
- [emscripten](https://emscripten.org/) (for emscripten builds)

### Core & Editor

> [!NOTE]  
> All of these libraries are included in this repository, and do not need to be installed.

- [SDL2](https://www.libsdl.org/)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- [uthash](https://github.com/troydhanson/uthash)
- [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear)
- [jansson](https://github.com/akheron/jansson)

### Launcher / Project Manager

- [tkinter](https://docs.python.org/3/library/tkinter.html)
- [CustomTkinter](https://customtkinter.tomschimansky.com/)
- [CTkMessagebox](https://github.com/Akascape/CTkMessagebox)

## Getting Started

The reccomended way to use yoyoengine is via a submodule inside your main git project, where you build and run the tooling yourself.
In the future I might release standalone versions of the editor and launcher, but for now that hasnt happened.

1. Create a new git repo
2. Run the command `git submodule add https://github.com/zoogies/yoyoengine.git` in the root of the project
3. Navigate into `/yoyoengine/editor` and run `build_linux.sh` to create the editor binary
4. Navigate into `/yoyoengine/launcher` and run the command `python3 launcher.py` to open the project manager
5. Using the project manager, select the location of the editor binary, and then create a new project inside your repo
6. The editor should automatically launch into a default scene. Done!

## Launcher

There is a project manager and editor launcher in the "editor" directory. This is currently a work in progress and requires some python tkinter deps to be installed, but if you run it, it should function. It is currently only tested on linux.

You can create, delete, and launch projects through this easily.

## Editor

> [!WARNING]  
> The editor has only been tested on linux, and is incomplete. IT WILL NOT RUN ON WINDOWS!!!

The editor is currently being worked on as a visual scene editor and build manager.

## Core

The core engine is implemented in C, Extensive docs to the API will be linked here when complete.

## Plans

These plans are currently sorted in order of my anticipated completion.

- [X] Basic engine functionality (Audio, Rendering, Text, etc)
- [X] Nuklear UI integration
- [x] Entity Component System
- [X] Serialization
- [X] Visual Editor
- [X] Implement native C scripting
- [X] Lua Scripting API
- [X] Collision detection
- [X] emscripten build target
- [X] Stabilize the core framework API
- [ ] Overhaul the Project Manager
- [ ] Example projects and tutorials
- [ ] Documentation website

Some features that aren't planned but I would like to implement eventually:

- [ ] Networking (Stretch for after 1.0)
- [ ] Implement a Cpython wrapper
- [X] Bundling assets into production data files
- [ ] 3D rendering
- [ ] Mac support

## Path to 1.0

Seeing as I'm reaching 12 months of development, and more or less wrapping up the most important stuff: im going to write a little bit about whats left before a prerelease or 1.0.

### Major Milestones

- [ ] Finish a stable lua API (in progress)
- [ ] Rework and fix audio (audiosource) subsystem
- [ ] Try to take a pass at UX and cleanup loose ends

### Minor things (could be left out of a 1.0)

- [ ] potential animation system rework (behavior trees / state machines)
- [ ] Turn tilemap system into something more fleshed-out

## C scripting

Since I have transitioned this project to be more of an engine and less of a framework, by using the launcher to create a new project you will recieve a structure that is preconfigured to build and run a game by default.
In order to define custom behavior (ie: actually script your game) you should make note of the `custom/` directory created in your project folder.

```txt
custom
├── include
│   └── yoyo_c_api.h
├── lib
└── src
```

This directory is where all of your code for the game should go. You will notice that its only populated with one file by default, which we will touch on in a minute.

### To link libraries

As of writing this, I am working on rewriting build scripts. This is subject to change and will be updated soon.

### Headers

`include` is provided for your convenience, and is added to the include path automatically. You can place any headers you want to use in here. The compiler knows to look in this directory, so you can define any headers here. I dont think you can nest in subdirectories though but if you want to it should be trivial to modify the build script or you can always open an issue and id be happy to add that.

`yoyo_c_api.h` is an interesting workaround to a cmake issue. We cant define macros inside of source files, so you will need to open this header and uncomment any of the macros for implementing api functions. I will provide an example of this below if that didnt make sense.

### Scripting

Any Scripting you do in C will be done through interfacing api functions extended by the engine and game template. If you really want to, you could modify `entry.c` to customize some behavior, but practically speaking anything you could change in there can also be done through the C api.

To get started, lets assume we made a file called `custom/src/game.c` and we want to print out "Hello Yoyo Engine!" when the engine starts.
Taking a look at the contents of `custom/include/yoyo_c_api.h` we can see that there is a macro called `YOYO_POST_INIT` that lets the engine know that we have implemented the signature `yoyo_post_init()`.

Knowing this, all we need to do is uncomment the macro defining that function, and then actually implement it somewhere in our source. Let's go into `game.c` and do so now.

```c
#include "yoyo_c_api.h"

void yoyo_post_init(){
    printf("Hello Yoyo Engine!\n");
}
```

And its really as simple as that. Taking a peek at `yoyo_c_api.h` you can see more information on what callbacks are available to you and when they are run, which can be combined in such a manner that you can achieve any behavior you want.

### Parting Notes

Is this not enough to achieve what you want?
Keep in mind that you can also use lua scripting to more directly and easily interface with the engine, but you might not have the same level of control as C. Does that also not solve your problem? The final option open to you is writing a custom Trick (The engines name for a plugin). More information on that follows in the next section.

## Tricks (Plugin/Module System)

> [!WARNING]  
> The structure and metadata requirements for tricks are still very subject to change.

Plugins are called "tricks". They live in subdirectories of the project folder `/tricks` and have a directory structure like this:

```txt
example/
├── include
├── lib
├── src
│   └── main.c
└── trick.yoyo
```

`trick.yoyo` contains the metadata for the trick, which is required to load it. It is a json file with the following structure:

```json
{
    "name":"example_trick",
    "description":"example trick",
    "author":"Ryan Zmuda",
    "version":"1.0"
}
```

The developer of each trick is responsible for implementing a `CMakeLists.txt` build file, which incorporates the output target directories discussed in a few paragraphs.

In order to setup a trick to be loaded, you should `#include <yoyoengine/yoyo_trick_boilerplate.h>` in your source, and implement the `yoyo_trick_init` function.

It is reccomended (required if you wish for the engine to automatically callback into your trick) to call `ye_register_trick` with a `struct ye_trick_node` that contains any metadata and callbacks you wish for the engine to be aware of.

You can also leverage the power of `ye_register_trick_lua_bindings(lua_State *state)` to push to EVERY lua state that is created by the engine any functions you wish. This is useful for exposing your own lua api to user scripts.

A minimal example of a complete trick can be seen below:

```c
#include <yoyoengine/yoyoengine.h>
#include <yoyoengine/yoyo_trick_boilerplate.h>

void yoyo_trick_on_load(){
    /*
        runs when the trick is first mounted, which happens after all other init
    */
}

void yoyo_trick_on_unload(){
    /*
        runs when the trick is about to be unloaded, which is one of the
        first things to happen on engine shutdown
    */
}

void yoyo_trick_on_update(){
    /*
        runs once every ye_process_frame call, before the lua script
        callbacks and before rendering
    */
}

/* Example function that we will expose to lua */
void test_lua_bind(lua_State *L){
    ye_logf(warning, "TEST LUA BIND\n");
}

/* Callback to register our test function */
void yoyo_trick_lua_bind(lua_State *L){
    lua_register(L, "example_test", test_lua_bind);
}

/* The only required signature in this file, entry point to our trick */
void yoyo_trick_init(){
    ye_register_trick((struct ye_trick_node){
        .name = "example",
        .author = "Ryan Zmuda",
        .description = "example trick",
        .version = "1.0.0",
        .on_load = yoyo_trick_on_load,
        .on_unload = yoyo_trick_on_unload,
        .on_update = yoyo_trick_on_update,
        .lua_bind = yoyo_trick_lua_bind
    });
}
```

The engine at build time will make a few assumptions about your trick:

- It has a valid `trick.yoyo` in its root
- It has a valid `CMakeLists.txt` in its root
- It's `CMakeLists.txt` is setup in a way to build a shared library into `YOYO_TRICK_BUILD_DIR`
- Any headers that you want to be accessable to the runtime are in an `include/` directory in the root of the trick

I've deliberately left it up to the developer to create their own build script, as it gives much more control over dependancy management and build settings in general.

Because of this, it is also up to the developer to make sure they are outputting the trick in the directory that the engine will be looking for during project build time, which is `YOYO_TRICK_BUILD_DIR`.

You can ensure you are building into the correct directory by adding the following to your `CMakeLists.txt`:

```cmake
set_target_properties(example PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${YOYO_TRICK_BUILD_DIR}) # linux
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${YOYO_TRICK_BUILD_DIR}) # windows
```

You need ***BOTH*** of these properties to account for windows AND linux builds.

> [!WARNING]  
> Because tricks load after initialization and are unloaded shortly before shutdown, **it is imperative that their exposed functionality is not attempted to be invoked outside the duration of its lifecycle**. This is not enforced by the engine, and will result in undefined behavior if violated.

## Credit

- The listed dependencies and Jojo's Bizarre Adventure for the bootup screen sound effect.
