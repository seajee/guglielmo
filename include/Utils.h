#pragma once

#include <string>
#include <cassert>

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) do { } while (0);
#endif

std::string ReadFile(const std::string& filePath);
