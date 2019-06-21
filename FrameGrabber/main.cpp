#define CVUI_IMPLEMENTATION
#include "cvui.h"
//#include "EnhancedWindow.h"

#include "VideoManagement.h" // to change frames in video
#include "main.h"

using namespace cv;
using namespace std;

#define WINDOW_NAME "CVUI Hello World!"

int main() {

	// captured keys
	int butt = 0;

	int fps = 24;

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
	int choose_action_record = -1;

	// extern variable 
	extern VideoCapture video_cap;
	extern Mat frame_video, frame_video1;
	extern int trackbar_value_frame;
	extern int number_of_frames;

	// flags
	bool prev_scene = false;
	bool next_scene = false;
	bool file_name_flag = false;
	bool choose_source_flag = false;
	bool save_video_flag = false;
	bool record = false;

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

			//choose record option
			if (!choose_source_flag) {
				if (cvui::button(frame, 150, 20, 500, 100, "Record from camera")) {
					choose_action_record = 1;
					choose_source_flag = true;
				}
				if (cvui::button(frame, 150, 120, 500, 100, "Record from video")) {
					choose_action_record = 2;
					choose_source_flag = true;
				}
			}
			else {
				//set files
				if (!file_name_flag) {
					if (choose_action_record == 1) {
						file_name = "cap";
						file_name_flag = true;
						continue;
					}
					cvui::text(frame, 100, 80, "Image/Video file:", 0.4);
					cvui::input(frame, 250, 80, 150, "file_name", file_name);

					if (cvui::button(frame, 100, 160, 100, 20, "Submit")) {
						file_name_flag = true;
					}
					if (cvui::button(frame, 100, 190, 100, 20, "Default")) {
						file_name = videoName;
						file_name_flag = true;
					}
					cvui::text(frame, 100, 250, message, 0.4);
					cvui::update;
				}
				else if (choose_action_record == 1) {
					if (cvui::button(frame, 50, 20, "Start recording") || GetAsyncKeyState(0x53) && !record) {
						record = true;
						save_video_flag = false;
					}

					if (cvui::button(frame, 50, 50, "Stop recording") || GetAsyncKeyState(0x45) && record) {
						record = false;
						save_video_flag = true;
						video_writer.release();
						message = "No recording";
					}

					cvui::text(frame, 30, 120, "FPS value:", 0.7);
					cvui::trackbar(frame, 30, 180, 220, &fps, 0, 60, 1, "%.LF");

					if (!video_cap.isOpened()) {
						if (!video_cap.isOpened())
							video_cap = VideoCapture(0);

						if (!video_cap.isOpened()) {
							file_name_flag = false;
							save_video_flag = false;
							record = false;
							message = "Open cap failed !";
							continue;
						}
						else {
							message = "";
							if (!video_cap.read(frame_video)) {
								message = "Read camera failed";
								goto back1;
							};
						}
					}
					else {
						file_name = "cap";
						file_name_flag = true;
						cv::imshow(file_name, frame_video);
						if (!video_cap.read(frame_video)) {
							message = "Read camera failed";
							goto back1;
						}

						if (record) {
							putText(frame_video, "REC", Point2i(30, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
							message = "Recording";
							if (!save_video_flag) {
								video_writer = VideoWriter(make_file_name(file_name, true, false, true), CV_FOURCC('D', 'I', 'V', 'X'), fps, Size(static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH)), static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT))), true);
								save_video_flag = true;
							}

							if (save_video_flag) {
								video_writer.write(frame_video);
							}
						}
						else {
							message = "Recording stopped";
							putText(frame_video, "STOP", Point2i(30, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2);
						}
					}
				}
				else if (choose_action_record == 2) {
					if (cvui::button(frame, 30, 20, "Start recording") || GetAsyncKeyState(0x53) && !record) {
						record = true;
						save_video_flag = false;
					}

					if (cvui::button(frame, 30, 50, "Stop recording") || GetAsyncKeyState(0x45) && record) {
						record = false;
						save_video_flag = true;
						video_writer.release();
						message = "No recording";
					}

					cvui::text(frame, 30, 120, "FPS value:", 0.7);
					cvui::trackbar(frame, 30, 180, 220, &fps, 0, 60, 1, "%.LF");

					if (!video_cap.isOpened()) {
						if (!video_cap.isOpened())
							video_cap = VideoCapture(file_name);

						if (!video_cap.isOpened()) {
							file_name_flag = false;
							save_video_flag = false;
							record = false;
							message = "Open cap Failed !";
							continue;
						}
						else {
							message = "No recording";
							trackbar_value_frame = 0;
							number_of_frames = video_cap.get(CAP_PROP_FRAME_COUNT) - 1;
							read_frame();
							video_cap.read(frame_video);
						}
					}
					else {
						file_name_flag = true;
						cv::imshow(file_name, frame_video);

						if (cvui::trackbar(frame, 2, 550, 796, &trackbar_value_frame, 0, number_of_frames, 1, "%.LF")) {
							read_frame();
							cvui::update;
						}

						if (record) {
							video_cap.read(frame_video);
							message = "Recording";
							if (!save_video_flag) {
								//putText(frame_video, "REC", Point2i(30, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
								video_writer = VideoWriter(make_file_name(file_name, true, false, true), CV_FOURCC('D', 'I', 'V', 'X'), fps, Size(static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH)), static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT))), true);
								save_video_flag = true;
							}
							if (save_video_flag && trackbar_value_frame <= number_of_frames) {
								move_frames_right(1);
								read_frame();
								message = "Recording";
								if (trackbar_value_frame == number_of_frames) {
									save_video_flag = false;
									set_first_frame();
									message = "No recording";
								}
								video_writer.write(frame_video);
							}
						}
						else {
							message = "Recording stopped";
							putText(frame_video, "STOP", Point2i(30, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2);
						}
					}
				}
			}

			//cofanie
			butt = waitKeyEx(10);
			if (cvui::button(frame, 670, 20, 120, 28, "QUIT") || butt == 27) {
				destroyAllWindows();
				if (video_cap.isOpened())
					video_cap.release();
				if (video_writer.isOpened())
					video_writer.release();
				break;
			}

			if (cvui::button(frame, 670, 50, 120, 28, "Main menu")) {
				message = "";
			back1:
				action = 0;
				choose_action_record = -1;
				destroyWindow(file_name);
				record = false;
				file_name_flag = false;
				save_video_flag = false;
				choose_source_flag = false;
				if (video_cap.isOpened())
					video_cap.release();
				if (video_writer.isOpened())
					video_writer.release();
				continue;
			}
			cvui::text(frame, 100, 250, message, 0.4);
			cvui::update;
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