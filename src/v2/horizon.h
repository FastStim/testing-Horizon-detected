#ifndef HORIZON_H
#define HORIZON_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
using namespace cv;

class horizon
{
public:
	horizon(string in);

private:
	int detect(string input);
	Vec3f getMu(int y, int n, Mat mat);
	Mat getCov(int sPos, int n, Mat mat);
	int getHorizon(Mat mat);
};

#endif