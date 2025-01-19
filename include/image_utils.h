#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>

// Struct to represent an image
struct Image {
    int width;       // Image width
    int height;      // Image height
    int channels;    // Number of channels (e.g., 3 for RGB, 4 for RGBA)
    unsigned char* data; // Pointer to the raw pixel data

    // Constructor to load an image from a file
    Image(const std::string& filepath);
    // Destructor to free image memory
    ~Image();

    // Save the image in PNG format
    void save_as_png(const std::string& filepath) const;
    // Save the image in JPG format
    void save_as_jpg(const std::string& filepath, int quality = 100) const;

    // Convert the image to grayscale
    void convert_to_grayscale();
    // Convert the image to sepia tone
    void convert_to_sepia();



    void add_images(const unsigned char* img1, const unsigned char* img2, unsigned char* result, int width1, int height1, int channels1, int width2, int height2, int channels2);
    void subtract_images(const unsigned char* img1, const unsigned char* img2, unsigned char* result, int width1, int height1, int channels1, int width2, int height2, int channels2);
    void adjust_brightness(unsigned char* img, int width, int height, int channels, int adjustment);
    void adjust_contrast(unsigned char* img, int width, int height, int channels, float contrast_factor);
    void threshold_image(unsigned char* img, int width, int height, int channels, unsigned char threshold);
    void low_pass_filter(const unsigned char* img, unsigned char* result, int width, int height, int channels, int filter_type);
    void high_pass_filter(const unsigned char* img, unsigned char* result, int width, int height, int channels, int filter_type);
    void otsu_threshold(const unsigned char* img, unsigned char* result, int width, int height, int channels);
    void hough_transform(const unsigned char* img, unsigned char* result, int width, int height, int channels);

    void resize_image(const unsigned char* src, unsigned char* dest, int old_width, int old_height, int new_width, int new_height, int channels);
    void crop_image(const unsigned char* src, unsigned char* dest, int src_width, int src_height, int dest_width, int dest_height, int channels);

};

#endif // IMAGE_UTILS_H
