/**
 *
 * James George 
 * Quaternion Examples
 * 
 * A set of examples illustrating useful places to use quaternions
 *
 * ArcBall:
 * This example shows how to rotate a cube by dragging it like you'd imagine it to work physically
 */

#pragma once

#include "ofMain.h"
#include "ofxFft.h"

//-----------------------------------------------------------------------------
// Preprocessor definitions
//-----------------------------------------------------------------------------
#define MY_SRATE         44100            // sample rate
#define MY_CHANNELS      2                // number of channels
#define MY_BUFFERHISTORY 50               // number of buffers to save
#define MY_BUFFERSIZE    512              // number of frames in a buffer
#define MY_NBUFFERS      2                // number of buffers latency
#define MY_PIE           3.14159265358979 // for convenience



//-----------------------------------------------------------------------------
// Our Bokeh class
//-----------------------------------------------------------------------------
class Bokeh {
public:
    // Constructor and destructor -- do nothing
    Bokeh() {}
    ~Bokeh() {}
    
    void setup(){
        bokeh_image.load(ofToDataPath("Bokeh.png")); // Load the image
        x = ofRandomWidth();                         // Get random x position
        y = ofRandomHeight();                        // Get random y position
        width = int(ofRandom(20, 500));              // Get random width(height)
        bokeh_color.r = ofRandom(0, 255);            // Get random color (r)
        bokeh_color.g = ofRandom(0, 255);            // Get random color (g)
        bokeh_color.b = ofRandom(0, 255);            // Get random color (b)
    }
    
    void update(float alphaValue) {
        // Update the Bokeh's alpha value, so it can be responsive to input
        // (in our case, the magnitude of an FFT bin.
        bokeh_color.a = alphaValue * 255;
        
        // Resize the image. NOTE: we're doing this in the update function in
        // case we want to resize it later.
        bokeh_image.resize(width, width);
    }
    
    void draw() {
        // Set the current color
        ofSetColor(bokeh_color);
        
        // Draw the image at the right location
        bokeh_image.draw(y, x);
    }
private:
    // Member variables
    ofImage bokeh_image;     // Holds our "Bokeh" image data
    ofColor bokeh_color;     // Keep track of this bokeh's color
    int x, y, width, height; // Position member variables
};

class ofApp : public ofBaseApp{

 public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    ofMaterial material;
	
    void audioIn( float* input, int bufferSize, int nChannels);
    //void audioOut( float* output, int bufferSize, int nChannels);
    
	//current state of the rotation  
    ofQuaternion curRot;  
	
    //a place to store the mouse position so we can measure incremental change  
    ofVec2f lastMouse;
	
	//slows down the rotation 1 = 1 degree per pixel
	float dampen;
    
private:
    
    ofSoundStream sound_stream;
    
    // Vectors for our left- and right-channel waveforms
    vector<float> left, right;
    
    void plot(vector<float>& buffer, float scale);

    
    ofxFft* fft;
    float* fftData;
    
    // Number of Bokehs in this app
    const static int nBokeh = MY_BUFFERSIZE/4;
    // Our Bokeh instances
    Bokeh bokeh[nBokeh];
    
    ofEasyCam cam;
};
    
        








