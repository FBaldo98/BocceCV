#include<opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>


// Threshold values for colors in hsv
cv::Scalar THRESH_YELLOW_LOW = cv::Scalar(15, 210, 20);
cv::Scalar THRESH_YELLOW_UP = cv::Scalar(35, 255, 255);
cv::Scalar THRESH_GREEN_LOW = cv::Scalar(31, 125, 125);
cv::Scalar THRESH_GREEN_UP = cv::Scalar(75, 255, 255);
cv::Scalar THRESH_BLUE_LOW = cv::Scalar(100, 125, 125);
cv::Scalar THRESH_BLUE_UP = cv::Scalar(130, 255, 255);
cv::Scalar THRESH_RED_LOW = cv::Scalar(170, 125, 125);
cv::Scalar THRESH_RED_UP = cv::Scalar(179, 255, 255);

struct triple {
	int x;
	int y;
	int d;
};

/* ritorna un vector con tutte le immagini per ogni colore
   in questo modo è possibile elaborare ogni boccia
*/
std::vector<cv::Mat> colors(cv::Mat &original){
	std::vector<cv::Mat> vec(5);
	cv::Mat hsv;
	cv::Mat red, yellow, blue, green, black;

	cv::cvtColor(original, hsv, CV_BGR2HSV);

	cv::inRange(hsv, THRESH_YELLOW_LOW, THRESH_YELLOW_UP, yellow);
	cv::inRange(hsv, THRESH_BLUE_LOW, THRESH_BLUE_UP, blue);
	cv::inRange(hsv, THRESH_GREEN_LOW, THRESH_GREEN_UP, green);
	cv::inRange(hsv, THRESH_RED_LOW, THRESH_RED_UP, red);
	cv::inRange(hsv, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), black);

	vec.at(0) = yellow;
	vec.at(1) = blue;
	vec.at(2) = green;
	vec.at(3) = red;
	vec.at(4) = black;

	return vec;
}

std::vector<triple> find_centers(cv::Mat &boccino_img) {
	std::vector<triple> centers(2);
	cv::Mat blur, edges;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::GaussianBlur(boccino_img, blur, cv::Size(3, 3), 3);
	cv::findContours(blur, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		cv::Moments M = cv::moments((cv::Mat)contours[i]);
		int x = M.m10 / M.m00;
		int y = M.m01 / M.m00;
		centers.at(i).x = x;
		centers.at(i).y = y;
	}

	return centers;
}

int distance(int x1, int y1, int x2, int y2) {
	int dis;
	int xd = x1 - x2;
	int yd = y1 - y2;

	if (xd < 0)
		xd = -xd;
	if (yd < 0)
		yd = -yd;
	
	int temp = pow(xd, 2) + pow(yd, 2);
	dis = sqrt(temp);

	return dis;
}

int main() {
	cv::Mat original;
	std::vector<cv::Mat> bocce(5);
	std::vector<triple> yellow(2);
	std::vector<triple> blue(2);
	std::vector<triple> red(2);
	std::vector<triple> green(2);
	std::vector<triple> black(2);
	

	original = cv::imread("Images/bocce4.png", 1);
	cv::imshow("Test", original);
	cv::waitKey(0);

	bocce = colors(original);
	/*
	cv::imshow("Giallo", bocce.at(0));
	cv::imshow("Blu", bocce.at(1));
	cv::imshow("Verde", bocce.at(2));
	cv::imshow("Rosso", bocce.at(3));
	cv::imshow("Boccino", bocce.at(4));
	*/
	cv::waitKey(0);

	yellow = find_centers(bocce.at(0));
	blue = find_centers(bocce.at(1));
	green = find_centers(bocce.at(2));
	red = find_centers(bocce.at(3));
	black = find_centers(bocce.at(4));
	std::cout << "Boccino: " << black.at(0).x << " " << blac.at(0).y << std::endl;
	std::cout << "Gialli: " << yellow.at(0).x << " " << yellow.at(0).y << std::endl;

	cv::imshow("Test", original);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

