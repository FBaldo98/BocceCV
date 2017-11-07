#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <vector>
#include <string.h>
#include "utils.h"


/* ritorna un vector con tutte le immagini per ogni colore
in questo modo è possibile elaborare ogni boccia
*/
std::vector<cv::Mat> colors(cv::Mat &original);

// Trova il centro delle bocce utilizzando i momenti delle immagini
std::vector<triple> find_centers(cv::Mat &img, cv::Mat &output);

// Calcola la distanza delle bocce
// Metto tutto in un vector che verrà poi ordinato
std::vector<triple> getDistance(std::vector<triple>& input, std::vector<triple> boccino);