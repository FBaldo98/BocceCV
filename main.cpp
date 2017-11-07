#include "utils.h"
#include "algoBocce.h"
#include "bocce.h"
#include "calibration.h"

int main() {
	
	int choice;

	std::cout << "Cosa vorresti fare? \n1 - Gioco\n2 - Calibrazione\n3 - Esci\nInserisci numero opzione: ";
	std::cin >> choice;

	if (choice == 1) {
		std::system("CLS");
		game();
	}
	else if (choice == 2) {
		std::system("CLS");
		cv::Mat calib_image = cv::imread("Images/ref2_101.bmp");
		calibration(calib_image, cv::Size(12, 7), true);
	}
	else if (choice == 3)
		exit(0);
	else
		std::cout << "Opzione non valida." << std::endl;

	cv::waitKey(10);

	return 0;
}
