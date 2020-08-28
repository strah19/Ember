#ifndef EMBER_H
#define EMBER_H

#include "Core/EventHandlers/Input.h"
#include "Core/AssetLoaders/AssetInitializers.h"
#include "Core/AssetLoaders/Assets.h"
#include "Core/Camera.h"
#include "Core/Collision.h"
#include "Core/GuiCore/GuiObjects.h"
#include "Core/GuiCore/Button.h"

#include "Animation/SpriteSheet.h"

#include "Platform/Window/WindowAPI.h"
#include "Platform/Window/WindowsWindow.h"
#include "Renderer/2DRenderer.h"

#include "Structures/Color.h"
#include "Structures/Vec2.h"
#include "Structures/Texture.h"
#include "Structures/FileManager.h"

#include "ECS/Entity.h"
#include "ECS/Components/PositionComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/DebugComponent.h"

#include "TileSystem/Grid.h"
#include "TileSystem/TileMap.h"

#endif // !EMBER_H
