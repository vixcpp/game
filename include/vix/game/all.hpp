/**
 *
 *  @file all.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/game
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#ifndef VIX_GAME_ALL_HPP
#define VIX_GAME_ALL_HPP

#include <vix/game/version.hpp>

#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>

#include <vix/game/App.hpp>
#include <vix/game/AppConfig.hpp>

#include <vix/game/Frame.hpp>
#include <vix/game/GameClock.hpp>
#include <vix/game/GameLoop.hpp>
#include <vix/game/TimeStep.hpp>

#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>

#include <vix/game/Scene.hpp>
#include <vix/game/SceneData.hpp>
#include <vix/game/SceneId.hpp>
#include <vix/game/SceneManager.hpp>
#include <vix/game/SceneSerializer.hpp>

#include <vix/game/Entity.hpp>
#include <vix/game/EntityId.hpp>
#include <vix/game/Component.hpp>
#include <vix/game/System.hpp>
#include <vix/game/Registry.hpp>
#include <vix/game/RegistryView.hpp>

#include <vix/game/Asset.hpp>
#include <vix/game/AssetCache.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/AssetPath.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/AsyncAssetLoader.hpp>

#include <vix/game/JobHandle.hpp>
#include <vix/game/JobPriority.hpp>
#include <vix/game/JobSystem.hpp>

#include <vix/game/InputAction.hpp>
#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>
#include <vix/game/InputMap.hpp>
#include <vix/game/InputState.hpp>
#include <vix/game/InputSystem.hpp>

#include <vix/game/Camera2D.hpp>
#include <vix/game/Color.hpp>
#include <vix/game/Rect.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/Transform2D.hpp>
#include <vix/game/Vec2.hpp>

#include <vix/game/NullRenderer.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/Render2DSystem.hpp>
#include <vix/game/RenderCommand.hpp>
#include <vix/game/TextureHandle.hpp>

#include <vix/game/NullWindow.hpp>
#include <vix/game/Window.hpp>
#include <vix/game/WindowBackend.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>

#include <vix/game/GameContext.hpp>
#include <vix/game/GameRuntime.hpp>

#include <vix/game/EditorContext.hpp>
#include <vix/game/EditorRuntime.hpp>

#include <vix/game/SceneRuntime.hpp>
#include <vix/game/ScriptRuntime.hpp>
#include <vix/game/AudioRuntime.hpp>
#include <vix/game/PhysicsRuntime.hpp>

#include <vix/game/GamePackage.hpp>

#include <vix/game/RuntimeDiagnostics.hpp>
#include <vix/game/GameExportConfig.hpp>
#include <vix/game/GameExporter.hpp>
#include <vix/game/GameExportManifest.hpp>
#include <vix/game/GameAssetPipeline.hpp>
#include <vix/game/GameExportAsset.hpp>

#endif // VIX_GAME_ALL_HPP
