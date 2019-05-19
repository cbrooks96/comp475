#include "GPaint.h"
#include "GPixel.h"
#include "Blend.h"

GPixel Clear(const GPixel s, const GPixel d){
    return GPixel_PackARGB(0, 0, 0, 0);
}

GPixel Src(const GPixel s, const GPixel d) {
    return s;
}

GPixel Dst(const GPixel s, const GPixel d) {
    return d;
}

GPixel SrcOver(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = sA + ((((255 - sA) * dA) + 127) / 255);
    int r = sR + ((((255 - sA) * dR) + 127) / 255);
    int g = sG + ((((255 - sA) * dG) + 127) / 255);
    int b = sB + ((((255 - sA) * dB) + 127) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel DstOver(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = dA + ((((255 - dA) * sA) + 127) / 255);
    int r = dR + ((((255 - dA) * sR) + 127) / 255);
    int g = dG + ((((255 - dA) * sG) + 127) / 255);
    int b = dB + ((((255 - dA) * sB) + 127) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel SrcIn(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = ((((sA * dA) + 127)) / 255);
    int r = ((((dA * sR) + 127)) / 255);
    int g = ((((dA * sG) + 127)) / 255);
    int b = ((((dA * sB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel DstIn(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = ((((dA * sA) + 127)) / 255);
    int r = ((((sA * dR) + 127)) / 255);
    int g = ((((sA * dG) + 127)) / 255);
    int b = ((((sA * dB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel SrcOut(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = ((((sA * (255 - dA)) + 127)) / 255);
    int r = (((((255 - dA) * sR) + 127)) / 255);
    int g = (((((255 - dA) * sG) + 127)) / 255);
    int b = (((((255 - dA) * sB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel DstOut(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = ((((dA * (255 - sA)) + 127)) / 255);
    int r = (((((255 - sA) * dR) + 127)) / 255);
    int g = (((((255 - sA) * dG) + 127)) / 255);
    int b = (((((255 - sA) * dB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel SrcATop(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = dA;
    int r = ((((dA * sR) + 127)) / 255) + (((((255 - sA) * dR) + 127)) / 255);
    int g = ((((dA * sG) + 127)) / 255) + (((((255 - sA) * dG) + 127)) / 255);
    int b = ((((dA * sB) + 127)) / 255) + (((((255 - sA) * dB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel DstATop(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = sA;
    int r = ((((sA * dR) + 127)) / 255) + (((((255 - dA) * sR) + 127)) / 255);
    int g = ((((sA * dG) + 127)) / 255) + (((((255 - dA) * sG) + 127)) / 255);
    int b = ((((sA * dB) + 127)) / 255) + (((((255 - dA) * sB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

GPixel Xor(const GPixel s, const GPixel d) {
    int sA = GPixel_GetA(s);
    int sR = GPixel_GetR(s);
    int sG = GPixel_GetG(s);
    int sB = GPixel_GetB(s);

    int dA = GPixel_GetA(d);
    int dR = GPixel_GetR(d);
    int dG = GPixel_GetG(d);
    int dB = GPixel_GetB(d);

    int a = sA + dA - (2 * ((((sA * dA) + 127)) / 255));
    int r = (((((255 - dA) * sR) + 127)) / 255) + (((((255 - sA) * dR) + 127)) / 255);
    int g = (((((255 - dA) * sG) + 127)) / 255) + (((((255 - sA) * dG) + 127)) / 255);
    int b = (((((255 - dA) * sB) + 127)) / 255) + (((((255 - sA) * dB) + 127)) / 255);

    return GPixel_PackARGB(a, r, g, b);
}

ModeProc blendProc(const GBlendMode mode) {

    return proc[static_cast<int>(mode)];
}

ModeProc blendProc(const GBlendMode mode, const GPixel src) {

    return blendProc(mode);
}