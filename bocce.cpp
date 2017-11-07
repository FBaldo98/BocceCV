#include "algoBocce.h"
#include "utils.h"

void game() {
	cv::Mat original;
	std::vector<cv::Mat> bocce(5);
	std::vector<triple> yellow(2);
	std::vector<triple> blue(2);
	std::vector<triple> red(2);
	std::vector<triple> green(2);
	std::vector<triple> black(2);
	std::vector<triple> bocceTot;
	std::vector<triple> bocceSort;


	original = cv::imread("Images/bocce4.png", 1);
	cv::imshow("Test", original);
	cv::waitKey(0);

	bocce = colors(original);
	/*
	cv::imshow("Giallo", bocce[0]);
	cv::imshow("Blu", bocce[1]);
	cv::imshow("Verde", bocce[2]);
	cv::imshow("Rosso", bocce[3]);
	cv::imshow("Boccino", bocce[4]);
	*/
	
	yellow = find_centers(bocce[0], original);
	yellow[0].color = "Giallo";
	yellow[1].color = "Giallo";
	blue = find_centers(bocce[1], original);
	blue[0].color = "Blu";
	blue[1].color = "Blu";
	green = find_centers(bocce[2], original);
	green[0].color = "Verde";
	green[1].color = "Verde";
	red = find_centers(bocce[3], original);
	red[0].color = "Rosso";
	red[1].color = "Rosso";

	black = find_centers(bocce[4], original);

	for (int i = 0; i < 2; i++) {
		bocceTot.push_back(yellow[i]);
		bocceTot.push_back(blue[i]);
		bocceTot.push_back(green[i]);
		bocceTot.push_back(red[i]);
	}

	bocceSort = getDistance(bocceTot, black);

	std::cout << "Boccino: " << black[0].x << " " << black[0].y << std::endl;

	// Stampa sulle bocce il numero che indica il risultato ottenuto
	for (int i = 0; i < bocceSort.size(); i++) {
		int x = bocceSort[i].x;
		int y = bocceSort[i].y;
		cv::putText(original, std::to_string(i + 1), cv::Point(x, y), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
	}

	// Stampo il colore del vincitore ed alcuni dati nella linea di comando
	cv::putText(original, ("Vincitore: " + bocceSort[0].color), cv::Point(20, 25), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
	std::cout << "Il vincitore e': " << bocceSort[0].color << std::endl;
	std::cout << "Distante dal boccino: " << bocceSort[0].d << "px" << std::endl;
	cv::imshow("Test", original);
	cv::waitKey(0);
	cv::destroyAllWindows();
}