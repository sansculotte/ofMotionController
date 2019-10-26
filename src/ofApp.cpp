#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	

    // basic values
    ofSeedRandom();
    width = 320;
    height = 200;

    screenWidth  = 320;
    screenHeight = 200;

    gridWidth  = 5;
    gridHeight = 5;
    //gridThreshold = 10;

    fader		= 1.618033;	// fade with PHI

    xlen = (int) screenWidth / gridWidth;
    ylen = (int) screenHeight / gridHeight;

    // komplexere datenstrukturen, groessen initialisieren
    dataGrayCurrent 	= new unsigned char[width*height]; // grarray
    dataGrayPast		= new unsigned char[width*height]; // grarray
    dataGrayDiff		= new unsigned char[width*height]; // grarray
    dataGrayThreshold	= new unsigned char[width*height]; // grarray

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(width, height);
    vidTexture.allocate(width, height, GL_RGB);

    oscSender.setup(OSC_ADDRESS, OSC_PORT);

}


//--------------------------------------------------------------
void ofApp::update(){

    int channels = 3;
    int totalBytes = width * height * channels;
    int counter = 0;

    // set background black
    ofBackground(0,0,0);

    vidGrabber.update();

    // motion dedection action
    if(vidGrabber.isFrameNew()) {

        ofPixels pixels = vidGrabber.getPixels();

        // convert to gray with supersimple-average-all-channels geayscale converter
        for (int i = 0; i < totalBytes; i += channels){
            int avg = 0;
            for (int curByte =0;curByte < 3; curByte++) {
                avg += pixels[i + curByte];
            }
            avg = (avg / 3);
            dataGrayCurrent[counter] = avg;
            counter++;
        }


        // get the difference between this frame and the last frame
        for (int i = 0; i < width*height; i++){
            dataGrayDiff[i] = abs(dataGrayCurrent[i] - dataGrayPast[i]);
        }

        //fade grid
        for (int i = 0; i < gridHeight * gridWidth; i++) {
            if (gridData[i] > 0) gridData[i] = gridData[i] / fader;
        }

        // threshold the motion data
        int threshold = (int) (mousePct * 255);
        //int threshold = (int) (thresholdKey * 2);
        int row = 0;
        int col = 0;

        // weigh gridData Elements against threshold
        for (int i=0; i<height; i++){
            for (int j=0; j<width; j++){
                row = (int) ((height - i) * gridHeight / height);
                col = (int) ((width - j) * gridWidth / width);

                if(dataGrayDiff[width * i + j] < threshold) {
                    dataGrayThreshold[width * i + j] = 0;
                }
                else {
                    dataGrayThreshold[width * i + j] = 255;
                    gridData[row * gridWidth + col]++;
                    //diffSum++;
                }
            }
        }

        vidTexture.loadData(dataGrayThreshold, width, height, GL_LUMINANCE);

        // calculate mean to dedect sudden movements
        //diffMean     = diffSum / width * height;
        //historyMean  = (lastDiffMean + diffMean) / 2;
        //lastDiffMean = diffMean;
    }

    //update dataGrayPast
    memcpy(dataGrayPast, dataGrayCurrent, width*height);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetupScreen();

    int row = 0;
    int xpos = 0;
    int ypos = screenHeight;

    //	vidTexture.draw(360,20,width,height);


    for (int i = 0; i < gridHeight * gridWidth; i++ ) {
        if (i % gridWidth == 0) row++;
        //		cout << (xlen * ylen * (movementThreshold / 100.0f)) << " " << endl;
        if (gridData[i] > 0 ) {
            //cout << i << " - " << gridData[i] << " " << endl;
            xpos = (int) (i % gridWidth) * xlen;
            ypos = (int) (screenHeight - row * ylen);
            ofSetColor(0, 0, gridData[i] / 2);
            ofRect( xpos, ypos, xlen, ylen);
        }
        sendOsc(i, gridData[i]);
    }

    //	ofSetColor(0xffffff);
    //vidGrabber.draw(20,20);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    // in fullscreen mode, on a pc at least, the
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...

    if (key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
    if (key == 't') {
        // just ignore ... this is the test buttin ;)
        // OSC stuff
        sendOsc(1, 3);
    }
    if (key == '+' && threshold <= 100){
        thresholdKey += 1;
    }
    if (key == '-' && threshold > 0){
        thresholdKey -= 1;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousePct = (float) x / (float) screenWidth;
    //cout << mousePct << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(){

}

//-------------------------------------------------------------------------------------------------------
void ofApp::sendOsc (int slot, int value) {

    // osc stuff
    ofxOscMessage msg;
    //		UdpTransmitSocket transmitSocket( IpEndpointName( ADDRESS, PORT ) );

    msg.setAddress("/ctrl");
    msg.addIntArg(slot);
    msg.addIntArg(value);
    oscSender.sendMessage(msg);
    //		p << osc::BeginBundleImmediate
    //			p << osc::BeginMessage( name ) << value << osc::EndMessage;
    //		<< osc::EndBundle;
    //		transmitSocket.Send( p.Data(), p.Size() );
}
