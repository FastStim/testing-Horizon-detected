#include "horizon.h"

horizon::horizon(string in)
{
	int error = detect(in);

	if (error == -1)
	{
		cerr << "\033[1;31mERROR!\033[0m Wrong input file!" << endl;
	}
}

int horizon::detect(string in)
{
	int n = 36;
	double mu = 0;

	Mat frame, gray, edge;
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

		getMu(n, frame);
		// GaussianBlur(frame, frame, Size(3, 3), 7);
		// cvtColor(frame, gray, COLOR_BGR2GRAY);
		// Canny(frame, edge, 100, 300, 3);

		imshow("File: " + in + " View", frame);
	}
	
	return 0;
}

Vec3f horizon::getMu(int n, Mat mat)
// Получение Мю (Фактически мат ожидания)
// Переделать
{
	Vec3f mu = {(float)1/n, (float)1/n, (float)1/n};
	Vec3f s;
	for (int y = 0; y < mat.rows; y++)
		for (int x = 0; x < mat.cols;  x++)
		{
			s += mat.at<Vec3f>(Point(x,y));
		}

	mu[0] = mu[0] * s[0];
	mu[1] = mu[1] * s[1];
	mu[2] = mu[2] * s[2];

	return mu;
}

Vec3f horizon::getCov(int)