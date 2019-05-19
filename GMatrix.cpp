#include <math.h>
#include "GMatrix.h"
#include "GPoint.h"

void GMatrix::setIdentity()
{
    this->fMat[SX] = this->fMat[SY] = 1;
    this->fMat[KY] = this->fMat[TX] = this->fMat[KX] = this->fMat[TY] = 0;
}

void GMatrix::setTranslate(float tx, float ty)
{
    this->fMat[SX] = this->fMat[SY] = 1;
    this->fMat[KX] = this->fMat[KY] = 0;
    this->fMat[TX] = tx;
    this->fMat[TY] = ty;
}

void GMatrix::setScale(float sx, float sy)
{
    this->fMat[SX] = sx;
    this->fMat[SY] = sy;
    this->fMat[KY] = this->fMat[TX] = this->fMat[KX] = this->fMat[TY] = 0;
}

void GMatrix::setRotate(float radians)
{
    this->fMat[SX] = this->fMat[SY] = std::cos(radians);
    this->fMat[KX] = -(std::sin(radians));
    this->fMat[TX] = this->fMat[TY] = 0;
    this->fMat[KY] = std::sin(radians);
}

void GMatrix::setConcat(const GMatrix& secundo, const GMatrix& primo)
{
    float sx = (secundo.fMat[SX] * primo.fMat[SX]) + (secundo.fMat[KX] * primo.fMat[KY]);
    float kx = (secundo.fMat[SX] * primo.fMat[KX]) + (secundo.fMat[KX] * primo.fMat[SY]);
    float tx = (secundo.fMat[SX] * primo.fMat[TX]) + (secundo.fMat[KX] * primo.fMat[TY]) + secundo.fMat[TX];
    float ky = (secundo.fMat[KY] * primo.fMat[SX]) + (secundo.fMat[SY] * primo.fMat[KY]);
    float sy = (secundo.fMat[KY] * primo.fMat[KX]) + (secundo.fMat[SY] * primo.fMat[SY]);
    float ty = (secundo.fMat[KY] * primo.fMat[TX]) + (secundo.fMat[SY] * primo.fMat[TY]) + secundo.fMat[TY];

    this->fMat[SX] = sx;
    this->fMat[KX] = kx;
    this->fMat[TX] = tx;
    this->fMat[KY] = ky;
    this->fMat[SY] = sy;
    this->fMat[TY] = ty;
}

bool GMatrix::invert(GMatrix* inverse) const
{
    float det = (this->fMat[SX] * this->fMat[SY]) - (this->fMat[KX] * this->fMat[KY]);

    if (det == 0)
    {
        return false;
    }

    if (inverse == nullptr)
    {
        return true;
    }

    float sx = this->fMat[SY] / det;
    float kx = -(this->fMat[KX]) / det;
    float tx = ((this->fMat[KX] * this->fMat[TY]) - (this->fMat[SY] * this->fMat[TX])) / det;
    float ky = -(this->fMat[KY]) / det;
    float sy = this->fMat[SX] / det;
    float ty = ((this->fMat[KY] * this->fMat[TX]) - (this->fMat[SX] * this->fMat[TY])) / det;

    inverse->fMat[SX] = sx;
    inverse->fMat[KX] = kx;
    inverse->fMat[TX] = tx;
    inverse->fMat[KY] = ky;
    inverse->fMat[SY] = sy;
    inverse->fMat[TY] = ty;

    return true;
}

void GMatrix::mapPoints(GPoint dst[], const GPoint src[], int count) const {

    for (int i = 0; i < count; ++i) {
        GPoint temp;

        temp.fX = (this->fMat[SX] * src[i].fX) + (this->fMat[KX] * src[i].fY) + this->fMat[TX];
        temp.fY = (this->fMat[KY] * src[i].fX) + (this->fMat[SY] * src[i].fY) + this->fMat[TY];

        dst[i].fX = temp.fX;
        dst[i].fY = temp.fY;
    }
}
