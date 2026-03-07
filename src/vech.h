#ifndef VECH_H
#define VECH_H

// -- types --

typedef struct VecH {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };

        float v[4];
    };
} VecH;

// -- interface --

float VecH_Dot(VecH a, VecH b);

#endif