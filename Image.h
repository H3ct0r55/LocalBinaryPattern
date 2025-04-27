//
// Created by Hector van der Aa on 4/22/25.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <random>
using std::cout, std::ostream, std::endl, std::cerr,
std::fstream, std::ios, std::streamsize,
std::random_device, std::mt19937, std::uniform_real_distribution,
std::string;


class Image {
    int m_width, m_height;
    uint8_t** m_p_data;
public:
    enum class EdgeType {
        CropEdge,
        WhiteBorder,
        BlackBorder,
        MirrorBorder
    };
    enum class ColorType {
        Grayscale,
        RGB
    };

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
    void writeIMAT(const char* filename);
    void writeTGA(const char* filename, ColorType colorType);
    void writeTIF(const char* filename, ColorType colorType);
    void readIMAT(const char* filename);
    void readTGA(const char* filename);
    void setVal(int x, int y, uint8_t val);
    uint8_t* unwrapLocal(int x, int y);
    //uint8_t* unwrapLocal(int x, int y, int edgeType);
    uint8_t* localLBP(int x, int y);
    Image computeLBP(EdgeType edgeType);
    uint8_t* computeRawHist();
    double* computeNormHist();

    friend ostream& operator<<(ostream& os, const Image& image);
    friend ostream& operator<<(ostream& os, EdgeType edgeType);
    friend ostream& operator<<(ostream& os, ColorType colorType);
    Image& operator=(const Image& image);

};

uint8_t castToInt(const uint8_t* input);
void writeRHIST(uint8_t* histogram, const char* filename);
void writeNHIST(double* histogram, const char* filename);
uint8_t* readRHIST(const char* filename);
double* readNHIST(const char* filename);

#endif //IMAGE_H
