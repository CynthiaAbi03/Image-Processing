#include "../include/image_utils.h"


#include <stdexcept>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image/stb_image_write.h"


// Constructor: Load an image from a file
Image::Image(const std::string& filepath) {
    data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Error loading image: " + filepath);
    }
}

// Destructor: Free the image memory
Image::~Image() {
    if (data != nullptr) {
        stbi_image_free(data);
        data = nullptr; // Ensuring no dangling pointer
    }
}

// Save the image as PNG
void Image::save_as_png(const std::string& filepath) const {
    if (!stbi_write_png(filepath.c_str(), width, height, channels, data, width * channels)) {
        throw std::runtime_error("Error saving PNG image: " + filepath);
    }
}

// Save the image as JPG
void Image::save_as_jpg(const std::string& filepath, int quality) const {
    if (!stbi_write_jpg(filepath.c_str(), width, height, channels, data, quality)) {
        throw std::runtime_error("Error saving JPG image: " + filepath);
    }
}

// Convert the image to grayscale
void Image::convert_to_grayscale() {
    if (channels < 3) {
        throw std::runtime_error("Image must have at least 3 channels for grayscale conversion.");
    }

    for (int i = 0; i < width * height; ++i) {
        unsigned char* pixel = data + i * channels;
        unsigned char gray_value = static_cast<unsigned char>(0.3 * pixel[0] + 0.59 * pixel[1] + 0.11 * pixel[2]);
        pixel[0] = gray_value;
        pixel[1] = gray_value;
        pixel[2] = gray_value;
    }
}

// Convert the image to sepia tone
void Image::convert_to_sepia() {
    if (channels < 3) {
        throw std::runtime_error("Image must have at least 3 channels for sepia conversion.");
    }

    for (int i = 0; i < width * height; ++i) {
        unsigned char* pixel = data + i * channels;

        unsigned char red = pixel[0];
        unsigned char green = pixel[1];
        unsigned char blue = pixel[2];

        pixel[0] = std::min(255.0, 0.393 * red + 0.769 * green + 0.189 * blue);
        pixel[1] = std::min(255.0, 0.349 * red + 0.686 * green + 0.168 * blue);
        pixel[2] = std::min(255.0, 0.272 * red + 0.534 * green + 0.131 * blue);
    }
    
    
}
// void Image::resize_image(const unsigned char* src, unsigned char* dest, int old_width, int old_height, int new_width, int new_height, int channels) {
//     for (int y = 0; y < new_height; ++y) {
//         for (int x = 0; x < new_width; ++x) {
//             int src_x = static_cast<int>((x / static_cast<float>(new_width)) * old_width);
//             int src_y = static_cast<int>((y / static_cast<float>(new_height)) * old_height);
//             for (int c = 0; c < channels; ++c) {
//                 dest[(y * new_width + x) * channels + c] = src[(src_y * old_width + src_x) * channels + c];
//             }
//         }
//     }
// }

void Image::crop_image(const unsigned char* src, unsigned char* dest, int src_width, int src_height, int dest_width, int dest_height, int channels) {
    for (int y = 0; y < dest_height; ++y) {
        for (int x = 0; x < dest_width; ++x) {
            for (int c = 0; c < channels; ++c) {
                dest[(y * dest_width + x) * channels + c] = src[(y * src_width + x) * channels + c];
            }
        }
    }
}
// Add two images pixel by pixel
void Image::add_images(const unsigned char* img1_data, const unsigned char* img2_data, unsigned char* result_data, int img1_width, int img1_height, int img1_channels, int img2_width, int img2_height, int img2_channels) {
    
    // Ensure both images have the same dimensions (width, height, channels)
    if (img1_width != img2_width || img1_height != img2_height || img1_channels != img2_channels) {
        throw std::invalid_argument("Images must have the same dimensions and number of channels.");
    }

    // Iterate over each pixel and add corresponding values
    int total_pixels = img1_width * img1_height * img1_channels;
    for (int i = 0; i < total_pixels; i++) {
        // Add the pixel values from both images and clamp the result between 0 and 255
        result_data[i] = std::clamp(img1_data[i] + img2_data[i], 0, 255);
    }
}

// Subtract one image from another pixel by pixel
void Image::subtract_images(const unsigned char* img1_data, const unsigned char* img2_data, unsigned char* result_data, int img1_width, int img1_height, int img1_channels, int img2_width, int img2_height, int img2_channels) {
    // Ensure both images have the same dimensions (width, height, channels)
    if (img1_width != img2_width || img1_height != img2_height || img1_channels != img2_channels) {
        throw std::invalid_argument("Images must have the same dimensions and number of channels.");
    }

    // Iterate over each pixel and subtract corresponding values
    int total_pixels = img1_width * img1_height * img1_channels;
    for (int i = 0; i < total_pixels; i++) {
        // Subtract the pixel values from both images and clamp the result between 0 and 255
        result_data[i] = std::clamp(img1_data[i] - img2_data[i], 0, 255);
    }
}


// Adjust brightness of the image
void Image::adjust_brightness(unsigned char* img_data, int img_width, int img_height, int img_channels, int adjustment_value) {
    int total_pixels = img_width * img_height * img_channels;
    for (int i = 0; i < total_pixels; ++i) {
        img_data[i] = std::clamp(static_cast<int>(img_data[i]) + adjustment_value, 0, 255);
    }
}

// Adjust contrast of the image
void Image::adjust_contrast(unsigned char* img, int width, int height, int channels, float contrast_factor) {
    float midpoint = 127.5f;
    for (int i = 0; i < width * height * channels; ++i) {
        img[i] = std::clamp(static_cast<int>((img[i] - midpoint) * contrast_factor + midpoint), 0, 255);
    }
}
// Apply a binary threshold to the image
void Image::threshold_image(unsigned char* img, int width, int height, int channels, unsigned char threshold) {
    for (int i = 0; i < width * height * channels; ++i) {
        img[i] = (img[i] >= threshold) ? 255 : 0;
    }
}

// Apply a low-pass filter (e.g., blur) to the image
void Image::low_pass_filter(const unsigned char* img, unsigned char* result, int width, int height, int channels, int filter_size) {
    int offset = filter_size / 2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                int count = 0;
                for (int fy = -offset; fy <= offset; ++fy) {
                    for (int fx = -offset; fx <= offset; ++fx) {
                        int nx = x + fx;
                        int ny = y + fy;
                        if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                            sum += img[(ny * width + nx) * channels + c];
                            ++count;
                        }
                    }
                }
                result[(y * width + x) * channels + c] = sum / count;
            }
        }
    }
}

// Apply a high-pass filter (e.g., edge detection) to the image
void Image::high_pass_filter(const unsigned char* img, unsigned char* result, int width, int height, int channels, int filter_size) {
    int kernel[3][3] = {{-1, -1, -1},
                        {-1,  8, -1},
                        {-1, -1, -1}};
    int offset = filter_size / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;
                for (int fy = -offset; fy <= offset; ++fy) {
                    for (int fx = -offset; fx <= offset; ++fx) {
                        int nx = x + fx;
                        int ny = y + fy;
                        if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                            sum += img[(ny * width + nx) * channels + c] * kernel[fy + offset][fx + offset];
                        }
                    }
                }
                result[(y * width + x) * channels + c] = std::clamp(sum, 0, 255);
            }
        }
    }
}

// Resize the image
void Image::resize_image(const unsigned char* src, unsigned char* dest, int old_width, int old_height, int new_width, int new_height, int channels) {
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            int src_x = static_cast<int>((x / static_cast<float>(new_width)) * old_width);
            int src_y = static_cast<int>((y / static_cast<float>(new_height)) * old_height);
            for (int c = 0; c < channels; ++c) {
                dest[(y * new_width + x) * channels + c] = src[(src_y * old_width + src_x) * channels + c];
            }
        }
    }
}

// Perform Otsu's thresholding
void Image::otsu_threshold(const unsigned char* img, unsigned char* result, int width, int height, int channels) {
    int histogram[256] = {0};

    for (int i = 0; i < width * height * channels; i += channels) {
        ++histogram[img[i]];
    }

    int total = width * height;
    float sum = 0;
    for (int t = 0; t < 256; ++t) {
        sum += t * histogram[t];
    }

    float sum_b = 0;
    int weight_b = 0;
    int weight_f = 0;

    float max_var = 0;
    int threshold = 0;

    for (int t = 0; t < 256; ++t) {
        weight_b += histogram[t];
        if (weight_b == 0) continue;

        weight_f = total - weight_b;
        if (weight_f == 0) break;

        sum_b += t * histogram[t];
        float mean_b = sum_b / weight_b;
        float mean_f = (sum - sum_b) / weight_f;

        float var_between = weight_b * weight_f * (mean_b - mean_f) * (mean_b - mean_f);

        if (var_between > max_var) {
            max_var = var_between;
            threshold = t;
        }
    }

    for (int i = 0; i < width * height * channels; ++i) {
        result[i] = (img[i] > threshold) ? 255 : 0;
    }
}

// Apply Hough Transform (placeholder example for line detection)
void Image::hough_transform(const unsigned char* img, unsigned char* result, int width, int height, int channels) {
    // Placeholder: Implement Hough Transform logic here
    std::memcpy(result, img, width * height * channels);
}



