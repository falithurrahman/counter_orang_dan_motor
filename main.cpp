#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main() {
	Mat frame;
	Mat gray;
	Mat it0;
	Mat it1;
	Mat hasil;
	int cnt = 0;
	Scalar rata2;
	
	Mat element = getStructuringElement(MORPH_RECT, Size(13, 13), Point(-1, -1));
	Mat element_close = getStructuringElement(MORPH_ELLIPSE, Size(9, 9), Point(-1, -1));
	Mat element_open = getStructuringElement(MORPH_ELLIPSE, Size(9, 9), Size(-1, -1));

	vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;

	VideoCapture capture("maul.mp4");

	int deteksimotor = 0, deteksiorang=0;
	int status = 0;

	for (;;) {
		capture >> frame;
		if (frame.empty()) break;
		//Mat roi = frame(Rect(600, 540, 600, 500));
		Mat roi = frame(Rect(350, 200, 200, 200));
		imshow("original", frame);
		imshow("test", roi);

		cvtColor(roi, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(7, 7), 0);

		it0 = gray.clone();
		if (cnt == 0) {
			it1 = it0.clone();
			cnt = 1;
			hasil = it0.clone();
		}

		absdiff(it0, it1, hasil);

		it1 = it0.clone();

		//rata2 = mean(hasil);
		
		threshold(hasil, hasil, 10, 255, THRESH_BINARY);
		dilate(hasil, hasil, element);
		dilate(hasil, hasil, element);
		//dilate(hasil, hasil, element);
		morphologyEx(hasil, hasil, MORPH_OPEN, element_open, Point(-1, -1));
		morphologyEx(hasil, hasil, MORPH_CLOSE, element_close, Point(-1, -1));

		findContours(hasil, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

		vector<Rect> boundrect(contours.size());

		Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
		

		for (int i = 0; i < contours.size(); i++)
		{
			drawContours(drawing, contours, i, Scalar(128, 255, 255), 1, 8, vector<Vec4i>(), 0, Point());
			boundrect[i] = boundingRect(contours[i]);
			rectangle(drawing, boundrect[i], Scalar(255, 255, 0), 2);
			
			Moments m = moments(contours[i], true);
			Point p(m.m10 / m.m00, m.m01 / m.m00);
			circle(drawing, p, 5, Scalar(128, 0, 0), -1);

			//cout << p.x << "," << p.y << endl;
			//int luasbaru = boundrect[i].area()*boundrect[i].area();
			cout << boundrect[i].area() << endl;
			

			if (p.y<100) {
				status = 1;
			}
			else if (status == 1 && p.y>=100) {
				//deteksi++;
				//status = 0;
				if (boundrect[i].area() > 7000 && boundrect[i].area() < 10000) {
					deteksimotor++;
					status = 0;
				}
				else if (boundrect[i].area() > 2000 && boundrect[i].area() < 6000) {
					deteksiorang++;
					status = 0;
				}
			}
		}
		putText(drawing, format("orang:%d", deteksiorang), Point(30, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 200, 200), 1);
		putText(drawing, format("motor:%d", deteksimotor), Point(30, 50), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(50, 200, 200), 1);

		imshow("threshold", hasil);
		imshow("hasil", drawing);
		if (waitKey(10) == 27)break;

	}
	return 0;
}
