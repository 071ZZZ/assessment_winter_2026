#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "detector.hpp"
static const cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<
    1284.547433507284, 0.0, 651.7508235222139,
    0.0, 1284.2364120767988, 526.5637803861928,
    0.0, 0.0, 1.0
);

static const cv::Mat distCoeffs = (cv::Mat_<double>(5,1) <<
    -0.37358321087789575,
    0.17992488578084675,
    -0.0003940895801771612,
    -0.0007097534681170694,
    0.0
);

static const double real_radius = 0.0084; 

cv::Mat Detector::preprocess(const cv::Mat& img){
    cv::Mat img_hsv,mask;
    cv::cvtColor(img,img_hsv,cv::COLOR_BGR2HSV);
    cv::Scalar lower_green = cv::Scalar(25, 40, 100); 
    cv::Scalar upper_green = cv::Scalar(80, 120, 240);
    inRange(img_hsv, lower_green, upper_green, mask);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);  // 开运算：分离粘连、去小点
    morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel); // 闭运算：修复弹丸轮廓
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
    cv::Mat clean_mask = preprocess(img);
    std::vector<cv::Vec3f> circles = findcircle(clean_mask);

    img_clone = img.clone();
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img_clone, center, radius, cv::Scalar(0, 255, 0), 2, 8, 0);
    
        std::vector<cv::Point3f> object_points;
        std::vector<cv::Point2f> image_points;
        object_points.emplace_back(-real_radius, 0, 0);
        object_points.emplace_back( real_radius, 0, 0);
        object_points.emplace_back(0, -real_radius, 0);
        object_points.emplace_back(0,  real_radius, 0); 
        object_points.emplace_back(0, 0, 0);

        image_points.emplace_back(center.x - radius, center.y);
        image_points.emplace_back(center.x + radius, center.y); 
        image_points.emplace_back(center.x, center.y - radius);
        image_points.emplace_back(center.x, center.y + radius);
        image_points.emplace_back(center.x, center.y);
        cv::Mat rvec, tvec;
        bool success = cv::solvePnP(
            object_points,
            image_points,
            cameraMatrix,
            distCoeffs,
            rvec,
            tvec
        );
        if (success) {
            double distance = tvec.at<double>(2);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(6) << distance << " m";
            cv::putText(
                img_clone,
                ss.str(),
                cv::Point(center.x, center.y - 10),
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                cv::Scalar(0, 255, 255),
                1
            );
        } else {
            std::cerr << "弹丸 " << i << " PnP 求解失败！" << std::endl;
        }
    }
}
int main() {
    cv::Mat img = cv::imread("/home/lqy/assessment_winter_2026/assignment_2/detector/imgs/test1.jpg");
    if (img.empty()) {
        return -1;
    }
    cv::Mat img_clone,img2;
    Detector dec;
    img2=dec.preprocess(img);
    dec.balldetector(img,img_clone);
    cv::imshow("result",img_clone);
    cv::imwrite("/home/lqy/assessment_winter_2026/assignment_2/detector/imgs/result1.jpg",img_clone);
    cv::waitKey(0);
    return 0;
}
