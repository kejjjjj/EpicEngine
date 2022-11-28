#pragma once

#ifndef epicengine_h
#define epicengine_h

#define NOMINMAX

#pragma warning(disable : 4996)
#pragma warning(disable : 4005) //DIRECTX11 STOP!!!!

#ifdef _WIN64
typedef unsigned long long UPTR;
#else
typedef unsigned long UPTR;
#endif

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

#include <d3d11.h>
#include <d3dx11.h>

#include "timing.hpp"
#include "osinfo.hpp"
#include "appwindows.hpp"
#include "processlist.h"
#include "filetools.hpp"
#include "winfont.hpp"
#include "imageresources.hpp"
#include "basewindow.hpp"
#include "main_window.hpp"
#include "scanwindow.hpp"
#include "memorybytes.hpp"
#include "activeprocess.hpp"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#endif
