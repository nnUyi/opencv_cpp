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
   CameraDetector() {}
   // 析构函数
   ~CameraDetector() {}

 public:
   void is_cam_trans(char* cap_path);
   void is_cam_trans_re(Mat cur_frame, Mat pre_frame);
   // 帧差法
   bool abs_frame(Mat cur_frame, Mat pre_frame, bool &is_trans, Mat& mask, float &percent);
   void crop_image_top(Mat img, Point rook_points[1][4]);
   void crop_image_bottom(Mat img, Point rook_points[1][4]);
   void resize_image(Mat& img);
   // 图像预处理
   void image_preprocess(Mat& img, Point top_rook_points[1][4], Point bottom_rook_points[1][4]);
   bool sift_detection(Mat img, Mat kp_img, Mat& des_img, Mat rect_top, Mat rect_bottom); 
};
