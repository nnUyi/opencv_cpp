/*
 * is_cam_trans.cpp
 * Copyright (C) 2020 chadyang <chadyang@tencent.com>
 * Create time: 2020-05-20 16:52
 * Distributed under terms of the Tencent license.
 */

#include "is_cam_trans.h"
#include<iostream>

using namespace std;

// 全局变量
const int THRESHOLD = 0;

bool CameraDetector::is_cam_trans(String cap_path) {
    return true;
}

bool CameraDetector::abs_frame(Mat queue_gray) {
    return true;
}

Mat CameraDetector::crop_image_top(Mat img) {
    return img;
}
Mat CameraDetector::crop_image_bottom(Mat img) {
    return img;
}
Mat CameraDetector::resize_image(Mat img) {
    return img;
}
Mat CameraDetector::image_preprocess(Mat img) {
    return img;
}

bool CameraDetector::sift_detection(Mat img, Mat kp_img, \
                                        Mat des_img, Mat rect_top, \
                                        Mat rect_bottom) {
    return true;                               
}

int main() { 
    cout << "hello world" << endl;
    return 0;
}
