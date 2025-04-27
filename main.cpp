#include <iostream>
#include "Image.h"
using Image::EdgeType::CropEdge, Image::EdgeType::WhiteBorder, Image::EdgeType::BlackBorder, Image::EdgeType::MirrorBorder,
Image::ColorType::Grayscale, Image::ColorType::RGB;

int main() {
    Image image;
    image.readTGA("demo");
    Image lbp=image.computeLBP(MirrorBorder);
    lbp.writeTIF("lbp", RGB);
    lbp.writeTGA("lbp", RGB);
    // Image image(1920,1080,true);
    // Image lbp=image.computeLBP(4);

    // Image readTest;
    // readTest.readTGA("test.nonsense");
    // cout << readTest;


    // uint8_t* hist = lbp.computeRawHist();
    // for (int i = 0; i < 256; i++) {
    //     cout << i << ": " << static_cast<int>(hist[i]) << endl;
    // }
    //
    // double* hist2 = lbp.computeNormHist();
    // for (int i = 0; i < 256; i++) {
    //     cout << i << ": " << hist2[i] << endl;
    // }

    return 0;
}
