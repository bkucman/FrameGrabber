#pragma once
#include<iostream>
#include <conio.h>
#include <windows.h>
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