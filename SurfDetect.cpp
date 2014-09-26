/*
 * SurfDetect.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: sulman
 */

#include "SurfDetect.h"

SurfDetect::SurfDetect() {
	iMinHessian = 400;
}

void SurfDetect::getDescriptor(cv::Mat mFrame, std::vector<cv::KeyPoint> &kp,
		cv::Mat &mDescription) {
	cv::SurfFeatureDetector detector(iMinHessian);
	detector.detect(mFrame, kp);
	extractor.compute(mFrame, kp, mDescription);
}

std::vector<cv::DMatch> SurfDetect::getMatches(const cv::Mat mDescript1,
		const cv::Mat mDescript2, const float minDist) {
	cv::BFMatcher matcher(cv::NORM_L2);
	std::vector<cv::DMatch> matches;
	matcher.match(mDescript1, mDescript2, matches);
	double max_dist = 0;
	double min_dist = 100;
	for (int i = 0; i < mDescript2.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}
	std::vector<cv::DMatch> good_matches;

	for (int i = 0; i < mDescript2.rows; i++) {
		if (matches[i].distance <= minDist) {
			good_matches.push_back(matches[i]);
		}
	}
	/*for (unsigned int i = 0; i < good_matches.size(); i++) {
		std::cout << good_matches[i].distance << std::endl;
	}*/
	return good_matches;
}

