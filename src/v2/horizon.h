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
	horizon(string in, int n, int rs, int pos);

private:
	int detect(string input, int n, int rs, int pos);
	Vec3f getMu(int n, vector<Vec3f> x);
	Mat getCov(int n, vector<Vec3f> x);
	int getHorizon(int n, Mat mat);
};

#endif