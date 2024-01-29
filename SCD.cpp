#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>



bool isStar(const std::vector<cv::Point>& contour) {
    if (contour.size() % 2 != 0) {
      
        return false;
    }

    int outerVertices = contour.size() / 2;
    std::vector<cv::Point> outerContour(contour.begin(), contour.begin() + outerVertices);
    std::vector<cv::Point> innerContour(contour.begin() + outerVertices, contour.end());

 
    double angleThreshold = 120.0;

    for (int i = 0; i < outerVertices; ++i) {
        cv::Point prev = outerContour[i];
        cv::Point current = outerContour[(i + 1) % outerVertices];
        cv::Point next = outerContour[(i + 2) % outerVertices];

        double angle = std::abs(180.0 - cv::fastAtan2(next.y - current.y, next.x - current.x) +
                                cv::fastAtan2(prev.y - current.y, prev.x - current.x));

        if (angle > angleThreshold) {
            return false;
        }
    }

    return true;
}

std::string getShapeName(int vertices, const std::vector<cv::Point>& contour) {
    if (vertices == 3) {
        return "Triangle";
    } else if (vertices == 4) {
        return "Rectangle or Square";
    } else if (vertices == 5) {
        return "Pentagon";
    } else if (vertices == 6) {
        return "Hexagon";
    } else if (vertices == 8) {
        return "Circle";
    } else if (isStar(contour)) {
        return "Star";
    } else {
        return "Unknown";
    }
}



int main() {
    //cv::VideoCapture cap(0);

     cv::VideoCapture cap("/dev/video0", cv::CAP_V4L);

    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error reading frame from the camera!" << std::endl;
            break;
        }


         cv::resize(frame, frame, cv::Size(800, 700));

        cv::Mat grayImage;
        cv::cvtColor(frame, grayImage, cv::COLOR_BGR2GRAY);

        cv::GaussianBlur(grayImage, grayImage, cv::Size(5, 5), 0);
        cv::Canny(grayImage, grayImage, 50, 150);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(grayImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double epsilon = 0.02 * cv::arcLength(contour, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, epsilon, true);

            int vertices = approx.size();

            cv::drawContours(frame, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);

            std::string shapeName = getShapeName(vertices, approx);
            cv::putText(frame, shapeName, cv::Point(approx[0].x, approx[0].y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        }

        cv::imshow("Detected Shapes", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}