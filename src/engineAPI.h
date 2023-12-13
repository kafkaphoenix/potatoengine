#pragma once

// potatoengine API

namespace engine = potatoengine;

#include "pch.h"

// core
#include "core/state.h"
#include "ui/imguiLogSink.h"

// ui
#include "ui/imguiAPI.h"

// scene
#include "scene/components/common/cName.h"
#include "scene/components/common/cUUID.h"
#include "scene/entity.h"
#include "scene/meta.h"
#include "scene/sceneManager.h"

// renderer
#include "renderer/renderer.h"

// assets
#include "assets/assetsManager.h"
#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/scene.h"

// events
#include "events/appEvent.h"
#include "events/event.h"

// utils
#include "utils/timer.h"