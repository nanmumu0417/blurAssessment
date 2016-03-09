/**************************************************
FILE   : base.c
INSTRUCTION : basic algorithm of image processing
AUTHOR：    GPF
DATE :    2016/02/24
copyright by Xia'men Flywins Technology Co., Ltd.
***************************************************/
//2017/08/10 edited, test for gitkraken

#include "base.h"
#include <math.h>

#define PI 3.1415926535
#define RADIAN(angle) ((angle)*PI/180.0) //converts angle to radian
#define GRAD_PARA 0.25
#define BASE_ABS(a) ( (a)> 0? (a):-(a) )


void  rgb2gray(unsigned char* srcImage, unsigned char* dstImage, int imgWidth,
			   int imgeHeight, int dataOrder)//dataOrder 1:RGB;2:BGR
{
	int x, y;
	unsigned char* srcTemp;
	unsigned char* srcLine;
	unsigned char* dstTemp;
            int lineByte;
             int lineByte2;          
               lineByte =  (imgWidth*3 + 3) / 4 * 4;
            lineByte2 = (imgWidth+3) / 4 *4;
	if (srcImage == NULL || dstImage == NULL)
	{
		printf("Invalid argument[function @rgb2gray]\n");
		return;
	}



	for (y = 0; y < imgeHeight; y++)
	{
		srcLine = srcImage + lineByte * y;
		for (x = 0; x < imgWidth; x++)
		{
			srcTemp = srcLine + 3 * x;
			if (dataOrder == 1)
				dstImage[y*lineByte2 + x] = ((*srcTemp + 2) * 117 + (*(srcTemp + 1)) * 601 + (*(srcTemp)) * 306) >> 10;
			else if (dataOrder == 2)
				dstImage[y*lineByte2 + x] = ((*srcTemp) * 117 + (*(srcTemp + 1)) * 601 + (*(srcTemp + 2)) * 306) >> 10;
			
		}
	}

}

void rgbSeparate(unsigned char* srcImage, unsigned char* rComponent, unsigned char* gComponent,
				 unsigned char* bComponent, int imgHeight, int imgWidth, int dataOrder)//dataOrder 1:RGB;2:BGR
{
	int x, y;
	unsigned char* rTemp;
	unsigned char* gTemp;
	unsigned char* bTemp;
	unsigned char* srcTemp;
	unsigned char* srcLine;
            int lineByte;
            int lineByte2;            
            lineByte =  (imgWidth*3 + 3) / 4 * 4;
            lineByte2 = (imgWidth+3) / 4 *4;
	if (srcImage == NULL || rComponent == NULL || gComponent == NULL || bComponent == NULL)
	{
		printf("Invalid argument[function @rgbSeparate]\n");
		return;
	}

	rTemp = rComponent;
	gTemp = gComponent;
	bTemp = bComponent;

	for (y = 0; y < imgHeight; y++)
	{
		srcLine = srcImage + lineByte * y;
		for (x = 0; x < imgWidth; x++)
		{
			srcTemp = srcLine + 3 * x;
			if (dataOrder == 1)
			{
				rComponent[y*lineByte2 + x] = *srcTemp;
				gComponent[y*lineByte2 + x] = *(srcTemp + 1);
				bComponent[y*lineByte2 + x] = *(srcTemp + 2);
			}
			else if (dataOrder == 2)
			{
				rComponent[y*lineByte2 + x] = *(srcTemp + 2);
				gComponent[y*lineByte2 + x] = *(srcTemp + 1);
				bComponent[y*lineByte2 + x] = *srcTemp;
			}

			
			
	
		}

	}

}
//Clockwise rotation,around its center
//The origin of coordinates is at the up-left corner of the sorce image
/*********************************************Conversion matrix***************************
|      1           0        0 |   |  cosA   sinA   0  |    |    1           0       0 |
(X0 Y0 1) = (X1 Y1 1)  |      0          -1        0 |   | -sinA   cosA   0  |    |    0          -1      0 |
|  -1/2Wn   1/2Hn   1 |   |    0         0      1  |    | 1/2Wo  1/2Ho   1 |
**********************************************************************************************/
void imgRotated(unsigned char* srcImage, unsigned char* dstImage, int imgHeight, int imgWidth, int angle, int biBitCount)
{
	//define the vertices coordinate in the new coordinate system
	float src_x1, src_y1, src_x2, src_y2, src_x3, src_y3, src_x4, src_y4;
	float dst_x1, dst_y1, dst_x2, dst_y2, dst_x3, dst_y3, dst_x4, dst_y4;
	int x, y, x0, y0; // x0, y0 is for the sorce image
	float t1, t2;
	int lineByte, lineByte2;
	//the size of the image after rotation
	int wnew, hnew;
	float sina, cosa, temp1, temp2, alpha;
	if (srcImage == NULL || dstImage == NULL)
	{
		printf("Invalid argument[function @imgRotated]\n");
		return;
	}
	////converts angle to radian
	alpha = RADIAN(angle);

	cosa = float(cos(double(alpha)));
	sina = float(sin(double(alpha)));

	//vertices coordinate of the sorce image
	src_x1 = float(-0.5 * imgWidth);
	src_y1 = float(0.5 * imgHeight);
	src_x2 = float(0.5 * imgWidth);
	src_y2 = src_y1;
	src_x3 = src_x1;
	src_y3 = float(-0.5 * imgHeight);
	src_x4 = src_x2;
	src_y4 = src_y3;

	//vertices coordinate of the new image
	dst_x1 = cosa * src_x1 + sina * src_y1;
	dst_y1 = -sina * src_x1 + cosa * src_y1;

	dst_x2 = cosa * src_x2 + sina * src_y2;
	dst_y2 = -sina * src_x2 + cosa * src_y2;

	dst_x3 = cosa * src_x3 + sina * src_y3;
	dst_y3 = -sina * src_x3 + cosa * src_y3;

	dst_x4 = cosa * src_x4 + sina * src_y4;
	dst_y4 = -sina * src_x4 + cosa * src_y4;

	//calculate the size of the image after rotation
	t1 = fabs(dst_x4 - dst_x1) + 1;
	t2 = fabs(dst_x3 - dst_x2) + 1;
	wnew = int(t1 > t2 ? t1 : t2);
	t1 = fabs(dst_y4 - dst_y1) + 1;
	t2 = fabs(dst_y3 - dst_y2) + 1;
	hnew = int(t1 > t2 ? t1 : t2);

	// calculate the two constants
	temp1 = float(-0.5 * wnew * cosa - 0.5 * hnew * sina + 0.5 * imgWidth);
	temp2 = float(-0.5 * wnew * sina + 0.5 * hnew * cosa + 0.5 * imgHeight);
	////4 byte alignment
	lineByte = (imgWidth * biBitCount / 8 + 3) / 4 * 4;
	lineByte2 = (wnew * biBitCount / 8 + 3) / 4 * 4;
	dstImage = new unsigned char[lineByte2 * hnew];
	//set the new image color
	memset(dstImage, 0, lineByte2 * hnew);



	for (y = 0; y < hnew; y++)
		for (x = 0; x < wnew; x++)
		{
			x0 = int(x * cosa - y * sina + temp1);
			y0 = int(x * sina + y * cosa + temp2);

			if ( (x0 >= 0) && (x0 < imgWidth) && (y0 >= 0) && (y0 < imgHeight))
			{
				*(dstImage + lineByte2 * y + x) = *(srcImage + lineByte * y0 + x0);
			}
		}
}


void bubble_sort(int array[], int arraySize)
{
	int i, j, temp;

	for (i = 0; i < arraySize - 1; i++)
	{
		for (j = 0; j < arraySize - 1 - i; j++)
		{
			if (array[j] > array[j + 1])
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}

		}
	}
}

void bubbleSort(int inputArray[],int outputArray[], int arraySize)
{
	int i, j,k,temp;
             for (k = 0; k < arraySize; ++k)
             {
             	outputArray[k] = inputArray[k];
             }
	for (i = 0; i < arraySize - 1; i++)
	{
		for (j = 0; j < arraySize - 1 - i; j++)
		{
			if (outputArray[j] > outputArray[j + 1])
			{
				temp = outputArray[j];
				outputArray[j] = outputArray[j + 1];
				outputArray[j + 1] = temp;
			}

		}
	}


}

void medianFilter(unsigned char* srcImage, unsigned char* dstImage, int imgWidth, int imgHeight)
{
	int x, y, k;
	int wndGrayValue[9];
	unsigned char* srcTempLine0;
	unsigned char* srcTempLine1;
	unsigned char* srcTempLine2;
            int lineByte2;
            lineByte2 = (imgWidth+3) / 4 *4;
	if (srcImage == NULL || dstImage == NULL)
	{
		printf("Invalid argument[function @medianFilter]\n");
		return;
	}
	

	for (y = 1; y < imgHeight - 2; ++y)
	{
      		
		for (x = 1; x < imgWidth - 2; ++x)
		{
			
			srcTempLine0 = srcImage + (y - 1) * lineByte2 + x -1;
			srcTempLine1 = srcImage + y * lineByte2 + x - 1;
			srcTempLine2 = srcImage + (y + 1) * lineByte2 + x -1;

			for (k = 0; k < 3; ++k)
			{
				wndGrayValue[0 + k * 3] = *srcTempLine0;
				wndGrayValue[1 + k * 3] = *srcTempLine1;
				wndGrayValue[2 + k * 3] = *srcTempLine2;

				srcTempLine0++;
				srcTempLine1++;
				srcTempLine2++;
			}

			bubble_sort(wndGrayValue, 9);

			dstImage[y*lineByte2 + x] = (unsigned char)wndGrayValue[4];
			
		}

	}

}

void gradAImageFilter(unsigned char* srcImage,unsigned char*gradAImage,int imageWidth,int imageHeight)
{
	int x, y;
	int detaX;
	int detaY;
	float gradAI;
     	int lineByte2;
            lineByte2 = (imageWidth+3) / 4 *4;
	if (srcImage == NULL || gradAImage == NULL)
	{
		printf("Invalid argument[function @gradAImageFilter]\n");
		return;
	}
 

	for (y = 1; y < imageHeight-1; y++)
	{
		for (x = 1; x < imageWidth -1; x++)
		{
			detaX = BASE_ABS(srcImage[y*lineByte2+x+1] - srcImage[y*lineByte2+x-1]);
			detaY = BASE_ABS(srcImage[(y+1)*lineByte2 + x] - srcImage[(y-1)*lineByte2 + x]);
			
			gradAI = sqrt((detaX*detaX+detaY*detaY)/(detaX*detaX+detaY*detaY+100.00));
			// *dstTemp = gradAI * 255;
                                    //gradAI = detaX + detaY;
                                    gradAImage[y*lineByte2 + x]= gradAI *255;
		            /*if(gradAI > 0.5)
				*dstTemp = 255;
			else
				*dstTemp = 0;*/
		}
	}
}