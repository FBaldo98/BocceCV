#pragma once

#include <iostream>
#include <cstdio>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\features2d.hpp>
#include <vector>
#include "utils.h"

void calibration(cv::Mat &calib_image, cv::Size chessboard_size, bool debug = false);

void compute_distance(std::vector<cv::Point2f> corners, cv::Size chessboard_size, float &width, float &height, bool debug);

void load_calibration(float* x_pixels, float* y_pixels);

double* pixel_per_mm(int width, int height, int mm_width, int mm_height);