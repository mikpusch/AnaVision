/*///http://www.tomgibara.com/computer-vision/CannyEdgeDetector.java  */

//import java.awt.image.BufferedImage;
//import java.util.Arrays;

/**
 * <p><em>This software has been released into the public domain.
 * <strong>Please read the notes in this source file for additional information.
 * </strong></em></p>
 * 
 * <p>This class provides a configurable implementation of the Canny edge
 * detection algorithm. This classic algorithm has a number of shortcomings,
 * but remains an effective tool in many scenarios. <em>This class is designed
 * for single threaded use only.</em></p>
 * 
 * <p>Sample usage:</p>
 * 
 * <pre><code>
 * //create the detector
 * CannyEdgeDetector detector = new CannyEdgeDetector();
 * //adjust its parameters as desired
 * detector.setLowThreshold(0.5f);
 * detector.setHighThreshold(1f);
 * //apply it to an image
 * detector.setSourceImage(frame);
 * detector.process();
 * BufferedImage edges = detector.getEdgesImage();
 * </code></pre>
 * 
 * <p>For a more complete understanding of this edge detector's parameters
 * consult an explanation of the algorithm.</p>
 * 
 * @author Tom Gibara
 *
 */

#include "stdafx.h"
#include "CannyEdge.h"
#include "utils.h"
#include <math.h>

CannyEdgeDetector::CannyEdgeDetector(int w, int h) {
	width = w;
	height = h;

	GAUSSIAN_CUT_OFF = 0.005;
	MAGNITUDE_SCALE = 100;
	MAGNITUDE_LIMIT = 1000;
	MAGNITUDE_MAX = (int) (MAGNITUDE_SCALE * MAGNITUDE_LIMIT);


		lowThreshold = 2.5;
		highThreshold = 7.5;
		gaussianKernelRadius = 2.0;
		gaussianKernelWidth = 16;
		contrastNormalized = false;
		picsize = width * height;
		initArrays();
}
void CannyEdgeDetector::initArrays() {
	if (true){//data == NULL || picsize != data.length) {
//			data = new int[picsize];
			magnitude.resize(picsize);
//			magnitude = new int[picsize];

			xConv.resize(picsize);
			yConv.resize(picsize);
			xGradient.resize(picsize);
			yGradient.resize(picsize);
//			xConv = new float[picsize];
//			yConv = new float[picsize];
//			xGradient = new float[picsize];
//			yGradient = new float[picsize];
		}
}

CannyEdgeDetector::~CannyEdgeDetector(){
//	delete [] yGradient;
//	delete [] xGradient;
//	delete [] yConv;
//	delete [] xConv;
//	delete [] magnitude;
//	delete [] data ;
}
bool CannyEdgeDetector::isContrastNormalized() {
		return contrastNormalized;
}
	
	
	// methods
	
// void CannyEdgeDetector::process(int * inoutdata) {
 void CannyEdgeDetector::process(vector<int> & inoutdata) {
//		width = sourceImage.getWidth();
//		height = sourceImage.getHeight();
		pdata = &inoutdata;
		vector<int> & data = *pdata;

		int Max = 0;
		int Min = 214748364;

		for (int i= 0; i<picsize; i++){
			int value = data[i];
			if (value>Max) Max = value;
			if (value<Min) Min = value;
		}
		float scale = 255.0/float(Max-Min);
		for (int i=0; i<picsize; i++){
			float value = (data[i]-Min);
			data[i] = int(value*scale);
//			ShowFloat(data[i], "data");
		}
//		readLuminance();
//		if (contrastNormalized) normalizeContrast();
//		Alert0("Compute grads");
		computeGradients(gaussianKernelRadius, gaussianKernelWidth);
//		Alert0("Computed grads");
		int low = int(lowThreshold * MAGNITUDE_SCALE);
		int high = int( highThreshold * MAGNITUDE_SCALE);
		performHysteresis(low, high);
//		return;
		thresholdEdges();
//		writeEdges(data);
	}
 
	// private utility methods
	
	
	//NOTE: The elements of the method below (specifically the technique for
	//non-maximal suppression and the technique for gradient computation)
	//are derived from an implementation posted in the following forum (with the
	//clear intent of others using the code):
	//  http://forum.java.sun.com/thread.jspa?threadID=546211&start=45&tstart=0
	//My code effectively mimics the algorithm exhibited above.
	//Since I don't know the providence of the code that was posted it is a
	//possibility (though I think a very remote one) that this code violates
	//someone's intellectual property rights. If this concerns you feel free to
	//contact me for an alternative, though less efficient, implementation.
	
void CannyEdgeDetector::computeGradients(float kernelRadius, int kernelWidth) {
		
		//generate the gaussian convolution masks
		vector<int> & data = *pdata;

		float * kernel = new float[kernelWidth];
		float * diffKernel = new float[kernelWidth];
		int kwidth;
		for (kwidth = 0; kwidth < kernelWidth; kwidth++) {
			float g1 = gaussian(kwidth, kernelRadius);
			if (g1 <= GAUSSIAN_CUT_OFF && kwidth >= 2) break;
			float g2 = gaussian(kwidth - 0.5f, kernelRadius);
			float g3 = gaussian(kwidth + 0.5f, kernelRadius);
			kernel[kwidth] = (g1 + g2 + g3) / 3.0 / (2.0 * PIGRECO * kernelRadius * kernelRadius);
			diffKernel[kwidth] = g3 - g2;
		}

		int initX = kwidth - 1;
		int maxX = width - (kwidth - 1);
		int initY = width * (kwidth - 1);
		int maxY = width * (height - (kwidth - 1));
		
		//perform convolution in x and y directions
		for (int x = initX; x < maxX; x++) {
			for (int y = initY; y < maxY; y += width) {
				int index = x + y;
				float sumX = data[index] * kernel[0];
				float sumY = sumX;
				int xOffset = 1;
				int yOffset = width;
				for(; xOffset < kwidth ;) {
					sumY += kernel[xOffset] * (data[index - yOffset] + data[index + yOffset]);
					sumX += kernel[xOffset] * (data[index - xOffset] + data[index + xOffset]);
					yOffset += width;
					xOffset++;
				}
				
				yConv[index] = sumY;
				xConv[index] = sumX;
			}
 
		}
 
		for (int x = initX; x < maxX; x++) {
			for (int y = initY; y < maxY; y += width) {
				float sum = 0;
				int index = x + y;
				for (int i = 1; i < kwidth; i++)
					sum += diffKernel[i] * (yConv[index - i] - yConv[index + i]);
 
				xGradient[index] = sum;
			}
 
		}

		for (int x = kwidth; x < width - kwidth; x++) {
			for (int y = initY; y < maxY; y += width) {
				float sum = 0.0f;
				int index = x + y;
				int yOffset = width;
				for (int i = 1; i < kwidth; i++) {
					sum += diffKernel[i] * (xConv[index - yOffset] - xConv[index + yOffset]);
					yOffset += width;
				}
 
				yGradient[index] = sum;
			}
 
		}
 
		initX = kwidth;
		maxX = width - kwidth;
		initY = width * kwidth;
		maxY = width * (height - kwidth);
		for (int x = initX; x < maxX; x++) {
			for (int y = initY; y < maxY; y += width) {
				int index = x + y;
				int indexN = index - width;
				int indexS = index + width;
				int indexW = index - 1;
				int indexE = index + 1;
				int indexNW = indexN - 1;
				int indexNE = indexN + 1;
				int indexSW = indexS - 1;
				int indexSE = indexS + 1;
				
				float xGrad = xGradient[index];
				float yGrad = yGradient[index];
				float gradMag = hypot(xGrad, yGrad);

				//perform non-maximal supression
				float nMag = hypot(xGradient[indexN], yGradient[indexN]);
				float sMag = hypot(xGradient[indexS], yGradient[indexS]);
				float wMag = hypot(xGradient[indexW], yGradient[indexW]);
				float eMag = hypot(xGradient[indexE], yGradient[indexE]);
				float neMag = hypot(xGradient[indexNE], yGradient[indexNE]);
				float seMag = hypot(xGradient[indexSE], yGradient[indexSE]);
				float swMag = hypot(xGradient[indexSW], yGradient[indexSW]);
				float nwMag = hypot(xGradient[indexNW], yGradient[indexNW]);
				float tmp;
				/*
				 * An explanation of what's happening here, for those who want
				 * to understand the source: This performs the "non-maximal
				 * supression" phase of the Canny edge detection in which we
				 * need to compare the gradient magnitude to that in the
				 * direction of the gradient; only if the value is a local
				 * maximum do we consider the point as an edge candidate.
				 * 
				 * We need to break the comparison into a number of different
				 * cases depending on the gradient direction so that the
				 * appropriate values can be used. To avoid computing the
				 * gradient direction, we use two simple comparisons: first we
				 * check that the partial derivatives have the same sign (1)
				 * and then we check which is larger (2). As a consequence, we
				 * have reduced the problem to one of four identical cases that
				 * each test the central gradient magnitude against the values at
				 * two points with 'identical support'; what this means is that
				 * the geometry required to accurately interpolate the magnitude
				 * of gradient function at those points has an identical
				 * geometry (upto right-angled-rotation/reflection).
				 * 
				 * When comparing the central gradient to the two interpolated
				 * values, we avoid performing any divisions by multiplying both
				 * sides of each inequality by the greater of the two partial
				 * derivatives. The common comparand is stored in a temporary
				 * variable (3) and reused in the mirror case (4).
				 * 
				 */
				if (xGrad * yGrad <= 0.0 /*(1)*/
					? fabs(xGrad) >= fabs(yGrad) /*(2)*/
						? (tmp = fabs(xGrad * gradMag)) >= fabs(yGrad * neMag - (xGrad + yGrad) * eMag) /*(3)*/
							&& tmp > fabs(yGrad * swMag - (xGrad + yGrad) * wMag) /*(4)*/
						: (tmp = fabs(yGrad * gradMag)) >= fabs(xGrad * neMag - (yGrad + xGrad) * nMag) /*(3)*/
							&& tmp > fabs(xGrad * swMag - (yGrad + xGrad) * sMag) /*(4)*/
					: fabs(xGrad) >= fabs(yGrad) /*(2)*/
						? (tmp = fabs(xGrad * gradMag)) >= fabs(yGrad * seMag + (xGrad - yGrad) * eMag) /*(3)*/
							&& tmp > fabs(yGrad * nwMag + (xGrad - yGrad) * wMag) /*(4)*/
						: (tmp = fabs(yGrad * gradMag)) >= fabs(xGrad * seMag + (yGrad - xGrad) * sMag) /*(3)*/
							&& tmp > fabs(xGrad * nwMag + (yGrad - xGrad) * nMag) /*(4)*/
					) {
					magnitude[index] = gradMag >= MAGNITUDE_LIMIT ? MAGNITUDE_MAX : (int) (MAGNITUDE_SCALE * gradMag);
					//NOTE: The orientation of the edge is not employed by this
					//implementation. It is a simple matter to compute it at
					//this point as: Math.atan2(yGrad, xGrad);
				} else {
					magnitude[index] = 0;
				}
			}
		}
		delete [] diffKernel;
		delete [] kernel; 
}
 
	//NOTE: It is quite feasible to replace the implementation of this method
	//with one which only loosely approximates the hypot function. I've tested
	//simple approximations such as Math.abs(x) + Math.abs(y) and they work fine.
float CannyEdgeDetector::hypot(float x, float y) {
		return _hypot(x, y);
	}
 
float CannyEdgeDetector::gaussian(float x, float sigma) {
		return exp(-(x * x) / (2.0 * sigma * sigma));
}
 
void CannyEdgeDetector::performHysteresis(int low, int high) {
		//NOTE: this implementation reuses the data array to store both
		//luminance data from the image, and edge intensity from the processing.
		//This is done for memory efficiency, other implementations may wish
		//to separate these functions.
		vector<int> & data = *pdata;

	for (int i=0; i<picsize; i++){
		data[i] = 0;
		if (magnitude[i] != 0){
//			ShowFloat(magnitude[i], "magn");
//			ShowFloat(i, "i");
		}
//		data[i] = magnitude[i];
	}
//	return;
//		Arrays.fill(data, 0);
 
		int offset = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (data[offset] == 0 && magnitude[offset] >= high) {
					follow(x, y, offset, low);
				}
				offset++;
			}
		}
	for (int i=0; i<picsize; i++){
		if (data[i] != 0){
		 
	//		ShowFloat(data[i], "magn");
	//		ShowFloat(i, "i");
		}
//		data[i] = magnitude[i];
	}
 	}
 
void CannyEdgeDetector::follow(int x1, int y1, int i1, int threshold) {
		vector<int> & data = *pdata;

		int x0 = x1 == 0 ? x1 : x1 - 1;
		int x2 = x1 == width - 1 ? x1 : x1 + 1;
		int y0 = y1 == 0 ? y1 : y1 - 1;
		int y2 = y1 == height -1 ? y1 : y1 + 1;
		
		data[i1] = magnitude[i1];
		for (int x = x0; x <= x2; x++) {
			for (int y = y0; y <= y2; y++) {
				int i2 = x + y * width;
				if ((y != y1 || x != x1)
					&& data[i2] == 0 
					&& magnitude[i2] >= threshold) {
					follow(x, y, i2, threshold);
					return;
				}
			}
		}
	}

void CannyEdgeDetector::thresholdEdges() {
		vector<int> & data = *pdata;

		for (int i = 0; i < picsize; i++) {
			if (data[i]>0) data[i]=1;
//			data[i] = data[i] > 0 ? -1 : 0xff000000;
		}
	}
	
int CannyEdgeDetector::luminance(float r, float g, float b) {
		return int (0.299 * r + 0.587 * g + 0.114 * b);
}
	
void CannyEdgeDetector::readLuminance() {
//		int type = sourceImage.getType();
/*
if (type == BufferedImage.TYPE_INT_RGB || type == BufferedImage.TYPE_INT_ARGB) {
			int[] pixels = (int[]) sourceImage.getData().getDataElements(0, 0, width, height, null);
			for (int i = 0; i < picsize; i++) {
				int p = pixels[i];
				int r = (p & 0xff0000) >> 16;
				int g = (p & 0xff00) >> 8;
				int b = p & 0xff;
				data[i] = luminance(r, g, b);
			}
		} else if (type == BufferedImage.TYPE_BYTE_GRAY) {
			byte[] pixels = (byte[]) sourceImage.getData().getDataElements(0, 0, width, height, null);
			for (int i = 0; i < picsize; i++) {
				data[i] = (pixels[i] & 0xff);
			}
		} else if (type == BufferedImage.TYPE_USHORT_GRAY) {
			short[] pixels = (short[]) sourceImage.getData().getDataElements(0, 0, width, height, null);
			for (int i = 0; i < picsize; i++) {
				data[i] = (pixels[i] & 0xffff) / 256;
			}
		} else if (type == BufferedImage.TYPE_3BYTE_BGR) {
            byte[] pixels = (byte[]) sourceImage.getData().getDataElements(0, 0, width, height, null);
            int offset = 0;
            for (int i = 0; i < picsize; i++) {
                int b = pixels[offset++] & 0xff;
                int g = pixels[offset++] & 0xff;
                int r = pixels[offset++] & 0xff;
                data[i] = luminance(r, g, b);
            }
        } else {
			throw new IllegalArgumentException("Unsupported image type: " + type);
		}
*/
	}
 
void CannyEdgeDetector::normalizeContrast() {
#define histogramlength 256
		vector<int> & data = *pdata;

		int histogram[histogramlength];
		int remap[histogramlength];
		for (int i = 0; i < histogramlength; i++) {
			histogram[i]=0;
			remap[i]=0;
		}

//		for (int i = 0; i < data.length; i++) {
		for (int i = 0; i < picsize; i++) {
			histogram[data[i]]++;
		}
		int sum = 0;
		int j = 0;
		for (int i = 0; i < histogramlength; i++) {
			sum += histogram[i];
			int target = sum*(histogramlength-1)/picsize;
			for (int k = j+1; k <=target; k++) {
				remap[k] = i;
			}
			j = target;
		}
		
		for (int i = 0; i < histogramlength; i++) {
			data[i] = remap[data[i]];
		}
#undef histogramlength
}
	
void CannyEdgeDetector::writeEdges(int pixels[]) {
		//NOTE: There is currently no mechanism for obtaining the edge data
		//in any other format other than an INT_ARGB type BufferedImage.
		//This may be easily remedied by providing alternative accessors.
//		if (edgesImage == NULL) {
//			edgesImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
//		}
//		edgesImage.getWritableTile(0, 0).setDataElements(0, 0, width, height, pixels);
//	}
 
}

