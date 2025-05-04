//
// Created by Hector van der Aa on 4/22/25.
//

#ifndef EdgeType
#define EdgeType
#define CropEdge 0
#define BlackBorder 1
#define WhiteBorder 2
#define MirrorBorder 3
#define UNSUPPORTED_EDGETYPE (-1)
#endif

#ifndef ColorType
#define ColorType
#define Grayscale 0
#define RGB 1
#endif

#ifndef FileType
#define FileType
#define IMAT 0
#define TIFF 1
#define TGA 2
#define CSV 3
#define HIST 4
#define UNSUPPORTED_FILETYPE (-1)
#endif

#ifndef ComputeType
#define ComputeType
#define LocalBinaryPattern 0
#define Histogram 1
#define UNSUPPORTED_COMPUTETYPE (-1)
#endif

#ifndef HistType
#define HistType
#define RAW 0
#define NORMAL 1
#define UNSUPPORTED_HISTTYPE (-1)
#endif

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <filesystem>
#include <cstring>
#include <algorithm>
//#include <opencv2/opencv.hpp>
using std::cout, std::ostream, std::endl, std::cerr, std::setprecision,
std::fstream, std::ios, std::streamsize,
std::random_device, std::mt19937, std::uniform_real_distribution,
std::string, std::strlen, std::transform,
std::filesystem::path, std::filesystem::create_directories;


class Image {
    int m_width, m_height;
    uint8_t** m_p_data;
public:

    Image();
    Image(int width, int height);
    Image(int width, int height, bool randFill);
    Image(int width, int height, int valFill);
    Image(const Image& image);
    Image(const Image& image, int borderWidth, int borderValue);
    Image(const Image& image, int mirrorBorderWidth);
    ~Image();

    void randFill() const;
    void valFill(int value) const;
    bool writeIMAT(const path& filename);
    bool writeTGA(const path& filename, int colorType);
    bool writeTIF(const path& filename, int colorType);
    void readIMAT(const path& filename);
    void readTGA(const path& filename);
    void setVal(int x, int y, uint8_t val);
    void displayImage();
    uint8_t* unwrapLocal(int x, int y);
    //uint8_t* unwrapLocal(int x, int y, int edgeType);
    uint8_t* localLBP(int x, int y);
    Image computeLBP(int edgeType);
    uint32_t* computeRawHist();
    double* computeNormHist();

    friend ostream& operator<<(ostream& os, const Image& image);
    Image& operator=(const Image& image);

};

uint8_t castToInt(const uint8_t* input);
bool writeRHIST(uint32_t* histogram, const path& filename);
bool writeRHISTCSV(uint32_t* histogram, const path& filename);
bool writeNHIST(double* histogram, const path& filename);
bool writeNHISTCSV(double* histogram, const path& filename);
uint32_t* readRHIST(const path& filename);
double* readNHIST(const path& filename);
void clearCache();
void displayImage(const path& filename);

#endif //IMAGE_H
