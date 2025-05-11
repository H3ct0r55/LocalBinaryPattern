#include <iostream>
#include "Image.h"
#include "help_text.h"
#include <filesystem>
#include <cctype>
#include "InteractiveWizard.h"
#include "TypeDetect.h"
namespace fs = std::filesystem;
using std::cout, std::endl;
string NAME = "LocalBinaryPattern";
string VERSION = "v0.1.2";
string RELEASE = "0.1.2-alpha";
string RELEASE_DATE = "2025-05-10";


string toLowercase(const string& input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(), tolower);
    return result;
}

void printHelp() {
    cout << "Usage: LocalBinaryPattern [OPTIONS]" << endl << endl;
    cout << "Version: " << RELEASE << endl;
    cout << "Released: " << RELEASE_DATE << endl;
    cout << HELP_TEXT << endl;
}

int detectEdgeType(const string& edge) {
    if (edge == "cropedge") return CropEdge;
    if (edge == "blackedge") return BlackBorder;
    if (edge == "whiteedge") return WhiteBorder;
    if (edge == "mirroredge") return MirrorBorder;
    return UNSUPPORTED_EDGETYPE;
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

    bool rotationSpecified = false;
    int rotationType = CW;

    bool startPositionSpecified = false;
    int startPosition = TL;

    bool rotationInvariant = false;

    bool flagErrors = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            string arg = argv[i];
            if (arg == "--help") {
                printHelp();
                return 0;
            }
            if (arg == "--interactive") {
                cout << "Starting Interactive Wizard, please wait..." << endl << endl;
                LaunchInteractiveWizard();
                return 0;
            }
        }
        if (argv[i][0] == '-' && argv[i][1] != '-') {
            switch (argv[i][1]) {
                case 'v': {
                    cout << NAME << " " << VERSION << "\n\tRelease: " << RELEASE << "\n\tRelease Date: " << RELEASE_DATE << endl;
                    return 0;
                    break;
                }
                case 't': {
                    displayTestImage();
                    return 0;
                    break;
                }
                case 'h': {
                    printHelp();
                    return 0;
                    break;
                }
                case 'i': {
                    if (!input) {
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no input file specified after -i" << endl;
                            flagErrors = true;
                            break;
                        }
                        input = true;
                        inputFile = path(argv[i + 1]);
                        ++i; // skip the value we just consumed
                        break;
                    }
                    cout << "Error: cannot specify more than one input file" << endl;
                    flagErrors = true;
                    break;
                }
                case 'o': {
                    if (!output) {
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no output file specified after -o" << endl;
                            flagErrors = true;
                            break;
                        }
                        output = true;
                        outputFile = path(argv[i + 1]);
                        ++i; // skip the value we just consumed
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
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no histogram type specified after -H" << endl;
                            flagErrors = true;
                            break;
                        }
                        computeSpecified = true;
                        computeType = Histogram;

                        if (!histSpecified) {
                            histSpecified = true;
                            histType = detectHistType(toLowercase(argv[i + 1]));
                            if (histType == UNSUPPORTED_HISTTYPE) {
                                cout << "Error: unknown histogram type \"" << argv[i + 1] << "\". ";
                                switch (argv[i + 1][0]) {
                                    case 'n': case 'N': cout << "Did you mean \"Normalized\" ?" << endl; break;
                                    case 'r': case 'R': cout << "Did you mean \"Raw\" ?" << endl; break;
                                    default: cout << endl; break;
                                }
                                flagErrors = true;
                                break;
                            }
                        } else {
                            cout << "Error: cannot specify more than one histogram type" << endl;
                            flagErrors = true;
                            break;
                        }
                        ++i; // skip the value we just consumed
                        break;
                    }
                    cout << "Error: cannot specify more than one instruction" << endl;
                    flagErrors = true;
                    break;
                }
                case 'e': {
                    if (!edgeSpecified) {
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no edge type specified after -e" << endl;
                            flagErrors = true;
                            break;
                        }
                        edgeSpecified = true;
                        edgeType = detectEdgeType(toLowercase(argv[i + 1]));
                        if (edgeType == UNSUPPORTED_EDGETYPE) {
                            cout << "Error: unknown edge type \"" << argv[i + 1] << "\". ";
                            switch (argv[i + 1][0]) {
                                case 'c': case 'C': cout << "Did you mean \"CropEdge\" ?" << endl; break;
                                case 'b': case 'B': cout << "Did you mean \"BlackEdge\" ?" << endl; break;
                                case 'w': case 'W': cout << "Did you mean \"WhiteEdge\" ?" << endl; break;
                                case 'm': case 'M': cout << "Did you mean \"MirrorEdge\" ?" << endl; break;
                                default: cout << endl; break;
                            }
                            flagErrors = true;
                            break;
                        }
                        ++i; // skip the value we just consumed
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
                case 'P': {
                    if (!startPositionSpecified) {
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no start position specified after -P" << endl;
                            flagErrors = true;
                            break;
                        }
                        startPositionSpecified = true;
                        startPosition = detectStartPosition(toLowercase(argv[i + 1]));
                        if (startPosition == UNSUPPORTED_POSITION) {
                            cout << "Error: unknown start position \"" << argv[i + 1] << "\"." << endl;
                            flagErrors = true;
                            break;
                        }
                        ++i; // skip the value we just consumed
                    } else {
                        cout << "Error: cannot specify more than one start position" << endl;
                        flagErrors = true;
                    }
                    break;
                }
                case 'R': {
                    if (!rotationSpecified) {
                        if (i + 1 >= argc || argv[i + 1][0] == '-') {
                            cout << "Error: no rotation type specified after -R" << endl;
                            flagErrors = true;
                            break;
                        }
                        rotationSpecified = true;
                        rotationType = detectRotationType(toLowercase(argv[i + 1]));
                        if (rotationType == UNSUPPORTED_ROTATION) {
                            cout << "Error: unknown rotation type \"" << argv[i + 1] << "\"." << endl;
                            flagErrors = true;
                            break;
                        }
                        ++i; // skip the value we just consumed
                    } else {
                        cout << "Error: cannot specify more than one rotation type" << endl;
                        flagErrors = true;
                    }
                    break;
                }
                case 'I': {
                    rotationInvariant = true;
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
        inputType = detectFileType(toLowercase(inputFile.extension().string()), true);
        if (inputType == UNSUPPORTED_FILETYPE) {
            cout << "Error: input file type: " << inputFile.extension() << " not supported" << endl;
            flagErrors = true;
        }
    }

    if (output) {
        outputType = detectFileType(toLowercase(outputFile.extension().string()), false);
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

    if ((rotationSpecified || startPositionSpecified) && rotationInvariant) {
        cout << "Error: cannot run in rotation invariant mode with start position and direction" << endl;
        flagErrors = true;
    }

    if (flagErrors) {
        cout << "Run with -h flag for the help manual" << endl;
        return 0;
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
                image.readIMAT(inputFile);
                break;
            }
            case TGA: {
                image.readTGA(inputFile);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputType << "\"" << endl;
                return 0;
            }
        }
        Image lbp;
        if (!rotationInvariant) {
            lbp = image.computeLBP(edgeType, startPosition, rotationType);
        } else {
            lbp = image.computeRILBP(edgeType);
        }
        bool writeSuccess = false;
        switch (outputType) {
            case IMAT: {
                writeSuccess = lbp.writeIMAT(outputFile);
                break;
            }
            case TGA: {
                writeSuccess = lbp.writeTGA(outputFile, Grayscale);
                break;
            }
            case TIFF: {
                writeSuccess = lbp.writeTIF(outputFile, Grayscale);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid output type \"" << outputType << "\"" << endl;
                return 0;
            }
        }
        if (writeSuccess) {
            cout << "File successfully written as " << outputFile << endl;
        } else {
            cout << "Error while writing output file " << outputFile << endl;
        }
        if (displayOutput) {
            displayImage(outputFile);
        }

    } else if (computeType == Histogram) {
        Image image;
        switch (inputType) {
            case IMAT: {
                image.readIMAT(inputFile);
                break;
            }
            case TGA: {
                image.readTGA(inputFile);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputType << "\"" << endl;
                return 0;
            }
        }
        Image lbp;
        if (!rotationInvariant) {
            lbp = image.computeLBP(edgeType, startPosition, rotationType);
        } else {
            lbp = image.computeRILBP(edgeType);
        }
        bool writeSuccess = false;
        uint32_t* rHist;
        double* nHist;
        switch (histType) {
            case RAW: {
                rHist = lbp.computeRawHist();
                switch (outputType) {
                    case HIST: {
                        writeSuccess = writeRHIST(rHist, outputFile);
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeRHISTCSV(rHist, outputFile);
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return 0;
                    }
                }
                break;
            }
            case NORMAL: {
                nHist = lbp.computeNormHist();
                switch (outputType) {
                    case HIST: {
                        writeSuccess = writeNHIST(nHist, outputFile);
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeNHISTCSV(nHist, outputFile);
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return 0;
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