/**************************************************
FILE   : base.h
INSTRUCTION : basic algorithm of image processing
AUTHOR：    GPF
DATE :    2016/02/25
copyright by Xia'men Flywins Technology Co., Ltd.
***************************************************/
#pragma once
#include <stdio.h>
#include <string.h>
void  rgb2gray(unsigned char* srcImage, 
		unsigned char* dstImage, 
		int imgWidth,
              	int imgeHeight, 
              	int dataOrder);


void rgbSeparate(unsigned char* srcImage, 
		     unsigned char* rComponent, 
		     unsigned char* gComponent,
                 	     unsigned char* bComponent, 
                 	     int imgHeight, 
                 	     int imgWidth, 
                 	     int dataOrder);


void imgRotated(unsigned char* srcImage, 
		    unsigned char* dstImage, 
		    int imgHeight, 
		    int imgWidth, 
		    int angle, 
		    int biBitCount);

void bubble_sort(int array[], int arraySize);

void bubbleSort(int inputArray[],int outputArray[],int arraySize);

void medianFilter(unsigned char* srcImage, 
		      unsigned char* dstImage, 
		      int imgWidth, 
		      int imgHeight);

void gradAImageFilter(unsigned char* srcImage,
				unsigned char*gradAImage,
				int imageWidth,
				int imageHeight);