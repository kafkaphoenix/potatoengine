#pragma once

#define BIND_EVENT(f) [this](auto &&...args) { return f(std::forward<decltype(args)>(args)...); }

#include <algorithm>
#include <any>
#include <array>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <print>
#include <random>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "potatoengine/config.h"
#include "potatoengine/core/log.h"
