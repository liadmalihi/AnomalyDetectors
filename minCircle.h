/*
 * minCircle.h
 *
 * Author: 318948809 Liad Malihi
 */
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
//#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <math.h>
using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//  float x,y;
//  Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
  Point center;
  float radius;
  Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------


float theDis(const Point& p1, const Point& p2);
Circle circleFromTwoPoints(const Point& p1, const Point& p2);
Circle getCircleTreePoints(const Point& p1, const Point& p2, const Point& p3);
bool is_exist(const Circle& c,const Point& a);
Circle getMinCircle(vector<Point>& points,size_t size,vector<Point> rad);
vector<Point> getVectorOfPoint(Point point,size_t size);
Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */