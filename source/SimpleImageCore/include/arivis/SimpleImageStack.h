#pragma once

#ifdef SIMPLEIMAGECORE_EXPORTS
#define SIMPLEIMAGECORE_API __declspec(dllexport)
#else
#define SIMPLEIMAGECORE_API __declspec(dllimport)
#endif

namespace arivis { namespace simpleimagecore {

/**
 * Specifies the type of data for each pixel in a SimpleImageStack
 */
enum SIMPLEIMAGECORE_API PixelType
{
	/**
	 * Specifies that the type is 8 bits per pixel
	 */
	PT_8BIT = 1,

	/**
	 * Specifies that the type is 8 bits per pixel
	 */
	PT_16BIT = 2,

	/**
	 * Specifies that the type is undefined
	 */
	PT_UNDEFINED = 255
};

/**
 * Stores a set of four integers that represent the location and size of a rectangle.
 * A rectangle is defined by its width, height, and upper-left corner.
 */
struct SIMPLEIMAGECORE_API Rectangle
{
	Rectangle();
	Rectangle(__int32 x, __int32 y, __int32 width, __int32 height);
	/**
	 * The x-coordinate of the upper-left corner of this Rectangle structure. 
	 */
	__int32 x;

	/**
	 * The y-coordinate of the upper-left corner of this Rectangle structure. 
	 */
	__int32 y;

	/**
	 * The width of this Rectangle structure. 
	 */
	__int32 width;
	
	/**
	 * The height of this Rectangle structure. 
	 */
	__int32 height;
};

/**
 * Represents an RGBA pixel
 */
struct SIMPLEIMAGECORE_API RGBAPixel
{
	unsigned __int8 Blue;
	unsigned __int8 Green;
	unsigned __int8 Red;
	unsigned __int8 Alpha;
};

/**
 * Represents an RGB image that stores the red, green and blue
 * components of an image in seperate buffers for each color channel
 */
struct SIMPLEIMAGECORE_API PlanarImage
{
	/** buffer containing the red color channel */
	unsigned __int8* red;

	/** buffer containing the green color channel */
	unsigned __int8* green;

	/** buffer containing the blue color channel */
	unsigned __int8* blue;
};

/**
 * Creates a planar image for the given size in pixel.
 *
 * This creates a new PlanarImage with allocates memory for the channel buffers
 * to hold the specified number of pixels.
 */
extern SIMPLEIMAGECORE_API PlanarImage* CreatePlanarImage(__int32 width, __int32 height);

/**
 * Releases a planar image that was created with CreatePlanarImage(__int32,__int32)
 *
 * This releases the memory allocated for the channel buffers and the
 * planar image itself.
 */
extern SIMPLEIMAGECORE_API void ReleasePlanarImage(PlanarImage** hImage);

/**
 * The SimpleImageStack class provides read and write operations
 * to work with images stored in the arivis SIS format.
 */
class SIMPLEIMAGECORE_API SimpleImageStack {
public:

	/**
	 * Returns the PixelType for this SimpleImageStack.
	 *
	 * @return
	 *    the pixel type for this SimpleImageStack
	 */
	virtual arivis::simpleimagecore::PixelType getPixelType() = 0;

	/**
	 * Returns the location and size of this SimpleImageStack
	 *
	 * @return
	 *		a rectangle that represents the bounds of this SimpleImageStack
	 */
	virtual arivis::simpleimagecore::Rectangle getBounds() = 0;

	/**
	 * Reads a subregion of image data from this SimpleImageStack and stores
	 * an RGBA representation of the image data in the specified pixelBuffer.
	 *
	 * The requested <var>region</var> refers to a region within the zoomed
	 * bounds of this SimpleImageStack<p>
	 *
	 * @param region
	 *		The requested image region. This should by a subregion of the
	 *      zoomed bounds of this image stack
	 *
	 * @param zoom	
	 *		The zoom factor that is applied to the bounds of this SimpleImageStack
	 *         
	 * @param pixelBuffer
	 *		The buffer, to write the image data to. The number of pixels stored
	 *      in this buffer is defined as the product of width and height of the
	 *      specified region. The buffer must be large enough to hold at least
	 *      this number of pixels.
	 *      
	 * @param plane
	 *		The 0-based index of the plane to read
	 *
	 * @param frame
	 *		The 0-based index of the plane to read
	 */
	virtual void readRGBA(const arivis::simpleimagecore::Rectangle& region, double zoom, arivis::simpleimagecore::RGBAPixel* pixelBuffer, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Reads a subregion of image data from this SimpleImageStack and stores
	 * an RGB representation of the image data in the specified pixelBuffer.
	 *
	 * The requested <var>region</var> refers to a region within the zoomed
	 * bounds of this SimpleImageStack<p>
	 *
	 * @param region
	 *		The requested image region. This should by a subregion of the
	 *      zoomed bounds of this image stack
	 *
	 * @param zoom	
	 *		The zoom factor that is applied to the bounds of this SimpleImageStack
	 *         
	 * @param planarImage
	 *		The image, to write the image data to. The number of pixels stored
	 *      in this image is defined as the product of width and height of the
	 *      specified region. The channel buffers of that image must be large 
	 *      enough to hold at least this number of pixels.
	 *      
	 * @param plane
	 *		The 0-based index of the plane to read
	 *
	 * @param frame
	 *		The 0-based index of the plane to read
	 */
	virtual void readPlanarImage(const arivis::simpleimagecore::Rectangle& region, double zoom, arivis::simpleimagecore::PlanarImage* planarImage, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Reads a subregion of image data from this SimpleImageStack and stores
	 * an the image data for the specified channel in the specified buffer.
	 *
	 * The requested <var>region</var> refers to a region within the zoomed
	 * bounds of this SimpleImageStack<p>
	 *
	 * @param region
	 *		The requested image region. This should by a subregion of the
	 *      zoomed bounds of this image stack
	 *
	 * @param zoom	
	 *		The zoom factor that is applied to the bounds of this SimpleImageStack
	 *         
	 * @param buffer
	 *		The buffer, to write the channel data to. The number of intensity value
	 *	    stored in this buffer is defined as the product of width and height of the
	 *      specified region. The buffer must be large enough to hold at least
	 *      this number of intensity values.
	 *      
	 * @param plane
	 *		The 0-based index of the channel to read
	 *
	 * @param plane
	 *		The 0-based index of the plane to read
	 *
	 * @param frame
	 *		The 0-based index of the plane to read
	 */
	virtual void read8bitChannelData(const arivis::simpleimagecore::Rectangle& region, double zoom, unsigned __int8* buffer, unsigned __int32 channel, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Writes a subregion of image data to the SimpleImageStack.
	 *
	 * @param region
	 *		The image region to write to. This should by a subregion of the
	 *      bounds of this image stack
	 *
	 * @param buffer
	 *		The buffer to write. The number of intensity value contained in this
	 *      buffer is defined as the product of width and height of the specified region.
	 *	    The buffer must contain at leas this number of intensity values.
	 *      
	 * @param plane
	 *		The 0-based index of the channel to write to
	 *
	 * @param plane
	 *		The 0-based index of the plane to write to
	 *
	 * @param frame
	 *		The 0-based index of the plane to write to
	 */
	virtual void write8bitChannelData(const arivis::simpleimagecore::Rectangle& region, unsigned __int8* buffer, unsigned __int32 channel, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Writes a subregion of image data to the SimpleImageStack.
	 *
	 * @param region
	 *		The image region to write to. This should by a subregion of the
	 *      bounds of this image stack
	 *
	 * @param buffer
	 *		The buffer to write. The number of intensity value contained in this
	 *      buffer is defined as the product of width and height of the specified region.
	 *	    The buffer must contain at leas this number of intensity values.
	 *      
	 * @param plane
	 *		The 0-based index of the channel to write to
	 *
	 * @param plane
	 *		The 0-based index of the plane to write to
	 *
	 * @param frame
	 *		The 0-based index of the plane to write to
	 */
	virtual void write16bitChannelData(const arivis::simpleimagecore::Rectangle& region, unsigned __int16* buffer, unsigned __int32 channel, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Reads a subregion of image data from this SimpleImageStack and stores
	 * an the image data for the specified channel in the specified buffer.
	 *
	 * The requested <var>region</var> refers to a region within the zoomed
	 * bounds of this SimpleImageStack<p>
	 *
	 * @param region
	 *		The requested image region. This should by a subregion of the
	 *      zoomed bounds of this image stack
	 *
	 * @param zoom	
	 *		The zoom factor that is applied to the bounds of this SimpleImageStack
	 *         
	 * @param buffer
	 *		The buffer, to write the channel data to. The number of intensity value
	 *	    stored in this buffer is defined as the product of width and height of the
	 *      specified region. The buffer must be large enough to hold at least
	 *      this number of intensity values.
	 *      
	 * @param plane
	 *		The 0-based index of the channel to read
	 *
	 * @param plane
	 *		The 0-based index of the plane to read
	 *
	 * @param frame
	 *		The 0-based index of the plane to read
	 */
	virtual void read16bitChannelData(const arivis::simpleimagecore::Rectangle& region, double zoom, unsigned __int16* buffer, unsigned __int32 channel, unsigned __int32 plane, unsigned __int32 frame) = 0;

	/**
	 * Returns the number of planes of this SimpleImageStack
	 *
	 * @return
	 *		the number of planes of this SimpleImageStack
	 */
	virtual unsigned __int32 getPlaneCount() = 0;

	/**
	 * Returns the number of frames of this SimpleImageStack
	 *
	 * @return
	 *		the number of frames of this SimpleImageStack
	 */
	virtual unsigned __int32 getFrameCount() = 0;

	/**
	 * Returns the number of channels of this SimpleImageStack
	 *
	 * @return
	 *		the number of channels of this SimpleImageStack
	 */
	virtual unsigned __int32 getChannelCount() = 0;

protected:

	/**
	 * This interface is not intended to be directly instanciated or 
	 * implemented by clients.
	 */
	SimpleImageStack();

	/**
	 * This interface must not be deleted by clients.
	 */
	virtual ~SimpleImageStack();
};

}} // namespace arivis::simpleimagecore
