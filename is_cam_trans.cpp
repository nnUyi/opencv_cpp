/*
 * is_cam_trans.cpp
 * Copyright (C) 2020 chadyang <chadyang@tencent.com>
 * Create time: 2020-05-20 16:52
 * Distributed under terms of the Tencent license.
 */

#include "is_cam_trans.h"
#include <iostream>
#include <queue>

using namespace std;
using namespace cv;

// 全局变量
const int CROP_SPLIT = 8;
const int TRANS_ANGLE_THRESHOLD = 10;
const int KEYPOINTS_THRESHOLD = 300;
const int MIN_MATCH_COUNT = 10;
const int MAX_MATCH_COUNT = 20;
// initialize the number of not open img
const int ERROR_OPEN = 0;
// initialize the number of img that can not calculate move matrix
const int ERROR_MOVE_MATRIX = 0;
const int FLANN_INDEX_KDTREE = 0;
const float DISTANCE_THRESHOLD = 0.4;
const float FRAME_DIFF_THRESHOLD = 0.68;

void CameraDetector::is_cam_trans(char* cap_path) {
    VideoCapture video_capture;
    video_capture.open(cap_path);
    int fps = video_capture.get(CAP_PROP_FPS);
    int count = 0;
    queue<Mat> q_gray;
    Mat frame, gray;
    Mat mask;
    while (video_capture.read(frame)) {
        count += 1;
        if (count % fps == 0) {
            resize_image(frame);
            cvtColor(frame, gray, COLOR_BGR2GRAY);

            q_gray.push(gray);
            if (q_gray.size() == 2) {
                bool is_trans;
                float percent;
                Mat pre_frame = q_gray.front();
                q_gray.pop();
                Mat cur_frame = q_gray.front();
                abs_frame(cur_frame, pre_frame, is_trans, mask, percent);
                if (is_trans) {
                    // sift detection (关键点检测)
                    is_cam_trans_re(cur_frame, pre_frame);
                }
            }
        }
    }
}

void CameraDetector::is_cam_trans_re(Mat cur_frame, Mat pre_frame) {
    Point cur_top_points[1][4];
    Point cur_bottom_points[1][4];

    Point pre_top_points[1][4];
    Point pre_bottom_points[1][4];
    image_preprocess(cur_frame, cur_top_points, cur_bottom_points);
    image_preprocess(pre_frame, pre_top_points, pre_bottom_points);
}

bool CameraDetector::abs_frame(Mat cur_frame, Mat pre_frame, bool &is_trans, Mat& mask, float &percent) {
    is_trans = false;
    absdiff(cur_frame, pre_frame, mask);

    threshold(mask, mask, 40, 255, CV_THRESH_BINARY);

    int total_pix = mask.rows * mask.cols;
    percent = (total_pix - countNonZero(mask)) / total_pix;
    if (percent < FRAME_DIFF_THRESHOLD) {
        is_trans = true;
        cout << "camera trans" << endl;
    }
    return true;
}

void CameraDetector::crop_image_top(Mat img, Point rook_points[1][4]) {
    int h, w;
    h = img.rows;
    w = img.cols;
    int top = int(h / CROP_SPLIT);

    int top_text_x0, top_text_y0 = 10000;
    int top_text_x1, top_text_y1 = 0;
    
    for(int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            if (i<=top && ((int(img.at<uchar>(i,j)) >= 0 && int(img.at<uchar>(i,j)) <= 10) || \
                                        (int(img.at<uchar>(i,j)) >= 245 && int(img.at<uchar>(i,j)) <= 255))) {
                if (i < top_text_y0) {
                    top_text_y0 = i;
                }
                    
                if (j < top_text_x0) {
                    top_text_x0 = j;
                }
                    
                if (i > top_text_y1) {
                    top_text_y1 = i;
                }
                
                if (j > top_text_x1) {
                    top_text_x1 = j;
                }
            }
        }
    }

    if (top_text_x0 == 10000 || top_text_y0 == 10000) {
        top_text_x0 = 0;
        top_text_y0 = 0;
    }

    rook_points[0][0] = Point(top_text_x0, top_text_y0);
    rook_points[0][1] = Point(top_text_x0, top_text_y1);
    rook_points[0][2] = Point(top_text_x1, top_text_y1);
    rook_points[0][3] = Point(top_text_x1, top_text_y0);

    const Point* ppt[1] = {rook_points[0]};
    int npt[] = {4};
    fillPoly(img, ppt, npt, 1, 0);
}

void CameraDetector::crop_image_bottom(Mat img, Point rook_points[1][4]) {
    int h, w;
    h = img.rows;
    w = img.cols;
    int bottom = h - int(h / CROP_SPLIT);

    int bottom_text_x0, bottom_text_y0 = 10000;
    int bottom_text_x1, bottom_text_y1 = 0;

    for(int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            if (i >= bottom && ((int(img.at<uchar>(i,j)) >= 0 && int(img.at<uchar>(i,j)) <= 10) || (int(img.at<uchar>(i,j)) >= 245 && int(img.at<uchar>(i,j)) <= 255))) {
                if (i < bottom_text_y0) {
                    bottom_text_y0 = i;
                }
                    
                if (j < bottom_text_x0) {
                    bottom_text_x0 = j;
                }
                    
                if (i > bottom_text_y1) {
                    bottom_text_y1 = i;
                }
                
                if (j > bottom_text_x1) {
                    bottom_text_x1 = j;
                }
            }
        }
    }

    if (bottom_text_x0 == 10000 || bottom_text_y0 == 10000) {
        bottom_text_x0 = 0;
        bottom_text_y0 = 0;
    }

    rook_points[0][0] = Point(bottom_text_x0, bottom_text_y0);
    rook_points[0][1] = Point(bottom_text_x0, bottom_text_y1);
    rook_points[0][2] = Point(bottom_text_x1, bottom_text_y1);
    rook_points[0][3] = Point(bottom_text_x1, bottom_text_y0);

    const Point* ppt[1] = {rook_points[0]};
    int npt[] = {4};
    fillPoly(img, ppt, npt, 1, 0);
}

void CameraDetector::resize_image(Mat& img) {
    int rows = img.rows;
    int cols = img.cols;

    while (cols >= 1200 and rows >= 600) {
        resize(img, img, Size(int(cols / 4 * 3), int(rows / 4 * 3)), 0, 0, INTER_CUBIC);
        rows = img.rows;
        cols = img.cols;
    }
}

void CameraDetector::image_preprocess(Mat& img, Point top_rook_points[1][4], Point bottom_rook_points[1][4]) {
    resize_image(img);
    crop_image_top(img, top_rook_points);
    crop_image_bottom(img, bottom_rook_points);
}

bool CameraDetector::sift_detection(Mat img, Mat kp_img, \
                                        Mat des_img, Mat rect_top, \
                                        Mat rect_bottom) {
    int h, w;
    h = img.rows;
    w = img.cols;
    int h_half = int(h/2);
    int w_half = int(w/2);

    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            int h_begin = i * h_half;
            int h_end = (i + 1) * h_half;
            if (h_end >= h) {
                h_end = h;
            }

            int w_begin = j * w_half;
            int w_end = (j + 1) * w_half;
            if (w_end >= w) {
                w_end = w;
            }
            Mat img_roi = img(Range(h_begin, h_end), Range(w_begin, w_end)).clone();
            
        }
    }

    return true;                              
}

int main() { 
    bool is_trans;
    float percent;
    CameraDetector camera_detector;
    Mat cur_frame = imread("1.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat pre_frame = imread("1.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat mask;
    Point top_rook_points[1][4];
    Point bottom_rook_points[1][4];

    // abs_frame(cur_frame, pre_frame, is_trans, mask, percent);
    // crop_image_top(cur_frame, rook_points);
    // crop_image_bottom(cur_frame, rook_points);
    // resize_image(cur_frame);
    camera_detector.image_preprocess(cur_frame, top_rook_points, bottom_rook_points);
    imshow("crop", cur_frame);
    waitKey(0);
    // imshow("image", img);
    // waitKey(0);
    // cout << percent << int(is_trans) << endl;
    cout << "hello world" << endl;
    return 0;
    cout << "hello world" << endl;
    return 0;
}
