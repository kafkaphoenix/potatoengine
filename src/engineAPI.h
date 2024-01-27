#pragma once

// potatoengine API

namespace engine = potatoengine;

#include "pch.h"
#include "settings.h"

// core
#include "core/application.h"
#include "core/input.h"
#include "core/keyCodes.h"
#include "core/state.h"
#include "core/time.h"
#include "ui/imguiLogSink.h"

// ui
#include "ui/imguiAPI.h"

// scene
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/core/cDeleted.h"
#include "scene/components/core/cName.h"
#include "scene/components/core/cTag.h"
#include "scene/components/core/cTime.h"
#include "scene/components/core/cUUID.h"
#include "scene/components/graphics/cBody.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cMesh.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/graphics/cShape.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/input/cActiveInput.h"
#include "scene/components/input/cInput.h"
#include "scene/components/physics/cCollider.h"
#include "scene/components/physics/cGravity.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/terrain/cChunk.h"
#include "scene/components/terrain/cChunkManager.h"
#include "scene/components/utils/cNoise.h"
#include "scene/components/world/cLight.h"
#include "scene/components/world/cSkybox.h"
#include "scene/meta.h"
#include "scene/sceneManager.h"
#include "scene/system.h"

// serializers
#include "serializers/sSettings.h"

// renderer
#include "renderer/buffer.h"
#include "renderer/renderer.h"
#include "renderer/rendererAPI.h"

// assets
#include "assets/assetsManager.h"

// events
#include "events/appEvent.h"
#include "events/event.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"
#include "events/windowEvent.h"

// utils
#include "utils/numericComparator.h"
#include "utils/timer.h"
