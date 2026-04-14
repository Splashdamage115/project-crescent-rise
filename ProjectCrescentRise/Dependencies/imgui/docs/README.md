Dear ImGui
=====

<center><b><i>"Give someone state and they'll have a bug one day, but teach them how to represent state in two separate locations that have to be kept in sync and they'll have bugs for a lifetime."</i></b></center> <a href="https:

----

[![Build Status](https:

<sub>(This library is available under a free and permissive license, but needs financial support to sustain its continued improvements. In addition to maintenance and stability there are many desirable features yet to be added. If your company is using Dear ImGui, please consider reaching out.)</sub>

Businesses: support continued development and maintenance via invoiced sponsoring/support contracts:
<br>&nbsp;&nbsp;_E-mail: contact @ dearimgui dot com_
<br>Individuals: support continued development and maintenance [here](https:

| [The Pitch](#the-pitch) - [Usage](#usage) - [How it works](#how-it-works) - [Releases & Changelogs](#releases--changelogs) - [Demo](#demo) - [Getting Started & Integration](#getting-started--integration) |
:----------------------------------------------------------: |
| [Gallery](#gallery) - [Support, FAQ](#support-frequently-asked-questions-faq) -  [How to help](#how-to-help) - **[Funding & Sponsors](https:
| [Wiki](https:

### The Pitch

Dear ImGui is a **bloat-free graphical user interface library for C++**. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline-enabled application. It is fast, portable, renderer agnostic, and self-contained (no external dependencies).

Dear ImGui is designed to **enable fast iterations** and to **empower programmers** to create **content creation tools and visualization / debug tools** (as opposed to UI for the average end-user). It favors simplicity and productivity toward this goal and lacks certain features commonly found in more high-level libraries. Among other things, full internationalization (right-to-left text, bidirectional text, text shaping etc.) and accessibility features are not supported.

Dear ImGui is particularly suited to integration in game engines (for tooling), real-time 3D applications, fullscreen applications, embedded applications, or any applications on console platforms where operating system features are non-standard.

 - Minimize state synchronization.
 - Minimize UI-related state storage on user side.
 - Minimize setup and maintenance.
 - Easy to use to create dynamic UI which are the reflection of a dynamic data set.
 - Easy to use to create code-driven and data-driven tools.
 - Easy to use to create ad hoc short-lived tools and long-lived, more elaborate tools.
 - Easy to hack and improve.
 - Portable, minimize dependencies, run on target (consoles, phones, etc.).
 - Efficient runtime and memory consumption.
 - Battle-tested, used by [many major actors in the game industry](https:

### Usage

**The core of Dear ImGui is self-contained within a few platform-agnostic files** which you can easily compile in your application/engine. They are all the files in the root folder of the repository (imgui*.cpp, imgui*.h). **No specific build process is required**. You can add the .cpp files into your existing project.

**Backends for a variety of graphics API and rendering platforms** are provided in the [backends/](https:

See the [Getting Started & Integration](#getting-started--integration) section of this document for more details.

After Dear ImGui is set up in your application, you can use it from \_anywhere\_ in your program loop:
```cpp
ImGui::Text("Hello, world %d", 123);
if (ImGui::Button("Save"))
    MySaveFunction();
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```
![sample code output (dark, segoeui font, freetype)](https:
![sample code output (light, segoeui font, freetype)](https:

```cpp

ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
if (ImGui::BeginMenuBar())
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
        if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}


ImGui::ColorEdit4("Color", my_color);


float samples[100];
for (int n = 0; n < 100; n++)
    samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
ImGui::PlotLines("Samples", samples, 100);


ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
ImGui::BeginChild("Scrolling");
for (int n = 0; n < 50; n++)
    ImGui::Text("%04d: Some text", n);
ImGui::EndChild();
ImGui::End();
```
![my_first_tool_v188](https:

Dear ImGui allows you to **create elaborate tools** as well as very short-lived ones. On the extreme side of short-livedness: using the Edit&Continue (hot code reload) feature of modern compilers you can add a few widgets to tweak variables while your application is running, and remove the code a minute later! Dear ImGui is not just for tweaking values. You can use it to trace a running algorithm by just emitting text commands. You can use it along with your own reflection data to browse your dataset live. You can use it to expose the internals of a subsystem in your engine, to create a logger, an inspection tool, a profiler, a debugger, an entire game-making editor/framework, etc.

### How it works

The IMGUI paradigm through its API tries to minimize superfluous state duplication, state synchronization, and state retention from the user's point of view. It is less error-prone (less code and fewer bugs) than traditional retained-mode interfaces, and lends itself to creating dynamic user interfaces. Check out the Wiki's [About the IMGUI paradigm](https:

Dear ImGui outputs vertex buffers and command lists that you can easily render in your application. The number of draw calls and state changes required to render them is fairly small. Because Dear ImGui doesn't know or touch graphics state directly, you can call its functions  anywhere in your code (e.g. in the middle of a running algorithm, or in the middle of your own rendering process). Refer to the sample applications in the examples/ folder for instructions on how to integrate Dear ImGui with your existing codebase.

_A common misunderstanding is to mistake immediate mode GUI for immediate mode rendering, which usually implies hammering your driver/GPU with a bunch of inefficient draw calls and state changes as the GUI functions are called. This is NOT what Dear ImGui does. Dear ImGui outputs vertex buffers and a small list of draw calls batches. It never touches your GPU directly. The draw call batches are decently optimal and you can render them later, in your app or even remotely._

### Releases & Changelogs

See [Releases](https:
Reading the changelogs is a good way to keep up to date with the things Dear ImGui has to offer, and maybe will give you ideas of some features that you've been ignoring until now!

### Demo

Calling the `ImGui::ShowDemoWindow()` function will create a demo window showcasing a variety of features and examples. The code is always available for reference in `imgui_demo.cpp`. 
- [Web version of the demo](https:
- [Screenshot of the demo](https:

You should be able to build the examples from sources. If you don't, let us know! If you want to have a quick look at some Dear ImGui features, you can download Windows binaries of the demo app here:
- [imgui-demo-binaries-20250625.zip](https:

### Getting Started & Integration

See the [Getting Started](https:

On most platforms and when using C++, **you should be able to use a combination of the [imgui_impl_xxxx](https:

Integrating Dear ImGui within your custom engine is a matter of mainly 1) wiring mouse/keyboard/gamepad inputs 2) uploading a texture to your GPU/render engine 3) providing a render function that can create/update textures and render textured triangles. This is exactly what backends are doing.
- The [examples/](https:
- The [Getting Started](https:
- The [Backends](https:
- Generally, **make sure to spend time reading the [FAQ](https:

Officially maintained backends (in repository):
- Renderers: DirectX9, DirectX10, DirectX11, DirectX12, Metal, OpenGL/ES/ES2, SDL_GPU, SDL_Renderer2/3, Vulkan, WebGPU.
- Platforms: GLFW, SDL2/SDL3, Win32, Glut, OSX, Android.
- Frameworks: Allegro5, Emscripten.

[Third-party backends/bindings](https:
- Languages: C, C# and: Beef, ChaiScript, CovScript, Crystal, D, Go, Haskell, Haxe/hxcpp, Java, JavaScript, Julia, Kotlin, Lobster, Lua, Nim, Odin, Pascal, PureBasic, Python, ReaScript, Ruby, Rust, Swift, Zig...
- Frameworks: AGS/Adventure Game Studio, Amethyst, Blender, bsf, Cinder, Cocos2d-x, Defold, Diligent Engine, Ebiten, Flexium, GML/Game Maker Studio, GLEQ, Godot, GTK3, Irrlicht Engine, JUCE, LÖVE+LUA, Mach Engine, Magnum, Marmalade, Monogame, NanoRT, nCine, Nim Game Lib, Nintendo 3DS/Switch/WiiU (homebrew), Ogre, openFrameworks, OSG/OpenSceneGraph, Orx, Photoshop, px_render, Qt/QtDirect3D, raylib, SFML, Sokol, Unity, Unreal Engine 4/5, UWP, vtk, VulkanHpp, VulkanSceneGraph, Win32 GDI, WxWidgets.
- Many bindings are auto-generated (by good old [cimgui](https:

[Useful Extensions/Widgets](https:
- Automation/testing, Text editors, node editors, timeline editors, plotting, software renderers, remote network access, memory editors, gizmos, etc. Notable and well supported extensions include [ImPlot](https:

Also see [Wiki](https:

### Gallery

Examples projects using Dear ImGui: [Tracy](https:

For more user-submitted screenshots of projects using Dear ImGui, check out the [Gallery Threads](https:

For a list of third-party widgets and extensions, check out the [Useful Extensions/Widgets](https:

|  |  |
|--|--|
| Custom engine [erhe](https:
| Custom engine (untitled)<BR>[![editor white](https:

### Support, Frequently Asked Questions (FAQ)

See: [Frequently Asked Questions (FAQ)](https:

See: [Getting Started](https:

See: [Articles about the IMGUI paradigm](https:

See: [Upcoming Changes](https:

See: [Dear ImGui Test Engine + Test Suite](https:

For the purposes of getting search engines to crawl the wiki, here's a link to the [Crawlable Wiki](https:

Getting started? For first-time users having issues compiling/linking/running or issues loading fonts, please use [GitHub Discussions](https:

Private support is available for paying business customers (E-mail: _contact @ dearimgui dot com_).

**Which version should I get?**

We occasionally tag [Releases](https:

**Who uses Dear ImGui?**

See the [Quotes](https:

How to help
-----------

**How can I help?**

- See [GitHub Forum/Issues](https:
- You may help with development and submit pull requests! Please understand that by submitting a PR you are also submitting a request for the maintainer to review your code and then take over its maintenance forever. PR should be crafted both in the interest of the end-users and also to ease the maintainer into understanding and accepting it.
- See [Help wanted](https:
- Be a [Funding Supporter](https:

Sponsors
--------

Ongoing Dear ImGui development is and has been financially supported by users and private sponsors.
<BR>Please see the **[detailed list of current and past Dear ImGui funding supporters and sponsors](https:
<BR>From November 2014 to December 2019, ongoing development has also been financially supported by its users on Patreon and through individual donations.

**THANK YOU to all past and present supporters for helping to keep this project alive and thriving!**

Dear ImGui is using software and services provided free of charge for open source projects:
- [PVS-Studio](https:
- [GitHub actions](https:
- [OpenCppCoverage](https:

Credits
-------

Developed by [Omar Cornut](https:

Recurring contributors include Rokas Kupstys [@rokups](https:

Maintenance/support contracts, sponsoring invoices and other B2B transactions are hosted and handled by [Disco Hello](https:

Omar: "I first discovered the IMGUI paradigm at [Q-Games](https:

Embeds [ProggyClean.ttf](https:
<br>Embeds [stb_textedit.h, stb_truetype.h, stb_rect_pack.h](https:

Inspiration, feedback, and testing for early versions: Casey Muratori, Atman Binstock, Mikko Mononen, Emmanuel Briney, Stefan Kamoda, Anton Mikhailov, Matt Willis. Special thanks to Alex Evans, Patrick Doane, Marco Koegler for kindly helping. Also thank you to everyone posting feedback, questions and patches on GitHub.

License
-------

Dear ImGui is licensed under the MIT License, see [LICENSE.txt](https:
