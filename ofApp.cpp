#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();

	this->cap.open(0);
	this->cap.set(cv::CAP_PROP_FPS, 30);
	this->cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->cap >> this->frame;
	cv::flip(this->frame, this->frame, -1);
	cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2RGB);

	this->mesh.clear();

	int span = 1;
	for (int x = 0; x < this->frame.cols; x += span) {

		for (int y = 0; y < this->frame.rows; y += span) {

			glm::vec3 vertex = glm::vec3(x, y, 0) - glm::vec3(320, 180, 0);
			auto angle = ofMap(ofNoise(vertex.y * 0.005 + ofGetFrameNum() * 0.05), 0, 1, -PI, PI);
			auto rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
			vertex = glm::vec4(vertex, 0) * rotation;

			cv::Vec3b tmp = this->frame.at<cv::Vec3b>(y, x);
			auto color = ofColor(tmp[0], tmp[1], tmp[2]);

			this->mesh.addVertex(vertex);
			this->mesh.addColor(color);
		}
	}

	int cols = this->frame.rows / span;
	int rows = this->frame.cols / span;

	for (int col = 1; col < cols; col++) {

		for (int row = 1; row < rows; row++) {

			this->mesh.addIndex(col + row * cols);
			this->mesh.addIndex((col - 1) + (row - 1) * cols);
			this->mesh.addIndex((col - 1) + row * cols);

			this->mesh.addIndex(col + row * cols);
			this->mesh.addIndex((col - 1) + (row - 1) * cols);
			this->mesh.addIndex(col + (row - 1) * cols);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->mesh.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}