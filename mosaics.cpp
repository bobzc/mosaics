#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <string>


#include <math.h>
#include <limits.h>
#include <stdio.h>

#define DB_PATH "./src/" 							// path of the database
#define IMG_SRC_PATH "./src/" 					  	 // path of image sources
#define PROJ_PATH "/home/bobzc/pool/mosaics/"         // path of the project
#define RST_IMG_PATH "./result/"						 // path of result images
#define TMP_IMG_PATH "./target/"

#define MAXSIZE 256
#define MINSIZE 16
#define NUMOFIMG 2088
#define THRESHOLD 7500

using namespace cv;
using namespace std;


string to_string(int);
void load_db(Mat *, int, int);
void show_image(Mat);

int main(int argc, char* argv[]){
	string fpath("");
	string fname;
	
	// get the path of the target image
	cout << "Input the name of the target image: ";
	cin >> fname;
	fpath.append(IMG_SRC_PATH).append(fname);
	cout << "Processing " << fpath << " ..." << endl;

	//load target image
	Mat src = imread(fpath, 1);

	// result image
	Mat result = src.clone();

	// match
	for (int i = 0; i < src.cols/MINSIZE; i++){
		for (int j = 0; j < src.rows/MINSIZE; j++){
			int origin_x = i * MINSIZE;
			int origin_y = j * MINSIZE;
		
			Mat tile (src, Rect(origin_x, origin_y, MINSIZE, MINSIZE));

			// invoke script to match image
			string tmp_path(TMP_IMG_PATH);
			imwrite(tmp_path.append("tmp.jpg"), tile);
			string commend("./match_img.py "); 
			system(commend.append("tmp.jpg").c_str());

			// get result
			string img_name;
			ifstream fp("tmp.txt");
			getline(fp, img_name);

			// read image
			string img_path("");
			img_path.append(IMG_SRC_PATH).append(to_string(MINSIZE)).append("/").append(img_name);
			Mat match = imread(img_path, 1);
			match.copyTo(result(Rect(origin_x, origin_y, MINSIZE, MINSIZE)));
		}
		cout << i / (src.cols/ (float) MINSIZE) * 100 << "%" << endl;
	}

	string rst_path(RST_IMG_PATH);
	imwrite(rst_path.append(fname) ,result);
}


// convert integer to string
string to_string(int number){
    string number_string = "";
    char ones_char;
    int ones = 0;
    while(true){
        ones = number % 10;
        switch(ones){
            case 0: ones_char = '0'; break;
            case 1: ones_char = '1'; break;
            case 2: ones_char = '2'; break;
            case 3: ones_char = '3'; break;
            case 4: ones_char = '4'; break;
            case 5: ones_char = '5'; break;
            case 6: ones_char = '6'; break;
            case 7: ones_char = '7'; break;
            case 8: ones_char = '8'; break;
            case 9: ones_char = '9'; break;
			default: perror("Trouble in converting int to string!");
        }
        number -= ones;
        number_string = ones_char + number_string;
        if(number == 0){
            break;
        }
        number = number/10;
    }
    return number_string;
}


void load_db(Mat *db, int img_num, int scl_num){
	printf("Loading data...");
	for (int i = 1; i <= img_num ;i++){
		for ( int j = 0; j < scl_num; j ++){
			int k = 16 * pow(2, j);
			string img_path(DB_PATH);
			img_path.append(to_string(k)).append("/");
			img_path.append(to_string(i)).append(".jpg");
			db[5 * (i-1) + j] = imread(img_path, 1);
		}
	}
	printf("\t[ OK ]\n"); 
}


void show_image(Mat image){
	namedWindow("Show Image", CV_WINDOW_AUTOSIZE);
	imshow("Show Image", image);
	waitKey(0);
}



