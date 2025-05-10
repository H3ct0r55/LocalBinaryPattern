//
// Created by Hector van der Aa on 5/10/25.
//

#include "TypeDetect.h"
#include "Image.h"

int detectFileType(const string& ext, bool input) {
    if (ext == ".imat") return IMAT;
    if ((ext == ".tif" || ext == ".tiff") && !input) return TIFF;
    if (ext == ".tga") return TGA;
    if (ext == ".csv" && !input) return CSV;
    if (ext == ".hist" && !input) return HIST;
    return UNSUPPORTED_FILETYPE;
}

int detectHistType(const string& hist) {
    if (hist == "raw") return RAW;
    if (hist == "normalized") return NORMAL;
    return UNSUPPORTED_HISTTYPE;
}

int detectRotationType(const string& rot) {
    if (rot == "cw") return CW;
    if (rot == "ccw") return CCW;
    return UNSUPPORTED_ROTATION;
}

int detectStartPosition(const string& pos) {
    if (pos == "tl") return TL;
    if (pos == "tc") return TC;
    if (pos == "tr") return TR;
    if (pos == "cr") return CR;
    if (pos == "bl") return BL;
    if (pos == "bc") return BC;
    if (pos == "br") return BR;
    if (pos == "cl") return CL;
    return UNSUPPORTED_POSITION;
}

bool matchOutput(const int outputType, const int computeType) {
    if (computeType == LocalBinaryPattern && outputType != CSV && outputType != HIST) return true;
    if (computeType == Histogram && (outputType == CSV || outputType == HIST)) return true;
    return false;
}