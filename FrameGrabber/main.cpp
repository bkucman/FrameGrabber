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
	int action_make_images = 0;
	int action_type_logo = 0;

	// extern variable 
	extern VideoCapture video_cap;
	extern Mat frame_video, frame_video1;
	extern int trackbar_value_frame;
	extern int number_of_frames;
	extern int count_images;
	extern int x, y;

	// flags
	bool prev_scene = false;
	bool next_scene = false;
	bool file_name_flag = false;
	bool choose_source_flag = false;
	bool save_video_flag = false;
	bool record = false;
	bool choose_action_images = false;
	bool file_logo_flag = false;

	// checkbox
	bool checked_source = false;

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME, 2);

	while (true) {
		frame = cv::Scalar(49, 52, 49);
		// main menu
		if (action == 0) {
			if (cvui::button(frame, 150, 20, 500, 100, "Frame Grabber")) {
				action = 1;
			}
			if (cvui::button(frame, 150, 120, 500, 100, "Add logo")) {
				action = 2;
			}
			if (cvui::button(frame, 150, 220, 500, 100, "Record")) {
				action = 3;
			}
			if (cvui::button(frame, 150, 320, 500, 100, "Save to images")) {
				action = 4;
			}
			if (cvui::button(frame, 150, 420, 500, 100, "Video from images")) {
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
				
		}
		// frame grabber
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
					action = 0;
					file_name_flag = false;
					continue;
				}
				cvui::update;
				if (cvui::button(frame, 650, 20, 120, 28, "QUIT") || butt == 27)
				{
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

				else if (cvui::button(frame, 50, 250, 120, 28, "-15 frames") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_RIGHT)) {
					move_frames_right(15);
					read_frame();
					cvui::update;
				}
				else if (cvui::button(frame, 50, 280, 120, 28, "+15 frames") || GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_LEFT)) {
					move_frames_left(15);
					read_frame();
					cvui::update;

				}
				else if (cvui::button(frame, 50, 340, 120, 28, "+50 frames") || GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_LEFT)) {
					move_frames_left(50);
					read_frame();
					cvui::update;

				}
				else if (cvui::button(frame, 50, 310, 120, 28, "-50 frames") || GetAsyncKeyState(VK_NEXT)) {
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
					action = 0;
					prev_scene = false;
					next_scene = false;
					file_name_flag = false;
					video_cap.release();
					destroyWindow(file_name);
					destroyWindow(file_name + "1");
					clear_variable();
					continue;
				}
				cvui::update;
				if (cvui::button(frame, 650, 20, 120, 28, "QUIT") || butt == 27)
				{
					video_cap.release();
					destroyAllWindows();
					break;
				}
				cvui::update;
				cvui::imshow(WINDOW_NAME, frame);

				cv::imshow(file_name, frame_video);
				cv::imshow(file_name + "1", frame_video1);
			}
		}
		// add logo
		else if (action == 2) {

			cvui::text(frame, 330, 30, "Add logo to:", 1);
			// choose where insert logo
			if (!choose_source_flag) {
				if (cvui::button(frame, 150, 120, 500, 100, "Image")) {
					action_type_logo = 1;
					choose_source_flag = true;
				}
				else if (cvui::button(frame, 150, 220, 500, 100, "Video")) {
					action_type_logo = 2;
					choose_source_flag = true;
				}
				else if (cvui::button(frame, 150, 320, 500, 100, "Camera")) {
					action_type_logo = 3;
					choose_source_flag = true;
				}
			}
			else {
				// set files
				if (!file_name_flag || !file_logo_flag) {
					if (action_type_logo != 3) {
						cvui::text(frame, 100, 80, "Image/Video file:", 0.4);
						cvui::input(frame, 250, 80, 150, "file_name", file_name);
					}

					cvui::text(frame, 100, 110, "Logo file:", 0.4);
					cvui::input(frame, 250, 110, 150, "logo_name", logo_name);

					if (cvui::button(frame, 100, 160, 100, 20, "Submit")) {
						if (action_type_logo == 3)
							file_name = "cap";
						file_name_flag = true;
						file_logo_flag = true;
					}
					if (cvui::button(frame, 100, 190, 100, 20, "Default")) {
						if (action_type_logo == 1)
							file_name = image_name;
						else if (action_type_logo == 2)
							file_name = videoName;
						else if (action_type_logo == 3)
							file_name = "cap";
						logo_name = logoName;
						file_name_flag = true;
						file_logo_flag = true;
					}
					cvui::text(frame, 100, 250, message, 0.4);
					cvui::update;
				}
				// add to image
				else if (action_type_logo == 1) {
					if (image.empty() || logo.empty()) {
						image = imread(file_name);
						image.copyTo(image_copy);
						if (image.empty()) {
							file_name_flag = false;
							message = "Open file failed.";
							continue;
						}
						else
							message = "";
						logo = imread(logo_name, -1);
						if (logo.empty()) {
							file_logo_flag = false;
							message = "Open logo failed.";
							continue;
						}
						else
							message = "";

					}
					else {
						if (cvui::button(frame, 50, 190, 120, 28, "Right") || GetAsyncKeyState(VK_RIGHT)) {
							if (x < image.cols - logo.cols) x++;
							message = "";
						}
						else if (cvui::button(frame, 50, 220, 120, 28, "Left") || GetAsyncKeyState(VK_LEFT)) {
							if (x > 0) x--;
							message = "";
						}
						else if (cvui::button(frame, 200, 190, 120, 28, "Up") || GetAsyncKeyState(VK_UP)) {
							if (y > 0) y--;
							message = "";
						}
						else if (cvui::button(frame, 200, 220, 120, 28, "Down") || GetAsyncKeyState(VK_DOWN)) {
							if (y < image.rows - logo.rows) y++;
							message = "";

						}
						else if (cvui::button(frame, 350, 190, 120, 56, "SAVE") || GetAsyncKeyState(83)) {
							imwrite(make_file_name(file_name, false, false), image);
							cout << "Zapis";
							message = "Saved";
						}

						image_copy.copyTo(image);
						roi = Rect(x, y, logo.cols, logo.rows);
						image(roi) = logo + image(roi);
						cv::imshow(file_name, image);
					}

					cvui::text(frame, 50, 100, "Image name: " + file_name, 0.7);
					cvui::text(frame, 50, 130, "Logo name: " + logo_name, 0.7);

				}
				// add to video
				else if (action_type_logo == 2) {
					if (!video_cap.isOpened() || logo.empty()) {
						if (!video_cap.isOpened())
							video_cap = VideoCapture(file_name);
						if (!video_cap.isOpened()) {
							file_name_flag = false;
							message = "Open video failed !";
							continue;
						}
						else {
							message = "No recording";
							trackbar_value_frame = 0;
							number_of_frames = video_cap.get(CAP_PROP_FRAME_COUNT) - 1;
							read_frame();
						}
						logo = imread(logo_name, -1);
						if (logo.empty()) {
							file_logo_flag = false;
							message = "Open logo failed.";
							continue;
						}
						else {
							message = "No recording";
						}
					}
					else {
						if (cvui::button(frame, 50, 190, 120, 28, "Right") || GetAsyncKeyState(VK_RIGHT)) {
							if (x < frame_video.cols - logo.cols) x++;
						}
						else if (cvui::button(frame, 50, 220, 120, 28, "Left") || GetAsyncKeyState(VK_LEFT)) {
							if (x > 0) x--;

						}
						else if (cvui::button(frame, 200, 190, 120, 28, "Up") || GetAsyncKeyState(VK_UP)) {
							if (y > 0) y--;
						}
						else if (cvui::button(frame, 200, 220, 120, 28, "Down") || GetAsyncKeyState(VK_DOWN)) {
							if (y < frame_video.rows - logo.rows) y++;

						}
						frame_video1.copyTo(frame_video);
						roi = Rect(x, y, logo.cols, logo.rows);
						frame_video(roi) = logo + frame_video(roi);

						cv::imshow(file_name, frame_video);
						if ((cvui::button(frame, 330, 190, 120, 56, "SAVE") || GetAsyncKeyState(83)) && !save_video_flag) {
							cout << make_file_name(file_name, true, false);
							video_writer = VideoWriter(make_file_name(file_name, true, false), CV_FOURCC('D', 'I', 'V', 'X'), video_cap.get(CV_CAP_PROP_FPS), Size(static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH)), static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT))), true);
							save_video_flag = true;
						}
						else if ((cvui::button(frame, 480, 190, 120, 56, "END") || GetAsyncKeyState(69)) && save_video_flag) {
							video_writer.release();
							save_video_flag = false;
							message = "No recording";
						}
						if (save_video_flag && trackbar_value_frame <= number_of_frames) {

							video_writer.write(frame_video);
							move_frames_right(1);
							read_frame();
							message = "Recording";
							if (trackbar_value_frame == number_of_frames) {
								set_first_frame();
								save_video_flag = false;
								message = "No recording";
							}
						}
						cvui::text(frame, 50, 100, "Image name: " + file_name, 0.7);
						cvui::text(frame, 50, 130, "Logo name: " + logo_name, 0.7);
					}
				}
				// add to cap
				else if (action_type_logo == 3) {
					if (!video_cap.isOpened() || logo.empty()) {
						if (!video_cap.isOpened())
							video_cap = VideoCapture(0);
						if (!video_cap.isOpened()) {
							file_name_flag = false;
							message = "Open cap failed !";
							continue;
						}
						else {
							message = "";
							video_cap.read(frame_video);
						}
						logo = imread(logo_name, -1);
						if (logo.empty()) {
							file_logo_flag = false;
							message = "Open logo failed.";
							continue;
						}
						else {
							message = "No recording";
						}
					}
					else {
						if (cvui::button(frame, 50, 190, 120, 28, "Right") || GetAsyncKeyState(VK_RIGHT)) {
							if (x < frame_video.cols - logo.cols) x++;
						}
						else if (cvui::button(frame, 50, 220, 120, 28, "Left") || GetAsyncKeyState(VK_LEFT)) {
							if (x > 0) x--;

						}
						else if (cvui::button(frame, 200, 190, 120, 28, "Up") || GetAsyncKeyState(VK_UP)) {
							if (y > 0) y--;
						}
						else if (cvui::button(frame, 200, 220, 120, 28, "Down") || GetAsyncKeyState(VK_DOWN)) {
							if (y < frame_video.rows - logo.rows) y++;

						}
						roi = Rect(x, y, logo.cols, logo.rows);
						frame_video(roi) = logo + frame_video(roi);

						cv::imshow(file_name, frame_video);
						if ((cvui::button(frame, 330, 190, 120, 56, "SAVE") || GetAsyncKeyState(83)) && !save_video_flag) {
							cout << make_file_name(file_name, true, false);
							video_writer = VideoWriter(make_file_name(file_name, true, false), CV_FOURCC('D', 'I', 'V', 'X'), 15, Size(static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH)), static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT))), true);

							save_video_flag = true;
							message = "Recording";
						}
						else if ((cvui::button(frame, 470, 190, 120, 56, "END") || GetAsyncKeyState(69)) && save_video_flag) {
							video_writer.release();
							save_video_flag = false;
							message = "No recording";
						}
						if (save_video_flag) {
							video_writer.write(frame_video);
						}

						if (!video_cap.read(frame_video)) {
							message = "Read camera failed";
							goto back;
						}
						cvui::text(frame, 50, 100, "Image name: " + file_name, 0.7);
						cvui::text(frame, 50, 130, "Logo name: " + logo_name, 0.7);
					}
				}
			}

			butt = waitKeyEx(5);
			// QUIT
			if (cvui::button(frame, 650, 20, 120, 28, "QUIT") || butt == 27) {
				destroyAllWindows();
				if (video_cap.isOpened())
					video_cap.release();
				if (video_writer.isOpened())
					video_writer.release();
				break;
			}
			// Main menu
			if (cvui::button(frame, 650, 50, 120, 28, "Main menu")) {
				message = "";
			back:
				action = 0;
				x = y = 0;
				destroyWindow(file_name);
				action_type_logo = -1;
				choose_source_flag = false;
				file_name_flag = false;
				file_logo_flag = false;
				save_video_flag = false;
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
			// choose source
			if (!choose_action_images) {
				if (cvui::button(frame, 150, 20, 500, 100, "From Video")) {
					action_make_images = 1;
					choose_action_images = true;
				}
				else if (cvui::button(frame, 150, 120, 500, 100, "From camera")) {
					action_make_images = 2;
					choose_action_images = true;
				}
			}
			else {
				// save from video
				if (action_make_images == 1) {
					if (!file_name_flag) {

						cvui::text(frame, 100, 80, "Video file:", 0.4);
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
					else {
						if (!video_cap.isOpened()) {
							if (!video_cap.isOpened())
								video_cap = VideoCapture(file_name);
							if (!video_cap.isOpened()) {
								file_name_flag = false;
								message = "Open video Filed !";
								continue;
							}
							else {
								message = "No recording";
								trackbar_value_frame = 0;
								number_of_frames = video_cap.get(CAP_PROP_FRAME_COUNT) - 1;
								read_frame();
								remove_images(file_name);
							}
						}
						else {
							cv::imshow(file_name, frame_video);
							if (cvui::trackbar(frame, 2, 550, 796, &trackbar_value_frame, 0, number_of_frames, 1, "%.LF")) {
								read_frame();
								cvui::update;
							}
							else if ((cvui::button(frame, 330, 190, 120, 60, "SAVE") || GetAsyncKeyState(83)) && !save_video_flag) {
								cout << make_file_name(file_name, true, false);

								save_video_flag = true;
							}
							else if ((cvui::button(frame, 450, 190, 120, 60, "END") || GetAsyncKeyState(69)) && save_video_flag) {
								save_video_flag = false;
								message = "No recording";
							}
							if (save_video_flag && trackbar_value_frame <= number_of_frames) {

								if ((save_video_flag = check_end_video(file_name)))
									message = "Recording";
								else
									message = "No recording";
							}
						}
					}
				}
				// save from camera
				else if (action_make_images == 2) {
					if (!video_cap.isOpened()) {
						if (!video_cap.isOpened())
							video_cap = VideoCapture(0);
						if (!video_cap.isOpened()) {
							message = "Open cap Filed !";
							continue;
						}
						else {
							message = "No recording";
							file_name = "cap";
							remove_images(file_name);
							if (!video_cap.read(frame_video)) {
								message = "Read camera failed";
								goto back2;
							}
						}
					}
					else {
						cv::imshow(file_name, frame_video);
						if ((cvui::button(frame, 330, 190, 120, 60, "SAVE") || GetAsyncKeyState(83)) && !save_video_flag) {
							cout << make_file_name(file_name, true, false);

							save_video_flag = true;
						}
						else if ((cvui::button(frame, 450, 190, 120, 60, "END") || GetAsyncKeyState(69)) && save_video_flag) {
							save_video_flag = false;
							message = "No recording";
						}
						if (save_video_flag) {

							imwrite(set_name_images(file_name), frame_video);

							message = "Recording";
						}
						if (!video_cap.read(frame_video)) {
							message = "Read camera failed";
							goto back2;
						}
					}
				}
			}

			butt = waitKeyEx(10);
			if (cvui::button(frame, 670, 20, 120, 28, "QUIT") || butt == 27) {
				destroyAllWindows();
				break;
			}

			if (cvui::button(frame, 670, 50, 120, 28, "Main menu")) {
				message = "";
			back2:
				action = 0;
				save_video_flag = false;
				file_name_flag = false;
				choose_action_images = false;
				count_images = 0;
				action_make_images = 0;
				destroyWindow(file_name);
				if (video_cap.isOpened())
					video_cap.release();
				continue;
			}
			cvui::update;
			cvui::text(frame, 100, 250, message, 0.4);
			cvui::imshow(WINDOW_NAME, frame);
		}
		// make video from images
		else if (action == 5) {
			// make from video
			if (!file_name_flag) {

				cvui::text(frame, 100, 83, "Video file:", 0.4);
				cvui::input(frame, 200, 80, 150, "file_name", file_name);
				cvui::text(frame, 380, 83, "without extension", 0.4);
				cvui::text(frame, 120, 120, "if camera, mark checkbox and Submit", 0.4);
				cvui::checkbox(frame, 210, 163, "Camera", &checked_source);

				if (cvui::button(frame, 100, 160, 100, 20, "Submit")) {
					if (checked_source) {
						file_name = "cap";
					}
					message = "";
					file_name_flag = true;
					cvui::imshow(WINDOW_NAME, frame);
				}
				if (cvui::button(frame, 100, 190, 100, 20, "Default")) {
					file_name = videoName;
					file_name_flag = true;
					message = "";
					cvui::imshow(WINDOW_NAME, frame);
				}
				cvui::text(frame, 100, 240, "FPS value:", 1);
				cvui::trackbar(frame, 100, 280, 220, &fps, 0, 60, 1, "%.LF");
				cvui::text(frame, 100, 350, message, 0.4);
				cvui::update;

			}
			else {
				message = make_video_from_images(file_name, fps);
				file_name_flag = false;
				checked_source = false;
			}

			butt = waitKeyEx(10);
			if (cvui::button(frame, 670, 20, 120, 28, "QUIT") || butt == 27) {
				destroyAllWindows();
				break;
			}

			if (cvui::button(frame, 670, 50, 120, 28, "Main menu")) {
				action = 0;
				message = "";
				file_name_flag = false;
				count_images = 0;
				checked_source = false;
				file_name = "";
				continue;
			}
			cvui::update;
			cvui::imshow(WINDOW_NAME, frame);
		}
	}
	return 0;
}