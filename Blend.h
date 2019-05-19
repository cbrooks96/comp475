#ifndef Blend_DEFINED
#define Blend_DEFINED

#include "GPaint.h"
#include "GPixel.h"

GPixel Clear(const GPixel source, const GPixel dest);
GPixel Src(const GPixel source, const GPixel dest);
GPixel Dst(const GPixel source, const GPixel dest);
GPixel SrcOver(const GPixel source, const GPixel dest);
GPixel DstOver(const GPixel source, const GPixel dest);
GPixel SrcIn(const GPixel source, const GPixel dest);
GPixel DstIn(const GPixel source, const GPixel dest);
GPixel SrcOut(const GPixel source, const GPixel dest);
GPixel DstOut(const GPixel source, const GPixel dest);
GPixel SrcATop(const GPixel source, const GPixel dest);
GPixel DstATop(const GPixel source, const GPixel dest);
GPixel Xor(const GPixel source, const GPixel dest);

typedef GPixel (*ModeProc)(GPixel, GPixel);

const ModeProc proc[12]={Clear, Src, Dst, SrcOver, DstOver, SrcIn, DstIn, SrcOut, DstOut, SrcATop, DstATop, Xor};

ModeProc blendProc(const GBlendMode mode);

ModeProc blendProc(const GBlendMode mode, const GPixel src);

#endif
