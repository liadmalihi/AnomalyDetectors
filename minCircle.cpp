/*
 * minCircle.cpp
 *
 * Author: 318948809 Liad Malihi
 */
#include "minCircle.h" 


float theDis(const Point& p1, const Point& p2) 
{ 
  float x = p1.x - p2.x;
  float y = p1.y - p2.y;
  float powx = x*x;
  float powy = y*y;
    return sqrt(powx + powy); 
} 
//The function receives 2 points, creates a circle from them and returns it
Circle circleFromTwoPoints(const Point& p1, const Point& p2) 
{ 
    // Set the center to be the midpoint of A and B
  float avgX =  (p1.x + p2.x) / 2;
  float avgY = (p1.y + p2.y) / 2;
    Point centerPoint = { avgX,avgY }; 
  
    // Set the radius to be half the distance AB 
    return { centerPoint, theDis(p1, p2) / 2}; 
} 
//The function receives 3 points, creates a circle from them and returns it
Circle getCircleTreePoints(const Point& p1, const Point& p2, const Point& p3) 
{ 
  float x1 = p2.x - p1.x;
  float y1 = p2.y - p1.y;
  float x2 = p3.x - p1.x;
  float y2 =  p3.y - p1.y;
  float B = x1 * x1 + y1 * y1; 
    float C = x2 * x2 + y2 * y2; 
    float D = x1 * y2 - y1 * x2; 
    Point E= { (y2 * B - y1 * C) / (2 * D), 
             (x1 * C - x2 * B) / (2 * D) }; 
  
    E.x += p1.x; 
    E.y += p1.y; 
    return { E, theDis(E, p1) }; 
} 
//The function receives a point and a circle and returns if the point is in a circle
bool is_exist(const Circle& c,const Point& a){
  float dis=theDis(c.center,a);
  if(c.radius>=dis){
    return true;
  }
  return false;

}
//The function gets vector of points, size, vector of radius and returns the minimum circle for all points
//(Traverses with the size reduction and checks for each point if it is in the circle it creates)
Circle getMinCircle(vector<Point>& points,size_t size,vector<Point> rad){
  int r_s=rad.size();
  if(r_s==3){
    //if we can create circle from 2 points
    for (int i = 0; i < 3; i++) {
      int j=(i+1)%3;
            Circle c2 = circleFromTwoPoints(rad[i], rad[j]);
             if (is_exist(c2,rad[3-i-j]))
              return c2;
        }
    return getCircleTreePoints(rad[0],rad[1],rad[2]);
  }
  else if(size==0){
    if(r_s==0){
      return Circle(Point(0,0),0);
    }
    //If the point outside the circle - you will create a new circle that the center is this point
    else if(r_s==1){
      return Circle(Point(rad[0].x,rad[0].y),0);
    }
    //If there are 2 points outside the circle - you will create them in a new circle
    else if(r_s==2){
      return circleFromTwoPoints(rad[0],rad[1]);
    }
    return Circle(Point(0,0),0);

  }
  
  Circle c=getMinCircle(points,size-1,rad);
  //if the point exist in this circle
  if (is_exist(c,points[size-1])){
    return c;
  }
  //if the point outside the circle-add the point to vector rad
  rad.push_back(points[size-1]);
  return getMinCircle(points,size-1,rad);
}
//The function gets a pointer to an array of points, and its size — and creates a vector of points
vector<Point> getVectorOfPoint(Point** point,size_t size){
  vector<Point> points;
  for(int i=0;i<size;i++){
    Point p(point[i]->x,point[i]->y);
    points.push_back(p);
  }
  return points;
}
// The base function - receives a pointer to an array of points and size - and returns a minimum circle
Circle findMinCircle(Point** points,size_t size){
  vector<Point> r;
  vector<Point> p;
  p=getVectorOfPoint(points,size);
  return getMinCircle(p,size,r);
}