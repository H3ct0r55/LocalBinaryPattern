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
        cerr << "Error: Invalid image dimensions (" << width << "x" << height << "), initializing empty image." << endl;
    }
}

Image::Image(int width, int height, int value) : m_width(width), m_height(height), m_p_data(nullptr) {
    if (width > 0 && height > 0) {
        m_p_data = new uint8_t *[m_height];
        for (int i = 0; i < m_height; i++) {
            m_p_data[i] = new uint8_t[m_width];
        }
        if (value >= 0 && value <= 255) {
            this->valFill(value);
        } else {
            cerr << "Error: Invalid fill value (" << value << "), expected 0‑255. Filling with zeros." << endl;
            this->valFill(0);
        }
    } else {
        m_width = 0;
        m_height = 0;
        m_p_data = nullptr;
        cerr << "Error: Invalid image dimensions (" << width << "x" << height << "), initializing empty image." << endl;
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
        cerr << "Error: Invalid image dimensions (" << width << "x" << height << "), initializing empty image." << endl;
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
        cerr << "Error: Invalid source image for copy constructor, initializing empty image." << endl;
    }
}

Image::Image(const Image &image, int borderWidth, int borderValue) : m_width(image.m_width + 2 * borderWidth),
                                                                     m_height(image.m_height + 2 * borderWidth),
                                                                     m_p_data(nullptr) {
    if (m_width > 0 && m_height > 0 && image.m_p_data != nullptr) {
        if (borderValue < 0 || borderValue > 255) {
            cerr << "Error: Invalid border value (" << borderValue << "), expected 0‑255. Using 0 instead." << endl;
            borderValue = 0;
        }
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
        cerr << "Error: Invalid source image or dimensions for border copy constructor, initializing empty image." << endl;
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
        cerr << "Error: Invalid source image or dimensions for mirror‑border copy constructor, initializing empty image." << endl;
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
    } else {
        cerr << "Error: Cannot perform randFill on empty or uninitialized image." << endl;
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
    else {
        if (value < 0 || value > 255) {
            cerr << "Error: Invalid fill value (" << value << "), expected 0-255." << endl;
        } else {
            cerr << "Error: Cannot perform valFill on empty or uninitialized image." << endl;
        }
    }
}

void Image::fillRange(int startX, int startY, int endX, int endY, uint8_t val) {
    if (m_width > 0 && m_height > 0 && m_p_data != nullptr) {
        if (startX < 0 || startY < 0 || endX > m_width || endY > m_height || startX >= endX || startY >= endY) {
            cerr << "Error: Invalid fill range (" << startX << "," << startY << ") to (" << endX << "," << endY << ")." << endl;
            return;
        }
        for (int i = startY; i < endY; ++i) {
            for (int j = startX; j < endX; ++j) {
                m_p_data[i][j] = val;
            }
        }
    } else {
        cerr << "Error: Cannot perform fillRange on empty or uninitialized image." << endl;
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
        if (file.fail()) {
            cerr << "Error: Write failure for file: " << filename.filename() << endl;
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    cerr << "Error: Empty or invalid image, no file written." << endl;
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
    if (m_width <= 0 || m_height <= 0) {
        cerr << "Error: Invalid dimensions read from file: " << filename.filename() << endl;
        file.close();
        return;
    }

    m_p_data = new uint8_t *[m_height];
    for (int i = 0; i < m_height; i++) {
        m_p_data[i] = new uint8_t[m_width];
        file.read(reinterpret_cast<char *>(m_p_data[i]), static_cast<streamsize>(m_width * sizeof(uint8_t)));
    }
    if (file.fail()) {
        cerr << "Error: Unexpected end of file or read failure for file: " << filename.filename() << endl;
        for (int i = 0; i < m_height; ++i) {
            delete[] m_p_data[i];
        }
        delete[] m_p_data;
        m_p_data = nullptr;
        file.close();
        return;
    }

    file.close();
}


ostream &operator<<(ostream &os, const Image &image) {
    if (image.m_p_data == nullptr || image.m_width <= 0 || image.m_height <= 0) {
        cerr << "Error: Attempt to stream empty or uninitialized image." << endl;
        return os;
    }
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
    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: unwrapLocal called on empty or uninitialized image." << endl;
        return nullptr;
    }
    if (x > 0 && y > 0 && x < m_width - 1 && y < m_height - 1) {
        auto *temp = new uint8_t[8];
        for (int i = 0; i < 3; i++) {
            temp[i] = m_p_data[y - 1][x + i - 1];
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
    cerr << "Error: unwrapLocal cannot operate on edge pixel (" << x << "," << y << ")." << endl;
    return nullptr;
}

int Image::startPosRLBP(int x, int y) {
    if (m_p_data == nullptr) {
        cerr << "Error: startPosRLBP called on empty or uninitialized image." << endl;
        return -1;
    }
    if (x <= 0 || y <= 0 || x >= m_width - 1 || y >= m_height - 1) {
        cerr << "Error: startPosRLBP called on edge pixel (" << x << "," << y << ")." << endl;
        return -1;
    }
    auto* temp = this->unwrapLocal(x, y, TL, CW);
    if (temp == nullptr) {
        cerr << "Error: startPosRLBP failed to retrieve local window at (" << x << "," << y << ")." << endl;
        return -1;
    }
    int largest = 0;
    int largestVal = 0;
    int largestPos = 0;
    for (int i = 0; i < 8; i++) {
        int val = static_cast<int>(temp[i]);
        int diff = abs(val-m_p_data[y][x]);
        if (diff > largest) {
            largest = diff;
            largestVal = m_p_data[y][x];
            largestPos = i;
        }
    }
    delete[] temp;
    return largestPos;
}

uint8_t *Image::localLBP(int x, int y, int startPos, int rotation) {
    if (m_p_data == nullptr) {
        cerr << "Error: localLBP called on empty or uninitialized image." << endl;
        return nullptr;
    }
    if (x <= 0 || y <= 0 || x >= m_width - 1 || y >= m_height - 1) {
        cerr << "Error: localLBP called on edge pixel (" << x << "," << y << ")." << endl;
        return nullptr;
    }
    uint8_t *temp = this->unwrapLocal(x, y, startPos, rotation);
    if (temp == nullptr) {
        cerr << "Error: localLBP failed to retrieve local window at (" << x << "," << y << ")." << endl;
        return nullptr;
    }
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
    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: computeRawHist called on empty or uninitialized image." << endl;
        return nullptr;
    }
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
    if (temp == nullptr) {
        cerr << "Error: computeNormHist failed because raw histogram computation returned nullptr." << endl;
        return nullptr;
    }
    int total = 0;
    for (int i = 0; i < 256; i++) {
        total += static_cast<int>(temp[i]);
    }
    if (total == 0) {
        cerr << "Error: computeNormHist found total pixel count zero." << endl;
        delete[] temp;
        return nullptr;
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
        if (input.m_p_data == nullptr || input.m_width <= 0 || input.m_height <= 0) {
            cerr << "Error: Assignment from empty or uninitialized image." << endl;
            if (m_p_data != nullptr) {
                for (int i = 0; i < m_height; ++i) {
                    delete[] m_p_data[i];
                }
                delete[] m_p_data;
            }
            m_p_data = nullptr;
            m_width = 0;
            m_height = 0;
            return *this;
        }
        m_width = input.m_width;
        m_height = input.m_height;
        if (m_p_data != nullptr) {
            for (int i = 0; i < m_height; i++) {
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
    if (input == nullptr) {
        cerr << "Error: castToInt called with nullptr input." << endl;
        return 0;
    }
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t bit = input[7 - i];
        if (bit != 0 && bit != 1) {
            cerr << "Error: castToInt expects bits 0 or 1, found " << static_cast<int>(bit) << " at position " << (7 - i) << "." << endl;
            return 0;
        }
        if (bit == 1) {
            result |= static_cast<uint8_t>(1 << i);
        }
    }
    return result;
}

uint8_t castToInt(const uint8_t *input, bool rotationInvariant) {
    if (input == nullptr) {
        cerr << "Error: castToInt (rotationInvariant) called with nullptr input." << endl;
        return 0;
    }
    for (int i = 0; i < 8; ++i) {
        if (input[i] != 0 && input[i] != 1) {
            cerr << "Error: castToInt expects bits 0 or 1, found " << static_cast<int>(input[i]) << " at position " << i << "." << endl;
            return 0;
        }
    }
    if (!rotationInvariant) {
        return castToInt(input);
    }
    uint8_t minValue = 255;
    for (int r = 0; r < 8; ++r) {
        uint8_t value = 0;
        for (int i = 0; i < 8; ++i) {
            if (input[(r + 7 - i) % 8] == 1) {
                value |= static_cast<uint8_t>(1 << i);
            }
        }
        if (value < minValue) {
            minValue = value;
        }
    }
    return minValue;
}

bool writeRHIST(uint32_t *histogram, const path& filename) {
    if (histogram == nullptr) {
        cerr << "Error: writeRHIST called with nullptr histogram." << endl;
        return false;
    }

    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }

    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return false;
    }

    file.write(reinterpret_cast<char *>(histogram), 256 * sizeof(uint32_t));

    if (file.fail()) {
        cerr << "Error: Write failure for file: " << filename.filename() << endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool writeRHISTCSV(uint32_t *histogram, const path& filename) {
    if (histogram == nullptr) {
        cerr << "Error: writeRHISTCSV called with nullptr histogram." << endl;
        return false;
    }

    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }

    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return false;
    }

    const char* TopLine = "Value, Occurence\n";
    file.write(TopLine, static_cast<streamsize>(strlen(TopLine)));
    for (int i = 0; i < 256; i++) {
        file << i << ", " << static_cast<int>(histogram[i]) << "\n";
    }

    if (file.fail()) {
        cerr << "Error: Write failure for file: " << filename.filename() << endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool writeNHISTCSV(double *histogram, const path& filename) {
    if (histogram == nullptr) {
        cerr << "Error: writeNHISTCSV called with nullptr histogram." << endl;
        return false;
    }

    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }

    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return false;
    }

    const char* TopLine = "Value, Occurence\n";
    file.write(TopLine, static_cast<streamsize>(strlen(TopLine)));
    for (int i = 0; i < 256; i++) {
        file << i << ", " << std::fixed << setprecision(15) << histogram[i] << "\n";
    }

    if (file.fail()) {
        cerr << "Error: Write failure for file: " << filename.filename() << endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}


bool writeNHIST(double *histogram, const path& filename) {
    if (histogram == nullptr) {
        cerr << "Error: writeNHIST called with nullptr histogram." << endl;
        return false;
    }

    path dir = filename.parent_path();
    if (!dir.empty() && !exists(dir)) {
        create_directories(dir);
    }

    fstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file written" << endl;
        return false;
    }

    file.write(reinterpret_cast<char *>(histogram), 256 * sizeof(double));

    if (file.fail()) {
        cerr << "Error: Write failure for file: " << filename.filename() << endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

uint32_t *readRHIST(const path& filename) {
    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file read" << endl;
        return nullptr;
    }

    auto *histogram = new uint32_t[256];
    file.read(reinterpret_cast<char *>(histogram), 256 * sizeof(uint32_t));

    if (file.fail()) {
        cerr << "Error: Read failure for file: " << filename.filename() << endl;
        delete[] histogram;
        file.close();
        return nullptr;
    }

    file.close();
    return histogram;
}

double *readNHIST(const path& filename) {
    fstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename.filename() << ", no file read" << endl;
        return nullptr;
    }

    auto *histogram = new double[256];
    file.read(reinterpret_cast<char *>(histogram), 256 * sizeof(double));

    if (file.fail()) {
        cerr << "Error: Read failure for file: " << filename.filename() << endl;
        delete[] histogram;
        file.close();
        return nullptr;
    }

    file.close();
    return histogram;
}


Image Image::computeLBP(int edgeType, int startPos, int rotation) {
    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: computeLBP called on empty or uninitialized image." << endl;
        return {};
    }
    if (startPos < 0 || startPos > 7) {
        cerr << "Error: computeLBP received invalid startPos (" << startPos << "), expected 0‑7. Using 0." << endl;
        startPos = 0;
    }
    switch (edgeType) {
        case 0 : {
            if (m_width < 3 || m_height < 3) {
                cerr << "Error: Image too small (" << m_width << "x" << m_height << ") for CropEdge LBP (requires ≥3x3)." << endl;
                return {};
            }
            Image lbp(m_width - 2, m_height - 2);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for CropEdge LBP image." << endl;
                return {};
            }
            for (int i = 0; i < m_height - 2; i++) {
                for (int j = 0; j < m_width - 2; j++) {
                    if (lbp.m_p_data) {
                        auto *bits = this->localLBP(j + 1, i + 1, startPos, rotation);
                        lbp.m_p_data[i][j] = castToInt(bits);
                        delete[] bits;
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: CropEdge, of dimension smaller than 3x3" << endl;
                    }
                }
            }
            return lbp;
        }
        case 1 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for LBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1, 0);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, rotation);
                        lbp.m_p_data[i][j] = castToInt(bits);
                        delete[] bits;
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: WhiteBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 2 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for LBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1, 255);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, rotation);
                        lbp.m_p_data[i][j] = castToInt(bits);
                        delete[] bits;
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: BlackBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 3 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for LBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, rotation);
                        lbp.m_p_data[i][j] = castToInt(bits);
                        delete[] bits;
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
    if (m_p_data == nullptr || m_width <= 0 || m_height <= 0) {
        cerr << "Error: computeRILBP called on empty or uninitialized image." << endl;
        return {};
    }
    switch (edgeType) {
        case 0 : {
            if (m_width < 3 || m_height < 3) {
                cerr << "Error: Image too small (" << m_width << "x" << m_height << ") for CropEdge RILBP (requires ≥3x3)." << endl;
                return {};
            }
            Image lbp(m_width - 2, m_height - 2);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for CropEdge RILBP image." << endl;
                return {};
            }
            for (int i = 0; i < m_height - 2; i++) {
                for (int j = 0; j < m_width - 2; j++) {
                    if (lbp.m_p_data) {
                        int startPos = this->startPosRLBP(j + 1, i + 1);
                        if (startPos < 0) {
                            lbp.m_p_data[i][j] = 0;
                        } else {
                            auto *bits = this->localLBP(j + 1, i + 1, startPos, CW);
                            lbp.m_p_data[i][j] = castToInt(bits);
                            delete[] bits;
                        }
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: CropEdge, of dimension smaller than 3x3" << endl;
                    }
                }
            }
            return lbp;
        }
        case 1 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for RILBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1, 0);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        if (startPos < 0) {
                            lbp.m_p_data[i][j] = 0;
                        } else {
                            auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, CW);
                            lbp.m_p_data[i][j] = castToInt(bits);
                            delete[] bits;
                        }
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: WhiteBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 2 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for RILBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1, 255);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        if (startPos < 0) {
                            lbp.m_p_data[i][j] = 0;
                        } else {
                            auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, CW);
                            lbp.m_p_data[i][j] = castToInt(bits);
                            delete[] bits;
                        }
                    } else {
                        cerr << "Error: Unable to calculate LBP for edgeType: BlackBorder, of dimension smaller than 1x1" << endl;
                    }
                }
            }
            return lbp;
        }
        case 3 : {
            Image lbp(m_width, m_height);
            if (lbp.m_p_data == nullptr) {
                cerr << "Error: Memory allocation failed for RILBP image." << endl;
                return {};
            }
            Image tempBorder(*this, 1);
            for (int i = 0; i < m_height; i++) {
                for (int j = 0; j < m_width; j++) {
                    if (lbp.m_p_data) {
                        int startPos = tempBorder.startPosRLBP(j + 1, i + 1);
                        if (startPos < 0) {
                            lbp.m_p_data[i][j] = 0;
                        } else {
                            auto *bits = tempBorder.localLBP(j + 1, i + 1, startPos, CW);
                            lbp.m_p_data[i][j] = castToInt(bits);
                            delete[] bits;
                        }
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

    if (colorType != 0 && colorType != 1) {
        cerr << "Error: Invalid colorType: " << colorType << endl;
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
        }
        case 1: {
            header[2] = 2;
            header[16] = 24;
            break;
        }
        default: {
            break;
        }
    }
    header[12] = static_cast<uint8_t>(m_width % 256);
    header[13] = static_cast<uint8_t>(m_width / 256);
    header[14] = static_cast<uint8_t>(m_height % 256);
    header[15] = static_cast<uint8_t>(m_height / 256);
    header[17] = 0x20;

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
        default: {
            break;
        }
    }
    if (file.fail()) {
        cerr << "Error: Write failure for file: " << filename.filename() << endl;
        file.close();
        return false;
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

    if (width <= 0 || height <= 0) {
        cerr << "Error: Invalid dimensions (" << width << "x" << height << ") in TGA header." << endl;
        file.close();
        return;
    }

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
    if (m_p_data == nullptr) {
        cerr << "Error: Memory allocation failed for TGA image data." << endl;
        file.close();
        return;
    }

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
    }
    if (file.fail()) {
        cerr << "Error: Unexpected end of file or read failure for file: " << filename.filename() << endl;
        for (int i = 0; i < m_height; ++i) {
            delete[] m_p_data[i];
        }
        delete[] m_p_data;
        m_p_data = nullptr;
        file.close();
        return;
    }
    file.close();
}

void Image::setVal(int x, int y, uint8_t val) {
    if (m_p_data == nullptr || x < 0 || y < 0 || x >= m_width || y >= m_height) {
        cerr << "Error: setVal called with out‑of‑bounds coordinates (" << x << "," << y << ") or uninitialized image." << endl;
        return;
    }
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

    uint8_t header[4] = {0};
    header[0] = 0x49;
    header[1] = 0x49;
    header[2] = 0x2A;
    header[3] = 0x00;
    uint32_t ifd_offset = 8;

    file.write(reinterpret_cast<const char *>(header), sizeof(header));
    file.write(reinterpret_cast<const char *>(&ifd_offset), sizeof(uint32_t));

    uint16_t entry_count = 9;
    file.write(reinterpret_cast<const char *>(&entry_count), sizeof(uint16_t));

    auto writeTag = [&](uint16_t tag, uint16_t type, uint32_t count, uint32_t value_or_offset) {
        file.write(reinterpret_cast<const char *>(&tag), sizeof(uint16_t));
        file.write(reinterpret_cast<const char *>(&type), sizeof(uint16_t));
        file.write(reinterpret_cast<const char *>(&count), sizeof(uint32_t));
        file.write(reinterpret_cast<const char *>(&value_or_offset), sizeof(uint32_t));
    };

    uint32_t width = static_cast<uint32_t>(m_width);
    uint32_t height = static_cast<uint32_t>(m_height);
    uint32_t bits_per_sample = 8;
    uint32_t samples_per_pixel;
    uint32_t strip_byte_count = width * height;
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
    uint32_t rows_per_strip = height;

    uint32_t bits_per_sample_offset = 0;
    if (isRGB) {
        bits_per_sample_offset = 8 + 2 + (entry_count * 12) + 4;
    }
    uint32_t strip_offset = 8 + 2 + (entry_count * 12) + 4;
    if (isRGB) {
        strip_offset += 6;
    }

    writeTag(256, 4, 1, width);
    writeTag(257, 4, 1, height);
    if (!isRGB) {
        writeTag(258, 3, 1, bits_per_sample);
    } else {
        writeTag(258, 3, 3, bits_per_sample_offset);
    }
    writeTag(259, 3, 1, 1);
    if (!isRGB) {
        writeTag(262, 3, 1, 1);
    } else {
        writeTag(262, 3, 1, 2);
    }
    writeTag(273, 4, 1, strip_offset);
    writeTag(277, 3, 1, samples_per_pixel);
    writeTag(278, 4, 1, rows_per_strip);
    writeTag(279, 4, 1, strip_byte_count);

    uint32_t next_ifd_offset = 0;
    file.write(reinterpret_cast<const char *>(&next_ifd_offset), sizeof(uint32_t));

    if (isRGB) {
        uint16_t bps[3] = {8, 8, 8};
        file.write(reinterpret_cast<const char *>(bps), 3 * sizeof(uint16_t));
    }

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
    path cacheDir("cache");
    if (!exists(cacheDir)) {
        create_directories(cacheDir);
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0, 100000);
    int rand = dist(gen);
    string filename = "cache/temp_" + std::to_string(rand) + ".tif";
    if (!this->writeTIF(filename, Grayscale)) {
        cerr << "Error: Unable to write temporary TIF file for display." << endl;
        return;
    }
#ifdef __APPLE__
    string command = "open -a Preview \"" + filename + "\"";
#elif defined(_WIN32)
    string command = "start \"\" \"" + filename + "\"";
#else
    string command = "xdg-open \"" + filename + "\"";
#endif
    if (int ret = system(command.c_str()); ret != 0) {
        cerr << "Error: Failed to open image viewer (command returned " << ret << ")." << endl;
    }
}

void displayImage(const path& filename) {
    if (!exists(filename)) {
        cerr << "Error: displayImage called with non‑existent file \"" << filename.filename() << "\"." << endl;
        return;
    }
    string filenameStr = filename.string();
#ifdef __APPLE__
    string command = "open -a Preview \"" + filenameStr + "\"";
#elif defined(_WIN32)
    string command = "start \"\" \"" + filenameStr + "\"";
#else
    string command = "xdg-open \"" + filenameStr + "\"";
#endif
    if (int ret = system(command.c_str()); ret != 0) {
        cerr << "Error: Failed to open image viewer (command returned " << ret << ")." << endl;
    }
}

void displayTestImage() {
    Image image(1920, 1080, 0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distX(0, 1920 - 1);
    uniform_int_distribution<> distY(0, 1080 - 1);
    uniform_int_distribution<> distSize(5, 50);

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