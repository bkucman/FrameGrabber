#pragma once
#include<iostream>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

void move_frames_right(int n);
void move_frames_left(int n);
void set_trackbar_frames();
void read_frame();
void set_first_frame();
void set_last_frame();
void read_frame_1();
void read_frame_lastORFirst(int act);
void clear_variable();
std::string make_file_name(cv::String file_name, bool video, bool images, bool record = false);
std::string make_std_string(cv::String file_name);
std::string set_name_images(cv::String file_name);
void remove_images(cv::String file_name);
bool check_end_video(cv::String file_name);
std::string make_video_from_images(cv::String file_name, int fps);