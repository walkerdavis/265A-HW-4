/*256A HW 4 Interactive Audio Visual
 *Walker Davis (again with much help from Mark Hertensteiner and Tim O'Brien
 *This project places the user at the top of digital globe which
 *he or she can move about by clicking and dragging.  The ring around the planet
 *reacts to the left audio buffer.
 *
 *I borrowed alot from the below example(I have kept the title/subtitle.
 *I have also borrowed significantly from Tim O'Brien's "Bokas" example, thankx!
 */






/**
 * Quaternion Example for rotating a sphere as an arcball
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofNoFill();
    ofSetVerticalSync(true);
	ofSetFrameRate(30); //was orignally 60 for the fft ea
	ofBackground(0);
    
    int bufferSize = 256;
    
    
    // Enable additive blending, which makes our overlapping Bokehs look nice
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    sound_stream.setup(this, 2, 2, 44100, bufferSize, 4);
    
    // Resize and initialize left and right (audio) buffers...
    left.resize( bufferSize, 0 );
    right.resize( bufferSize, 0 );
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    
    fftData = new float[fft->getBinSize()];

    //    fft.setup(16384);

	//this slows down the rotate a little bit
	dampen = .1;
}




//--------------------------------------------------------------
void ofApp::update(){
    
    // Normalize the left channel waveform
    float maxValue = 0;
    for(int i = 0; i < left.size(); i++) {
        if(abs(left[i]) > maxValue) { maxValue = abs(left[i]); }
    }
    for(int i = 0; i < left.size(); i++) { left[i] /= maxValue; }
    
    // Take the FFT of the left channel
    fft->setSignal(&left[0]);
    
    // Get the magnitudes and copy them to audioBins
    float* fftData = fft->getAmplitude();
    for (int i = 0; i< fft->getBinSize(); i++) {
        fftData[i] = fft->getAmplitudeAtBin((float)i);
    }
    fftData = fft->getAmplitude();
    
    // Now normalize the FFT magnitude values
    maxValue = 0;
    for(int i = 0; i < fft->getBinSize(); i++) {
        if(abs(fftData[i]) > maxValue) { maxValue = abs(fftData[i]); }
    }
    for(int i = 0; i < fft->getBinSize(); i++) { fftData[i] /= maxValue; }
    
    // Update the bokeh with its amplitude value (taken from an FFT bin)
//    for(int i=0; i<nBokeh; i++){
//        bokeh[i].update( fftData[int( i * fft->getBinSize() / nBokeh )] );
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    //cam.begin();
    
    
    ofBackground(0, 0, 0);
    ofPushMatrix();
        //ofTranslate(0,0,-20);
        for(int i=0; i<nBokeh; i++){
            bokeh[i].draw();
        }
    ofPopMatrix();
   
    
        //home planet
        //translate so that 0,0 is the center of the screen
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()*1.6, 40);
        //Extract the rotation from the current rotation
        ofVec3f axis;
        ofVec3f faceNormal;
        float angle;
        curRot.getRotate(angle, axis);  
        
        //apply the quaternion's rotation to the viewport and draw the sphere
        ofRotate(angle, axis.x, axis.y, axis.z);
        ofSetColor(0, 150, 255, 95);
        ofDrawSphere(0, 0, 0, 800);
        //ofEnableAlphaBlending();
        ofPushMatrix();

    
    
    
    
    
        //moon 1
        ofPushMatrix();
        ofSetColor(200, 256, 256, 256);
        //ofTranslate(ofGetWidth()/2, ofGetHeight()/6, 80);
        ofTranslate(ofGetHeight()*1.4*sin(angle/180*PI),-ofGetHeight()*7*cos(angle/180*PI), 50);
        //ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);
        ofBeginShape();
        float x, y, z, r, theta, phi;
        for (int i=0; i < fft->getBinSize(); i++ ){
            r = 50;
            phi = 25*PI*i/float(left.size());
            theta = PI*i/float(left.size());
            r += left[i];
            x = r * std::sin(theta) * std::cos(phi);
            y = r * std::sin(theta) * std::sin(phi);
            z = r * std::cos(theta);
            ofVertex(x, y, z);
        }
        
        ofEndShape();
        
        ofPopMatrix();
    
    
    
    
    
    
        //moon 2
        ofPushMatrix();
        ofSetColor(256, 0, 170);
        //ofTranslate(ofGetWidth()/2, ofGetHeight()/6, 80);
        ofTranslate(ofGetHeight()*1.4*sin(angle/180*PI),ofGetHeight()*7*cos(angle/180*PI), 50);
        //ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);

    
        ofBeginShape();
        float xb, yb, zb, rb, thetab, phib;
        for (int i=0; i < fft->getBinSize(); i++ ){
            rb = 150;
            phib = 36*PI*i/float(left.size());
            thetab = 2*PI*i/float(left.size());
            rb += left[i];
            xb = rb * std::sin(thetab) * std::cos(phib);
            yb = rb * std::sin(thetab) * std::sin(phib);
            zb = rb * std::cos(thetab);
            ofVertex(xb, yb, zb);
        }
        
        ofEndShape();
    
        ofPopMatrix();
    
    
    
    
    
        //moon 3
        ofPushMatrix();
        ofSetColor(256, 1, 20);
        ofTranslate(-ofGetHeight()*7.7*sin(angle/180*PI),ofGetHeight()*1.2*cos(angle/180*PI), 20);
        ofDrawSphere(0, 0, 0, 600);
        ofPopMatrix();
    
    
    
    
        //audio buffer ring
    
        for (int j = 0; j<1000; j++){
    
        //ofPushMatrix();
            float xAngle = ofRandom(-1, 100);
            float yAngle = ofRandom(0, 200/(left.size()));
//            float zAngle = ofRandom(0, 360);    //switching these two is cool
            float zAngle = 20;                //
            ofRotateX(xAngle);
            ofRotateY(yAngle);
            
//            ofRotateZ(zAngle);
            
//             for (int i=0; i < fft->getBinSize(); i++ ){
//                 
//                 ofRotateX(binAmps[i]*2-1);
//             }
//            
            
            
            ofPushMatrix();
                ofSetColor(ofRandom(250,300), ofRandom(0,150), ofRandom(50,60));
                //ofTranslate(ofGetWidth()/2, ofGetHeight()/6, 80);
                //ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);
                ofTranslate(ofGetHeight()*1.4*sin(zAngle/180*PI),-ofGetHeight()*7*cos(zAngle/180*PI), 40);
 
                //ofTranslate(0,-ofGetHeight()*1.4*cos(xAngle/180*PI), ofGetHeight()*1.4*sin(xAngle/180*PI));
                //ofTranslate(ofGetHeight()*1.4*sin(zAngle/180*PI),-ofGetHeight()*1.4*cos(zAngle/180*PI), 40);
            
                ofBeginShape();
                float x, y, z, r, theta, phi;
                for (int i=0; i < left.size(); i++ ){
                //ofPushMatrix();
                //ofRotateZ(zAngle);
                //ofPushMatrix();

                //ofTranslate(ofGetHeight()*1.4*sin(zAngle*i/180*PI),-ofGetHeight()*1.4*cos(zAngle*i/180*PI), binAmps[i]*10);
                //0, 0, binAmps[i]*10);
                    r = 1;
                    phi = 18*PI*i/float(left.size());
                    theta = PI*i/float(left.size());
                    r += (left[i])*500;
                    x = r * std::sin(theta) * std::cos(phi);
                    y = r * std::sin(theta) * std::sin(phi);
                    z = r * std::cos(theta);
                    ofVertex(x, y, z);
                }
                ofEndShape();
            ofPopMatrix();
        }
        ofPopMatrix();
    ofPopMatrix();

    
    
    
    ofPushMatrix();
    ofEnableAlphaBlending();
    
    string msg = ofToString((int) ofGetFrameRate()) + " fps";
    ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
    //ofDrawBitmapString(msg, ofGetWidth(), ofGetHeight());
    
    ofPopMatrix();
    
    //cam.end();
}


//--------------------------------------------------------------

void ofApp::plot(vector<float>& buffer, float scale) {
    ofNoFill();
    int n = MIN(1024, buffer.size());
    //ofDrawRectangle(0 + ofGetWidth(), 0 + ofGetHeight(), n, scale);
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight());
    ofScale(1, -ofGetHeight()*.5);
    ofBeginShape();
    for (int i = 0; i < n; i++) {
        ofVertex(i, buffer[i]);
    }
    ofEndShape();
    ofPopMatrix();
}


//--------------------------------------------------------------
// audioIn() -- deal with incoming audio buffer
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // Write incoming audio to buffer. Note: samples are interleaved.
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }
 //   fft->setSignal(input);
//    for (int i = 0; i< fft->getBinSize(); i++) {
//        binAmps[i] = fft->getAmplitudeAtBin(i);
//    }
    
}


    
    
    


//--------------------------------------------------------------

void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
	//every time the mouse is dragged, track the change
	//accumulate the changes inside of curRot through multiplication
    ofVec2f mouse(x,y);  
    ofQuaternion yRot((x-lastMouse.x)*dampen, ofVec3f(0,1,0));  
    ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));  
    curRot *= yRot*xRot;  
    lastMouse = mouse;  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //store the last mouse point when it's first pressed to prevent popping
	lastMouse = ofVec2f(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
