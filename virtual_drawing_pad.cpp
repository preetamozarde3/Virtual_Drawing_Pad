#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture cap(0); //capture the video from webcam
	if (!cap.isOpened()) //if not success, exit program
	{
		cout << "Canot open the web cam" << endl;
		return -1;
	}

	namedWindow("Drawing Pad", CV_WINDOW_AUTOSIZE); //create a window called "Drawing Pad"
	namedWindow("Original", CV_WINDOW_AUTOSIZE);

	int iLastX = -1;
	int iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_BUC1);
	Mat channels[3];
	Mat circles;
	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); //read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl
			break;
		}
		split(imgOriginal, channels);
		channels[0]=channels[0]-(channels[1]); //extracting blue color

		Mat imgThresholded;

		//morphological opening (removes small objects from the foreground)
		erode(channels[0],channels[0],getStructureElement(MORPH_ELLIPSE, Size(5,5));
		dilate(channels[0],channels[0],getStructureElement(MORPH_ELLIPSE, Size(5,5));

		//morphological closing (removes small holes from the foreground)
		erode(channels[0],channels[0],getStructureElement(MORPH_ELLIPSE, Size(5,5));
		dilate(channels[0],channels[0],getStructureElement(MORPH_ELLIPSE, Size(5,5));

		GuassianBlur(channels[0],channels[0],Size(5,5),0,0);
		//medianBlur(imgThresholded, imgThresholded,5)

		threshold(channels[0],imgThresholded,10,255,THRESH_BINARY);

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		//if the area <= 10000, I consider that there are no object in the image and it's because of the noise and area is not zero
		if (dArea > 500000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >=0 && posY >=0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), 255, 2);
			}

			iLastX = posX;
			iLastY = posY;
		}

		//imgLines+=imgThresholded;
		imshow("Original", imgOriginal);
		imshow("Pointer", imgThresholded);
		imshow("Drawing Pad", imgLines); //show the pad image
		if (waitKey(30) == 27) //wait for 'esc' key process for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}
