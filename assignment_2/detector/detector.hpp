#ifndef detector_hpp
#define detector_hpp
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
class Detector
{
    public:
    cv::Mat preprocess(const cv::Mat& img);//预处理（HSV+膨胀腐蚀）
    std::vector<cv::Vec3f> findcircle(const cv::Mat& mask);//找圆
    void balldetector(const cv::Mat& img,cv::Mat& img_clone);//画圆
    
};

#endif