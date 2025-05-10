//
// Created by Hector van der Aa on 4/22/25.
//


#include "Image.h"

Image::Image() : m_width(0), m_height(0), m_p_data(nullptr) {
}

Image::Image(int width, int height) : m_width(width), m_height(height), m_p_data(nullptr) {
    if (width > 0 && height > 0) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        this->valFill(0);
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::Image(int width, int height, int value) : m_width(width), m_height(height), m_p_data(nullptr) {
    if (width > 0 && height > 0) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        this->valFill(value);
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::Image(int width, int height, bool randFill) : m_width(width), m_height(height), m_p_data(nullptr) {
    if (width > 0 && height > 0) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        if (randFill) {
            this->randFill();
        } else {
            this->valFill(0);
        }
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::Image(const Image &image) : m_width(image.m_width), m_height(image.m_height), m_p_data(nullptr) {
    if (m_width > 0 && m_height > 0 && image.m_p_data != nullptr) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                m_p_data[i][j] = image.m_p_data[i][j];
            }
        }
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::Image(const Image &image, int borderWidth, int borderValue) : m_width(image.m_width + 2 * borderWidth),
                                                                     m_height(image.m_height + 2 * borderWidth),
                                                                     m_p_data(nullptr) {
    if (m_width > 0 && m_height > 0 && image.m_p_data != nullptr) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }

        for (int i = 0; i < image.m_height; i++) {
            for (int j = 0; j < image.m_width; j++) {
                m_p_data[i + borderWidth][j + borderWidth] = image.m_p_data[i][j];
            }
        }

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                if (i < borderWidth || j < borderWidth || i >= m_height - borderWidth || j >= m_width - borderWidth) {
                    m_p_data[i][j] = static_cast<uint8_t>(borderValue);
                }
            }
        }
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::Image(const Image &image, int mirrorBorderWidth) : m_width(image.m_width + 2 * mirrorBorderWidth),
                                                          m_height(image.m_height + 2 * mirrorBorderWidth),
                                                          m_p_data(nullptr) {
    if (m_width > 0 && m_height > 0 && image.m_p_data != nullptr) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }

        for (int i = 0; i < image.m_height; i++) {
            for (int j = 0; j < image.m_width; j++) {
                m_p_data[i + mirrorBorderWidth][j + mirrorBorderWidth] = image.m_p_data[i][j];
            }
        }

        for (int i = 0; i < mirrorBorderWidth; i++) {
            for (int j = 0; j < image.m_width; j++) {
                m_p_data[i][j + mirrorBorderWidth] = image.m_p_data[mirrorBorderWidth - i - 1][j];
                m_p_data[m_height - i - 1][j + mirrorBorderWidth] = image.m_p_data[
                    image.m_height - (mirrorBorderWidth - i)][j];
            }
        }

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < mirrorBorderWidth; j++) {
                m_p_data[i][j] = m_p_data[i][2 * mirrorBorderWidth - j - 1];
                m_p_data[i][m_width - j - 1] = m_p_data[i][m_width - 2 * mirrorBorderWidth + j];
            }
        }
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
    }
}

Image::~Image() {
    if (m_p_data != nullptr) {
        for (int i = 0; i < m_height; i++) {
            delete[] m_p_data[i];
        }
        delete[] m_p_data;
    }
}

void Image::randFill() const {
    if (m_width > 0 && m_height > 0 && m_p_data != nullptr) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(0, 255);
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                m_p_data[i][j] = static_cast<uint8_t>(dist(gen));
            }
        }
    }
}

void Image::valFill(int value) const {
    if (m_width > 0 && m_height > 0 && m_p_data != nullptr && value >= 0 && value <= 255) {
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                m_p_data[i][j] = static_cast<uint8_t>(value);
            }
        }
    }
}

void Image::fillRange(int startX, int startY, int endX, int endY, uint8_t val) {
    if (m_width > 0 && m_height > 0 && m_p_data != nullptr) {
        for (int i = startY; i < endY; i++) {
            for (int j = startX; j < endX; j++) {
                m_p_data[i][j] = val;
            }
        }
    }
}


bool Image::writeIMAT(const path& filename) {
    if (m_width > 0 && m_height > 0 && m_p_data != nullptr) {

        path dir = filename.parent_path();
        if (!dir.empty() && !exists(dir)) {
            create_directories(dir);
        }

        fstream file(filename, ios::out | ios::binary);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
            file.close();
            return false;
        }
        file.write(reinterpret_cast<const char *>(&m_width), sizeof(int));
        file.write(reinterpret_cast<const char *>(&m_height), sizeof(int));
        for (int i = 0; i < m_height; i++) {
            file.write(reinterpret_cast<const char *>(m_p_data[i]), static_cast<streamsize>(m_width * sizeof(uint8_t)));
        }
        file.close();
        return true;
    }
    return false;
}

void Image::readIMAT(const path& filename) {

    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return;
    }
    if (m_p_data != nullptr) {
        for (int i = 0; i < m_height; i++) {
            delete[] m_p_data[i];
        }
        delete[] m_p_data;
        m_p_data = nullptr;
    }

    file.read(reinterpret_cast<char *>(&m_width), sizeof(int));
    file.read(reinterpret_cast<char *>(&m_height), sizeof(int));

    m_p_data = new uint8_t *[m_height];
    for (int i = 0; i < m_height; i++) {
        m_p_data[i] = new uint8_t[m_width];
        file.read(reinterpret_cast<char *>(m_p_data[i]), static_cast<streamsize>(m_width * sizeof(uint8_t)));
    }

    file.close();
}


ostream &operator<<(ostream &os, const Image &image) {
    for (int i = 0; i < image.m_height; i++) {
        for (int j = 0; j < image.m_width; j++) {
            os << static_cast<int>(image.m_p_data[i][j]) << "\t";
        }
        os << "\n";
    }
    os << endl;
    return os;
}


uint8_t *Image::unwrapLocal(int x, int y, int startPos, int rotation) {
    if (x > 0 && y > 0 && x < m_width - 1 && y < m_height - 1) {
        auto *temp = new uint8_t[8];
        for (int i = 0; i < 3; i++) {
            if (m_p_data) {
                temp[i] = m_p_data[y - 1][x + i - 1];
            } else {
                cerr << "ERROR: nullptr called into unwarpLocal, exiting..." << endl;cerr << "ERROR: nullptr called into unwarpLocal, exiting..." << endl;
                exit(12);
            }
        }
        temp[3] = m_p_data[y][x + 1];
        for (int i = 0; i < 3; i++) {
            temp[i + 4] = m_p_data[y + 1][x + 1 - i];
        }
        temp[7] = m_p_data[y][x - 1];
        if (startPos != 0) {
            auto* shift = new uint8_t[8];
            for (int i = 0; i < 8; i++) {
                shift[i] = temp[(startPos + i) % 8];
            }
            delete[] temp;
            temp = shift;
        }
        if (rotation != 0) {
            auto *rotate = new uint8_t[8];
            for (int i = 0; i < 8; i++) {
                rotate[i] = temp[(8 - i) % 8];
            }
            delete[] temp;
            temp = rotate;
        }
        return temp;
    }
    cerr << "ERROR: Unable to unwrapLocal for edge pixels, exiting...";
    exit(10);
}

int Image::startPosRLBP(int x, int y) {
    auto* temp = this->unwrapLocal(x, y, TL, CW);
    int largest = 0;
    int largestPos = 0;
    for (int i = 0; i < 8; i++) {
        int val = static_cast<int>(temp[i]);
        int diff = abs(val-m_p_data[y][x]);
        if (diff > largest) {
            largest = diff;
            largestPos = i;
        }
    }
    return largestPos;
}


/*uint8_t *Image::unwrapLocal(int x, int y, int edgeType) {
        switch (edgeType) {
            case 0: {
                return this->unwrapLocal(x, y);
            }
            case 1: {
                Image temp1(*this, 1, 0);
                return temp1.unwrapLocal(x+1, y+1);
            }
            case 2: {
                Image temp2(*this, 1, 255);
                return temp2.unwrapLocal(x+1, y+1);
            }
            case 3: {
                Image temp3(*this, 1);
                return temp3.unwrapLocal(x+1, y+1);
            }
            default: {
                cerr << "ERROR: Unknown unwrap edge type: " << edgeType << " exiting..." ;
                exit(11);
            }
        }
}*/

uint8_t *Image::localLBP(int x, int y, int startPos, int rotation) {
    uint8_t *temp = this->unwrapLocal(x, y, startPos, rotation);
    auto LBP = new uint8_t[8];
    for (int i = 0; i < 8; i++) {
        if (temp[i] < m_p_data[y][x]) {
            LBP[i] = 0;
        } else {
            LBP[i] = 1;
        }
    }
    delete[] temp;
    return LBP;
}

uint32_t *Image::computeRawHist() {
    auto *temp = new uint32_t[256];
    for (int i = 0; i < 256; i++) {
        temp[i] = 0;
    }

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            temp[m_p_data[i][j]] += 1;
        }
    }
    return temp;
}

double *Image::computeNormHist() {
    uint32_t *temp = this->computeRawHist();
    int total = 0;
    for (int i = 0; i < 256; i++) {
        total += static_cast<int>(temp[i]);
    }
    auto *hist = new double[256];
    for (int i = 0; i < 256; i++) {
        hist[i] = static_cast<double>(temp[i]) / static_cast<double>(total);
    }
    delete[] temp;
    return hist;
}

Image &Image::operator=(const Image &input) {
    if (this != &input) {
        m_width = input.m_width;
        m_height = input.m_height;
        if (m_p_data != nullptr) {
            for (int i = 0; i < input.m_height; i++) {
                delete[] m_p_data[i];
            }
            delete[] m_p_data;
        }
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                m_p_data[i][j] = input.m_p_data[i][j];
            }
        }
    }
    return *this;
}


uint8_t castToInt(const uint8_t *input) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (input[7 - i] == 1) {
            result += static_cast<uint8_t>(1 << i);
        }
    }
    return result;
}

uint8_t castToInt(const uint8_t *input, bool rotationInvariant) {
    if (!rotationInvariant) {
        return castToInt(input);
    }

    uint8_t minValue = 255;
    for (int r = 0; r < 8; ++r) {
        uint8_t value = 0;
        for (int i = 0; i < 8; ++i) {
            if (input[(r + 7 - i) % 8] == 1) {
                value += static_cast<uint8_t>(1 << i);
            }
        }
        if (value < minValue) {
            minValue = value;
        }
    }
    return minValue;
}

bool writeRHIST(uint32_t *histogram, const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        file.close();
        return false;
    }
    file.write(reinterpret_cast<char *>(histogram), 256 * sizeof(uint32_t));
    file.close();
    return true;
}

bool writeRHISTCSV(uint32_t *histogram, const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        file.close();
        return false;
    }
    const char* TopLine = "Value, Occurence\n";
    file.write(TopLine, static_cast<streamsize>(strlen(TopLine)));
    for (int i = 0; i < 256; i++) {
        file << i << ", " << static_cast<int>(histogram[i]) << "\n";
    }
    file.close();
    return true;
}

bool writeNHISTCSV(double *histogram, const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << ", no file written" << endl;
        file.close();
        return false;
    }
    const char* TopLine = "Value, Occurence\n";
    file.write(TopLine, static_cast<streamsize>(strlen(TopLine)));
    for (int i = 0; i < 256; i++) {
        file << i << ", " << std::fixed << setprecision(15) << histogram[i] << "\n";
    }
    file.close();
    return true;
}


bool writeNHIST(double *histogram, const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        file.close();
        return false;
    }
    file.write(reinterpret_cast<char *>(histogram), 256 * sizeof(double));
    file.close();
    return true;
}

uint32_t *readRHIST(const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    auto *histogram = new uint32_t[256];
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return {};
    }
    file.read(reinterpret_cast<char *>(histogram), 256 * sizeof(uint32_t));
    file.close();
    return histogram;
}

double *readNHIST(const path& filename) {
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    auto *histogram = new double[256];
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return {};
    }
    file.read(reinterpret_cast<char *>(histogram), 256 * sizeof(double));
    file.close();
    return histogram;
}


Image Image::computeLBP(int edgeType, int startPos, int rotation) {
    switch (edgeType) {
        case 0 : {
            Image lbp(m_width - 2, m_height - 2);
            for (int i = 0; i < m_height - 2; i++) {
                for (int j = 0; j < m_width - 2; j++) {
                    if (lbp.m_p_data) {
                        lbp.m_p_data[i][j] = castToInt(this->localLBP(j + 1, i + 1, startPos, rotation));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: CropEdge, of dimension smaller than 3x3" << endl;
                    }
                }
            }
            return lbp;
        }
        case 1 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1, 0);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, rotation));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: WhiteBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 2 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1, 255);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, rotation));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: BlackBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 3 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, rotation));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: MirrorBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        default: {
            cerr << "Error: Invalid edgeType: " << edgeType << endl;
            return {};
        }
    }
}

Image Image::computeRILBP(int edgeType) {
    switch (edgeType) {
        case 0 : {
            Image lbp(m_width - 2, m_height - 2);
            for (int i = 0; i < m_height - 2; i++) {
                for (int j = 0; j < m_width - 2; j++) {
                    if (lbp.m_p_data) {
                        int startPos = this->startPosRLBP(j + 1, i + 1);
                        lbp.m_p_data[i][j] = castToInt(this->localLBP(j + 1, i + 1, startPos, CW));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: CropEdge, of dimension smaller than 3x3" << endl;
                    }
                }
            }
            return lbp;
        }
        case 1 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1, 0);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, CW));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: WhiteBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 2 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1, 255);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, CW));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: BlackBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 3 : {
            Image lbp(m_width, m_height);
            Image tempBorder(*this, 1);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        lbp.m_p_data[i][j] = castToInt(tempBorder.localLBP(j + 1, i + 1, startPos, CW));
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: MirrorBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        default: {
            cerr << "Error: Invalid edgeType: " << edgeType << endl;
            return {};
        }
    }
}

bool Image::writeTGA(const path& filename, int colorType) {

    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: Empty or invalid image, not writing TGA." << endl;
        return false;
    }
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        file.close();
        return false;
    }

    uint8_t header[18] = {0};
    switch (colorType) {
        case 0: {
            header[2] = 3;
            header[16] = 8;
            break;
        } // Image type: uncompressed grayscale
        case 1: {
            header[2] = 2;
            header[16] = 24;
            break;
        }
        default: {
            cerr << "Error: Invalid colorType: " << colorType << endl;
            file.close();
            return false;
        }
    }
    header[12] = static_cast<uint8_t>(m_width % 256);
    header[13] = static_cast<uint8_t>(m_width / 256);
    header[14] = static_cast<uint8_t>(m_height % 256);
    header[15] = static_cast<uint8_t>(m_height / 256);
    header[17] = 0x20; // No alpha, no origin flip

    file.write(reinterpret_cast<const char *>(header), sizeof(header));

    switch (colorType) {
        case 0: {
            for (int i = 0; i < m_height; i++) {
                file.write(reinterpret_cast<const char *>(m_p_data[i]),
                           static_cast<streamsize>(m_width * sizeof(uint8_t)));
            }
            break;
        }
        case 1: {
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    for (int k = 0; k < 3; k++) {
                        file.write(reinterpret_cast<const char *>(&m_p_data[i][j]), sizeof(uint8_t));
                    }
                }
            }
            break;
        }
    }
    file.close();
    return true;
}

void Image::readTGA(const path& filename) {

    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file read" << endl;
        return;
    }

    uint8_t header[18] = {0};
    file.read(reinterpret_cast<char *>(header), sizeof(header));

    int imageType = header[2];
    int width = header[12] + (header[13] << 8);
    int height = header[14] + (header[15] << 8);
    uint8_t bitDepth = header[16];

    if (imageType != 3 && imageType != 2) {
        cerr << "Error: TGA file is not grayscale (type 3) or RGB (type 2), aborting read." << endl;
        file.close();
        return;
    }

    if (imageType == 3 && bitDepth != 8) {
        cerr << "Error: TGA grayscale file is not 8-bit, aborting read." << endl;
        file.close();
        return;
    }

    if (imageType == 2 && bitDepth != 24) {
        cerr << "Error: TGA RGB file is not 24-bit, aborting read." << endl;
        file.close();
        return;
    }

    if (m_p_data != nullptr) {
        for (int i = 0; i < m_height; i++) {
            delete[] m_p_data[i];
        }
        delete[] m_p_data;
    }

    m_width = width;
    m_height = height;

    m_p_data = new uint8_t *[m_height];
    for (int i = 0; i < m_height; i++) {
        m_p_data[i] = new uint8_t[m_width];
    }

    // if (imageType == 3) {
    //     // Grayscale image: read directly
    //     for (int i = 0; i < m_height; i++) {
    //         file.read(reinterpret_cast<char *>(m_p_data[i]), static_cast<streamsize>(m_width * sizeof(uint8_t)));
    //     }
    // } else if (imageType == 2) {
    //     // RGB image: read and convert to grayscale
    //     for (int i = 0; i < m_height; i++) {
    //         for (int j = 0; j < m_width; j++) {
    //             uint8_t bgr[3];
    //             file.read(reinterpret_cast<char *>(bgr), 3);
    //             m_p_data[i][j] = static_cast<uint8_t>(
    //                 (static_cast<int>(bgr[0]) + static_cast<int>(bgr[1]) + static_cast<int>(bgr[2])) / 3);
    //         }
    //     }
    // }

    switch (imageType) {
        case 3: {
            for (int i = 0; i < m_height; i++) {
                file.read(reinterpret_cast<char *>(m_p_data[i]), static_cast<streamsize>(m_width * sizeof(uint8_t)));
            }
            break;
        }
        case 2: {
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    uint8_t bgr[3];
                    file.read(reinterpret_cast<char *>(bgr), 3);
                    m_p_data[i][j] = static_cast<uint8_t>(
                        (static_cast<int>(bgr[0]) + static_cast<int>(bgr[1]) + static_cast<int>(bgr[2])) / 3);
                }
            }
            break;
        }
        default: {
            cerr << "Error: TGA file is not grayscale (type 3) or RGB (type 2), aborting read." << endl;
            file.close();
            return;
        }
    }

    file.close();
}

void Image::setVal(int x, int y, uint8_t val) {
    m_p_data[y][x] = val;
}

bool Image::writeTIF(const path& filename, int colorType) {

    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: Empty or invalid image, not writing TIF." << endl;
        return false;
    }
    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }
    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        file.close();
        return false;
    }

    // --- HEADER (8 bytes) ---
    uint8_t header[4] = {0};
    header[0] = 0x49; // 'I' = Little-endian
    header[1] = 0x49; // 'I'
    header[2] = 0x2A; // 42 magic number
    header[3] = 0x00;
    uint32_t ifd_offset = 8; // Offset to IFD (immediately after header)

    file.write(reinterpret_cast<const char *>(header), sizeof(header));
    file.write(reinterpret_cast<const char *>(&ifd_offset), sizeof(uint32_t));

    // --- IFD (Image File Directory) ---
    uint16_t entry_count = 9;
    file.write(reinterpret_cast<const char *>(&entry_count), sizeof(uint16_t)); // Number of IFD entries (2 bytes)

    // Helper lambda to write raw tag entries (2 + 2 + 4 + 4 = 12 bytes each)
    auto writeTag = [&](uint16_t tag, uint16_t type, uint32_t count, uint32_t value_or_offset) {
        file.write(reinterpret_cast<const char *>(&tag), sizeof(uint16_t));
        file.write(reinterpret_cast<const char *>(&type), sizeof(uint16_t));
        file.write(reinterpret_cast<const char *>(&count), sizeof(uint32_t));
        file.write(reinterpret_cast<const char *>(&value_or_offset), sizeof(uint32_t));
    };

    // Calculate needed values
    uint32_t width = static_cast<uint32_t>(m_width);
    uint32_t height = static_cast<uint32_t>(m_height);
    uint32_t bits_per_sample = 8;
    uint32_t samples_per_pixel;
    uint32_t strip_byte_count = width * height; // Size in bytes
    bool isRGB = false;
    switch (colorType) {
        case 0: {
            samples_per_pixel = 1;
            break;
        }
        case 1: {
            samples_per_pixel = 3;
            strip_byte_count = width * height * 3;
            isRGB = true;
            break;
        }
        default: {
            cerr << "Error: Invalid colorType: " << colorType << endl;
            file.close();
            return false;
        }
    }
    uint32_t rows_per_strip = height; // One strip for the whole image

    // For RGB, we need to write a BitsPerSample array of 3 shorts [8,8,8] after the IFD entries and next IFD offset
    uint32_t bits_per_sample_offset = 0;
    if (isRGB) {
        // header(4) + ifd_offset(4) + entry_count(2) + 9*12 (entries) + next_ifd(4) = 4+4+2+108+4=122
        bits_per_sample_offset = 8 + 2 + (entry_count * 12) + 4;
    }
    // Pixel data offset is after all that
    uint32_t strip_offset = 8 + 2 + (entry_count * 12) + 4;
    if (isRGB) {
        strip_offset += 6; // 3 shorts for BitsPerSample array
    }

    // Write IFD entries (order doesn't matter, but this is standard):
    writeTag(256, 4, 1, width); // ImageWidth (Tag 256, Type LONG = 4 bytes)
    writeTag(257, 4, 1, height); // ImageLength (Tag 257, Type LONG)
    if (!isRGB) {
        writeTag(258, 3, 1, bits_per_sample); // BitsPerSample (Tag 258, Type SHORT = 2 bytes)
    } else {
        // For RGB: count=3, value_or_offset=offset to bits_per_sample array
        writeTag(258, 3, 3, bits_per_sample_offset);
    }
    writeTag(259, 3, 1, 1); // Compression (Tag 259, Type SHORT, 1 = no compression)
    if (!isRGB) {
        writeTag(262, 3, 1, 1); // PhotometricInterpretation (Tag 262, Type SHORT, 1 = BlackIsZero)
    } else {
        writeTag(262, 3, 1, 2); // PhotometricInterpretation (Tag 262, Type SHORT, 2 = RGB)
    }
    writeTag(273, 4, 1, strip_offset); // StripOffsets (Tag 273, Type LONG, where pixel data starts)
    writeTag(277, 3, 1, samples_per_pixel); // SamplesPerPixel (Tag 277, Type SHORT, 1 for grayscale)
    writeTag(278, 4, 1, rows_per_strip); // RowsPerStrip (Tag 278, Type LONG)
    writeTag(279, 4, 1, strip_byte_count); // StripByteCounts (Tag 279, Type LONG, total pixel data size)

    // --- Next IFD offset (4 bytes) ---
    uint32_t next_ifd_offset = 0; // 0 = no more IFDs
    file.write(reinterpret_cast<const char *>(&next_ifd_offset), sizeof(uint32_t));

    // --- BitsPerSample array for RGB ---
    if (isRGB) {
        uint16_t bps[3] = {8, 8, 8};
        file.write(reinterpret_cast<const char *>(bps), 3 * sizeof(uint16_t));
    }

    // --- PIXEL DATA ---
    // Image is stored bottom-to-top, left-to-right (standard TIFF order for uncompressed)
    // If you want simple top-to-bottom (not strict TIFF spec but works for most viewers),
    // just loop y=0 to height-1
    switch (colorType) {
        case 0: {
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    file.write(reinterpret_cast<const char *>(&m_p_data[i][j]), sizeof(uint8_t));
                }
            }
            break;
        }
        case 1: {
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    uint8_t val = m_p_data[i][j];
                    file.write(reinterpret_cast<const char *>(&val), sizeof(uint8_t));
                    file.write(reinterpret_cast<const char *>(&val), sizeof(uint8_t));
                    file.write(reinterpret_cast<const char *>(&val), sizeof(uint8_t));
                }
            }
            break;
        }
    }

    file.close();
    return true;
}

void Image::displayImage() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0, 100000);
    int rand = dist(gen);
    string filename = "cache/temp_" + std::to_string(rand) + ".tif";
    this->writeTIF(filename, Grayscale);
#ifdef __APPLE__
    string command = "open -a Preview \"" + filename + "\"";
#elif defined(_WIN32)
    string command = "start \"\" \"" + filename + "\"";
#else
    string command = "xdg-open \"" + filename + "\"";
#endif
    system(command.c_str());
    // std::filesystem::remove(filename);
    // std::filesystem::remove_all("cache");
}

void displayImage(const path& filename) {
    string filenameStr = filename.string();
#ifdef __APPLE__
    string command = "open -a Preview \"" + filenameStr + "\"";
#elif defined(_WIN32)
    string command = "start \"\" \"" + filenameStr + "\"";
#else
    string command = "xdg-open \"" + filenameStr + "\"";
#endif
    system(command.c_str());
};

void displayTestImage() {
    Image image(1920, 1080, 0); // black background

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distX(0, 1920 - 1);
    uniform_int_distribution<> distY(0, 1080 - 1);
    uniform_int_distribution<> distSize(5, 50); // square sizes

    for (int i = 0; i < 100; ++i) {
        int x = distX(gen);
        int y = distY(gen);
        int size = distSize(gen);
        for (int dy = 0; dy < size && y + dy < 1080; ++dy) {
            for (int dx = 0; dx < size && x + dx < 1920; ++dx) {
                image.setVal(x + dx, y + dy, 255);
            }
        }
    }

    image.displayImage();
}