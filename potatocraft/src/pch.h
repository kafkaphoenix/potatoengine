#pragma once

#define BIND_EVENT(f) [this](auto &&...args) { return f(std::forward<decltype(args)>(args)...); }

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <format>

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "src/core/log.h"