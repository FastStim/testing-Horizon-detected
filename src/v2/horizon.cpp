#include "horizon.h"

horizon::horizon(string in, int n, int rs, int pos)
{
	int error = detect(in, n, rs, pos);

	if (error == -1)
	{
		cerr << "\033[1;31mERROR!\033[0m Wrong input file!" << endl;
	}
}

int horizon::detect(string in, int n, int rs, int pos)
{
	Mat frame;
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

	cap.set(CAP_PROP_POS_FRAMES, pos);
	while(waitKey(speed) != 27)
	{
		cap.read(frame);
		if (frame.empty())
			break;

		Mat rSize, gray;
		resize(frame, rSize, Size(width/rs,height/rs));
		GaussianBlur(rSize, rSize, Size(3, 3), 5);

		int h = getHorizon(n, rSize);

		line(frame, Point(0,h*rs), Point(frame.cols,h*rs),Scalar(0,0,255), 1);

		imshow("File: " + in + " View", frame);
	}
	
	return 0;
}

/**
 * @brief Функция получения мат. ожидания (Мю)
 * 
 * @param n Количество пикселей в блоке
 * @param x Вектор пикселей
 * 
 * @return Вектор мат. ожидания по цветам
 */
Vec3f horizon::getMu(int n, vector<Vec3f> x)
{
	Vec3f mu = {(float)1/n, (float)1/n, (float)1/n};
	Vec3f s = 0;
	for (int i = 0; i < n;  i++)
		s += x[i];

	mu[0] = mu[0] * s[0];
	mu[1] = mu[1] * s[1];
	mu[2] = mu[2] * s[2];

	return mu;
}

/**
 * @brief Функция получения ковариционной матрицы
 * 
 * @param n Количество пикселей в блоке
 * @param x Вектор пикселей
 * 
 * @return Ковариционная матрица
 */
Mat horizon::getCov(int n, vector<Vec3f> x)
{
	Mat cov;
	for (int i = 0; i * n < x.size(); i++)
	{
		int pos = i * n;
		vector<Vec3f> tX;
		if (pos + n < x.size())
			tX.assign(x.begin()+pos, x.begin()+pos+n);
		else
			tX.assign(x.begin()+pos, x.end());

		Vec3f mu = getMu(n, tX);	

		for (int j = pos; j < n && j < x.size(); j++)
		{
			Mat xMu = (Mat)x[j] - mu;
			Mat xMuT;
			transpose(xMu, xMuT);
			cov = cov + xMu*xMuT;
		}
	}

	return (float)1/(n-1) * cov;
}

/**
 * @brief Получение строки с горизонтом
 * 
 * @param n Количество пикселей в блоке
 * @param mat Матрица изображения
 * 
 * @return Высоту горизонта
 */
int horizon::getHorizon(int n, Mat mat)
{
	double j;
	int h = 0;
	for (int i = 12; i < mat.rows-12; i++)
	{
		// Заполняем вектора неба и земли
		vector<Vec3f> xS;
		vector<Vec3f> xG;
		for (int y = 0; y < mat.rows; y++)
		{
			for (int x = 0; x < mat.cols; x++)
			{
				if (y <= i)
					xS.push_back(mat.at<Vec3b>(Point(x,y)));
				else
					xG.push_back(mat.at<Vec3b>(Point(x,y)));
			}
		}

		Mat covS = getCov(n, xS);
		Mat covG = getCov(n, xG);
		
		double detS = determinant(covS);
		double detG = determinant(covG);
		// Условие связанное что угрублением решения при доработки до полного ситуация будет исчерпана
		if (detS != 0 && detG != 0)
		{
			double tJ = 1 / detS + detG;
			if (tJ > j)
			{
				j = tJ;
				h = i;
			}
		}
	}

	return h;
}