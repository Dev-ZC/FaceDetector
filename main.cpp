#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/objdetect/face.hpp>
// Disregard any errors for above includes, when the program is 
// built the program has access to them

using namespace std; 
using namespace cv;
using namespace std::__fs::filesystem;

class Image {
private:
    Mat data; // OpenCV matrix to store image data
    int width;    // Image width
    int height;   // Image height
    path filepath; // Path to the image file
public:
    // Constructor to initialize image data from a file
    Image(path filename) {
        data = imread(filename.string(), IMREAD_COLOR);
        width = data.cols;
        height = data.rows;
        filepath = filename;
    }

    // Constructor to initialize image data from a cv::Mat object
    Image(Mat imageData) {
        data = imageData;
        width = data.cols;
        height = data.rows;
    }

    // Member function: gets img data
    Mat getData() {
        return data;
    }

    // Member function: gets img width
    int getWidth() {
        return width;
    }

    // Member function: get img height
    int getHeight() {
        return height;
    }

    // Function to displau img in separate window
    // Results in slow runtime while also displaying video
    void display() {
        imshow("Image", data);
        waitKey(0);
    }

    // Member function to save image to file
    void save(path outputDir) {
        static int count = 0;  // Counter for face images
        string filename = filepath.filename().stem().string();  // Extract the filename without extension
        path outputFile(outputDir / (filename + "_" + to_string(count) + ".jpg"));  // Creates a new file name for each call
        imwrite(outputFile.string(), data);  // Save the image
        count++;  // Increment the counter
    }
};

int main(int argc, char** argv) {

    VideoCapture video(0);
    if (!video.isOpened()) {
        cerr << "Failed to open video capture." << endl; // Error catch if camera not found
        return -1;
    }

    // Reads img from video capture
    Mat img;
    CascadeClassifier facedetect;

    char key_pressed; // Var for detected key press
    int numFaceHolder = 0; // Comparison var to check if number of faces increase

    facedetect.load("../build/haarcascade_frontalface_default.xml");

    while (true){

        video.read(img);

        vector<Rect> faces;

        facedetect.detectMultiScale(img, faces, 1.3, 5);

		for (int i = 0; i < faces.size(); i++) {
			rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 255, 255), 3);
			rectangle(img, Point(0,0), Point(280,70), Scalar(50, 50, 255), FILLED);
			putText(img, "Faces Found: "+to_string(faces.size()), Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1);
		}

        // Saves image when a face is revealed within frame
        if (faces.size() > numFaceHolder) {
            Image imgObj(img);
            path outputDir("../DetectedFaces"); // File path to output folder
            imgObj.save(outputDir);
            cout << "Face detected! New image saved." << endl;
        }

        imshow("Frame", img);
        if (waitKey(1) == 'q') {
            break;
        }

        numFaceHolder = faces.size(); // Resets comparison var
    }

    return 0;
}