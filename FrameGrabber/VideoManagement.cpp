#ifndef VIDEO_MANAGEMENT_H
#define VIDEO_MANAGEMENT_H
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace cv;
using namespace std;
using namespace chrono;

int trackbar_value_frame = 0;
int number_of_frames = 0;
int actual_frame_video = 0;
int x = 0, y = 0;
int count_images = 0;
VideoCapture video_cap, video_cap1;
Mat frame_video;
Mat frame_video1;
string message = "";

void move_frames_right(int n) {
	if (trackbar_value_frame + n > number_of_frames)
		trackbar_value_frame = number_of_frames;
	else
		trackbar_value_frame += n;
}
void move_frames_left(int n) {
	if (trackbar_value_frame - n < 0)
		trackbar_value_frame = 0;
	else
		trackbar_value_frame -= n;
}

void set_trackbar_frames() {
	trackbar_value_frame = actual_frame_video;
}
void read_frame() {

	video_cap.set(CV_CAP_PROP_POS_FRAMES, trackbar_value_frame);
	video_cap.read(frame_video);
	frame_video.copyTo(frame_video1);

}
void read_frame_1() {
	video_cap.set(CV_CAP_PROP_POS_FRAMES, trackbar_value_frame);
	video_cap.read(frame_video);
}
void read_frame_lastORFirst(int act) {
	if (act == 0) {
		video_cap.set(CV_CAP_PROP_POS_FRAMES, 0);
	}
	else {
		video_cap.set(CV_CAP_PROP_POS_FRAMES, number_of_frames);
	}
	video_cap.read(frame_video1);
	video_cap.set(CV_CAP_PROP_POS_FRAMES, trackbar_value_frame);
}

void set_first_frame() {
	video_cap.set(CV_CAP_PROP_POS_FRAMES, 0);
	trackbar_value_frame = 0;
	read_frame();
}
void set_last_frame() {
	video_cap.set(CV_CAP_PROP_POS_FRAMES, number_of_frames);
	trackbar_value_frame = number_of_frames;
	read_frame();
}
void clear_variable() {
	trackbar_value_frame = 0;
	number_of_frames = 0;
	actual_frame_video = 0;
}
std::string make_std_string(cv::String file_name) {
	string file = file_name.operator std::string();
	size_t lastindex = file.find_last_of(".");

	return file.substr(0, lastindex);
}

std::string make_file_name(cv::String file_name, bool video, bool images, bool record = false) {

	string file = make_std_string(file_name);

	chrono::steady_clock time;
	auto start = time.now();
	auto stop = time.now();
	auto time_span = static_cast<chrono::duration<double>>(stop - start);
	auto now1 = system_clock::now();
	auto in_time_t = system_clock::to_time_t(now1);

	stringstream a;
	a << put_time(localtime(&in_time_t), "REC%Y_%m_%d_%X");
	string fileName;
	if (record) {
		fileName = file + "_record_" + a.str() + ".avi";
	}
	else if (images) {
		fileName = file + "_images_" + a.str() + ".avi";
	}
	else if (video)
		fileName = file + "_logo_" + a.str() + ".avi";
	else
		fileName = file + "_logo_" + a.str() + ".png";
	replace(fileName.begin(), fileName.end(), ':', '-');

	return fileName;
}

#endif VIDEO_MANAGEMENT_H