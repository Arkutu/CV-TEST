#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <iostream>

int main() {
    // Load Dlib's face detection model
    dlib::shape_predictor sp;
    
    
    
    dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();





    // Load Dlib's face landmark model
    dlib::shape_predictor landmarkPredictor;
    dlib::deserialize("/home/arkutu/CVTest/shape_predictor_68_face_landmarks.dat") >> sp;

    // Load OpenCV video capture
    cv::VideoCapture cap(0);  // 0 for default camera, adjust if needed

    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    // Create a window for displaying the results
    cv::namedWindow("Face Recognition", cv::WINDOW_AUTOSIZE);

    while (true) {
        // Capture a frame from the camera
        cv::Mat frame;
        cap >> frame;

        // Convert OpenCV image to Dlib format
        dlib::cv_image<dlib::bgr_pixel> dlibImage(frame);

        // Detect faces in the image
        std::vector<dlib::rectangle> faces = faceDetector(dlibImage);

        // Iterate over detected faces
        for (const auto& face : faces) {
            // Draw rectangle around the face
            cv::rectangle(frame, cv::Point(face.left(), face.top()), cv::Point(face.right(), face.bottom()), cv::Scalar(0, 255, 0), 2);

            // Get the landmarks for the face
            dlib::full_object_detection landmarks = landmarkPredictor(dlibImage, face);

            // Draw landmarks on the face
            for (unsigned long i = 0; i < landmarks.num_parts(); ++i) {
                cv::circle(frame, cv::Point(landmarks.part(i).x(), landmarks.part(i).y()), 2, cv::Scalar(255, 0, 0), -1);
            }
        }

        // Display the frame
        cv::imshow("Face Recognition", frame);

        // Break the loop if 'Esc' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Release the camera
    cap.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();

    return 0;
}