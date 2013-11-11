#ifndef MATRIXMATH_H
#define MATRIXMATH_H

class CMatrix {
public:
    CMatrix(const CMatrix& x);
    CMatrix(int width, int height);
    CMatrix(int width, int height, const float* data);
    ~CMatrix();

    CMatrix& operator= (const CMatrix& x);

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float Get(int xpos, int ypos) const { return data[ypos*width+xpos]; }
    void Set(int xpos, int ypos, float value) {data[ypos*width+xpos] = value;}

    CMatrix Minor(int col, int row) const;
    float Determinant() const;
    float Cofactor(int col, int row) const;
    CMatrix CofactorMatrix() const;
    CMatrix Transpose() const;
    CMatrix Adjoint() const;
    CMatrix Inverse() const;

    CMatrix operator+(const CMatrix& x) const;
    CMatrix operator-(const CMatrix& x) const;
    CMatrix operator*(const CMatrix& x) const;

    CMatrix operator+(float x) const;
    CMatrix operator-(float x) const;
    CMatrix operator*(float x) const;
    CMatrix operator/(float x) const;

    void Print() const;

private:
    int width, height;
    float* data;
};

#endif

