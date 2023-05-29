#pragma once

#define BIND_EVENT(f) [this](auto &&...args) { return f(std::forward<decltype(args)>(args)...); }

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "potatoengine/core/log.h"