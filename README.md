# vix/game

Game foundation module for Vix.cpp.

**Loop. Scenes. Entities. Assets. Jobs.**

## Overview

`vix/game` is the game foundation layer of Vix.cpp.

It does not try to be a full game engine in V1.

Instead, it provides the core infrastructure needed to build games, simulations, tools, prototypes, and custom engines on top of Vix.

The module focuses on:

- application lifecycle
- game loop
- frame timing
- scenes
- events
- entities and components
- asset loading
- background jobs

This makes `vix/game` a clean base for future rendering, audio, input, physics, scripting, and editor modules.

## Philosophy

`vix/game` is built around one idea:

> Give developers the foundation to build game engines, not a locked game engine.

The module stays small, explicit, and portable.

It avoids depending on graphics APIs, windowing systems, physics engines, or audio libraries in V1.

That keeps the first version stable and easy to extend.

## What vix/game provides

### App lifecycle

`vix::game::App` is the main facade.

It coordinates:

- configuration
- game loop
- scene manager
- event bus
- asset manager
- job system

```cpp
#include <vix/game/game.hpp>
#include <vix/print.hpp>

int main()
{
  vix::game::App app;

  app.set_title("Hello Vix Game");
  app.set_target_fps(0);

  app.on_update(
      [&](const vix::game::Frame& frame)
      {
        vix::print("frame:", frame.index);
        app.stop();
      });

  auto result = app.run();

  if (!result)
  {
    vix::print("error:", result.error().message());
    return 1;
  }

  return 0;
}
```

## Core features

### Game loop

The game loop provides:

- variable update
- optional fixed update
- frame delta
- elapsed time
- frame index
- FPS limiting
- clean stop control

Main types:

- `GameLoop`
- `GameClock`
- `TimeStep`
- `Frame`

### Scenes

Scenes represent high-level game states.

Examples:

- main menu
- gameplay
- pause screen
- loading screen

Main types:

- `Scene`
- `SceneId`
- `SceneManager`

```cpp
class MainScene final : public vix::game::Scene
{
public:
  void on_update(const vix::game::Frame& frame) override
  {
    (void)frame;
    app().stop();
  }
};
```

### Events

The event system provides synchronous dispatch.

It supports:

- built-in event types
- custom named events
- global listeners
- event metadata fields

Main types:

- `Event`
- `EventBus`
- `EventType`

Built-in events include:

- app started
- app stopping
- app stopped
- frame started
- frame ended
- scene loaded
- scene changed
- asset loaded
- job started

### Entities and components

`vix/game` includes a simple ECS-style registry.

It supports:

- entity creation
- component storage by C++ type
- systems
- update forwarding

Main types:

- `Entity`
- `EntityId`
- `Component`
- `System`
- `Registry`

This is not yet a high-performance archetype ECS.

It is a clean V1 foundation.

### Assets

The asset manager loads raw assets from disk.

It supports:

- text assets
- JSON assets
- binary assets
- image/audio/shader/script classification
- lookup by id
- lookup by path
- asset events

Main types:

- `Asset`
- `AssetId`
- `AssetType`
- `AssetPath`
- `AssetManager`

```cpp
vix::game::AssetManager assets("assets");

auto id = assets.load("hello.txt");
if (!id)
{
  return 1;
}

const auto* asset = assets.get(id.value());
```

### Jobs

The job system wraps `vix/threadpool`.

It supports:

- background jobs
- detached jobs
- job priorities
- job handles
- cancellation through the underlying threadpool handle
- job lifecycle events

Main types:

- `JobSystem`
- `JobHandle`
- `JobPriority`

```cpp
vix::game::JobSystem jobs(2);

auto handle = jobs.submit(
    []()
    {
      // background work
    },
    vix::game::JobPriority::High);

if (handle)
{
  handle.value().wait();
}
```

## Dependencies

`vix/game` builds on existing Vix modules:

- `vix/error`
- `vix/time`
- `vix/fs`
- `vix/path`
- `vix/json`
- `vix/log`
- `vix/threadpool`

## What V1 does not include

V1 intentionally does not include:

- rendering
- window creation
- input handling
- audio engine
- physics engine
- animation system
- scripting runtime
- editor tooling

These should be built as future modules on top of the current foundation.

## Roadmap

### V1 - Completed

- App lifecycle
- Game loop
- `TimeStep` and `Frame`
- `EventBus`
- `SceneManager`
- Simple `Registry`
- `AssetManager`
- `JobSystem`
- Examples
- Tests

### V2 - Completed

- Input module
- Window module
- Renderer abstraction
- 2D rendering foundation
- Asset cache
- Async asset loading
- Scene serialization
- Stronger ECS iteration APIs

### V3 - Completed

- Editor runtime foundation
- Scripting integration
- Audio module
- Physics integration
- Game packaging workflow
- Game templates through `vix new game`

## License

MIT License.
