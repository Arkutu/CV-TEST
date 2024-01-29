#include <opencv2/opencv.hpp>
#include <unordered_map>

int main() {
 
    cv::CascadeClassifier faceCascade;

   
    if (!faceCascade.load("/home/arkutu/Downloads/opencv-4.8.1 (1)/opencv-4.8.1/data/haarcascades/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading face cascade file!" << std::endl;
        return -1;
    }

   
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Error opening camera!" << std::endl;
        return -1;
    }

    
    std::unordered_map<int, std::string> faceNames;

    faceNames[0] = "Face";

    while (true) {
      
        cv::Mat frame;
        cap >> frame;

        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

     
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(grayFrame, faces, 1.1, 4, 0, cv::Size(30, 30));

     
        for (size_t i = 0; i < faces.size(); ++i) {
            cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2);

      
            auto it = faceNames.find(i);
            if (it != faceNames.end()) {
                std::string name = it->second;
                cv::putText(frame, name, cv::Point(faces[i].x, faces[i].y - 10),
                            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2);
            }
        }

        cv::imshow("Real-Time Face Detection", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}