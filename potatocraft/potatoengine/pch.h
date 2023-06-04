#pragma once

#define BIND_EVENT(f) [this](auto &&...args) { return f(std::forward<decltype(args)>(args)...); }

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <print>

#include "potatoengine/core/log.h"