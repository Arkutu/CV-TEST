#include <opencv2/opencv.hpp>

int main() {
    // Read the image
    cv::Mat image = cv::imread("/home/arkutu/Pictures/star.jpg");

    // Check if the image is loaded successfully
    if (image.empty()) {
        std::cerr << "Error: Unable to load image." << std::endl;
        return -1;
    }

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    

    // Apply thresholding to create a binary image
    cv::Mat binary;
    cv::threshold(gray, binary, 200, 255, cv::THRESH_BINARY);


    // Find contours in the binary image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Draw contours on the original image
    cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 2);

    // Display the result
    cv::imshow("Star Detection", image);
    cv::waitKey(0);

    return 0;
}