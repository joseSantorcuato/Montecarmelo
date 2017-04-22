#pragma once

#include "ofMain.h"
#include "ofEvents.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioReceived( float *input, int bufferSize, int nChannels );
    
    
        void audioOut( float *output, int bufferSize, int nChannels );
    
       ofTrueTypeFont		font;
       ofTrueTypeFont      smallFont;
        ofSoundStream soundStream;
        ofImage las;
        ofSoundPlayer audio;
        ofArduino	ard;
        bool		bSetupArduino;			// flag variable for setting up arduino once
    
        private:
    
        void setupArduino(const int & version);
        void digitalPinChanged(const int & pinNum);
        void analogPinChanged(const int & pinNum);
        void updateArduino();
    
        string buttonStateUno;
        string buttonStateDos;
        string potValue;
    
    
		
};
