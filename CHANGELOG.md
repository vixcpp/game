# Changelog

All notable changes to `vix/game` are documented in this file.

## V5

### Added

- Added `GameExportConfig`.
- Added `GameExporter`.
- Added `GameExportManifest`.
- Added `GameExportAsset`.
- Added `GameAssetPipeline`.
- Added `export.json` generation during game export.
- Added asset scanning for exported game projects.
- Added asset type detection:
  - image
  - text
  - binary
- Added temporary and cache file filtering for asset export.
- Added exported asset metadata in `export.json`.
- Added `vix game export` CLI command.
- Added V5 `vix new --game` template.
- Added generated `game.package.json` support for exported game projects.
- Added generated `assets/` directory support.
- Added generated README for game projects.
- Added generated `Scene + GameRuntime` example.
- Added `export_project` example.

### Changed

- `GameExporter` now reads `game.package.json` when available.
- `GameExporter` now uses package metadata for:
  - export name
  - version
  - asset root
  - output directory
- `GameExporter` now writes an inspectable export manifest.
- `vix dev` now waits for file changes after a short-lived game exits cleanly instead of restarting it in a loop.
- `vix new --game` now generates a more complete runtime-first game project.

### Fixed

- Fixed generated game template workflow with `vix build`, `vix run`, `vix dev`, and `vix game export`.
- Fixed CLI game command support through `VIX_CLI_HAS_GAME`.
- Fixed export manifest copied file count consistency.
- Fixed asset pipeline ignoring `.gitkeep` and temporary/cache files.

## V4

### Added

- Added `GameRuntime` as the main runtime coordinator.
- Added `GameContext` backend installation helpers.
- Added headless runtime flow with `NullWindow` and `NullRenderer`.
- Added SDL window backend.
- Added SDL input mapping.
- Added SDL renderer backend.
- Added SDL texture upload.
- Added SDL sprite rendering.
- Added `Sprite`.
- Added `DrawSpriteCommand`.
- Added `RuntimeDiagnostics`.
- Added scene inspection APIs.
- Added editor runtime inspection APIs.
- Added SDL window/input example.
- Added SDL renderer demo.
- Added sprite demo.
- Added V4 architecture tests.

### Changed

- Improved runtime layering around `App`, `GameRuntime`, and `GameContext`.
- Improved renderer backend separation.
- Improved SDL integration while keeping the module backend-independent.
- Improved examples for real window, input, renderer, and sprite flows.

### Fixed

- Fixed headless runtime behavior for tests and CI.
- Fixed SDL window and renderer integration path.
- Fixed runtime diagnostics coverage for V4 architecture.

## V3

### Added

- Added `GameContext`.
- Added `GameRuntime`.
- Added `EditorContext`.
- Added `EditorRuntime`.
- Added `SceneRuntime`.
- Added `ScriptRuntime`.
- Added `AudioRuntime`.
- Added `PhysicsRuntime`.
- Added `GamePackage`.
- Added game package metadata model.
- Added `vix new --game` initial template.
- Added V3 runtime smoke example.

### Changed

- Split orchestration logic into dedicated runtime layers.
- Moved toward a cleaner architecture where `App` remains the root facade instead of becoming a full engine object.
- Prepared the module for editor, scripting, audio, physics, and packaging workflows.

## V2

### Added

- Added input module.
- Added `InputSystem`.
- Added input actions.
- Added input keys and buttons.
- Added window abstraction.
- Added `Window`.
- Added `WindowBackend`.
- Added `NullWindow`.
- Added renderer abstraction.
- Added `Renderer`.
- Added `RendererBackend`.
- Added `NullRenderer`.
- Added 2D rendering foundation.
- Added `Renderer2D`.
- Added `Render2DSystem`.
- Added camera, color, rect, vector, sprite-related foundations.
- Added asset cache.
- Added async asset loading.
- Added scene serialization.
- Added stronger ECS iteration APIs.

### Changed

- Expanded the module beyond the first application loop foundation.
- Improved separation between input, window, renderer, assets, and scenes.
- Prepared the architecture for real backends without forcing any specific backend.

## V1

### Added

- Added `App`.
- Added `AppConfig`.
- Added application lifecycle.
- Added `GameLoop`.
- Added `TimeStep`.
- Added `Frame`.
- Added fixed update support.
- Added `Event`.
- Added `EventBus`.
- Added `EventType`.
- Added `Scene`.
- Added `SceneId`.
- Added `SceneManager`.
- Added simple ECS-style `Registry`.
- Added `Entity`.
- Added `EntityId`.
- Added `Component`.
- Added `System`.
- Added `Asset`.
- Added `AssetId`.
- Added `AssetManager`.
- Added `AssetPath`.
- Added `AssetType`.
- Added `JobSystem`.
- Added `JobHandle`.
- Added `JobPriority`.
- Added examples.
- Added unit tests.

### Changed

- Established the first stable foundation for building games, simulations, tools, and custom engines on top of Vix.cpp.

## Notes

`vix/game` is intentionally not a full game engine.

It provides a modular C++ foundation for developers who want to build their own runtime, engine, tools, simulations, or game architecture on top of Vix.cpp.
