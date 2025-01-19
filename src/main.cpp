#include <iostream>
#include <string>
#include <memory>
#include "../include/image_utils.h"
#

// Helper function to validate image dimensions
bool validate_dimensions(const Image& img1, const Image& img2) {
    return img1.width == img2.width && img1.height == img2.height && img1.channels == img2.channels;
}

// Helper function to save an image with new data
void save_image_with_data(const Image& base_image, const unsigned char* data, const std::string& filename) {
    Image temp_image(base_image);
   // Create a mutable copy of the data
    std::unique_ptr<unsigned char[]> mutable_data(new unsigned char[base_image.width * base_image.height * base_image.channels]);
    std::memcpy(mutable_data.get(), data, base_image.width * base_image.height * base_image.channels);

    temp_image.data = mutable_data.get();
    temp_image.save_as_png(filename);

}

int main() {
    try {
        // Load two images for testing
        Image img1("/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/images/test_image1.png"); 
        Image img2("/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/images/test_image2.png");

        // Resize images if their dimensions differ
        if (!validate_dimensions(img1, img2)) {
            std::cout << "Resizing images to match dimensions..." << std::endl;

            int new_width = std::min(img1.width, img2.width);
            int new_height = std::min(img1.height, img2.height);

            std::unique_ptr<unsigned char[]> resized_data1(new unsigned char[new_width * new_height * img1.channels]);
            std::unique_ptr<unsigned char[]> resized_data2(new unsigned char[new_width * new_height * img2.channels]);

            img1.resize_image(img1.data, resized_data1.get(), img1.width, img1.height, new_width, new_height, img1.channels);
            img2.resize_image(img2.data, resized_data2.get(), img2.width, img2.height, new_width, new_height, img2.channels);

            img1.data = resized_data1.get();
            img1.width = new_width;
            img1.height = new_height;

            img2.data = resized_data2.get();
            img2.width = new_width;
            img2.height = new_height;
        }

        // Convert to grayscale
        // std::cout << "Converting to grayscale..." << std::endl;
        // img1.convert_to_grayscale();
        // img1.save_as_png("output_grayscale.png");

        // Convert to sepia
        // std::cout << "Converting to sepia..." << std::endl;
        // img2.convert_to_sepia();
        // img2.save_as_png("output_sepia.png");

        // Add two images
        std::cout << "Adding two images..." << std::endl;
        std::unique_ptr<unsigned char[]> add_result(new unsigned char[img1.width * img1.height * img1.channels]);
        img1.add_images(img1.data, img2.data, add_result.get(), img1.width, img1.height, img1.channels, img2.width, img2.height, img2.channels);
        save_image_with_data(img1, add_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_add.png");

        // Subtract two images
        // std::cout << "Subtracting two images..." << std::endl;
        // std::unique_ptr<unsigned char[]> sub_result(new unsigned char[img1.width * img1.height * img1.channels]);
        // img1.subtract_images(img1.data, img2.data, sub_result.get(), img1.width, img1.height, img1.channels, img2.width, img2.height, img2.channels);
        // save_image_with_data(img1, sub_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_subtract.png");

        //Adjust brightness
        // std::cout << "Adjusting brightness..." << std::endl;
        // img1.adjust_brightness(img1.data, img1.width, img1.height, img1.channels, 50);
        // img1.save_as_png("/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_brightness.png");

        // // Adjust contrast
        // std::cout << "Adjusting contrast..." << std::endl;
        // img1.adjust_contrast(img1.data, img1.width, img1.height, img1.channels, 1.5f);
        // img1.save_as_png("/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_contrast.png");

        // // Thresholding
        // std::cout << "Thresholding image..." << std::endl;
        // img1.threshold_image(img1.data, img1.width, img1.height, img1.channels, 128);
        // img1.save_as_png("/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_threshold.png");

        // // Low-pass filtering
        // std::cout << "Applying low-pass filter..." << std::endl;
        // std::unique_ptr<unsigned char[]> lp_result(new unsigned char[img1.width * img1.height * img1.channels]);
        // img1.low_pass_filter(img1.data, lp_result.get(), img1.width, img1.height, img1.channels, 0); // Averaging filter
        // save_image_with_data(img1, lp_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_lowpass.png");

        // // High-pass filtering
        // std::cout << "Applying high-pass filter..." << std::endl;
        // std::unique_ptr<unsigned char[]> hp_result(new unsigned char[img1.width * img1.height * img1.channels]);
        // img1.high_pass_filter(img1.data, hp_result.get(), img1.width, img1.height, img1.channels, 0); // Prewitt filter
        // save_image_with_data(img1, hp_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_highpass.png");



        // // Otsu's thresholding
        // std::cout << "Applying Otsu's thresholding..." << std::endl;
        // std::unique_ptr<unsigned char[]> otsu_result(new unsigned char[img1.width * img1.height * img1.channels]);
        // img1.otsu_threshold(img1.data, otsu_result.get(), img1.width, img1.height, img1.channels);
        // save_image_with_data(img1, otsu_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_otsu.png");

        // // Hough transform
        // std::cout << "Applying Hough transform..." << std::endl;
        // std::unique_ptr<unsigned char[]> hough_result(new unsigned char[img1.width * img1.height * img1.channels]);
        // img1.hough_transform(img1.data, hough_result.get(), img1.width, img1.height, img1.channels);
        // save_image_with_data(img1, hough_result.get(), "/Users/cynthiaabi/Desktop/WorkSpace/ImageProcessing2/output/output_hough.png");

        std::cout << "All operations completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
