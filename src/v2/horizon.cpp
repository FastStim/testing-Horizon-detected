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

		Mat rSize;
		resize(frame, rSize, Size(width/10,height/10));
		GaussianBlur(rSize, rSize, Size(3, 3), 5);

		int h = getHorizon(rSize);

		line(frame, Point(0,h*10), Point(frame.cols,h*10),Scalar(0,0,255));

		imshow("File: " + in + " View", frame);
		// imshow("File: " + in + " View", rSize);
	}
	
	return 0;
}

/**
 * @brief Функция получения мат. ожидания (Мю)
 * 
 * @param y Строка для которой получаем
 * @param n Количество столбцов
 * @param mat Матрица изображения
 * 
 * @return Вектор мат. ожидания по цветам
 */
Vec3f horizon::getMu(int y, int n, Mat mat)
{
	Vec3f mu = {(float)1/n, (float)1/n, (float)1/n};
	Vec3f s = 0;
	for (int x = 0; x < n;  x++)
	{
		// cout << "mat " << mat.at<Vec3b>(x,y) << " x = " << x << " y = " << y << " s = " << s << endl;
		s += mat.at<Vec3b>(x,y);
	}

	mu[0] = mu[0] * s[0];
	mu[1] = mu[1] * s[1];
	mu[2] = mu[2] * s[2];
	return mu;
}

/**
 * @brief Функция получения ковариционной матрицы
 * 
 * @param sPos Позиция начала матрицы Неба или Земли
 * @param n Количество строк
 * @param mat Матрица изображения
 * 
 * @return Ковариционная матрица
 */
Mat horizon::getCov(int sPos, int n, Mat mat)
{
	Mat cov;
	for (int x = 0; x < n; x++)
	{
		int y = x + sPos;
		Vec3f mu = getMu(y, mat.cols, mat);
		Mat xMu = (Mat)((Vec3f)mat.at<Vec3b>(x,y) - mu);
		Mat xMuT;
		transpose(xMu, xMuT);
		cov = cov + xMu*xMuT;
	}

	return (float)1/(n-1) * cov;
}

/**
 * @brief Получение строки с горизонтом
 * 
 * @param mat Матрица изображения
 * @return Высоту горизонта
 */
int horizon::getHorizon(Mat mat)
{
	double j = 0;
	int h = 0;
	int offset = 2;
	for (int y = 0 + offset; y < mat.rows-offset; y++)
	{
		Mat covS = getCov(0, y, mat);
		Mat covG = getCov(y, mat.rows - y, mat);

		double tJ = 1 / (determinant(covS) + determinant(covG));

		if (tJ > j)
		{
			j = tJ;
			h = y;
		}
	}

	return h;
}