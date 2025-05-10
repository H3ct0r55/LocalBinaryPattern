//
// Created by Hector van der Aa on 5/4/25.
//

#ifndef HELP_TEXT_H
#define HELP_TEXT_H

#pragma once
const char* HELP_TEXT = R"(

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

  -P <startPos>
      Specify the starting position for LBP.
      Accepted values: TL, TC, TR, CR, BR, BC, BL, CL

  -D <direction>
      Specify rotation direction for LBP.
      Accepted values: CW, CCW

  -I
      Enable rotation-invariant mode.
      Cannot be used with -P or -D.

  -H <histType>
      Perform histogram computation.
      Accepted values: Raw, Normalized

  -D
      Display the computed LBP image after processing.

  --interactive
      Launch the program in interactive mode.

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

  LocalBinaryPattern -i demo.tga -L -P TL -D CW
      Compute the LBP of "demo.tga" using top-left start and clockwise rotation.

  LocalBinaryPattern -i demo.tga -L -I
      Compute the rotation-invariant LBP of "demo.tga".

  LocalBinaryPattern --interactive
      Launch the application in interactive mode.
)";

#endif //HELP_TEXT_H
