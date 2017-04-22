#include "ofApp.h"

const int sampleRate = 44100;           //Sample rate del sonido
const float duracion = 7.0;            //duracion de la grabaci—n
const int N = duracion * sampleRate;    //Tama–o buffer

const float volume = 0.5;	//Amplitud del sonido

//Variables
vector<float> buffer;		//PCM buffer
int recPos = 0;				//Current recording position in the buffer
int playPos = 0;			//Current playing position in the buffer

int grabDisponible = 1;	//Is recording enabled
int repDisponible = 0;		//Is playing enabled




//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    ofBackground(255);
    
    // buttonStateUno = "digital pin:";
     //buttonStateDos = "digital pin:";
    //potValue = "analog pin:";
  

    
    ofSetColor(0);
       buffer.resize( N, 0.0 );
    
    //Start the sound output in stereo (2 channels)
    //and sound input in mono (1 channel)
    soundStream.setup( this, 2, 1, sampleRate, 256, 4 );
     audio.loadSound("paisaje.mp3");
    audio.setVolume(0.25f);
    las.loadImage("las.png");
   
    audio.setMultiPlay(TRUE);
    
    ard.connect("/dev/cu.usbmodem411", 57600);
    
    // listen for EInitialized notification. this indicates that
    // the arduino is ready to receive commands and it is safe to
    // call setupArduino()
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    font.loadFont("franklinGothic.otf", 20);
    smallFont.loadFont("franklinGothic.otf", 14);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();
    updateArduino();
	
}
void ofApp::setupArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(3, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    
    // set pin D13 as digital output
    ard.sendDigitalPinMode(13, ARD_OUTPUT);
    // set pin A4 as digital output
    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older
    
    // set pin D11 as PWM (analog output)
    ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
    
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

//--------------------------------------------------------------
void ofApp::updateArduino(){
    
    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();
    
    // do not send anything until the arduino has been set up
   
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    
    buttonStateUno = "Pin digital: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
    buttonStateDos = "Pin digital: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
    
    
    int uno = ard.getDigital(3);
    
    int dos = ard.getDigital(2);
    
    if (uno == 1){
      grabDisponible = 1; repDisponible = 0;
    }
    if (dos == 1){
       grabDisponible = 0; repDisponible = 1;    }

    
    
    


}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
   // potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    
    
    
    
    audio.setLoop(TRUE);
    
    ofEnableAlphaBlending();
   
    
    ofSetColor(0);
    
    if (!bSetupArduino){
        ofDrawBitmapString("Preparando Arduino...\n", 10, 400);
    } else {
        ofDrawBitmapString(potValue + "\n" + buttonStateDos +
                        "\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 10, 450);
        
      
        
        
    }
    
    

    
    
   
    
    
    int w = 1024;	//Width of the sound visualization picture
    
    ofSetLineWidth(4);
   
    //Graph of the sound buffer
    ofSetColor( 0, 0, 0 );
    for (int x=0; x<w; x++) {
        int i = float(x) * N / w;   //Convert x to buffer's position
        ofLine( x, 100, x, 100 - buffer[i] * 100 );
    }
    las.draw(10,200, 483, 126);
    
    ofSetColor(0);
    ofDrawBitmapString("Presiona la letra a para comenzar a reproducir el audio.", 10,500);
    ofDrawBitmapString("Presiona la letra s para activar el motor.", 10,550);

    ofDrawBitmapString("Jose Luis Santorcuato Tapia.Chile.2015.", 10,748);
    
    
  
    
    
    
    
   
   
}
void ofApp::audioReceived(
                            float *input, int bufferSize, int nChannels )
{
    //If recording is enabled by the user,
    //then store received data
    if ( grabDisponible ) {
        for (int i=0; i<bufferSize; i++) {
            buffer[ recPos ] = input[i];
            recPos++;
            //When the end of buffer is reached, recPos sets
            //to 0, so we record sound in a loop
            recPos %= N;
        }
    }
    
}

//--------------------------------------------------------------
//Audio output
void ofApp::audioOut(
                       float *output, int bufferSize, int nChannels)
{
    //If playing is enabled by the user, then do output sound
    if ( repDisponible ) {
       
        
        for (int i=0; i<bufferSize; i++) {
            output[ 2*i ] = output[ 2*i+1 ]
            = buffer[ playPos ] * volume;
            playPos++;
            //When the end of buffer is reached, playPos sets
            //to 0, so we hear looped sound
            playPos %= N;
        }
    }
   
  
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if ( key == 'a' ) {
        audio.play();
    
    }
    
    
    if ( key == 's' ) {
       
      ard.sendPwm(11, 64 );   // pwm...
    }
    

    //Enable recording mode
    if ( key == '1' ) {
        grabDisponible = 1; repDisponible = 0;
        
    }
    
    //Enable playing mode
    if ( key == '2' ) {
        grabDisponible = 0; repDisponible = 1;
       
    }
    
    //Save screen image to the file
    
    switch (key) {
        case OF_KEY_RIGHT:
            // rotate servo head to 180 degrees
            ard.sendServo(9, 180, false);
            ard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        case OF_KEY_LEFT:
            // rotate servo head to 0 degrees
            ard.sendServo(9, 0, false);
            ard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        default:
            break;
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ard.sendDigital(13, ARD_HIGH);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ard.sendDigital(13, ARD_LOW);


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
