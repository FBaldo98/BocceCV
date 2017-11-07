#include "calibration.h"

// Funzione calibrazione, immagine scacchiera 12x7
void calibration(cv::Mat &calib_image, cv::Size chessboard_size, bool debug) {
	cv::Mat greyImg; // Immagine in B/W
	cv::Mat threshImg;  // Immagine binarizzata
	int thresh = 127; // Soglia( TODO: Aggiungere input per modificare)
	FILE *calib_file = fopen("calib.txt", "w"); // File di calibrazione

	std::vector<cv::Point2f> chessPoints(chessboard_size.width * chessboard_size.height);
	bool chess_found;
	int calib_parameters = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS | CV_CALIB_CB_FAST_CHECK;

	// Coordinate origine
	cv::Point origin;

	// Dimensioni medie rettangoli
	float medium_width = 0;
	float medium_height = 0;


	/* Se l'immagine è nera esco
	if (cv::countNonZero(calib_image) < 1) {
		std::cerr << "CALIBRATION ERROR: Calib_Image is black" << std::endl;
		std::system("PAUSE");
		exit(1);
	}*/
	
	// Converto l'immagine in B/W
	cv::cvtColor(calib_image, greyImg, cv::COLOR_BGR2GRAY);

	// Risolvo i problemi di intensita' luminosa con l'equalizzazione adattativa dell'istogramma
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(6);
	clahe->apply(greyImg, greyImg);
	//cv::equalizeHist(greyImg, greyImg);

	// Trovo la scacchiera con la funzione apposita
	chess_found = cv::findChessboardCorners(greyImg, chessboard_size, chessPoints, calib_parameters);
	
	// Se non presente termino con errore 
	if (!chess_found) {
		std::cerr << "ERROR: Calibration pattern NOT FOUND! \n TERMINATING" << std::endl;
		return;
	}

	// cornerSubPix aumenta la precisione dei risultati ottenuti
	cv::cornerSubPix(greyImg, chessPoints, chessboard_size, cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
	// Disegno i punti sulla scacchiera
	cv::drawChessboardCorners(calib_image, chessboard_size, chessPoints, chess_found);

	compute_distance(chessPoints, chessboard_size, medium_width, medium_height, debug);

	double* pixels = pixel_per_mm(medium_width, medium_height, 10, 10);

	// Salvo le informazioni ricavate su file
	fprintf(calib_file, "%.3f\t%.3f\n", pixels[0], pixels[1]);
	fprintf(calib_file, "%d\n", chessPoints.size());
	for (int x = 0; x < chessPoints.size(); x++)
		fprintf(calib_file, "%.2f\t%.2f\n", chessPoints[x].x, chessPoints[x].y);
	fclose(calib_file);

	cv::namedWindow("Calibration", cv::WINDOW_NORMAL);
	cv::resizeWindow("Calibration", 800, 600);

	cv::imshow("Calibration", calib_image);

	if (debug) {
		std::cout << chessPoints.size() << std::endl;
		std::cout << medium_width << std::endl;
		std::cout << medium_height << std::endl;
		printf("%.2f \t %.2f", *pixels, *(pixels + 1));
	}

	cv::waitKey(0);
	
}


// Calcola la distanza media asse X ed asse Y tra i punti della scacchiera 
void compute_distance(std::vector<cv::Point2f> corners, cv::Size chessboard_size, float& width, float& height, bool debug) {
	
	for (int y = 0; y < chessboard_size.height; y++) {
		for (int x = 0; x < chessboard_size.width; x++) {

			// Prendendo il nodo successivo, devo fermarmi prima
			if(x < chessboard_size.width - 1)
				width += distance(corners[chessboard_size.width * y + (x + 1)], corners[chessboard_size.width * y + x]);
			if (y < chessboard_size.height - 1)
				height += distance(corners[chessboard_size.width * (y + 1) + x], corners[chessboard_size.width * y + x]);

		}
	}

	// Media dei valori
	width /= corners.size();
	height /= corners.size();
	
}

// Legge i valori dal file
void load_calibration(float* x_pixels, float* y_pixels)
{
	FILE* f = fopen("calib.txt", "r");

	fscanf(f, "%.3f\t%.3f", x_pixels, y_pixels);

	fclose(f);
}

/*
	Ritorna un array di due elementi
	 - 1: pixel/mm orizzontali
	 - 2: pixel/mm verticali
*/
double* pixel_per_mm(int pix_width, int pix_height, int mm_width, int mm_height)
{
	static double  pix_per_mm[2];

	pix_per_mm[0] = (double)pix_width / (double)mm_width;
	pix_per_mm[1] = (double)pix_height / (double)mm_height;

	return pix_per_mm;
}


