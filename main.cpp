#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

constexpr unsigned TRIANGLE = 3;		//sum of corner
constexpr unsigned SQUARE = 4;
constexpr unsigned PENTA = 5;
constexpr unsigned HEXA = 6;

void label(Mat img,const String& lb,vector<Point>& pn)
{
	int font_type = FONT_HERSHEY_SIMPLEX;
	double scale = 0.65;
	int thickness = 2;
	int baseline = 1;
	Size text_sz = getTextSize(lb,font_type,scale,thickness,&baseline);
	Rect r = boundingRect(pn);
	Point p{r.x+(r.width-text_sz.width)/2,r.y+(r.height-text_sz.height)/2};
	rectangle(img,p+Point{0,baseline},p+Point{text_sz.width,-text_sz.height},Scalar{230,255,200},-1);
	putText(img,lb,p,font_type,scale,Scalar{0,0,255},thickness);
}

void mark_object_detected(vector<Point> cn,const vector<Point>& point,
	Mat img,unsigned pn_sz,const string& label_img)
{
	Rect r = boundingRect(cn);
	rectangle(img,r,Scalar(255,2,22));
	for(unsigned i=0; i < pn_sz; ++i)
		circle(img,point[i],8,Scalar(100,100,1),-1);	
	label(img,label_img,cn);
	cout<<label_img<<" detected\n";
}

int main()
{
	namedWindow("win");
	Mat im = imread("../../all.png");	
	
	Mat gray;
	//convert the original image into 1-channel color (gray)
	cvtColor(im,gray,COLOR_RGB2GRAY);

	vector<vector<Point>> contours;
	vector<Point> pn;

	string img_label;	//label for object detected
	
	//first find the contour of all images (objects)
	findContours(gray,contours,RETR_LIST,CHAIN_APPROX_SIMPLE);


	for(unsigned int i=0;i<contours.size();++i)
	{
		//approximate a polygonal curve(s)
		approxPolyDP(contours[i],pn,arcLength(contours[i],true)*0.02,true);
		//give parameter for the window not detected**
		if(contourArea(contours[i]) > 100 && contourArea(contours[i]) <= 90000)
		{
			unsigned sum_of_pn = pn.size();	//sum of corner
			
			switch(sum_of_pn)		//check for number of corner
			{
				case TRIANGLE: img_label = "triangle"; break;
				case SQUARE: img_label = "square"; break;
				case PENTA: img_label = "penta"; break;
				case HEXA: img_label = "hexa";
			}
			//attach name (mark) to the object shape detected
			mark_object_detected(contours[i],pn,im,sum_of_pn,img_label);
		}
	}

	imshow("win",im);
	
	//quit when 'q' button is pressed
	while(static_cast<char>(waitKey(1)) != 'q');

	return 0;
}


/**note that this is where the problem on 
	"Detecting Basic Shape with OpenCV by Corners Quantity" 
	article is solved: by these parameters, the window (having 4 corners also)
	won't be detected when square is intended
*/


