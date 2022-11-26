#pragma once

#ifndef epicengine_h
#define epicengine_h

#define NOMINMAX

#pragma warning(disable : 4996)

#include <Windows.h>
#include <thread>
#include <vector>
#include <iostream>
#include <math.h>
#include <functional>
#include <mutex>         
#include <TlHelp32.h>
#include <Psapi.h>
#include <timeapi.h>
#include <filesystem>
#include <fstream>
#include <intrin.h>
#include <direct.h>
#include <utility>


#include "resource.h"
#include "framework.h"


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_stdlib.h"

#include "backward.hpp"
using namespace backward;

#include <d3d11.h>

#include "timing.hpp"
#include "appwindows.hpp"
#include "processlist.h"
#include "filetools.hpp"
#include "winfont.hpp"
#include "vectors.hpp"
#include "basewindow.hpp"
#include "main_window.hpp"
#include "activeprocess.hpp"


#endif
