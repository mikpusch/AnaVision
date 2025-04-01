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
#pragma once;

#include <vector>

using namespace std ;

class CannyEdgeDetector {
public:


	CannyEdgeDetector(int w, int h);
	~CannyEdgeDetector();
	// statics
	
	float GAUSSIAN_CUT_OFF ; //= 0.005f;
	float MAGNITUDE_SCALE; // = 100F;
	float MAGNITUDE_LIMIT; // = 1000F;
	int MAGNITUDE_MAX ; //= (int) (MAGNITUDE_SCALE * MAGNITUDE_LIMIT);

	// fields
	
	int height;
	int width;
	int picsize;
//	int *  data;
	vector<int> * pdata;

//	int * magnitude;
	vector<int> magnitude;
//	ate BufferedImage sourceImage;
//	int * sourceImage;
//	int * edgesImage;
//	private BufferedImage edgesImage;
	
	float gaussianKernelRadius;
	float lowThreshold;
	float highThreshold;
	int gaussianKernelWidth;
	bool contrastNormalized;

	vector<float> xConv;
	vector<float> yConv;
	vector<float> xGradient;
	vector<float> yGradient;
//	float * xConv;
//	float * yConv;
//	float * xGradient;
//	float * yGradient;

	bool isContrastNormalized() ;
	
	void setContrastNormalized(bool contrastNormalized) ;
	
	void process(int * inoutdata) ;
	void process(vector<int> & inoutdata) ;
	
	void initArrays() ;

	void computeGradients(float kernelRadius, int kernelWidth) ;
		
	float hypot(float x, float y) ;
 
	float gaussian(float x, float sigma) ;
 
	void performHysteresis(int low, int high) ;
 
	void follow(int x1, int y1, int i1, int threshold) ;

	void thresholdEdges() ;
	int luminance(float r, float g, float b) ;
	void readLuminance() ;
 
	void normalizeContrast() ;
	
	void writeEdges(int pixels[]) ;
		//NOTE: There is currently no mechanism for obtaining the edge data
};

