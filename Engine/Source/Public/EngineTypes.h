#pragma once
#include "Debug/GDebug.h"

//system libs
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

//redefine of the standard string
typedef std::string GString;

//redefine all of the unsigned ints 
typedef uint8_t GUi8;
typedef uint16_t GUi16;
typedef uint32_t GUi32;
typedef uint64_t GUi64;

//redefine the standard vector
template <typename T>
using TArray = std::vector<T>;

//redefine the smart pointers
//redefine shared pointer
template <typename T>
using TShared = std::shared_ptr<T>;

//redefine unique pointer
template <typename T>
using TUnique = std::unique_ptr<T>;

//redefine weak pointer
template <typename T>
using TWeak = std::weak_ptr<T>;

//redefine the make shared function
template <typename T, typename... Args> 
TShared<T> TMakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

//redefine the make unique function
template <typename T, typename... Args>
TUnique<T> TMakeUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

