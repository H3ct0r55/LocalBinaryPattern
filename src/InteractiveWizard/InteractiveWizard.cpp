//
// Created by Hector van der Aa on 5/10/25.
//

#include "InteractiveWizard.h"
#include "../Image/Image.h"
#include <iostream>
#include <filesystem>
#include "../TypeDetect/TypeDetect.h"

namespace fs = std::filesystem;
using std::cout, std::cin, std::endl, std::string, fs::path, std::getline;

void clearConsole() {
#ifdef __APPLE__
    string command = "clear";
#elif defined(_WIN32)
    string command = "cls";
#else
    string command = "clear";
#endif
    system(command.c_str());
}

void LaunchInteractiveWizard() {
    clearConsole();
    bool reselect = false;
    path input;
    path output;
    int inputFileType = UNSUPPORTED_FILETYPE;
    int outputFileType = UNSUPPORTED_FILETYPE;
    int edgeType = UNSUPPORTED_EDGETYPE;
    int computeType = UNSUPPORTED_COMPUTETYPE;
    int histType = UNSUPPORTED_HISTTYPE;
    bool rotationInvariance = false;
    cout << "Welcome to the LocalBinaryPattern Interactive Wizard!" << endl;
    cout << "For advanced options please exit with Ctrl+C and run \"LocalBinaryPattern --help\" for advanced usage options" << endl;
    cout << "Press Enter to continue..." << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clearConsole();
    inputFile:
        while (true) {
            cout << "Please enter your input file path: ";
            string inputStr;
            getline(cin, inputStr);
            input = path(inputStr);
            inputFileType = detectFileType(input.extension().string(), true);
            if (inputFileType != IMAT && inputFileType != TGA) {
                cout << "Unsupported input file type: " << input.extension() << ". Only .imat and .tga are supported." << endl;
                continue;
            }
            if (exists(input)) {
                break;
            } else {
                cout << "Invalid path. Please try again." << endl;
            }
        }
        clearConsole();
        if (reselect) {
            reselect = false;
            goto verification;
        }
    edgeType:
        cout << "Please enter an edge type:\n\t0.CropEdge\n\t1.BlackEdge\n\t2.WhiteEdge\n\t3.MirrorEdge\n";
        if (reselect) {
            edgeType = UNSUPPORTED_EDGETYPE;
        }
        while (edgeType == UNSUPPORTED_EDGETYPE) {
            cout << "Selection [0-3]:";
            cin >> edgeType;
            if (edgeType < 0 || edgeType > 3) {
                edgeType = UNSUPPORTED_EDGETYPE;
                cout << "Invalid edge type. Please try again." << endl;
            }
        }
        clearConsole();
        if (reselect) {
            cin.ignore();
            reselect = false;
            goto verification;
        }
    rotationInv:
        cout << "Rotation Invariant LBP?[y/n]:";
        while (true) {
            char ch;
            cin >> ch;
            if (ch == 'n' || ch == 'N') {
                rotationInvariance = false;
                break;
            } else if (ch == 'y' || ch == 'Y') {
                rotationInvariance = true;
                break;
            }
            cout << "Invalid input. Please try again:";
        }
        if (reselect) {
            cin.ignore();
            reselect = false;
            goto verification;
        }
    clearConsole();
    computeType:
        cout << "Please entre the operation you wish to complete:\n\t0.LocalBinaryPattern\n\t1.Histogram\n";
        if (reselect) {
            computeType = UNSUPPORTED_COMPUTETYPE;
        }
        while (computeType == UNSUPPORTED_COMPUTETYPE) {
            cout << "Selection [0/1]:";
            cin >> computeType;
            if (computeType < 0 || computeType > 1) {
                computeType = UNSUPPORTED_COMPUTETYPE;
                cout << "Invalid compute type. Please try again." << endl;
            }
        }
        if (computeType == Histogram) {
            clearConsole();
            cout << "Please enter the histogram type:\n\t0.Raw\n\t1.Normalized\n";
            if (reselect) {
                histType = UNSUPPORTED_HISTTYPE;
            }
            while (histType == UNSUPPORTED_HISTTYPE) {
                cout << "Selection [0/1]:";
                cin >> histType;
                if (histType < 0 || histType > 1) {
                    histType = UNSUPPORTED_HISTTYPE;
                    cout << "Invalid Histogram type. Please try again." << endl;
                }
            }
        }
        clearConsole();
        if (reselect) {
            cin.ignore();
            reselect = false;
            goto verification;
        }
    outputFile:
        cin.ignore();
        while (true) {
            cout << "Please enter your output file path: ";
            string outputStr;
            getline(cin, outputStr);
            output = path(outputStr);
            outputFileType = detectFileType(output.extension().string(), false);

            bool validOutput = false;
            if (computeType == LocalBinaryPattern) {
                validOutput = (outputFileType == IMAT || outputFileType == TIFF || outputFileType == TGA);
            } else if (computeType == Histogram) {
                validOutput = (outputFileType == CSV || outputFileType == HIST);
            }

            if (validOutput) {
                break;
            } else {
                cout << "Invalid file type for this operation: " << output.extension() << endl;
            }
        }
        clearConsole();
        if (reselect) {
            reselect = false;
            goto verification;
        }
    verification:
        cout << "Summary:\n";
        cout << "----------------------\n";
        cout << "1. Input File: " << input << " (Type: " << input.extension() << ")\n";
        cout << "2. Edge Type: ";
        switch (edgeType) {
            case CropEdge: cout << "CropEdge"; break;
            case BlackBorder: cout << "BlackEdge"; break;
            case WhiteBorder: cout << "WhiteEdge"; break;
            case MirrorBorder: cout << "MirrorEdge"; break;
            default: cout << "Unknown"; break;
        }
        cout << "\n";
        cout << "3. Rotation Invariant: " << (rotationInvariance ? "Yes" : "No") << "\n";
        cout << "4. Compute Type: ";
        if (computeType == LocalBinaryPattern) {
            cout << "LocalBinaryPattern";
        } else if (computeType == Histogram) {
            cout << (histType == 0 ? "Raw Histogram" : "Normalized Histogram");
        } else {
            cout << "Unknown";
        }
        cout << "\n";
        cout << "5. Output File: " << output << " (Type: " << output.extension() << ")\n";
        cout << "----------------------" << endl;
        cout << "Press Enter to execute or enter a number [1-5] to modify a selection: ";
        string choice;
        getline(cin, choice);
        if (!choice.empty()) {
            reselect = true;
            clearConsole();
            switch (stoi(choice)) {
                case 1: goto inputFile;
                case 2: goto edgeType;
                case 3: goto rotationInv;
                case 4: goto computeType;
                case 5: goto outputFile;
                default: break;
            }
        }

    execution:
    if (computeType == LocalBinaryPattern) {
        Image image;
        switch (inputFileType) {
            case IMAT: {
                image.readIMAT(input);
                break;
            }
            case TGA: {
                image.readTGA(input);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputFileType << "\"" << endl;
                return;
            }
        }
        Image lbp;
        if (!rotationInvariance) {
            lbp = image.computeLBP(edgeType, TL, CW);
        } else {
            lbp = image.computeRILBP(edgeType);
        }
        bool writeSuccess = false;
        switch (outputFileType) {
            case IMAT: {
                writeSuccess = lbp.writeIMAT(output);
                break;
            }
            case TGA: {
                writeSuccess = lbp.writeTGA(output, Grayscale);
                break;
            }
            case TIFF: {
                writeSuccess = lbp.writeTIF(output, Grayscale);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid output type \"" << outputFileType << "\"" << endl;
                return;
            }
        }
        if (writeSuccess) {
            cout << "File successfully written as " << output << endl;
        } else {
            cout << "Error while writing output file " << output << endl;
        }
        cout << "Display Result?[y/n]: ";
        char ch;
        cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            displayImage(output);
        }
        cout << "Execution finished, exiting...";

    } else if (computeType == Histogram) {
        Image image;
        switch (inputFileType) {
            case IMAT: {
                image.readIMAT(input);
                break;
            }
            case TGA: {
                image.readTGA(input);
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid input type \"" << inputFileType << "\"" << endl;
                return;
            }
        }
        Image lbp;
        if (!rotationInvariance) {
            lbp = image.computeLBP(edgeType, TL, CW);
        } else {
            lbp = image.computeRILBP(edgeType);
        }
        bool writeSuccess = false;
        uint32_t* rHist;
        double* nHist;
        switch (histType) {
            case RAW: {
                rHist = lbp.computeRawHist();
                switch (outputFileType) {
                    case HIST: {
                        writeSuccess = writeRHIST(rHist, output);
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeRHISTCSV(rHist, output);
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return;
                    }
                }
                break;
            }
            case NORMAL: {
                nHist = lbp.computeNormHist();
                switch (outputFileType) {
                    case HIST: {
                        writeSuccess = writeNHIST(nHist, output);
                        break;
                    }
                    case CSV: {
                        writeSuccess = writeNHISTCSV(nHist, output);
                        break;
                    }
                    default: {
                        cout << "Uncaught Runtime Error: Invalid outputType" << endl;
                        return;
                    }
                }
                break;
            }
            default: {
                cout << "Uncaught Runtime Error: Invalid histType" << endl;
            }
        }
        delete[] rHist;
        delete[] nHist;
        if (writeSuccess) {
            cout << "File successfully written as " << output << endl;
        } else {
            cout << "Error while writing output file " << output << endl;
        }
        cout << "Execution finished, exiting..." << endl;
    }
}
