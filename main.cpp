#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

using namespace std; 
using namespace cv;

int main(int argc, char** argv) {

    VideoCapture video(0);
    Mat img;
    CascadeClassifier facedetect;

    char key_pressed;
    //data = CascadeClassifier('/full/path/to/haarcascade_frontalface_default.xml')

    facedetect.load("../build/haarcascade_frontalface_default.xml");


    while (true){
        video.read(img);

        vector<Rect> faces;

        facedetect.detectMultiScale(img, faces, 1.3, 5);

        //cout << faces.size() << endl;

		for (int i = 0; i < faces.size(); i++) {
            /*
            if (cin.peek() != EOF) { // Check if a key has been pressed
                key_pressed = cin.get(); // Get the pressed key

                if (key_pressed == 'q') { // Exit the loop if 'q' is pressed
                    break;
                }
            }*/

			rectangle(img, faces[i].tl(), faces[i].br(), Scalar(50, 50, 255), 3);
			rectangle(img, Point(0,0), Point(250,70), Scalar(50, 50, 255), FILLED);
			putText(img, to_string(faces.size())+" Face Found", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1);
		}

        imshow("Frame", img);
        waitKey(1);

    }

    return 0;
}