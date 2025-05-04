//
// Created by Hector van der Aa on 5/4/25.
//

#ifndef HELP_TEXT_H
#define HELP_TEXT_H

#pragma once
const char* HELP_TEXT = R"(
Usage: LocalBinaryPattern [OPTIONS]

Version: 0.1.alpha
Released: 2025-05-03

Options:
  -h, --help
      Show this help manual and exit.

  -i <filename>
      Specify input file.
      Supported formats: .IMAT, .TGA

  -o <filename>
      Specify output file.
      Supported formats: .IMAT, .TGA, .TIF, .CSV

  -e <edgeType>
      Specify edge handling method.
      Accepted values: CropEdge, BlackEdge, WhiteEdge, MirrorEdge

  -L
      Perform Local Binary Pattern (LBP) computation.

  -H <histType>
      Perform histogram computation.
      Accepted values: Raw, Normalized

  -D
      Display the computed LBP image after processing.

Examples:
  LocalBinaryPattern -h
      Show this help manual.

  LocalBinaryPattern -i demo.tga
      Compute the LBP of "demo.tga" and save as "demo_lbp.tga".

  LocalBinaryPattern -i demo.tga -o output.tif
      Compute the LBP of "demo.tga" and save to "output.tif".

  LocalBinaryPattern -i demo.tga -H -o output.csv
      Compute the LBP histogram of "demo.tga" and save to "output.r.csv".

  LocalBinaryPattern -i demo.tga -H Normalized -o output.csv
      Compute the normalized LBP histogram and save to "output.n.csv".
)";

#endif //HELP_TEXT_H
