#ifndef UTILS_H
#define UTILS_H

/**

GLSL-like three dimensional vector
*/
typedef struct vec3
{
    double x;
    double y;
    double z;
} vec3;

/**

Color with RGB components
*/
typedef struct Color
{
    float red;
    float green;
    float blue;
} Color;
/**

Material
*/
typedef struct Material
{
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;
/**

Calculates radian from degree.
*/
double degree_to_radian(double degree);
/**

Generates random integer within bounds.
*/
int rand_int(int lower, int upper);
float rand_float(float min, float max);

#endif /* UTILS_H */