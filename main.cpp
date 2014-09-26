/*
 * haar.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: sulman
 */
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <fstream>
#include <string>
#include "SurfDetect.h"

using namespace std;
using namespace cv;

const string csWINDOW_OUTPUT = "OUTPUT VIDEO";
cv::Scalar scalarArray[5];

void help(char** argv);

int main(int argc, char** argv) {
	if (argc != 5) {
		cerr << "Invalid Number of parameters" << endl;
		cerr << "EXITING WITH ERROR" << endl << endl;
		help(argv);
		return -1;
	}

	string line;
	ifstream myfile(argv[1]);
	vector<Mat> DataArray;
	if (!myfile.is_open()) {
		cerr << "-----" << argv[1] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	} else {
		while (getline(myfile, line)) {
			Mat x = imread(line);
			if (x.empty()) {
				cerr << "Invalid text\nEXITING WITH ERROR" << endl;
				return -1;
			}
			DataArray.push_back(x);
		}
	}

	ifstream mythreshold(argv[2]);
	vector<float> ThreshArray;
	if (!mythreshold.is_open()) {
		cerr << "-----" << argv[2] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	} else {
		while (getline(mythreshold, line)) {
			string::size_type sz;
			float x = atof(line.c_str());
			if (x == 0.0) {
				cerr << "Invalid text\nEXITING WITH ERROR" << endl;
				return -1;
			}
			ThreshArray.push_back(x);
		}
	}
	/*for (unsigned int i = 0; i < DataArray.size(); i++)
	 cout << DataArray[i] << endl;*/

	VideoCapture VCcap(argv[3]);
	if (!VCcap.isOpened()) {
		cerr << "-----" << argv[3] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	}

	Size S = Size((int) VCcap.get(CV_CAP_PROP_FRAME_WIDTH), // Acquire input size
	(int) VCcap.get(CV_CAP_PROP_FRAME_HEIGHT));

	VideoWriter outputVideo;
	outputVideo.open(argv[4], CV_FOURCC('M', 'J', 'P', 'G'),
			VCcap.get(CV_CAP_PROP_FPS), S, true);
	if (!outputVideo.isOpened()) {
		cerr << "-----" << argv[4] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	}

	//vector<vector<KeyPoint>> kp;
	vector<Mat> mTDescript;
	SurfDetect sfd;

	for (unsigned int i = 0; i < DataArray.size(); i++) {
		vector<KeyPoint> kp1;
		Mat mTDescript1;
		sfd.getDescriptor(DataArray[i], kp1, mTDescript1);
		mTDescript.push_back(mTDescript1);
		//kp.push_back(kp1);
	}

	namedWindow(csWINDOW_OUTPUT, CV_WINDOW_AUTOSIZE);
	Mat mFrameInput;	// Frame from input video

	scalarArray[0] = Scalar(255,255,255);
	scalarArray[1] = Scalar(0,0,0);
	scalarArray[2] = Scalar(255,0,0);
	scalarArray[3] = Scalar(0,255,0);
	scalarArray[4] = Scalar(0,0,255);

	while (true) {
		VCcap >> mFrameInput;
		if (mFrameInput.empty()) {
			cout << "Video End" << endl;
			VCcap.open("/home/sulman/Documents/rawData/carTrack/highway.avi");
			continue;
		}
		vector<KeyPoint> kp2;
		Mat mQDescript;
		sfd.getDescriptor(mFrameInput, kp2, mQDescript);
		vector<DMatch> matches1;
		for (unsigned int i = 0; i < mTDescript.size(); i++) {
			matches1 = sfd.getMatches(mQDescript, mTDescript[i], ThreshArray[i]);
			if (matches1.size() > 0) {
				Rect x = Rect(
						Point(kp2[matches1[0].queryIdx].pt.x - 25,
								kp2[matches1[0].queryIdx].pt.y - 25),
						Size(70, 70));
				rectangle(mFrameInput, x, scalarArray[i], 2, 8, 0);
			}
		}
		outputVideo << mFrameInput;
		imshow(csWINDOW_OUTPUT, mFrameInput);
		int key = waitKey(30);
		if (key > 0) {
			cout << "Video Stopped.-----Exiting Program" << endl;
			break;
		}
	}

	return 0;
}

void help(char** argv) {
	cout << "Usage:" << endl;
	cout << "./" << argv[0]
			<< " PATH+NAME_TO_TEST_IMAGE PATH+NAME_TO_TEST_IMAGE PATH+NAME_TO_VIDEO_SEQUENCE PATH+NAME_TO_SAVE_VIDEO_SEQUENCE"
			<< endl << endl;
}
