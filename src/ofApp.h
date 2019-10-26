#include "ofMain.h"
#include "ofxOsc.h"

#define OSC_ADDRESS "127.0.0.1"
#define OSC_PORT 2323

struct ofApp : public ofBaseApp {

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();

    void sendOsc(int slot, int value);

    char   * text;
    int    width;
    int    height;
    float  mousePct;
    int    screenWidth;
    int    screenHeight;
    int    xlen;
    int    ylen;
    float  fader;

    int    threshold;
    int    thresholdKey;

    // grid
    int    gridData[256];
    int    lastGridData[256];
    float  gridHistory[256];
    bool   grid[256];
    int    gridWidth;
    int    gridHeight;
    int    gridThreshold;

    unsigned char * dataGray;
    unsigned char * dataGrayCurrent;
    unsigned char * dataGrayPast;
    unsigned char * dataGrayDiff;
    unsigned char * dataGrayThreshold;

    ofVideoGrabber vidGrabber;
    ofTexture      vidTexture;
    ofxOscSender   oscSender;
};
