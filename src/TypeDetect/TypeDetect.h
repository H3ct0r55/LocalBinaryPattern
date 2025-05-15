#ifndef TYPEDETECT_H
#define TYPEDETECT_H

#include <string>
using std::string;

int detectFileType(const string& ext, bool input);
int detectHistType(const string& hist);
int detectRotationType(const string& rot);
int detectStartPosition(const string& pos);
bool matchOutput(const int outputType, const int computeType);

#endif //TYPEDETECT_H
