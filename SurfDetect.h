/*
 * SurfDetect.h
 *
 *  Created on: Sep 23, 2014
 *      Author: sulman
 */

#ifndef SURFDETECT_H_
#define SURFDETECT_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>

class SurfDetect {
private:
	int iMinHessian;
	cv::SurfDescriptorExtractor extractor;
public:
	SurfDetect();
	void getDescriptor(const cv::Mat mFrame, std::vector<cv::KeyPoint> &kp,
			cv::Mat &mDescription);
	std::vector<cv::DMatch> getMatches(const cv::Mat mDescript1,
			const cv::Mat mDescript2, const float minDist);
};

#endif /* SURFDETECT_H_ */
