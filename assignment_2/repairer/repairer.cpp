#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
Mat removeNoiseWithMedian(const Mat& input) {
    Mat result;
    medianBlur(input, result, 3); 
    Mat bilateral;
    bilateralFilter(result, bilateral, 5, 50, 50);
    Mat sharpen_kernel = (Mat_<float>(3,3) << 
        0, -0.3, 0,
        -0.3, 2.2, -0.3,
        0, -0.3, 0);
    Mat sharpened;
    filter2D(bilateral, sharpened, -1, sharpen_kernel);
    return sharpened;
} //中值滤波去噪

int main() {
    Mat img = imread("/home/lqy/assessment_winter_2026/assignment_2/repairer/imgs/noisy.jpg");
    if (img.empty()) {
        return -1;
    }
    Mat output = removeNoiseWithMedian(img);
    imshow("original", img);
    imshow("medianBlur result", output);
    imwrite("/home/lqy/assessment_winter_2026/assignment_2/repairer/imgs/repair.jpg",output);
    waitKey(0);
    return 0;
}

