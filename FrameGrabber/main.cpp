#define CVUI_IMPLEMENTATION
#include "cvui.h"
//#include "EnhancedWindow.h"

#include "VideoManagement.h" // to change frames in video

using namespace cv;
using namespace std;

#define WINDOW_NAME "CVUI Hello World!"

int main() {

	// captured keys
	int butt = 0;

	// file names
	cv::String file_name = "";
	string name;
	string videoName = "robot.avi";
	string logoName = "logo.bmp";
	string image_name = "image.jpeg";
	string message = "";
	cv::String logo_name = "logo.bmp";

	Rect roi;
	// Mat
	// menu frame
	Mat frame = Mat(600, 800, CV_8UC3);
	Mat logo;
	Mat image, image_copy;
	Mat mask;
	Mat frameq;

	// video writer
	VideoWriter video_writer;

	// action varaible
	int action = 0;

	// extern variable 
	extern VideoCapture video_cap;
	extern Mat frame_video, frame_video1;
	extern int trackbar_value_frame;
	extern int number_of_frames;

	// flags
	bool prev_scene = false;
	bool next_scene = false;
	bool file_name_flag = false;

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME, 2);

	while (true) {
		frame = cv::Scalar(49, 52, 49);
		// main menu
		if (action == 0) {
			if (cvui::button(frame, 150, 20, 500, 100, "Frame Grabber")) {
				action = 1;
			}
			else if (cvui::button(frame, 150, 120, 500, 100, "Add logo")) {
				action = 2;
			}
			else if (cvui::button(frame, 150, 220, 500, 100, "Record")) {
				action = 3;
			}
			else if (cvui::button(frame, 150, 320, 500, 100, "Save to images")) {
				action = 4;
			}
			else if (cvui::button(frame, 150, 420, 500, 100, "Video from images")) {
				action = 5;
			}
			if (cvui::button(frame, 660, 20, 120, 28, "QUIT") || butt == 27)
			{
				destroyAllWindows();
				break;
			}

			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
		}
		// frame geabber
		else if (action == 1) {
			if (!file_name_flag) {
				cvui::text(frame, 260, 30, "Frame Grabber", 1);
				cvui::text(frame, 90, 160, "Enter the name of video (with extension)", 0.5);
				cvui::input(frame, 90, 190, 200, "input", file_name);


				if (cvui::button(frame, 90, 230, 120, 28, "Submit")) {

					file_name_flag = true;
				}
				if (cvui::button(frame, 90, 260, 120, 28, "Default file")) {

					file_name = videoName;

					file_name_flag = true;
				}
				if (cvui::button(frame, 650, 50, 120, 28, "Main menu")) {
					cout << "Jestem tutaj";
					action = 0;
					prev_scene = false;
					next_scene = false;
					file_name_flag = false;
					video_cap.release();
					destroyWindow(videoName);
					destroyWindow(videoName + "1");
					clear_variable();
					continue;
				}
				cvui::update;
				if (cvui::button(frame, 650, 20, 120, 28, "QUIT") || butt == 27)
				{
					//RELEASE !!!!
					video_cap.release();
					destroyAllWindows();
					break;
				}
				cvui::update();
				cvui::imshow(WINDOW_NAME, frame);
			}
			else {
				if (!video_cap.isOpened()) {
					video_cap = VideoCapture(file_name);
					if (!video_cap.isOpened()) {
						file_name = "";
						action = 0;
						file_name_flag = false;
						continue;
					}
					else {
						number_of_frames = video_cap.get(CAP_PROP_FRAME_COUNT) - 1;
						read_frame();
					}
				}

				cvui::text(frame, 260, 30, "Frame Grabber", 1);
				cout << next_scene;
				if (prev_scene || next_scene) {
					if (prev_scene) {
						read_frame_lastORFirst(0);
						if (trackbar_value_frame > 0) {
							move_frames_left(1);
							read_frame_1();
						}
						else
							prev_scene = false;
					}
					else if (next_scene) {
						read_frame_lastORFirst(1);
						if (trackbar_value_frame < number_of_frames) {
							move_frames_right(1);
							read_frame_1();
						}
						else
							next_scene = false;
					}
					cvui::trackbar(frame, 2, 550, 796, &trackbar_value_frame, 0, number_of_frames, 1, "%.LF", 0U, 2);
				}
				else if (cvui::trackbar(frame, 2, 550, 796, &trackbar_value_frame, 0, number_of_frames, 1, "%.LF")) {
					read_frame();
					cvui::update;
				}
				//cvui::button(frame, 150, 20, 500, 100, "FrameGrabberVideo")
				else if (cvui::button(frame, 50, 370, 120, 28, "Prev Scene") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LEFT))
				{
					prev_scene = true;
					cvui::update;
				}
				else if (cvui::button(frame, 50, 400, 120, 28, "Next Scene") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_RIGHT))
				{
					next_scene = true;
					cvui::update;

				}
				else if (cvui::button(frame, 50, 130, 120, 28, "Beginning") || GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LEFT)) {
					set_first_frame();
					read_frame();
					cvui::update;
				}
				else if (cvui::button(frame, 50, 160, 120, 28, "End") || GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_RIGHT)) {
					set_last_frame();
					read_frame();
					cvui::update;
				}

				else if (cvui::button(frame, 50, 250, 120, 28, "+15 frames") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_RIGHT)) {
					move_frames_right(15);
					read_frame();
					cvui::update;
				}
				else if (cvui::button(frame, 50, 280, 120, 28, "-15 frames") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_LEFT)) {
					move_frames_left(15);
					read_frame();
					cvui::update;

				}
				else if (cvui::button(frame, 50, 340, 120, 28, "-50 frames") || GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_LEFT)) {
					move_frames_left(50);
					read_frame();
					cvui::update;

				}
				else if (cvui::button(frame, 50, 310, 120, 28, "+50 frames") || GetAsyncKeyState(VK_NEXT)) {
					move_frames_right(50);
					read_frame();
					cvui::update;
				}
				else if (cvui::button(frame, 50, 190, 120, 28, "Next frame") || GetAsyncKeyState(VK_RIGHT)) {
					move_frames_right(1);
					read_frame();
					cvui::update;
				}
				else if (cvui::button(frame, 50, 220, 120, 28, "Previous frame") || GetAsyncKeyState(VK_LEFT)) {
					move_frames_left(1);
					read_frame();
					cvui::update;

				}

				cvui::text(frame, 200, 140, "Ctrl+Left", 0.4);
				cvui::text(frame, 200, 170, "Ctrl+Right", 0.4);
				cvui::text(frame, 200, 200, "Right", 0.4);
				cvui::text(frame, 200, 230, "Left", 0.4);
				cvui::text(frame, 200, 260, "Shift+Left", 0.4);
				cvui::text(frame, 200, 290, "Shift+Right", 0.4);
				cvui::text(frame, 200, 320, "Alt+Left", 0.4);
				cvui::text(frame, 200, 350, "Page Down", 0.4);
				cvui::text(frame, 200, 380, "Ctrl+Shift+Left", 0.4);
				cvui::text(frame, 200, 410, "Ctrl+Shift+Right", 0.4);

				if (cvui::button(frame, 650, 50, 120, 28, "Main menu")) {
					cout << "Jestem tutaj";
					action = 0;
					prev_scene = false;
					next_scene = false;
					file_name_flag = false;
					video_cap.release();
					destroyWindow(videoName);
					destroyWindow(videoName + "1");
					clear_variable();
					continue;
				}
				cvui::update;
				if (cvui::button(frame, 650, 20, 120, 28, "QUIT") || butt == 27)
				{
					//RELEASE !!!!
					video_cap.release();
					destroyAllWindows();
					break;
				}
				cvui::update;
				cvui::imshow(WINDOW_NAME, frame);

				cv::imshow(videoName, frame_video);
				cv::imshow(videoName + "1", frame_video1);
			}
		}
		// add logo
		else if (action == 2) {

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
			message = "Not implemented yet";
			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);
		}
		// zapis wideo
		else if (action == 3) {

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
			message = "Not implemented yet";
			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);
		}
		// save images
		else if (action == 4) {

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
			message = "Not implemented yet";
			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);
		}
		// make video from images
		else if (action == 5) {

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
			message = "Not implemented yet";
			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);
		}
	}
	return 0;
}