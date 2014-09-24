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
#include "SurfDetect.h"

using namespace std;
using namespace cv;

const string csWINDOW_OUTPUT = "OUTPUT VIDEO";

void help(char** argv);

int main(int argc, char** argv) {
	if (argc != 3) {
		cerr << "Invalid Number of parameters" << endl;
		cerr << "EXITING WITH ERROR" << endl << endl;
		help(argv);
		return -1;
	}
	VideoCapture VCcap(argv[2]);
	if (!VCcap.isOpened()) {
		cerr << "-----" << argv[2] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	}
	Mat mImageInput = imread(argv[1]);
	if (mImageInput.empty()) {
		cerr << "-----" << argv[1] << "-----\nEXITING WITH ERROR" << endl
				<< endl;
		help(argv);
		return -1;
	}

	vector<KeyPoint> kp1;
	Mat mTDescript;
	SurfDetect sfd;
	sfd.getDescriptor(mImageInput, kp1, mTDescript);

	namedWindow(csWINDOW_OUTPUT, CV_WINDOW_AUTOSIZE);
	Mat mFrameInput;	// Frame from input video

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
		vector<DMatch> matches;
		matches = sfd.getMatches(mQDescript, mTDescript);
		Mat img_matches;
		drawMatches(mFrameInput, kp2, mImageInput, kp1, matches, img_matches,
				Scalar::all(-1), Scalar::all(-1), vector<char>(),
				DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		if (matches.size() > 0) {
			Rect x = Rect(
					Point(kp2[matches[0].queryIdx].pt.x - 25,
							kp2[matches[0].queryIdx].pt.y - 25), Size(70, 70));
			rectangle(mFrameInput, x, Scalar(255, 0, 255), 2, 8, 0);
		}
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
	cout << "./" << argv[0] << " PATH_TO_TEST_IMAGE PATH_TO_VIDEO_SEQUENCE"
			<< endl << endl;
}
