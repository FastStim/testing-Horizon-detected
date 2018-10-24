#include "horizon.h"

horizon::horizon(string in)
{
	int error = get(in);

	if (error == -1)
	{
		cerr << "\033[1;31mERROR!\033[0m Wrong input file!" << endl;
	}
}

int horizon::get(string in)
{
	Mat frame, edge;
	VideoCapture cap;

	cap.open(in);
	if (!cap.isOpened())
	{
		return -1;		
	}

	int fps = (int) cap.get(5);
	int width = (int) cap.get(3);
    int height = (int) cap.get(4);
	int speed = 1000 / fps;

	while(waitKey(speed) != 27)
	{
		cap.read(frame);
		if (frame.empty())
			break;

		Mat gray, rSize;
		vector<Point> points;
		vector<Vec2f> lines;


		cvtColor(frame, gray, COLOR_BGR2GRAY);
		Canny(gray, edge, 100, 200, 3);
		HoughLines(edge, lines, 1, CV_PI / 180, width / 4);	

		for (unsigned int i = 0; i < lines.size(); ++i) {
		    float p = lines[i][0];
		    float theta = lines[i][1];
		    Point a(0, static_cast<int>(p / sin(theta)));
		    Point b(static_cast<int>(p / cos(theta)), 0);
		    line(frame, a, b, Scalar(0, 0, 255), 2);
		}

		imshow("File: " + in + " View", frame);
	}
	
	return 0;
}