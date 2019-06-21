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

	string message = "";
	// Mat
	// menu frame
	Mat frame = Mat(600, 800, CV_8UC3);

	// action varaible
	int action = 0;

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

			butt = waitKeyEx(10);
			if (butt == 27)
				break;
			message = "Not implemented yet";
			cvui::text(frame, 100, 550, message, 0.4);
			cvui::update();
			cvui::imshow(WINDOW_NAME, frame);
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