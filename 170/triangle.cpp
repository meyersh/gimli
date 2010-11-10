/*
 * Triangle.cpp - Shaun Meyer, Apr 2010
 * An exercise with basic euclidian geometry, 
 * points and a right-angle triangle in Object
 * form
 */


#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


/*
 * A geometric point!
 */

class point {
public:
  double x;
  double y;

  /* Constructor - default location is 0,0 */
  point(double x = 0.0, double y = 0.0)
  {
    this->x = x;
    this->y = y;
  }

  /* Nudge the X-coord a give amount */
  void nudgeX(double difference)
  {
    this->x += difference;
  }

  /* Nudge the Y-coord by a given amount */
  void nudgeY(double difference)
  {
    this->y += difference;
  }

  /*
   * Sum a given point to "nudge" it's coordinates
   */
  void nudge(point difference)
  {
    this->x += difference.x;
    this->y += difference.y;
  }

  /* Move coordinates to a given point */
  void reset(point newpoint)
  {
    this->x = newpoint.x;
    this->y = newpoint.y;
  }

  /* return pretty output. Eg, "x,y" */
  string pretty()
  {
    stringstream output;
    output << this->x  << ',' << this->y;
    return output.str();
  }
};

/*
 * A right triangle, a collection of two points in our case. 
 */

class rightTriangle {
private:
  /* Return absolute value, used for differencing. */
  double abs(double i)
  {
    if ( i < 0 )
      return i * -1;
    else
      return i;
  }


public:
  point a, b; /* A right-angle triangle only needs two points! */

  rightTriangle(point a, point b)
  {
    this->a = a;
    this->b = b;
  }


  /* return the distance between the `x`s */
  double xDistance()
  {
    return abs(this->a.x - this->b.x);
  }

  /* return the distance between the `y`s */
  double yDistance()
  {
    return abs(this->a.y - this->b.y);
  }


  /* return the c-side, or the hypotenuse. */
  double hypotenuse()
  {
    double a = this->xDistance();
    double b = this->yDistance();
    return sqrt(pow(a,2) + pow(b,2));
  }
};

int 
main()
{

  point apnt(5,5); /* point `a` */

  apnt.nudge( point(1,1) ); /* should be (6,6) now. */

  point bpnt(17,10); /* point `b` */

  rightTriangle rt( apnt, bpnt); /* a right triangle, `rt` */

  rt.b.nudge( point(1,1) ); /* should now be 18,11 */

  cout << "A side point: " << rt.a.pretty() << endl
       << "B side point: " << rt.b.pretty() << endl
       << "The Hypotenuse is: " << rt.hypotenuse() << endl;

  return 0;
}
