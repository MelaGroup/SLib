#ifndef SPROCESSING_H
#define SPROCESSING_H
#include <deque>
#include <random>
#include <vector>
#include <SLib/smatrix.h>
#include <SLib/ssegmentationmap.h>


void Threshold(SMatrix& src,int threshold);
void Threshold(SSegmentationMap& src,int threshold);
int OtsuThreshold(const SMatrix& src);
int floodFill(SMatrix& src,int value,int x,int y);
std::vector<int> postThresholdSegmentation(SMatrix& src);
SMatrix takeSegment(const SMatrix& src,const SMatrix& map,int segment_id);


#endif // SPROCESSING_H
