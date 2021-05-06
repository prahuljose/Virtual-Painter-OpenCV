#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

float width = 250, height = 350;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

vector<vector<int>> myColors
{
	{86,125,121,255,82,233} //blue
};

vector<Scalar> myColorValues
{
	{255,0,0} //blue
};

vector<vector<int>> myPoints;

Mat img;

Point getContours(Mat imgMask)
{

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> contourPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area >= 300)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], contourPoly[i], 0.02 * peri, true);
			//drawContours(img, contourPoly , i, Scalar(0, 0, 255), 3);
			cout << contourPoly[i] << endl;
			boundRect[i] = boundingRect(contourPoly[i]);
			myPoint.x = boundRect[i].x;
			myPoint.y = boundRect[i].y;

			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 255), 4);
			
		}
	}
	return(myPoint);
}


void findColor(Mat img)
{
	Mat imgHSV, imgMask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][2], myColors[i][4]);
		Scalar upper(myColors[i][1], myColors[i][3], myColors[i][5]);

		inRange(imgHSV, lower, upper, imgMask);
		imshow(to_string(i), imgMask);
		Point myPoint=getContours(imgMask);
		if (myPoint.x != 0 && myPoint.y!=0)
		{
			myPoints.push_back({ myPoint.x, myPoint.y,i });
		}
		
	}
}

void drawOnCanvas(vector<vector<int>> myPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < myPoints.size(); i++)
	{
		circle(img, (Point(myPoints[i][0], myPoints[i][1])),7,Scalar(myColorValues[myPoints[i][2]]),FILLED);
	}
}

/// Project 1
void main()
{
	VideoCapture cap(1);
	

	while (true)
	{
		cap.read(img);
		findColor(img);
		drawOnCanvas(myPoints, myColorValues);
		imshow("Original_Image", img);
		waitKey(1);
	}
	waitKey(0);
}
