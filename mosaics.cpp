#define _CRT_SECURE_NO_DEPRECATE
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>

#define DB_PATH "./src/" 							// path of the database
#define IMG_SRC_PATH "./src/" 					  	 // path of image sources
#define PROJ_PATH "/home/bobzc/pool/mosaics/"         // path of the project
#define RST_IMG_PATH "./result/"						 // path of result images


#define MAXSIZE 256
#define MINSIZE 16
#define NUMOFIMG 2088
#define THRESHOLD 7500

using namespace cv;
using namespace std;

IplImage *match(IplImage *src, int searchRange, IplImage *db[] )
{
	int i,j,k;
	int matchIndex = 0;
	int maxValid = 0;
	IplImage* src16 = cvCreateImage(cvSize(MINSIZE,MINSIZE), src->depth,src->nChannels );
	cvResize(src, src16, CV_INTER_LINEAR);
	uchar * src16Data = (uchar *)src16->imageData;
	int minDiff = INT_MAX ;
	for(int searchIndex = 1; searchIndex <= searchRange ;searchIndex++)
	{
		int valid = 0;
		IplImage* temp16 = db[5*(searchIndex-1)];
		uchar * tempData = (uchar *)temp16->imageData;
		for ( i = 0; i < src16->height ; i++)
			for ( j = 0; j < src16->width ; j++)
			{
				int diff = 0;
				for (k = 0; k < src16->nChannels ; k++)
				{
					int pixel1 = src16Data[i*src16->widthStep+j*src16->nChannels+k];
					int pixel2 = tempData[i*src16->widthStep+j*src16->nChannels+k];
					diff += ( pixel1 - pixel2 ) * (pixel1 - pixel2);
				}
				if ( diff<THRESHOLD )
				{
					valid++;
				}
			}
		if (valid > maxValid)
		{
			maxValid = valid;
			matchIndex = searchIndex;
		}
	}
	if (maxValid < src16->imageSize * 0.2)
	{
		return src16;
	}
	IplImage* matchedImage = db[5*(matchIndex-1)];
	cvReleaseImage(&src16);
	return matchedImage;

}

int main(int argc, char* argv[]){
	char fpath[256];
	char fname[128];
	
	// get the path of the target image
	printf("Input the name of the target image:\t");
	scanf("%s", fname);
	sprintf(fpath, "%s%s", IMG_SRC_PATH, fname);
	printf("Processing %s...\n", fpath);

	// load target image
	IplImage *src = cvLoadImage(fpath, 1);

	// load database images
	IplImage *db[NUMOFIMG * 5];
	printf("Loading data...");
	for (int i = 1; i <= NUMOFIMG ;i++)
	{
		for ( int j = 0; j < 5; j ++)
		{
			int k = 16 * pow(2, j);
			sprintf( fpath, "%s%d/%d.jpg", DB_PATH, k, i );		
			db[5 * (i-1) + j] = cvLoadImage(fpath, 1);
		}
	}
	printf("\t[ OK ]\n");  

	// match
	IplImage* matchTile;
	IplImage* result  = cvCreateImage(cvSize(src->width,src->height ),src->depth,src->nChannels );
	for (int i = 0; i < src->width/MINSIZE; i++)
	{
		for (int j = 0; j < src->height/MINSIZE; j++)
		{
			int origin_x = i * MINSIZE;
			int origin_y = j * MINSIZE;

			CvRect rect = cvRect( origin_x, origin_y, MINSIZE, MINSIZE);
			cvSetImageROI( src, rect );
			IplImage* dst = cvCreateImage(cvSize(MINSIZE ,MINSIZE ),src->depth,src->nChannels );
			cvCopy( src , dst , 0 );  
			cvResetImageROI( src );

			matchTile = match(dst, NUMOFIMG , db);

			for (int x = 0; x<MINSIZE; x++){
				for (int y = 0; y < MINSIZE; y++){
					for (int z = 0; z<3;z++){
						result->imageData[(j*MINSIZE+x)*result->widthStep+(i*MINSIZE+y)*result->nChannels+z] 
							= matchTile->imageData[x*matchTile->widthStep+y*matchTile->nChannels+z];
					}
				}
			}
			
			cvReleaseImage(&dst);
		}
		printf("\t%d%%\tFinished\n", 100* i / (src->width/MINSIZE));
	}

	// get result image path
	char rst_path[256];
	sprintf(rst_path, "%s%s", RST_IMG_PATH, fname);

	// save result image
	printf("Saving result image in %s ...\n", rst_path);
	cvSaveImage(rst_path, result);

	// release resources
	cvReleaseImage(&src);
	//cvReleaseImage(&result);
	for (int i = 0; i< NUMOFIMG ; i++) 
		for (int j =0; j< 5; j++)
			cvReleaseImage(&db[5 * i + j]);
	return 0;

	//IplImage *matchedImage = match(src , 2000);
	//cvNamedWindow("Target Image", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Target Image", src);
	//cvNamedWindow("Matched Image", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Matched Image", matchedImage);
	//cvWaitKey(0);
	//cvReleaseImage(&src);
	//cvReleaseImage(&matchedImage);
	//cvDestroyAllWindows();
	//targetLoad();
}
