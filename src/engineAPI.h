#pragma once

// potatoengine API

namespace engine = potatoengine;

#include "pch.h"
#include "settings.h"

// core
#include "core/application.h"
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

// serializers
#include "serializers/sSettings.h"

// renderer
#include "renderer/renderer.h"
#include "renderer/rendererAPI.h"

// assets
#include "assets/assetsManager.h"

// events
#include "events/appEvent.h"
#include "events/event.h"

// utils
#include "utils/numericComparator.h"
#include "utils/timer.h"
