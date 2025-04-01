#pragma once

#ifdef SIMPLEIMAGECORE_EXPORTS
#define SIMPLEIMAGECORE_API __declspec(dllexport)
#else
#define SIMPLEIMAGECORE_API __declspec(dllimport)
#endif

namespace arivis { namespace simpleimagecore {

/**
 * Shows a modal about dialog containing version information
 * and copyright notice.
 */
extern "C" SIMPLEIMAGECORE_API void About(void* hwnd);

}} // arivis::simpleimagecore
