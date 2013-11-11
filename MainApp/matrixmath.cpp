#include "matrixmath.h"
#include "fbtext.h"
#include <assert.h>


CMatrix::CMatrix(const CMatrix& x)
{
    this->width  = x.width;
    this->height = x.height;
    this->data = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        this->data[i] = x.data[i];
    }
}


CMatrix::CMatrix(int width, int height)
{
    this->width  = width;
    this->height = height;
    this->data = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        this->data[i] = 0.0;
    }
}


CMatrix::CMatrix(int width, int height, const float* data)
{
    this->width  = width;
    this->height = height;
    this->data = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        this->data[i] = data[i];
    }
}


CMatrix::~CMatrix()
{
    delete [] data;
}



CMatrix& CMatrix::operator= (const CMatrix& x)
{
    delete [] data;
    this->width  = x.width;
    this->height = x.height;
    this->data = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        this->data[i] = x.data[i];
    }
    return *this;
}



CMatrix CMatrix::operator+(const CMatrix& x) const
{
    assert(this->width == x.width);
    assert(this->height == x.height);
    CMatrix out(x.width, x.height);
    for (int r = 0; r < x.height; r++) {
	for (int c = 0; c < x.width; c++) {
	    out.Set(c, r, this->Get(c,r) + x.Get(c,r));
	}
    }
    return out;
}


CMatrix CMatrix::operator-(const CMatrix& x) const
{
    assert(this->width == x.width);
    assert(this->height == x.height);
    CMatrix out(x.width, x.height);
    for (int r = 0; r < x.height; r++) {
	for (int c = 0; c < x.width; c++) {
	    out.Set(c, r, this->Get(c,r) - x.Get(c,r));
	}
    }
    return out;
}


CMatrix CMatrix::operator*(const CMatrix& x) const
{
    assert(this->width == x.height);
    CMatrix out(x.width, this->height);
    for (int r = 0; r < this->height; r++) {
	for (int c = 0; c < x.width; c++) {
	    float sum = 0.0;
	    for (int i = 0; i < width; i++) {
	        sum += this->Get(i, r) * x.Get(c, i);
	    }
	    out.Set(c, r,  sum);
	}
    }
    return out;
}


CMatrix CMatrix::operator+(float x) const
{
    CMatrix out(height, width);
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    out.Set(c, r, Get(c, r) + x);
	}
    }
    return out;
}


CMatrix CMatrix::operator-(float x) const
{
    CMatrix out(height, width);
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    out.Set(c, r, Get(c, r) - x);
	}
    }
    return out;
}


CMatrix CMatrix::operator*(float x) const
{
    CMatrix out(height, width);
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    out.Set(c, r, Get(c, r) * x);
	}
    }
    return out;
}


CMatrix CMatrix::operator/(float x) const
{
    CMatrix out(height, width);
    float complement = 1.0 / x;
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    out.Set(c, r, Get(c, r) * complement);
	}
    }
    return out;
}


float CMatrix::Determinant() const
{
    assert(width == height);
    if (width == 1) {
        return Get(0,0);
    }
    float sum = 0.0;
    float subdet;
    for (int i = 0; i < width; i++) {
	if (width == 2) {
	    subdet = Get(1-i, 1);
	} else {
	    subdet = Minor(i,0).Determinant();
	}
	if (i & 0x1) {
	    sum -= Get(i, 0) * subdet;
	} else {
	    sum += Get(i, 0) * subdet;
	}
    }
    return sum;
}


CMatrix CMatrix::Minor(int col, int row) const
{
    CMatrix minor(width-1, height-1);
    int y = 0;
    for (int r = 0; r < height; r++) {
	if (r != row) {
	    int x = 0;
	    for (int c = 0; c < width; c++) {
		if (c != col) {
		    minor.Set(x, y, this->Get(c, r));
		    x++;
		}
	    }
	    y++;
	}
    }
    return minor;
}


float CMatrix::Cofactor(int col, int row) const
{
    return Minor(col, row).Determinant();
}


CMatrix CMatrix::CofactorMatrix() const
{
    assert(width == height);
    CMatrix cof(width, height);
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    if (((r+c)&0x1) == 0) {
		cof.Set(c, r, Cofactor(c, r));
	    } else {
		cof.Set(c, r, -Cofactor(c, r));
	    }
	}
    }
    return cof;
}


CMatrix CMatrix::Transpose() const
{
    CMatrix out(height, width);
    for (int r = 0; r < height; r++) {
	for (int c = 0; c < width; c++) {
	    out.Set(r, c, Get(c, r));
	}
    }
    return out;
}


CMatrix CMatrix::Adjoint() const
{
    return CofactorMatrix().Transpose();
}


CMatrix CMatrix::Inverse() const
{
    return (Adjoint() / Determinant());
}


void CMatrix::Print() const
{
    for (int row = 0; row < height; row++) {
	for (int col = 0; col < width; col++) {
	    txtPrintf("%8.4g ", Get(col, row));
	}
	txtPrint("\n");
    }
}




