#ifndef SPROCESSING_H
#define SPROCESSING_H
#include <deque>
#include <random>
#include <vector>
#include <SLib/smatrix.h>
#include <SLib/ssegmentationmap.h>


void Threshold(SMatrix& src,int threshold);
int OtsuThreshold(const SMatrix& src);
int floodFill(SMatrix& src,int value,int x,int y);



#endif // SPROCESSING_H
