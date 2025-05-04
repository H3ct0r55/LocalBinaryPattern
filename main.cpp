#include <iostream>
#include "Image.h"
#include "help_text.h"
#include <filesystem>
#include <cctype>
namespace fs = std::filesystem;
using std::cout, std::endl;
string NAME = "LocalBinaryPattern";
string RELEASE = "0.1.alpha";
string RELEASE_DATE = "2025-05-03";


string toLowercase(const string& input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(), tolower);
    return result;
}

void printHelp() {
    cout << HELP_TEXT << endl;
}

int detectFileType(const string& ext, bool input) {
    if (ext == ".imat") return IMAT;
    if ((ext == ".tif" || ext == ".tiff") && !input) return TIFF;
    if (ext == ".tga") return TGA;
    if (ext == ".csv" && !input) return CSV;
    if (ext == ".hist" && !input) return HIST;
    return UNSUPPORTED_FILETYPE;
}

int detectEdgeType(const string& edge) {
    if (edge == "cropedge") return CropEdge;
    if (edge == "blackedge") return BlackBorder;
    if (edge == "whiteedge") return WhiteBorder;
    if (edge == "mirroredge") return MirrorBorder;
    return UNSUPPORTED_EDGETYPE;
}

int detectHistType(const string& hist) {
    if (hist == "raw") return RAW;
    if (hist == "normalized") return NORMAL;
    return UNSUPPORTED_HISTTYPE;
}

bool matchOutput(const int outputType, const int computeType) {
    if (computeType == LocalBinaryPattern && outputType != CSV && outputType != HIST) return true;
    if (computeType == Histogram && (outputType == CSV || outputType == HIST)) return true;
    return false;
}

int main(int argc, char* argv[]) {
    bool input = false;
    int inputType = UNSUPPORTED_FILETYPE;
    path inputFile;

    bool output = false;
    int outputType = UNSUPPORTED_FILETYPE;
    path outputFile;

    bool computeSpecified = false;
    int computeType = UNSUPPORTED_COMPUTETYPE;

    bool edgeSpecified = false;
    int edgeType = CropEdge;
    bool histSpecified = false;
    int histType = UNSUPPORTED_HISTTYPE;

    bool displayOutput = false;

    bool flagErrors = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            switch (argv[i][1]) {
                case 'h': {
                    printHelp();
                    return 0;
                }
                case 'i': {
                    if (!input) {
                        input = true;
                        inputFile = path(argv[i+1]);
                        break;
                    }
                    cout << "Error: cannot specify more than one input file" << endl;
                    flagErrors = true;
                    break;
                }
                case 'o': {
                    if (!output) {
                        output = true;
                        outputFile = path(argv[i+1]);
                        break;
                    }
                    cout << "Error: cannot specify more than one output file" << endl;
                    flagErrors = true;
                    break;
                }
                case 'L': {
                    if (!computeSpecified) {
                        computeSpecified = true;
                        computeType = LocalBinaryPattern;
                        break;
                    }
                    cout << "Error: cannot specify more than one instruction" << endl;
                    flagErrors = true;
                    break;
                }
                case 'H': {
                    if (!computeSpecified) {
                        computeSpecified = true;
                        computeType = Histogram;
                        if (!histSpecified) {
                            if (!argv[i+1] || argv[i+1][0] == '-') {
                                cout << "Error: no histogram type specified" << endl;
                                flagErrors = true;
                                break;
                            }
                            histSpecified = true;
                            histType = detectHistType(toLowercase(argv[i+1]));
                            if (histType == UNSUPPORTED_HISTTYPE) {
                                cout << "Error: unknown histogram type \"" << argv[i+1] << "\". ";
                                switch (argv[i+1][0]) {
                                    case 'n': case 'N': {
                                        cout << "Did you mean \"Normalized\" ?" << endl;
                                        break;
                                    }
                                    case 'r': case 'R': {
                                        cout << "Did you mean \"Raw\" ?" << endl;
                                        break;
                                    }
                                    default: {
                                        cout << endl;
                                        break;
                                    }
                                }
                                flagErrors = true;
                                break;
                            }
                        } else {
                            cout << "Error: cannot specify more than one histogram type" << endl;
                            flagErrors = true;
                            break;
                        }
                        break;
                    }
                    cout << "Error: cannot specify more than one instruction" << endl;
                    flagErrors = true;
                    break;
                }
                case 'e': {
                    if (!edgeSpecified) {
                        if (!argv[i+1] || argv[i+1][0] == '-') {
                            cout << "Error: no edge type specified" << endl;
                            flagErrors = true;
                            break;
                        }
                        edgeSpecified = true;
                        edgeType = detectEdgeType(toLowercase(argv[i+1]));
                        if (edgeType == UNSUPPORTED_EDGETYPE) {
                            cout << "Error: unknown edge type \"" << argv[i+1] << "\". ";
                            switch (argv[i+1][0]) {
                                case 'c': case 'C': {
                                    cout << "Did you mean \"CropEdge\" ?" << endl;
                                    break;
                                }
                                case 'b': case 'B': {
                                    cout << "Did you mean \"BlackEdge\" ?" << endl;
                                    break;
                                }
                                case 'w': case 'W': {
                                    cout << "Did you mean \"WhiteEdge\" ?" << endl;
                                    break;
                                }
                                case 'm': case 'M': {
                                    cout << "Did you mean \"MirrorEdge\" ?" << endl;
                                    break;
                                }
                                default: {
                                    cout << endl;
                                    break;
                                }
                            }
                            flagErrors = true;
                            break;
                        }
                        break;
                    }
                    cout << "Error: cannot specify more than one edge type" << endl;
                    flagErrors = true;
                    break;
                }
                case 'D': {
                    displayOutput = true;
                    break;
                }
                default: {
                    cout << "Error: unknown option \"" << argv[i] << "\"" << endl;
                    flagErrors = true;
                    break;
                }
            }
        }
    }

    if (input) {
        inputType = detectFileType(toLowercase(inputFile.extension()), true);
        if (inputType == UNSUPPORTED_FILETYPE) {
            cout << "Error: input file type: " << inputFile.extension() << " not supported" << endl;
            flagErrors = true;
        }
    }

    if (output) {
        outputType = detectFileType(toLowercase(outputFile.extension()), false);
        if (outputType == UNSUPPORTED_FILETYPE) {
            cout << "Error: output file type: " << outputFile.extension() <<" not supported" << endl;
            flagErrors = true;
        }
        if (!matchOutput(outputType, computeType)) {
            cout << "Error: output type mismatch" << endl;
            flagErrors = true;
        }
    }

    if (!input) {
        cout << "Error: no input file specified" << endl;
        flagErrors = true;
    }

    if (!computeSpecified) {
        cout << "Error: no instruction specified" << endl;
        flagErrors = true;
    }

    if (displayOutput && computeType != LocalBinaryPattern) {
        cout << "Error: cannot display histogram output with -D" << endl;
        flagErrors = true;
    }

    if (flagErrors) {
        cout << "Run with -h flag for the help manual" << endl;
        return 1;
    }

    if (!output) {
        if (computeSpecified && computeType == LocalBinaryPattern) {
            outputFile = inputFile.stem().string() + "_lbp.tga";
            output = true;
            outputType = TGA;
        } else if (computeSpecified && computeType == Histogram) {
            if (histSpecified && histType == RAW) {
                outputFile = inputFile.stem().string() + "_hist.r.csv";
                output = true;
                outputType = CSV;
            } else if (histSpecified && histType == NORMAL) {
                outputFile = inputFile.stem().string() + "_hist.n.csv";
                output = true;
                outputType = CSV;
            }
        }
        cout << "Warning: no output file specified, saving output as " << outputFile << endl;
    }

    if (computeType == LocalBinaryPattern) {
        Image image;
        switch (inputType) {
            case IMAT: {
                image.readIMAT(inputFile.c_str());
                break;
            }
            case TGA: {
                image.readTGA(inputFile.c_str());
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputType << "\"" << endl;
                return 1;
            }
        }
        Image lbp = image.computeLBP(edgeType);
        bool writeSuccess = false;
        switch (outputType) {
            case IMAT: {
                writeSuccess = lbp.writeIMAT(outputFile.c_str());
                break;
            }
            case TGA: {
                writeSuccess = lbp.writeTGA(outputFile.c_str(), Grayscale, true);
                break;
            }
            case TIFF: {
                writeSuccess = lbp.writeTIF(outputFile.c_str(), Grayscale, true);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid output type \"" << outputType << "\"" << endl;
                return 1;
            }
        }
        if (writeSuccess) {
            cout << "File successfully written as " << outputFile << endl;
        } else {
            cout << "Error while writing output file " << outputFile << endl;
        }
        if (displayOutput) {
            displayImage(outputFile.c_str());
        }

    } else if (computeType == Histogram) {
        Image image;
        switch (inputType) {
            case IMAT: {
                image.readIMAT(inputFile.c_str());
                break;
            }
            case TGA: {
                image.readTGA(inputFile.c_str());
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputType << "\"" << endl;
                return 1;
            }
        }
        Image lbp = image.computeLBP(edgeType);
        bool writeSuccess = false;
        uint32_t* rHist;
        double* nHist;
        switch (histType) {
            case RAW: {
                rHist = lbp.computeRawHist();
                switch (outputType) {
                    case HIST: {
                        writeSuccess = writeRHIST(rHist, outputFile.c_str());
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeRHISTCSV(rHist, outputFile.c_str());
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return 1;
                    }
                }
                break;
            }
            case NORMAL: {
                nHist = lbp.computeNormHist();
                switch (outputType) {
                    case HIST: {
                        writeSuccess = writeNHIST(nHist, outputFile.c_str());
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeNHISTCSV(nHist, outputFile.c_str());
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return 1;
                    }
                }
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid histType" << endl;
            }
        }
        if (writeSuccess) {
            cout << "File successfully written as " << outputFile << endl;
        } else {
            cout << "Error while writing output file " << outputFile << endl;
        }
    }

    return 0;
}
