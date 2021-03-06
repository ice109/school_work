#ifndef SURFACES
#define SURFACES

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <GL/glut.h> // GLubyte
#include <iomanip> //std::setw

typedef boost::numeric::ublas::vector<int,std::vector<int> > int_vec;
typedef boost::numeric::ublas::vector<double,std::vector<double> > dbl_vec;
typedef boost::numeric::ublas::vector<long double,std::vector<long double> > long_dbl_vec;
typedef boost::numeric::ublas::vector<GLubyte,std::vector<GLubyte> > GLubyte_vec;
typedef boost::numeric::ublas::matrix<double> dbl_matrix;

#define CHECK_IMAGE_WIDTH 500
#define CHECK_IMAGE_HEIGHT 500
#define PHONG_EXP 15.0
#define K_S .2
#define K_D 1
#define K_A .2
#define ILLUM 1

static dbl_matrix rotation(3,3);
static dbl_vec light_src(3, std::vector<double>{1000,250,250});
static dbl_vec unit_light_src(light_src/norm_2(light_src));

//viewport pixel array
static GLubyte checkImage[CHECK_IMAGE_WIDTH][CHECK_IMAGE_HEIGHT][3];
static dbl_vec ray_directiion(3, std::vector<double>{-1,0,0});

//standard colors
static GLubyte_vec GLubyte_white(3,std::vector<GLubyte>{255,255,255});
static GLubyte_vec GLubyte_black(3,std::vector<GLubyte>{0,0,0});
static GLubyte_vec GLubyte_pink(3,std::vector<GLubyte>{255,102,102});
class surface{

public:

  virtual double intersection_test(dbl_vec, dbl_vec) = 0;
  virtual dbl_vec unit_normal(dbl_vec) = 0;
  virtual void test() = 0;
  GLubyte color[3];

  GLubyte_vec* shader(dbl_vec point);  
};

class triangle : public surface{

public:

  dbl_vec vertexA, vertexB, vertexC;
  dbl_vec edgeAB, edgeBC, edgeCA;
  //necessary for testing for intersection. plucker_edgeXY = {D, D X P} as discussed in
  //report.
  dbl_vec plucker_edgeAB[2];
  dbl_vec plucker_edgeBC[2];
  dbl_vec plucker_edgeCA[2];
  dbl_vec normal;

  triangle(GLubyte surface_color[3], dbl_vec, dbl_vec, dbl_vec);
  dbl_vec unit_normal(dbl_vec);
  double intersection_test(dbl_vec, dbl_vec);
  void test();

};


class sphere : public surface{ 

public:
  dbl_vec center;
  double radius;
  
  sphere(GLubyte surface_color[3], int_vec, double);

  dbl_vec unit_normal(dbl_vec);
  double intersection_test(dbl_vec, dbl_vec);
  void test();
};


inline void rotate_viewing_angle(double x, double y, double z){
  double PI = 3.141592653589793;
  double X = x*PI/180;
  double Y = y*PI/180;
  double Z = z*PI/180;

  rotation(0,0) = cos(Y)*cos(Z);
  rotation(0,1) = cos(Z)*sin(X)*sin(Y)-cos(X)*sin(Z);
  rotation(0,2) = cos(X)*cos(Z)*sin(Y)+sin(X)*sin(Z);

  rotation(1,0) = cos(Y)*sin(Z);
  rotation(1,1) = cos(X)*cos(Z)+sin(X)*sin(Y)*sin(Z);
  rotation(1,2) = -cos(Z)*sin(X)+cos(X)*sin(Y)*sin(Z);

  rotation(2,0) = -sin(Y);
  rotation(2,1) = cos(Y)*sin(X);
  rotation(2,2) = cos(X)*cos(Y);

  ray_directiion = prod(rotation,ray_directiion);
  // std::cout << "ray direction " << ray_directiion << std::endl;
}

inline dbl_vec cross_product(dbl_vec a, dbl_vec b){
  dbl_vec V_r(3,std::vector<double>{a[1]*b[2]-a[2]*b[1], 
	                            a[2]*b[0]-a[0]*b[2],
	                            a[0]*b[1]-a[1]*b[0]});
  return V_r;
}

static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;
    
    float ratio  =  x/(float)n;
    int   c      =  ratio * w;
 
    std::cout << std::setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) std::cout << "=";
    for (int x=c; x<w; x++) std::cout << " ";
    std::cout << "]\r" << std::flush;
}



#endif
