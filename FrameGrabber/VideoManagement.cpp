#ifndef VIDEO_MANAGEMENT_H
#define VIDEO_MANAGEMENT_H
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include <conio.h>
#include <windows.h>
using namespace cv;
using namespace std;

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


#endif VIDEO_MANAGEMENT_H