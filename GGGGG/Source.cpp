//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2\opencv.hpp"
#include <math.h>
#include <iostream>
using namespace cv;
using namespace std;

int tempI, tempJ;

void FeatureDection(Mat input, Mat feature)
{
	cout << "Feature Detecting..." << endl;

	Mat featureDetection = input;
	//Mat eye = imread("Russian Empire_feature1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat eye = feature;

	int cut = eye.rows - 1;

	Mat FD_output = Mat(featureDetection.rows, featureDetection.cols, CV_8UC1);

	int indexI, indexJ;
	float min = -10;

	for (int i = cut / 2; i < featureDetection.rows - cut / 2; i++)
	{
		for (int j = cut / 2; j < featureDetection.cols - cut / 2; j++)
		{
			float sum = 0;
			float sum2 = 0;
			float sum3 = 0;
			float gMed = 0, fMed = 0;

			for (int k = -cut / 2; k < cut / 2 + 1; k++)
			{
				for (int l = -cut / 2; l < cut / 2 + 1; l++)
				{
					gMed = gMed + (float)(eye.at<uchar>(k + cut / 2, l + cut / 2));
					fMed = fMed + (float)(featureDetection.at<uchar>(i + k, j + l));
				}
			}

			gMed = gMed / pow((cut + 1), 2);
			fMed = fMed / pow((cut + 1), 2);

			for (int k = -cut / 2; k < cut / 2 + 1; k++)
			{
				for (int l = -cut / 2; l < cut / 2 + 1; l++)
				{
					sum = sum + ((float)(featureDetection.at<uchar>(i + k, j + l)) - fMed)*((float)(eye.at<uchar>(k + cut / 2, l + cut / 2)) - gMed);
				}
			}

			for (int k = -cut / 2; k < cut / 2 + 1; k++)
			{
				for (int l = -cut / 2; l < cut / 2 + 1; l++)
				{

					sum2 = sum2 + pow(((float)(featureDetection.at<uchar>(i + k, j + l)) - fMed), 2);
					sum3 = sum3 + pow(((float)(eye.at<uchar>(k + cut / 2, l + cut / 2)) - gMed), 2);
				}
			}

			//cout << sum / sqrt(sum2 * sum3) << endl;

			if (min < sum / sqrt(sum2 * sum3))
			{
				min = sum / sqrt(sum2 * sum3);
				indexI = i;
				indexJ = j;
			}
		}
	}

	cout << "Index:(" << indexI << "." << indexJ << ")" << endl;

	tempI = indexI;
	tempJ = indexJ;
}


int main(){

	string pictureName;
	string featureName;

	cout << "Please input the file name of picture :" << endl;
	cin >> pictureName;
	cout << "Please input the file name of feature :" << endl;
	cin >> featureName;
	cout << endl;

	// Read input images
	// Picture is in GGGGG\bin
	Mat SrcImg = imread(pictureName, CV_LOAD_IMAGE_GRAYSCALE);

	int height = SrcImg.rows / 3;

	// Create a grayscale output image matrix
	Mat DstImg1 = Mat(height, SrcImg.cols, CV_8UC1);
	Mat DstImg2 = Mat(height, SrcImg.cols, CV_8UC1);
	Mat DstImg3 = Mat(height, SrcImg.cols, CV_8UC1);

	//Mat feature = Mat(21, 21, CV_8UC1);
	Mat feature = imread(featureName, CV_LOAD_IMAGE_GRAYSCALE);
	Mat finalOutput = Mat(height, SrcImg.cols, CV_8UC1);

	// Copy each pixel of the source image to the output image
	for (int i = 0; i < SrcImg.rows / 3; ++i)
	{
		for (int j = 0; j < SrcImg.cols; ++j)
		{
			DstImg1.at<uchar>(i, j) = SrcImg.at<uchar>(i, j);
		}
	}

	for (int i = height; i < height * 2; ++i)
	{
		for (int j = 0; j < SrcImg.cols; ++j)
		{
			DstImg2.at<uchar>(i - height, j) = SrcImg.at<uchar>(i, j);
		}
	}


	for (int i = height * 2; i < SrcImg.rows - 1; ++i)
	{
		for (int j = 0; j < SrcImg.cols; ++j)
		{
			DstImg3.at<uchar>(i - height * 2, j) = SrcImg.at<uchar>(i, j);
		}
	}
	/*
	for (int i = 0; i < feature.rows; ++i)
	{
		for (int j = 0; j < feature.cols; ++j)
		{
			feature.at<uchar>(i, j) = SrcImg.at<uchar>(i + height / 2, j + SrcImg.cols / 2);
		}
	}
	*/

	int index1[2];
	int index2[2];
	int index3[2];
	
	
	FeatureDection(DstImg1,feature);
	index1[0] = tempI;
	index1[1] = tempJ;

	FeatureDection(DstImg2,feature);
	index2[0] = tempI;
	index2[1] = tempJ;

	FeatureDection(DstImg3,feature);
	index3[0] = tempI;
	index3[1] = tempJ;

	int maxI = 0, maxJ = 0;
	
	if (maxI < index1[0])
		maxI = index1[0];

	if (maxI < index2[0])
		maxI = index2[0];

	if (maxI < index3[0])
		maxI = index3[0];

	if (maxJ < index1[1])
		maxJ = index1[1];

	if (maxJ < index2[1])
		maxJ = index2[1];

	if (maxJ < index3[1])
		maxJ = index3[1];

	int offest1_I = maxI - index1[0];
	int offest1_J = maxJ - index1[1];
	int offest2_I = maxI - index2[0];
	int offest2_J = maxJ - index2[1];
	int offest3_I = maxI - index3[0];
	int offest3_J = maxJ - index3[1];
	
	for (int i = 0 + offest1_I; i < SrcImg.rows / 3; ++i)
	{
		for (int j = 0 + offest1_J; j < SrcImg.cols; ++j)
		{
			DstImg1.at<uchar>(i, j) = SrcImg.at<uchar>(i - offest1_I, j - offest1_J);
		}
	}

	for (int i = height; i < height * 2; ++i)
	{
		for (int j = 0 + offest2_J; j < SrcImg.cols; ++j)
		{
			DstImg2.at<uchar>(i - height, j) = SrcImg.at<uchar>(i - offest2_I, j - offest2_J);
		}
	}


	for (int i = height * 2; i < SrcImg.rows - 1; ++i)
	{
		for (int j = 0 + offest3_J; j < SrcImg.cols; ++j)
		{
			DstImg3.at<uchar>(i - height * 2, j) = SrcImg.at<uchar>(i - offest3_I, j - offest3_J);
		}
	}
	
	Mat channel[3] = { DstImg1, DstImg2, DstImg3 };
	merge(channel, 3, finalOutput);
	
	// Show images
	imshow("Input Image", SrcImg);
	imshow("Output Image1", DstImg1);
	imshow("Output Image2", DstImg2);
	imshow("Output Image3", DstImg3);
	imshow("Final Output", finalOutput);

	imshow("Feature", feature);
	
	// Write output images
	imwrite("Final Output.jpg", finalOutput);

	waitKey(0);
	return 0;
}