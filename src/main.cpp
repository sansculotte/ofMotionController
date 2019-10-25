#include "ofMain.h"
#include "testApp.h"

//--------------------------------------------------------------
int main(){
	ofSetupOpenGL(320, 200, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
