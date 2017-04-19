#include "ofMain.h"
#include "ofApp.h"
#include <bricklelib.h>

//========================================================================
int main( ){
	Node rRoot(ContentType::Root);
	Node&rSprite=rRoot.addChildNode(ContentType::Sprite);
	{
		Node&rSprite=rRoot.addChildNode(ContentType::Sprite);
		Node&rSpriteSub=rSprite.addChildNode(ContentType::Sprite);
	}
	string buf;
	rRoot.serialize(buf,0);
	cout <<buf <<endl;

	NodeParser parser;
	Node* readedNode=parser.parseFile("data/test.json");
	if (readedNode!=nullptr) {
		string bufx;
		readedNode->serialize(bufx,0);
		cout <<bufx <<endl;
		delete readedNode;
	}

    ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}
