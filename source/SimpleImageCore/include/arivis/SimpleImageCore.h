#pragma once

#ifdef SIMPLEIMAGECORE_EXPORTS
#define SIMPLEIMAGECORE_API __declspec(dllexport)
#else
#define SIMPLEIMAGECORE_API __declspec(dllimport)
#endif

#include "SimpleImageStack.h"

namespace arivis { namespace simpleimagecore {

/**
 * Contains all parameters required to create a new image stack
 */
struct SIMPLEIMAGECORE_API StackCreateInfo
{
	/**
	 * Specifies the pixel type for the created image stack
	 */
	arivis::simpleimagecore::PixelType pixelType;

	/**
	 * Specifies the width for the created image stack
	 */
	unsigned __int32 width;

	/**
	 * Specifies the height for the created image stack
	 */
	unsigned __int32 height;

	/**
	 * Specifies the number of channels for the created image stack
	 */
	unsigned __int32 channels;

	/**
	 * Specifies the number of planes for the created image stack
	 */
	unsigned __int32 planes;

	/**
	 * Specifies the number of frames for the created image stack
	 */
	unsigned __int32 frames;
};

/**
 * The simple image core object is used to open and close image stacks.
 */
class SIMPLEIMAGECORE_API SimpleImageCore {
public:

	/**
	 * Opens the image stack, stored in the file named <var>filename</var>. 
	 *	
	 * The ImageStack should be closed by passing the retrieved pointer to 
	 * closeImageStack(SimpleImageStack*), when it is no longer needed.
	 * 
	 * @param filename The path to the file to open as an ANSI encoded string.
	 * @return A pointer to the opened SimpleImageStack.
	 */
	virtual arivis::simpleimagecore::SimpleImageStack* openImageStack(const char* filename) = 0;

	/**
	 * Closes the specified SimpleImageStack. The SimpleImageStack object
	 * will be deleted after closing the stack.
	 * All unsaved changes will be lost, see saveImageStack()
	 * 
	 * @param imageStack A pointer to the SimpleImageStack to close.
	 */
	virtual void closeImageStack(arivis::simpleimagecore::SimpleImageStack* imageStack) = 0;

	/**
	 * Creates a new image stack with the specified file name and PixelType
	 *	
	 * The ImageStack must be closed by passing the retrieved pointer to 
	 * closeImageStack(SimpleImageStack*), to write all data to disk.
	 * 
	 * @param filename The path to the file to create as an ANSI encoded string.
	 * @param pixelType the type of image data to store in the SimpleImageStack
	 *
	 * @return 
	 *      A pointer to the opened SimpleImageStack or NULL If the ImageStack could not
	 *      be created.
	 */
	virtual arivis::simpleimagecore::SimpleImageStack* createImageStack(const char* filename, const arivis::simpleimagecore::StackCreateInfo& stackCreateInfo) = 0;

	/**
	 * Saves the specified SimpleImageStack.
	 */
	virtual void saveImageStack(arivis::simpleimagecore::SimpleImageStack* imageStack) = 0;

protected:
	/**
	 * This interface is not intended to be directly instanciated or 
	 * implemented by clients.
	 *
	 * Use CreateImageCore() to create an instance of this class
	 */
	SimpleImageCore();


	/**
	 * This interface must not be deleted by clients.
	 * 
	 * Use DisposeImageCore(SimpleImageCore*) to delete an instance of 
	 * this class
	 */
	virtual ~SimpleImageCore();
};

/**
 * Creates a new SimpleImageCore object. A single instance of the
 * SimpleImageCore class should be used to handle all stack operations 
 * in an application.
 *
 * Using multiple instances of this class is NOT recommended.
 *
 * @param apiKey the license key provided with your simple image core license
 *               (this can be NULL if you aquired a free license)
 *
 * @return a new instance of the SimpleImageCore class or NULL if the api key
 * is invalid.
 *   
 */
extern SIMPLEIMAGECORE_API SimpleImageCore* CreateImageCore(const char* apiKey);

/**
 * Disposes a SimpleImageCore object.
 *
 * This frees all resources aquired by the specified SimpleImageCore object
 * including all image stacks. The SimpleImageCore object is deleted after all resources
 * have been released.
 */
extern SIMPLEIMAGECORE_API void DisposeImageCore(SimpleImageCore* imageCore);

}} // namespace arivis::simpleimagecore
