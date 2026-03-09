# DumpsterFire Engine

DumpsterFire Engine is a 3D engine using OpenGL as its graphical backend. 
At its current stage I can generate 3D and 2D objects, load meshes via a .obj loader, set textures and various lighting settings. The in-engine UI allows us to spawn entities and play around with them.

At its current stage, the engine can:
- Render basic 3D and 2D objects.
- Load meshes via a .obj loader.
- Apply diffuse + specular textures.
- Configure lighting in real time (Directional / Point / Spot) with support for up to 16 lights.

- Use ImGui panels to spawn/edit entities, adjust render settings, and debug systems while the engine is running.
- Save/load entity data via JSON serialization.

Project state / warning: many systems are still half-baked. For example, some asset loading currently requires typing full file paths, and “.obj” meshes are limited without triangularization.


## Controls

You can control the camera either via its ImGui panel or using the keyboard +mouse:

- WASD: Move
- Space: Raise camera
- Left Ctrl: Lower camera
- Left Shift: Increase camera move speed
- 1: toggle wireframe (leftover from the initial code that was used for the shader course.)


## ImGui Panels (Editor / Debug UI)

## Cube Controls (Entity Spawner + Inspector)
The main window responsible for spawning entities (initially cubes) and editing them live. Spawned entities are stored in an array and can be modified at runtime.

From this panel you can:
- Rename entities
- Edit transforms (position / rotation / scale)
- Configure physics settings (gravity, kinematic state, etc.)
- Enable collision and set collision behavior
- Assign texture + specular texture maps
- Load alternate models via `.obj` (instead of rendering as a cube)

## Behavior / Collision enums
Entities can be assigned a Behavior via enums to drive simple gameplay logic:
- Player: controllable via WASD + jump with Space; collides with entities marked Obstacle   
  and Platform. Otherwise it clips through the entity.
- Coin: acts as a simple trigger that warps the player to the next level.

## MipMap
We have a smaller window which sets mipmapping on or off. This can be seen when an entity is rendered and parts of it appear blurrier the further away we are with the camera, with it off it looks more jagged. Mipmapping basically creates many copies of the textures in smaller resolutions, 64x64, 32x32, 16x16 etc... Basically the further away we are from the texture we don't need to render higher resolutions of that texture. 


## Camera
Camera panel for adjusting:
- Position / rotation
- Orientation and up-vector (tilt/roll-style control)

Current limitation: No multi-camera system.


## Light Manager (Lighting Controls)
Lighting is controlled live through an ImGui Light Manager panel. You can add and remove lights and tweak parameters for each light, up to 16 active lights.

light types:
- Directional Light
  - Enabled by default as a neutral “scene light”
  - Direction changes are immediately visible in-scene
  - Used as the primary light for **directional shadow mapping** (when enabled)
- Spotlight
  - A focused cone of light that illuminates whatever it points toward
  - Includes cutoff/exponent-style controls for shaping the beam
- Point LighT
  - An omnidirectional light source emitted from a position
  - Typically controlled via attenuation values (constant/linear/quadratic-like behavior)





## JSON Serialization
Entities can be saved and loaded using JSON, including:
- Names
- Transforms / positions
- Physics flags (gravity, kinematic state, etc.)
- Other per-entity properties as implemented

## Graphics
Rendering-related systems, including:
- Primitive/cube rendering
- Textures (diffuse + specular maps)
- Framebuffer backends 
- Shader pipeline integration

## Debugging 
-Debug utilities are included such as a message system which can be used for pushing and popping any debug warning needed, this keeps it centralized and avoiding to write couts everywhere. 
-Memory Checker tracks memory / leaks.

## Shaders
Shader programs and shader-related functionality, including:
- Lighting shaders.
- Shadow mapping with directional light active.
- Vertex / fragment shader implementations.

## UI Manager & ImGui Integration
Owns the engine’s debug/editor UI:
- Panel creation and layout
- Runtime tweaking of entities, camera, lighting, and render states
-Toggle Play/Edit states, during Play, gamespeed and gravity can be tweaked.

## Physics 
Basic physics and collision:
- AABB (Axis-Aligned Bounding Box) collision detection
- Intended for simple collisions between entities like Player / Obstacle / Platform

## Game Behaviors
Higher-level gameplay logic and staging:
- Controllers
- Stages / levels (currently hardcoded teleport thresholds, level 2 = X 50 relative to World, level 3 = 100)

## Input Manager & Camera Controller
Handles inputs and camera movement controls, supporting:
- Direct camera navigation (WASD + vertical movement)
- Player behavior when entities are flagged as controllable, ergo Physics Settings set to Player.

## Limitations

- Asset loading requires full file paths, there is no robust asset pipeline implemented.
- “.obj” rendering is limited without triangularization, more advanced shapes will appear as malformed amalgamations of triangles, struggling to take the shape they were meant for.
- Single-camera support
- Various systems are half-baked and messy code-wise and functionality-wise. They will serve as a point of reference to create more robust and polished features.

## Licenses / Third-Party Libraries

This project uses the following third-party libraries:
- nlohmann/json
- EnTT
- glad
- *mGui
- stb_image
- OpenGL / GLSL