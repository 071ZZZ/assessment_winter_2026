#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "detector.hpp"
cv::Mat Detector::preprocess(const cv::Mat& img){
    cv::Mat img_hsv,mask;
    cv::cvtColor(img,img_hsv,cv::COLOR_BGR2HSV);
    cv::Scalar lower_green = cv::Scalar(35,40,40);
    cv::Scalar upper_green = cv::Scalar(77, 255, 255);
    inRange(img_hsv, lower_green, upper_green, mask);
    cv::Mat img_kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask,cv:: MORPH_CLOSE, img_kernel);
    return mask;
}
std::vector<cv::Vec3f> Detector::findcircle(const cv::Mat& mask) {
    std::vector<cv::Vec3f> circles;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        double perimeter = cv::arcLength(contours[i], true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);
        if (area > 500 && circularity > 0.5) { 
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contours[i], center, radius);
            circles.push_back(cv::Vec3f(center.x, center.y, radius));
        }
    }

    return circles;
}
void Detector::balldetector(const cv::Mat& img, cv::Mat& img_clone) {
    cv::Mat mask = preprocess(img);
    std::vector<cv::Vec3f> circles = findcircle(mask);

    img_clone = img.clone();
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img_clone, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
        circle(img_clone, center, radius, cv::Scalar(0, 255, 0), 2, 8, 0);
    }
}
int main() {
    cv::Mat img = cv::imread("/home/lqy/assessment_winter_2026/assignment_2/detector/imgs/test2.jpg");
    if (img.empty()) {
        return -1;
    }
    cv::Mat img_clone,img2;
    Detector dec;
    img2=dec.preprocess(img);
    dec.balldetector(img,img_clone);
    cv::imshow("result",img_clone);
    cv::waitKey(0);
    return 0;
}
