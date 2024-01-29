#include <opencv2/opencv.hpp>


std::string getShapeName(int vertices) {
    if (vertices == 3) {
        return "Triangle";
    } else if (vertices == 4) {
        return "Rectangle or Square";
    } else if (vertices == 5) {
        return "Pentagon";
    } else if (vertices == 6) {
        return "Hexagon";
    } else {
        return "Unknown";
    }
}

int main() {
    cv::Mat image = cv::imread("/home/arkutu/Pictures/GRR.jpg");

    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(grayImage, grayImage, cv::Size(5, 5), 0);

    cv::Mat edges;
    cv::Canny(grayImage, edges, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        double epsilon = 0.02 * cv::arcLength(contour, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, epsilon, true);

        int vertices = approx.size();

        cv::drawContours(image, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);

        std::string shapeName = getShapeName(vertices);
        cv::putText(image, shapeName, cv::Point(approx[0].x, approx[0].y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    }

    cv::imshow("Detected Shapes", image);
    cv::waitKey(0);

    return 0;
}