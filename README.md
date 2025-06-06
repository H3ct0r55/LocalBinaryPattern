# LocalBinaryPattern

**LocalBinaryPattern** is a C++ tool and library for computing Local Binary Patterns (LBPs) and their corresponding histograms. LBPs are useful in image processing for texture classification and feature extraction.

## Status ![status: release](https://img.shields.io/badge/status-release-blue)

Fully functional release version with documented APIs, a modular architecture, and reliable LBP/histogram pipelines.  
Latest Release: `v1.0.2`

## Features

- Computes LBPs using a 3×3 neighborhood, with optional rotation invariance.
- Supports `.TGA` and `.IMAT` (Proprietary Image Matrix format) for input.
- Outputs to `.TGA`, `.IMAT`, and `.TIF` formats for LBP images.
- Outputs histogram data to `.HIST` (Proprietary Histogram format) and `.CSV`.
- Optional display of computed LBP image.
- Customizable edge handling methods.
- Internally casts 8-bit binary arrays into a single decimal value to encode patterns
- Offers four strategies for edge handling: Crop, Black, White, and Mirror
- Supports both raw and normalized histogram generation
- Designed for modularity and portability, compatible with macOS, Linux, and Windows
- Implemented in modern C++20 with minimal dependencies

## Installation

Release builds are available. Visit the [Releases](https://github.com/H3ct0r55/LocalBinaryPattern/releases/) page to download the latest version for your platform.

You may also build from source using CMake if desired:
```
git clone https://github.com/H3ct0r55/LocalBinaryPattern.git
cd LocalBinaryPattern
mkdir build && cd build
cmake ..
make
```

## Usage

Options:
```
  -h, --help
      Show this help manual and exit.
```
```
  -i <filename>
      Specify input file.
      Supported formats: .IMAT, .TGA
```
```
  -o <filename>
      Specify output file.
      Supported formats: .IMAT, .TGA, .TIF, .CSV
```
```
  -e <edgeType>
      Specify edge handling method.
      Accepted values: CropEdge, BlackEdge, WhiteEdge, MirrorEdge
```
```
  -L
      Perform Local Binary Pattern (LBP) computation.
```
```
  -P <startPos>
      Specify the starting position for LBP.
      Accepted values: TL, TC, TR, CR, BR, BC, BL, CL
```
```
  -D <direction>
      Specify rotation direction for LBP.
      Accepted values: CW, CCW
```
```
  -I
      Enable rotation-invariant mode.
      Cannot be used with -P or -D.
```
```
  -H <histType>
      Perform histogram computation.
      Accepted values: Raw, Normalized
      The output histogram is written to a `.csv` or `.hist` file depending on the extension.
```
```
  -D
      Display the computed LBP image after processing.
```
```
  --interactive
      Launch the program in interactive mode.
```

## Examples

- Show help manual:
  ```
  LocalBinaryPattern -h
  ```

- Compute LBP and output to default name:
  ```
  LocalBinaryPattern -i demo.tga
  ```

- Compute LBP and save as a TIFF:
  ```
  LocalBinaryPattern -i demo.tga -o output.tif
  ```

- Compute LBP histogram and save as CSV:
  ```
  LocalBinaryPattern -i demo.tga -H -o output.csv
  ```

- Compute normalized LBP histogram and save:
  ```
  LocalBinaryPattern -i demo.tga -H Normalized -o output.csv
  ```

- Compute LBP with custom position and rotation:
  ```
  LocalBinaryPattern -i demo.tga -L -P TL -D CW
  ```

- Compute rotation-invariant LBP:
  ```
  LocalBinaryPattern -i demo.tga -L -I
  ```

- Launch interactive mode:
  ```
  LocalBinaryPattern --interactive (or) LocalBinaryPattern
  ```

## Documentation

[DOCS](https://github.com/H3ct0r55/LocalBinaryPattern/blob/main/DOCS/DOCS.pdf)


[French DOCS](https://github.com/H3ct0r55/LocalBinaryPattern/blob/main/DOCS/DOCS.FR.pdf)


## License

[MIT License](LICENSE)
