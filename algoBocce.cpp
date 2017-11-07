#include "algoBocce.h"

// Valori di soglia dei colori in hsv
// In un'applicazione reale non funzionerebbero. Necessaria calibrazione e set colori
cv::Scalar THRESH_YELLOW[2] = { cv::Scalar(15, 210, 20),  cv::Scalar(35, 255, 255) };
cv::Scalar THRESH_GREEN[2] = { cv::Scalar(31, 125, 125), cv::Scalar(75, 255, 255) };
cv::Scalar THRESH_BLUE[2] = { cv::Scalar(100, 125, 125), cv::Scalar(130, 255, 255) };
cv::Scalar THRESH_RED[2] = { cv::Scalar(170, 125, 125), cv::Scalar(179, 255, 255) };

/* ritorna un vector con tutte le immagini per ogni colore
in questo modo e' possibile elaborare ogni boccia
*/
std::vector<cv::Mat> colors(cv::Mat &original) {
	std::vector<cv::Mat> vec(5);
	cv::Mat hsv;
	cv::Mat red, yellow, blue, green, black;

	cv::cvtColor(original, hsv, CV_BGR2HSV);

	cv::inRange(hsv, THRESH_YELLOW[0], THRESH_YELLOW[1], yellow);
	cv::inRange(hsv, THRESH_BLUE[0], THRESH_BLUE[1], blue);
	cv::inRange(hsv, THRESH_GREEN[0], THRESH_GREEN[1], green);
	cv::inRange(hsv, THRESH_RED[0], THRESH_RED[1], red);
	cv::inRange(hsv, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), black);

	vec[0] = yellow;
	vec[1] = blue;
	vec[2] = green;
	vec[3] = red;
	vec[4] = black;

	return vec;
}

// Trova il centro delle bocce utilizzando i momenti delle immagini
std::vector<triple> find_centers(cv::Mat &img, cv::Mat &output) {
	std::vector<triple> centers(2);
	cv::Mat blur, edges;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	int x, y;

	// Sfoco l'immagine per una maggiore precisione
	cv::GaussianBlur(img, blur, cv::Size(3, 3), 3);

	// Cerco i contorni delle bocce, e li salvo nell'array contours
	cv::findContours(blur, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	// Per ogni contorno(quindi si suppone ogni boccia, dato che l'immagine per ora è artificiale)
	// Calcolo il centro utilizzando i momenti dell'immagine
	for (int i = 0; i < contours.size(); i++) {
		cv::Moments M = cv::moments((cv::Mat)contours[i]);
		x = M.m10 / M.m00;
		y = M.m01 / M.m00;
		// disegno un punto sul centro
		cv::circle(output, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), -1);

		// Salvo le coordinate nell'array centers
		centers[i].x = x;
		centers[i].y = y;
	}

	return centers;
}


// Calcola la distanza delle bocce
// Metto tutto in un vector che verr� poi ordinato
std::vector<triple> getDistance(std::vector<triple>& input, std::vector<triple> boccino) {
	std::vector<triple> bocceSorted(8);

	for (int i = 0; i < input.size(); i++) {
		input[i].d = distance(boccino[0].x, boccino[0].y, input[i].x, input[i].y);
	}

	std::sort(std::begin(input), std::end(input), sortByDistance);
	bocceSorted = input;

	return bocceSorted;
}