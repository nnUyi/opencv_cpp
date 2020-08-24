/*
 * is_cam_trans.hpp
 * Copyright (C) 2020 chadyang <chadyang@tencent.com>
 * Create time: 2020-05-20 16:52
 * Distributed under terms of the Tencent license.
 */

#include <opencv2/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <string.h>
using namespace cv;

class CameraDetector {
 public:
   // 构造函数
   CameraDetector();
   // 析构函数
   ~CameraDetector();

 public:
   bool is_cam_trans(String cap_path);
   bool abs_frame(Mat queue_gray);
   Mat crop_image_top(Mat img);
   Mat crop_image_bottom(Mat img);
   Mat resize_image(Mat img);
   Mat image_preprocess(Mat img);
   bool sift_detection(Mat img, Mat kp_img, Mat des_img, Mat rect_top, Mat rect_bottom);
};
