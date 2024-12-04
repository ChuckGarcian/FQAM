// Credit: 1. https://stackoverflow.com/questions/17044052/how-to-create-a-phase-plot-for-a-2d-array-of-complex-numbers-with-matplotlib
// Credit: 2. https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion

#include <stdio.h>
#include "FQAM.h"
#include <complex.h>
#include <math.h>
#include "raylib.h"
#include "stdlib.h"
# define M_PI 3.14159265358979323846


/* Returns the distance 'r' associated with the polar form of complex number 'z' */
double complex_magnitude (FLA_Obj z)
{
  double x, y;
  x = FLA_DOUBLE_COMPLEX_PTR(z)->real;
  y = FLA_DOUBLE_COMPLEX_PTR(z)->imag;
  return sqrt (x * x + y * y);
}

/* Returns the angle associated with the polar form of complex number 'z' */
double complex_angle (FLA_Obj z) 
{
  double x, y;
  x = FLA_DOUBLE_COMPLEX_PTR(z)->real;
  y = FLA_DOUBLE_COMPLEX_PTR(z)->imag;
  return atan2 (y, x); 
}

double hueToRgb (double p, double q, double t)
{
  if (t < 0)
    t += 1;
  if (t > 1.0)
    t -= 1;
  if (t < (1.0 / 6.0))
    return p + (q - p) * 6.0 * t;
  if (t < (1.0 / 2.0))
    return q;
  if (t < (2.0 / 3))
    return p + (q - p) * ((2.0 / 3.0) - t) * 6.0;
  return p;
}


/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from https://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   {number}  buf     3 element result tuple (r, g, b)
 * @param   {number}  h       The hue
 * @param   {number}  s       The saturation
 * @param   {number}  l       The lightness
 * @return  {Array}           The RGB representation
 */
void hslToRgb (double *buf, double h, double s, double l)
{
  double r, g, b;

  if (s == 0)
  {
    r = g = b = l; // achromatic
  }
  else
  {
    const double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    const double p = 2 * l - q;
    r = hueToRgb (p, q, h + 1.0 / 3);
    g = hueToRgb (p, q, h);
    b = hueToRgb (p, q, h - 1.0 / 3);
  }

  // Set return buffer
  buf[0] = r;
  buf[1] = g;
  buf[2] = b;
}

extern double normalize (double value, double min_old, double max_old, double min_new,
                  double max_new);


/* Returns raylib color from a FLA complex amplitude */
Color get_color_from_complex_amplitude (FLA_Obj z)
{  
  double x, y;
  x = FLA_DOUBLE_COMPLEX_PTR(z)->real;
  y = FLA_DOUBLE_COMPLEX_PTR(z)->imag;
  printf ("x: %f, y: %f \n", x, y);

  // Computing the Polar form
  double radius, angle;
  radius = complex_magnitude (z);
  angle = complex_angle (z);
  printf ("r:\t%f\nangle:\t%f\n\n", radius, angle);

  // Computing the HLS form
  double h, l, s;
  h = ((angle + M_PI) / (2 * M_PI) + 0.5);
  l = 1.0 - 1.0 / (1.0 + pow (radius, 0.3));
  s = 0.8;
  printf ("h:\t%f\nl:\t%f\ns:\t%f\n\n", h, l, s);
  
  // Converting HSL to RGB
  double buf[3];
  hslToRgb (buf, h, s, l);  
  printf ("r:\t%f\ng:\t%f\nb:\t%f\n\n", buf[0], buf[1], buf[2]);

  unsigned char trans;
  radius = pow(radius, 4);
  trans = normalize (radius, 0, 1, 50, 250);
  
  unsigned char r, g, b;  
  r = min(floor(buf[0]*200),255);  
  g = min(floor(buf[1]*256),255);  
  b = min(floor(buf[2]*256),255);  
  printf ("r:\t%d\ng:\t%d\nb:\t%d, trans:\t%d\n\n", r, g, b, trans);
  
  return (Color){r, g, b, trans};  
}


