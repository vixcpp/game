# vix/game

Game application foundation for Vix.cpp.

`vix/game` provides the core runtime layer needed to build games, simulations, interactive tools, prototypes, and custom engines on top of Vix.cpp.

It is not designed as a full game engine in its first versions.
It is designed as a clean, modular, backend-independent foundation that gives developers the building blocks required to create their own game architecture.

## Motivation

Most C++ game projects start with the same problems:

- setting up an application loop
- managing scenes
- tracking frame time
- dispatching events
- loading assets
- running background jobs
- handling input
- abstracting windows and renderers
- organizing runtime systems
- preparing editor, scripting, audio, and physics layers

These pieces are often rebuilt from scratch, tied to a specific engine, or mixed directly into application code.

`vix/game` exists to solve that problem.

The goal is to provide a small but serious foundation for real-time applications without forcing developers into a locked engine model.

Vix.cpp already focuses on developer experience, runtime workflows, fast builds, explicit APIs, and modular C++ development. The game module extends that philosophy to interactive software.

## Philosophy

`vix/game` follows a simple principle:

> Give developers the foundation to build engines, games, simulations, and tools without locking them into a complete engine.

The module is built around these ideas:

- **Backend-independent**: no forced dependency on SDL, GLFW, OpenGL, Vulkan, Box2D, Lua, or any specific backend.
- **Modular by design**: every subsystem can evolve independently.
- **Explicit APIs**: lifecycle, runtime, scene, input, rendering, audio, physics, and scripting concepts are visible and understandable.
- **Low-level friendly**: the module gives structure without hiding too much from the developer.
- **Runtime-first**: designed to work with the broader Vix.cpp runtime and build workflow.
- **Progressive adoption**: users can start with a simple loop and later add scenes, ECS, rendering, editor tools, scripting, and packaging.

## What vix/game provides

Current foundation includes:

- application lifecycle
- game loop
- frame timing
- fixed update support
- event system
- scene management
- simple ECS-style registry
- entities, components, systems
- asset management
- asset cache
- async asset loading
- background job system
- input system
- window abstraction
- renderer abstraction
- 2D rendering foundation
- scene serialization
- runtime context
- editor runtime foundation
- scripting runtime foundation
- audio runtime foundation
- physics runtime foundation
- game package metadata
- `vix new --game` project template

## Who is this for?

`vix/game` is useful if you want to build:

- a small C++ game
- a game prototype
- a simulation
- an editor tool
- a custom engine
- a 2D rendering experiment
- a real-time visualization tool
- an educational engine architecture
- a game framework on top of Vix.cpp

It is especially useful for developers who want the structure of an engine without giving up control over their architecture.

## Core architecture

The module is organized around a small set of runtime layers.

```text
App
  ├── GameLoop
  ├── EventBus
  ├── SceneManager
  ├── AssetManager
  ├── AsyncAssetLoader
  └── JobSystem

GameContext
  ├── App access
  ├── InputSystem
  ├── Window
  ├── Renderer
  └── Renderer2D

GameRuntime
  └── Coordinates runtime frame phases

EditorRuntime
  └── Editor mode, play mode, editor context

SceneRuntime
  └── High-level scene operations

ScriptRuntime
  └── Dependency-free scripting hook foundation

AudioRuntime
  └── Dependency-free audio foundation

PhysicsRuntime
  └── Dependency-free 2D physics foundation

GamePackage
  └── Project metadata for future packaging workflows
```

## Basic example

```cpp
#include <vix/game/all.hpp>
#include <vix/print.hpp>

class MainScene final : public vix::game::Scene
{
public:
  vix::game::GameBoolResult on_load() override
  {
    vix::print("Main scene loaded");
    return vix::game::Scene::on_load();
  }

  void on_update(const vix::game::Frame& frame) override
  {
    vix::print("frame:", frame.index);

    if (frame.index >= 5)
    {
      app().stop();
    }
  }
};

int main()
{
  vix::game::App app;
  app.set_title("My Game");

  vix::game::GameRuntime runtime(app);

  auto runtime_init = runtime.init();
  if (!runtime_init)
  {
    vix::print("runtime init failed:", runtime_init.error().message());
    return 1;
  }

  auto scene = app.scenes().create<MainScene>("main");
  if (!scene)
  {
    vix::print("scene creation failed:", scene.error().message());
    return 1;
  }

  auto active = app.scenes().set_active("main");
  if (!active)
  {
    vix::print("scene activation failed:", active.error().message());
    return 1;
  }

  auto result = app.run();
  if (!result)
  {
    vix::print("game failed:", result.error().message());
    return 1;
  }

  return 0;
}
```

## Create a game project

`vix/game` is integrated with the Vix CLI.

```bash
vix new mario --game
cd mario
vix build
vix run
```

Generated layout:

```text
mario/
  assets/
  game.package.json
  README.md
  src/
    main.cpp
  vix.app
  vix.json
```

The generated project uses:

- `vix/game`
- `GameRuntime`
- `Scene`
- `SceneManager`
- `GamePackage`
- `vix.app`
- `vix.json`

## App lifecycle

`App` is the main facade of the game module.

It owns and coordinates:

- game loop
- scene manager
- event bus
- asset manager
- async asset loader
- job system

Typical flow:

- create app
- configure app
- create scenes
- set active scene
- run app
- shutdown app

`App` is intentionally small and focused. It does not directly become a full engine. Instead, it gives the rest of the module a stable runtime root.

## Game loop and frame model

The game loop provides:

- frame index
- delta time
- elapsed time
- fixed update steps
- update callback
- fixed update callback

`Frame` is passed to scenes, systems, runtimes, and callbacks.

```cpp
void on_update(const vix::game::Frame& frame)
{
  auto ms = frame.delta_ms();
}
```

This gives game code access to timing without forcing a specific simulation model.

## Scenes

Scenes represent logical application states.

Examples:

- main menu
- gameplay
- loading screen
- pause menu
- editor scene
- simulation scene

A scene has lifecycle hooks:

- `on_attach`
- `on_load`
- `on_enter`
- `on_update`
- `on_fixed_update`
- `on_exit`
- `on_unload`
- `on_detach`

`SceneManager` owns scenes, loads them, activates them, updates them, and serializes the scene list.

## ECS foundation

`Registry` provides a simple ECS-style foundation.

It supports:

- entities
- components
- systems
- typed component storage
- typed views
- update and fixed update forwarding

This is not yet an archetype ECS.

It is intentionally simple, readable, and suitable as a stable foundation.

Future versions can improve performance without changing the high-level mental model.

## Assets

`AssetManager` handles asset loading and asset storage.

It supports:

- relative asset paths
- asset type inference
- text assets
- binary assets
- asset cache
- asset lifecycle events

`AsyncAssetLoader` adds background loading through `JobSystem`.

This keeps the public API simple while preparing the module for real-world asset workflows.

## Jobs

`JobSystem` wraps Vix threadpool functionality for game workloads.

It can be used for:

- async asset loading
- scene preparation
- CPU-heavy tasks
- future parallel systems
- editor background tasks

The API is intentionally game-facing, while the implementation stays connected to the broader Vix runtime.

## Input

`InputSystem` provides a backend-independent input layer.

It supports:

- key state
- pointer button state
- pointer position
- pointer delta
- wheel delta
- action bindings
- pressed, down, and released queries

Backends can feed raw input into `InputSystem`, while game code can query higher-level actions.

Example:

```cpp
input.bind_key("jump", vix::game::InputKey::Space);

if (input.action_pressed("jump"))
{
  // jump
}
```

## Window and renderer abstraction

`Window` and `Renderer` are backend-independent facades.

They do not force a specific platform or graphics API.

This allows future support for:

- null backend
- SDL backend
- GLFW backend
- OpenGL backend
- Vulkan backend
- software rendering
- custom engine backends

V4 adds the first real backend path:

- `SDLWindow`
- `SDLRenderer`
- SDL input event mapping
- SDL texture upload
- SDL sprite rendering

The null backends remain available for tests, CI, headless servers, and simulations.

## 2D rendering foundation

`Renderer2D` provides a stable 2D rendering API.

It supports:

- frame lifecycle forwarding
- clear color
- active camera
- sprite draw command collection

V4 connects `Renderer2D` to real backend rendering through `DrawSpriteCommand`.

The SDL backend can now upload image assets as textures and draw sprites through `SDLRenderer`.

The flow is:

```text
AssetManager
  -> Asset bytes
  -> Renderer::upload_texture()
  -> SDLRenderer texture store
  -> Sprite
  -> Renderer2D
  -> DrawSpriteCommand
  -> SDLRenderer::draw_sprite()
```

## Runtime foundation

V3 introduced runtime layers:

- `GameContext`
- `GameRuntime`
- `EditorContext`
- `EditorRuntime`
- `SceneRuntime`
- `ScriptRuntime`
- `AudioRuntime`
- `PhysicsRuntime`

This separates raw systems from orchestration.
Instead of placing every feature inside `App`, runtime-specific behavior now lives in dedicated runtime objects.
This keeps the architecture clean as the module grows.

## Editor foundation

The editor foundation is intentionally minimal.

It provides:

- edit mode
- play mode
- selected scene
- active tool
- dirty state
- editor runtime lifecycle

This prepares the module for future editor tools without coupling the core runtime to a UI framework.

## Scripting foundation

`ScriptRuntime` does not embed a scripting language yet.

Instead, it provides a dependency-free callback layer that can later support:

- Lua
- JavaScript
- Python
- custom scripting
- hot reload
- editor scripting

This avoids adding a heavy dependency too early while keeping the API direction clear.

## Audio foundation

`AudioRuntime` currently stores audio source metadata and playback state.
It prepares the API for future audio backends without requiring OpenAL, SDL audio, FMOD, or another dependency.

It supports:

- source loading
- play state
- stop state
- volume
- loop flag
- source lookup

## Physics foundation

`PhysicsRuntime` provides a small dependency-free 2D physics foundation.

It supports:

- bodies
- position
- velocity
- acceleration
- mass
- gravity
- fixed update integration

It is not meant to replace Box2D or Bullet.
It exists to provide a stable physics runtime surface where future backends can be integrated.

## Game package metadata

`GamePackage` describes a game project.

It includes:

- name
- version
- author
- entry scene
- asset root
- output directory
- scene list
- asset list

This is the base for future workflows:

- `vix new game`
- `vix game package`
- `vix game export`
- editor project loading
- asset packaging
- distribution metadata

## Project manifest

A generated game project uses `vix.app`.

Example:

```text
name = "mario"
type = "executable"
standard = "c++20"

sources = [
  "src/main.cpp",
]

include_dirs = [
  "src",
]

compile_features = [
  "cxx_std_20",
]

packages = [
  "vix",
]

links = [
  "vix::game",
  "vix::io",
]

resources = [
  "assets=assets",
  "game.package.json=game.package.json",
]

output_dir = "bin"
```

This keeps game projects simple while still using the Vix build system internally.

## Module status

### V1 completed

- App lifecycle
- Game loop
- `TimeStep`
- `Frame`
- `EventBus`
- `SceneManager`
- simple `Registry`
- `AssetManager`
- `JobSystem`
- examples
- tests

### V2 completed

- input module
- window module
- renderer abstraction
- 2D rendering foundation
- asset cache
- async asset loading
- scene serialization
- stronger ECS iteration APIs

### V3 completed

- `GameContext`
- `GameRuntime`
- `EditorContext`
- `EditorRuntime`
- `SceneRuntime`
- `ScriptRuntime`
- `AudioRuntime`
- `PhysicsRuntime`
- `GamePackage`
- `vix new --game` template
- V3 runtime smoke example

## Roadmap

### V4 completed

- `GameRuntime` integrated as the main runtime coordinator
- `GameContext` backend installation helpers
- headless runtime flow with `NullWindow` and `NullRenderer`
- SDL window backend
- SDL input mapping
- SDL window/input example
- SDL renderer backend
- SDL texture upload
- SDL sprite rendering
- sprite demo example
- `RuntimeDiagnostics`
- scene inspection APIs
- editor runtime inspection APIs
- V4 architecture tests

### V5 planned

- scripting backend integration
- audio backend integration
- physics backend integration
- game packaging command
- game export workflow
- project templates through `vix new game`
- editor tool APIs
- richer asset pipeline

## Design boundaries

`vix/game` does not try to do everything at once.

It avoids:

- forcing a rendering backend
- forcing a windowing library
- forcing a scripting language
- forcing a physics engine
- becoming a closed engine
- hiding the runtime model

This is intentional.

The module should stay useful for developers building their own systems on top of Vix.cpp.

## Build

From the module directory:

```bash
vix build --build-target all -v -- -DVIX_GAME_BUILD_EXAMPLES=ON
```

Build with SDL backend:
Deps:
```bash
sudo apt update
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
```

```bash
vix build --build-target all -v -- -DVIX_GAME_BUILD_TESTS=ON -DVIX_GAME_BUILD_EXAMPLES=ON -DVIX_GAME_ENABLE_SDL=ON
```

Run examples:

```bash
cd build-ninja/examples
./vix_game_hello_game
./vix_game_scene_demo
./vix_game_runtime_v3_demo
./vix_game_sdl_window_null_renderer_demo
./vix_game_window_input_demo
./vix_game_sdl_renderer_demo
./vix_game_sprite_demo
```

## Testing the generated game template

```bash
vix new mario --game
cd mario
vix build
vix run
```

Expected output:

```text
Main scene loaded
game app initialized title=mario
frame: 0
frame: 1
frame: 2
frame: 3
frame: 4
frame: 5
game app shutdown title=mario
```

## Why this matters

C++ is powerful for games and real-time software, but project structure can quickly become complex.

`vix/game` gives Vix.cpp a serious game foundation while keeping the developer in control.

It is not trying to compete with Unity, Unreal, Godot, or established engines.

It targets a different need:

> A clean C++ runtime foundation for developers who want to build games, simulations, tools, and custom engines with Vix.cpp.

## License

MIT
