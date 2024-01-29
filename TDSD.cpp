#include <opencv2/opencv.hpp>


int main() {
    // Read the image
    cv::Mat image = cv::imread("/home/arkutu/Pictures/ST.png", cv::IMREAD_GRAYSCALE);

    // Apply edge detection
    cv::Canny(image, image, 50, 150);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        // Get bounding box
        cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
        cv::Point2f vertices[4];
        rotatedRect.points(vertices);

        // Calculate angles
        std::vector<double> angles;
        for (int i = 0; i < 4; ++i) {
            cv::Point2f pt1 = vertices[i];
            cv::Point2f pt2 = vertices[(i + 1) % 4];
            cv::Point2f pt3 = vertices[(i + 2) % 4];

            double angle = std::atan2(pt3.y - pt2.y, pt3.x - pt2.x) -
                           std::atan2(pt1.y - pt2.y, pt1.x - pt2.x);
            angle = std::abs(angle * 180 / CV_PI);  // Convert to degrees
            angles.push_back(angle);
        }

        // Check if the shape is a square or rectangle
        double angleThreshold = 5.0;  // Adjust as needed
        bool isRectangle = true;
        for (const double& angle : angles) {
            if (std::abs(angle - 90.0) > angleThreshold) {
                isRectangle = false;
                break;
            }
        }

        if (isRectangle) {
            std::cout << "Rectangle" << std::endl;
        } else {
            std::cout << "Not a rectangle" << std::endl;
        }
    }

    cv::imshow("Image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}