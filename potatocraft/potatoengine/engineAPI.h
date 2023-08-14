#pragma once

// potatoengine API

namespace engine = potatoengine;

#include "potatoengine/pch.h"

// core
#include "potatoengine/core/application.h"
#include "potatoengine/core/input.h"
#include "potatoengine/core/state.h"

// scene
#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/scene.h"

// renderer
#include "potatoengine/renderer/camera/cameraController.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/renderer/shaderProgram.h"
#include "potatoengine/renderer/rendererAPI.h"
#include "potatoengine/renderer/vao.h"

// assets
#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/assets/shader.h"
#include "potatoengine/assets/texture.h"
#include "potatoengine/assets/prefab.h"
#include "potatoengine/assets/model.h"

// events
#include "potatoengine/events/keyEvent.h"

// utils
#include "potatoengine/utils/timer.h"