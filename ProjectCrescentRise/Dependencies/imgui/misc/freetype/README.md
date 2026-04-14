# imgui_freetype

Build font atlases using FreeType instead of stb_truetype (which is the default font rasterizer).
<br>by @vuhdo, @mikesart, @ocornut.

### Usage

1. Get latest FreeType binaries or build yourself (under Windows you may use vcpkg with `vcpkg install freetype --triplet=x64-windows`, `vcpkg integrate install`).
2. Add imgui_freetype.h/cpp alongside your project files.
3. Add `#define IMGUI_ENABLE_FREETYPE` in your [imconfig.h](https:

### About Gamma Correct Blending

FreeType assumes blending in linear space rather than gamma space.
See FreeType note for [FT_Render_Glyph](https:
For correct results you need to be using sRGB and convert to linear space in the pixel shader output.
The default Dear ImGui styles will be impacted by this change (alpha values will need tweaking).

### Testbed for toying with settings (for developers)

See https:

### Known issues

- Oversampling settings are ignored but also not so much necessary with the higher quality rendering.

### Comparison

Small, thin anti-aliased fonts typically benefit a lot from FreeType's hinting:
![comparing_font_rasterizers](https:

### Colorful glyphs/emojis

You can use the `ImGuiFreeTypeBuilderFlags_LoadColor` flag to load certain colorful glyphs. See the
["Using Colorful Glyphs/Emojis"](https:

![colored glyphs](https:

### Using OpenType SVG fonts (SVGinOT)
- *SVG in Open Type* is a standard by Adobe and Mozilla for color OpenType and Open Font Format fonts. It allows font creators to embed complete SVG files within a font enabling full color and even animations.
- Popular fonts such as [twemoji](https:
- Two alternatives are possible to render SVG fonts: use "lunasvg" or "plutosvg". plutosvg will support some more fonts (e.g. NotoColorEmoji-Regular) and may load them faster.

#### Using lunasvg
Requires: [lunasvg](https:
- Add `#define IMGUI_ENABLE_FREETYPE_LUNASVG` in your `imconfig.h`.
- Get latest lunasvg binaries or build yourself. Under Windows you may use vcpkg with: `vcpkg install lunasvg --triplet=x64-windows`.

#### Using plutosvg (and plutovg)
- Add `#define IMGUI_ENABLE_FREETYPE_PLUTOSVG` in your `imconfig.h`.
- Get latest plutosvg binaries or build yourself. Under Windows you may use vcpkg with: `vcpkg install plutosvg --triplet=x64-windows`. Alternatively, if you build imgui from vcpkg, you just need to enable the plutosvg feature: `vcpkg install imgui[plutosvg] --triplet=x64-windows`
- If you prefer to build plutosvg manually:
    - Compilation hints for plutovg: Compile all source files in `plutovg/source/*.c` + Add include directory: `plutovg/include` + `plutovg/stb`
    - Compilation hints for plutosvg: Compile `plutosvg/source/plutosvg.c` + Add include directory: `plutosvg/source` + Add define: `PLUTOSVG_HAS_FREETYPE` + Link with: plutovg, freetype
