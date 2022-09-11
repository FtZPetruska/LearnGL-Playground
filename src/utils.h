#ifndef utils_h
#define utils_h 1

typedef struct
{
    float r, g, b, a;
} Colour;

void updateColours(Colour *colour);

#define clamp(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

#endif /* utils_h */
