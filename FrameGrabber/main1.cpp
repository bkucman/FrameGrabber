///*
//This is a demo application to showcase keyboard shortcuts. 
//Author: Pascal Thomet
//*/
//
//#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//#define CVUI_IMPLEMENTATION
//#include "cvui.h"
//
//#define WINDOW_NAME	"Button shortcut"
//
//int main(int argc, const char *argv[])
//{
//	cv::Mat frame = cv::Mat(cv::Size(650, 150), CV_8UC3);
//
//	cvui::init(WINDOW_NAME, 20);
//	cv::String input1_value = "text1";
//	cv::String input2_value = "text2";
//
//	while (true) {
//		frame = cv::Scalar(49, 52, 49);
//
//		cvui::input(frame, 40, 40, 100, "input1", input1_value);
//		cvui::input(frame, 40, 80, 100, "input2", input2_value);
//
//		if (cvui::button(frame, 300, 100, "&Quit")) {
//			break;
//		}
//
//		cvui::update();
//		
//		cv::imshow(WINDOW_NAME, frame);
//	}
//
//	return 0;
//}
