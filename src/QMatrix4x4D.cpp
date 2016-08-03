/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "QMatrix4x4D.hpp"
#include <QtCore/qmath.h>
#include <QMatrix>
#include <QTransform>

/*!
    \class QMatrix4x4D
    \brief The QMatrix4x4D class represents a 4x4 transformation matrix in 3D space.
    \since 4.6

    \sa QVector3D, QGenericMatrix
*/

/*!
    \fn QMatrix4x4D::QMatrix4x4D()

    Constructs an identity matrix.
*/

/*!
    Constructs a matrix from the given 16 floating-point \a values.
    The contents of the array \a values is assumed to be in
    row-major order.

    If the matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4D to optimize further
    calls to translate(), scale(), etc.

    \sa toValueArray(), inferSpecialType()
*/
QMatrix4x4D::QMatrix4x4D(const double *values)
{
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            m[col][row] = values[row * 4 + col];
    flagBits = General;
}

/*!
    \fn QMatrix4x4D::QMatrix4x4D(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44)

    Constructs a matrix from the 16 elements \a m11, \a m12, \a m13, \a m14,
    \a m21, \a m22, \a m23, \a m24, \a m31, \a m32, \a m33, \a m34,
    \a m41, \a m42, \a m43, and \a m44.  The elements are specified in
    row-major order.

    If the matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4D to optimize further
    calls to translate(), scale(), etc.

    \sa inferSpecialType()
*/

#if !defined(QT_NO_MEMBER_TEMPLATES) || defined(Q_QDOC)

/*!
    \fn QMatrix4x4D::QMatrix4x4D(const QGenericMatrix<N, M, double>& matrix)

    Constructs a 4x4 matrix from the left-most 4 columns and top-most
    4 rows of \a matrix.  If \a matrix has less than 4 columns or rows,
    the remaining elements are filled with elements from the identity
    matrix.

    \sa toGenericMatrix(), qGenericMatrixToMatrix4x4()
*/

/*!
    \fn QGenericMatrix<N, M, double> QMatrix4x4D::toGenericMatrix() const

    Constructs a NxM generic matrix from the left-most N columns and
    top-most M rows of this 4x4 matrix.  If N or M is greater than 4,
    then the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixFromMatrix4x4()
*/

#endif

/*!
    \fn QMatrix4x4D qGenericMatrixToMatrix4x4(const QGenericMatrix<N, M, double>& matrix)
    \relates QMatrix4x4D

    Returns a 4x4 matrix constructed from the left-most 4 columns and
    top-most 4 rows of \a matrix.  If \a matrix has less than 4 columns
    or rows, the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixFromMatrix4x4()
*/

/*!
    \fn QGenericMatrix<N, M, double> qGenericMatrixFromMatrix4x4(const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns a NxM generic matrix constructed from the left-most N columns
    and top-most M rows of \a matrix.  If N or M is greater than 4,
    then the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixToMatrix4x4(), QMatrix4x4D::toGenericMatrix()
*/

/*!
    \internal
*/
QMatrix4x4D::QMatrix4x4D(const double *values, int cols, int rows)
{
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            if (col < cols && row < rows)
                m[col][row] = values[col * rows + row];
            else if (col == row)
                m[col][row] = 1.0;
            else
                m[col][row] = 0.0;
        }
    }
    flagBits = General;
}

/*!
    Constructs a 4x4 matrix from a conventional Qt 2D affine
    transformation \a matrix.

    If \a matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4D to optimize further
    calls to translate(), scale(), etc.

    \sa toAffine(), inferSpecialType()
*/
QMatrix4x4D::QMatrix4x4D(const QMatrix& matrix)
{
    m[0][0] = matrix.m11();
    m[0][1] = matrix.m12();
    m[0][2] = 0.0;
    m[0][3] = 0.0;
    m[1][0] = matrix.m21();
    m[1][1] = matrix.m22();
    m[1][2] = 0.0;
    m[1][3] = 0.0;
    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
    m[2][3] = 0.0;
    m[3][0] = matrix.dx();
    m[3][1] = matrix.dy();
    m[3][2] = 0.0;
    m[3][3] = 1.0;
    flagBits = General;
}

/*!
    Constructs a 4x4 matrix from the conventional Qt 2D
    transformation matrix \a transform.

    If \a transform has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4D to optimize further
    calls to translate(), scale(), etc.

    \sa toTransform(), inferSpecialType()
*/
QMatrix4x4D::QMatrix4x4D(const QTransform& transform)
{
    m[0][0] = transform.m11();
    m[0][1] = transform.m12();
    m[0][2] = 0.0;
    m[0][3] = transform.m13();
    m[1][0] = transform.m21();
    m[1][1] = transform.m22();
    m[1][2] = 0.0;
    m[1][3] = transform.m23();
    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
    m[2][3] = 0.0;
    m[3][0] = transform.dx();
    m[3][1] = transform.dy();
    m[3][2] = 0.0;
    m[3][3] = transform.m33();
    flagBits = General;
}

/*!
    \fn double QMatrix4x4D::operator()(int row, int column) const

    Returns the element at position (\a row, \a column) in this matrix.

    \sa column(), row()
*/

/*!
    \fn double& QMatrix4x4D::operator()(int row, int column)

    Returns a reference to the element at position (\a row, \a column)
    in this matrix so that the element can be assigned to.

    \sa inferSpecialType(), setColumn(), setRow()
*/

/*!
    \fn QVector4D QMatrix4x4D::column(int index) const

    Returns the elements of column \a index as a 4D vector.

    \sa setColumn(), row()
*/

/*!
    \fn void QMatrix4x4D::setColumn(int index, const QVector4D& value)

    Sets the elements of column \a index to the components of \a value.

    \sa column(), setRow()
*/

/*!
    \fn QVector4D QMatrix4x4D::row(int index) const

    Returns the elements of row \a index as a 4D vector.

    \sa setRow(), column()
*/

/*!
    \fn void QMatrix4x4D::setRow(int index, const QVector4D& value)

    Sets the elements of row \a index to the components of \a value.

    \sa row(), setColumn()
*/

/*!
    \fn bool QMatrix4x4D::isIdentity() const

    Returns true if this matrix is the identity; false otherwise.

    \sa setToIdentity()
*/

/*!
    \fn void QMatrix4x4D::setToIdentity()

    Sets this matrix to the identity.

    \sa isIdentity()
*/

/*!
    \fn void QMatrix4x4D::fill(double value)

    Fills all elements of this matrx with \a value.
*/

// The 4x4 matrix inverse algorithm is based on that described at:
// http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q24
// Some optimization has been done to avoid making copies of 3x3
// sub-matrices and to unroll the loops.

// Calculate the determinant of a 3x3 sub-matrix.
//     | A B C |
// M = | D E F |   det(M) = A * (EI - HF) - B * (DI - GF) + C * (DH - GE)
//     | G H I |
static inline double matrixDet3
    (const double m[4][4], int col0, int col1, int col2,
     int row0, int row1, int row2)
{
    return m[col0][row0] *
                (m[col1][row1] * m[col2][row2] -
                 m[col1][row2] * m[col2][row1]) -
           m[col1][row0] *
                (m[col0][row1] * m[col2][row2] -
                 m[col0][row2] * m[col2][row1]) +
           m[col2][row0] *
                (m[col0][row1] * m[col1][row2] -
                 m[col0][row2] * m[col1][row1]);
}

// Calculate the determinant of a 4x4 matrix.
static inline double matrixDet4(const double m[4][4])
{
    double det;
    det  = m[0][0] * matrixDet3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * matrixDet3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * matrixDet3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * matrixDet3(m, 0, 1, 2, 1, 2, 3);
    return det;
}

/*!
    Returns the determinant of this matrix.
*/
double QMatrix4x4D::determinant() const
{
    return double(matrixDet4(m));
}

/*!
    Returns the inverse of this matrix.  Returns the identity if
    this matrix cannot be inverted; i.e. determinant() is zero.
    If \a invertible is not null, then true will be written to
    that location if the matrix can be inverted; false otherwise.

    If the matrix is recognized as the identity or an orthonormal
    matrix, then this function will quickly invert the matrix
    using optimized routines.

    \sa determinant(), normalMatrix()
*/
QMatrix4x4D QMatrix4x4D::inverted(bool *invertible) const
{
    // Handle some of the easy cases first.
    if (flagBits == Identity) {
        if (invertible)
            *invertible = true;
        return QMatrix4x4D();
    } else if (flagBits == Translation) {
        QMatrix4x4D inv;
        inv.m[3][0] = -m[3][0];
        inv.m[3][1] = -m[3][1];
        inv.m[3][2] = -m[3][2];
        inv.flagBits = Translation;
        if (invertible)
            *invertible = true;
        return inv;
    } else if (flagBits == Rotation || flagBits == (Rotation | Translation)) {
        if (invertible)
            *invertible = true;
        return orthonormalInverse();
    }

    QMatrix4x4D inv(1); // The "1" says to not load the identity.

    double det = matrixDet4(m);
    if (det == 0.0) {
        if (invertible)
            *invertible = false;
        return QMatrix4x4D();
    }
    det = 1.0 / det;

    inv.m[0][0] =  matrixDet3(m, 1, 2, 3, 1, 2, 3) * det;
    inv.m[0][1] = -matrixDet3(m, 0, 2, 3, 1, 2, 3) * det;
    inv.m[0][2] =  matrixDet3(m, 0, 1, 3, 1, 2, 3) * det;
    inv.m[0][3] = -matrixDet3(m, 0, 1, 2, 1, 2, 3) * det;
    inv.m[1][0] = -matrixDet3(m, 1, 2, 3, 0, 2, 3) * det;
    inv.m[1][1] =  matrixDet3(m, 0, 2, 3, 0, 2, 3) * det;
    inv.m[1][2] = -matrixDet3(m, 0, 1, 3, 0, 2, 3) * det;
    inv.m[1][3] =  matrixDet3(m, 0, 1, 2, 0, 2, 3) * det;
    inv.m[2][0] =  matrixDet3(m, 1, 2, 3, 0, 1, 3) * det;
    inv.m[2][1] = -matrixDet3(m, 0, 2, 3, 0, 1, 3) * det;
    inv.m[2][2] =  matrixDet3(m, 0, 1, 3, 0, 1, 3) * det;
    inv.m[2][3] = -matrixDet3(m, 0, 1, 2, 0, 1, 3) * det;
    inv.m[3][0] = -matrixDet3(m, 1, 2, 3, 0, 1, 2) * det;
    inv.m[3][1] =  matrixDet3(m, 0, 2, 3, 0, 1, 2) * det;
    inv.m[3][2] = -matrixDet3(m, 0, 1, 3, 0, 1, 2) * det;
    inv.m[3][3] =  matrixDet3(m, 0, 1, 2, 0, 1, 2) * det;

    if (invertible)
        *invertible = true;
    return inv;
}

/*!
    Returns the normal matrix corresponding to this 4x4 transformation.
    The normal matrix is the transpose of the inverse of the top-left
    3x3 part of this 4x4 matrix.  If the 3x3 sub-matrix is not invertible,
    this function returns the identity.

    \sa inverted()
*/
QMatrix4x4D QMatrix4x4D::normalMatrix() const
{
    QMatrix4x4D inv;

    // Handle the simple cases first.
    if (flagBits == Identity || flagBits == Translation) {
        return inv;
    } else if (flagBits == Scale || flagBits == (Translation | Scale)) {
        if (m[0][0] == 0.0 || m[1][1] == 0.0 || m[2][2] == 0.0)
            return inv;
        inv.data()[0] = 1.0 / m[0][0];
        inv.data()[5] = 1.0 / m[1][1];
        inv.data()[10] = 1.0 / m[2][2];
        return inv;
    }

    double det = matrixDet3(m, 0, 1, 2, 0, 1, 2);
    if (det == 0.0)
        return inv;
    det = 1.0 / det;

    double *invm = inv.data();

    // Invert and transpose in a single step.
    invm[0 + 0 * 4] =  (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
    invm[1 + 0 * 4] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * det;
    invm[2 + 0 * 4] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * det;
    invm[0 + 1 * 4] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * det;
    invm[1 + 1 * 4] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
    invm[2 + 1 * 4] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * det;
    invm[0 + 2 * 4] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
    invm[1 + 2 * 4] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * det;
    invm[2 + 2 * 4] =  (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * det;

    return inv;
}

/*!
    Returns this matrix, transposed about its diagonal.
*/
QMatrix4x4D QMatrix4x4D::transposed() const
{
    QMatrix4x4D result(1); // The "1" says to not load the identity.
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[col][row] = m[row][col];
        }
    }
    return result;
}

/*!
    \fn QMatrix4x4D& QMatrix4x4D::operator+=(const QMatrix4x4D& other)

    Adds the contents of \a other to this matrix.
*/

/*!
    \fn QMatrix4x4D& QMatrix4x4D::operator-=(const QMatrix4x4D& other)

    Subtracts the contents of \a other from this matrix.
*/

/*!
    \fn QMatrix4x4D& QMatrix4x4D::operator*=(const QMatrix4x4D& other)

    Multiplies the contents of \a other by this matrix.
*/

/*!
    \fn QMatrix4x4D& QMatrix4x4D::operator*=(double factor)
    \overload

    Multiplies all elements of this matrix by \a factor.
*/

/*!
    \overload

    Divides all elements of this matrix by \a divisor.
*/
QMatrix4x4D& QMatrix4x4D::operator/=(double divisor)
{
    m[0][0] /= divisor;
    m[0][1] /= divisor;
    m[0][2] /= divisor;
    m[0][3] /= divisor;
    m[1][0] /= divisor;
    m[1][1] /= divisor;
    m[1][2] /= divisor;
    m[1][3] /= divisor;
    m[2][0] /= divisor;
    m[2][1] /= divisor;
    m[2][2] /= divisor;
    m[2][3] /= divisor;
    m[3][0] /= divisor;
    m[3][1] /= divisor;
    m[3][2] /= divisor;
    m[3][3] /= divisor;
    flagBits = General;
    return *this;
}

/*!
    \fn bool QMatrix4x4D::operator==(const QMatrix4x4D& other) const

    Returns true if this matrix is identical to \a other; false otherwise.
    This operator uses an exact doubleing-point comparison.
*/

/*!
    \fn bool QMatrix4x4D::operator!=(const QMatrix4x4D& other) const

    Returns true if this matrix is not identical to \a other; false otherwise.
    This operator uses an exact doubleing-point comparison.
*/

/*!
    \fn QMatrix4x4D operator+(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
    \relates QMatrix4x4D

    Returns the sum of \a m1 and \a m2.
*/

/*!
    \fn QMatrix4x4D operator-(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
    \relates QMatrix4x4D

    Returns the difference of \a m1 and \a m2.
*/

/*!
    \fn QMatrix4x4D operator*(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
    \relates QMatrix4x4D

    Returns the product of \a m1 and \a m2.
*/

#ifndef QT_NO_VECTOR3D

/*!
    \fn QVector3D operator*(const QVector3D& vector, const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied post-vector.
*/

/*!
    \fn QVector3D operator*(const QMatrix4x4D& matrix, const QVector3D& vector)
    \relates QMatrix4x4D

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied pre-vector.
*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    \fn QVector4D operator*(const QVector4D& vector, const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied post-vector.
*/

/*!
    \fn QVector4D operator*(const QMatrix4x4D& matrix, const QVector4D& vector)
    \relates QMatrix4x4D

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied pre-vector.
*/

#endif

/*!
    \fn QPoint operator*(const QPoint& point, const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied post-point.
*/

/*!
    \fn QPointF operator*(const QPointF& point, const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied post-point.
*/

/*!
    \fn QPoint operator*(const QMatrix4x4D& matrix, const QPoint& point)
    \relates QMatrix4x4D

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied pre-point.
*/

/*!
    \fn QPointF operator*(const QMatrix4x4D& matrix, const QPointF& point)
    \relates QMatrix4x4D

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied pre-point.
*/

/*!
    \fn QMatrix4x4D operator-(const QMatrix4x4D& matrix)
    \overload
    \relates QMatrix4x4D

    Returns the negation of \a matrix.
*/

/*!
    \fn QMatrix4x4D operator*(double factor, const QMatrix4x4D& matrix)
    \relates QMatrix4x4D

    Returns the result of multiplying all elements of \a matrix by \a factor.
*/

/*!
    \fn QMatrix4x4D operator*(const QMatrix4x4D& matrix, double factor)
    \relates QMatrix4x4D

    Returns the result of multiplying all elements of \a matrix by \a factor.
*/

/*!
    \relates QMatrix4x4D

    Returns the result of dividing all elements of \a matrix by \a divisor.
*/
QMatrix4x4D operator/(const QMatrix4x4D& matrix, double divisor)
{
    QMatrix4x4D m(1); // The "1" says to not load the identity.
    m.m[0][0] = matrix.m[0][0] / divisor;
    m.m[0][1] = matrix.m[0][1] / divisor;
    m.m[0][2] = matrix.m[0][2] / divisor;
    m.m[0][3] = matrix.m[0][3] / divisor;
    m.m[1][0] = matrix.m[1][0] / divisor;
    m.m[1][1] = matrix.m[1][1] / divisor;
    m.m[1][2] = matrix.m[1][2] / divisor;
    m.m[1][3] = matrix.m[1][3] / divisor;
    m.m[2][0] = matrix.m[2][0] / divisor;
    m.m[2][1] = matrix.m[2][1] / divisor;
    m.m[2][2] = matrix.m[2][2] / divisor;
    m.m[2][3] = matrix.m[2][3] / divisor;
    m.m[3][0] = matrix.m[3][0] / divisor;
    m.m[3][1] = matrix.m[3][1] / divisor;
    m.m[3][2] = matrix.m[3][2] / divisor;
    m.m[3][3] = matrix.m[3][3] / divisor;
    return m;
}

/*!
    \fn bool qFuzzyCompare(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
    \relates QMatrix4x4D

    Returns true if \a m1 and \a m2 are equal, allowing for a small
    fuzziness factor for doubleing-point comparisons; false otherwise.
*/

#ifndef QT_NO_VECTOR3D

/*!
    Multiplies this matrix by another that scales coordinates by
    the components of \a vector.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::scale(const QVector3D& vector)
{
    double vx = vector.x();
    double vy = vector.y();
    double vz = vector.z();
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
        m[2][2] *= vz;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        m[2][0] *= vz;
        m[2][1] *= vz;
        m[2][2] *= vz;
        m[2][3] *= vz;
        flagBits = General;
    }
    return *this;
}
#endif

#if 1

/*!
    Multiplies this matrix by another that scales coordinates by
    the components of \a vector.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::scale(const QVector3DD& vector)
{
    if (flagBits == Identity) {
        m[0][0] = vector.xp;
        m[1][1] = vector.yp;
        m[2][2] = vector.zp;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vector.xp;
        m[1][1] *= vector.yp;
        m[2][2] *= vector.zp;
    } else if (flagBits == Translation) {
        m[0][0] = vector.xp;
        m[1][1] = vector.yp;
        m[2][2] = vector.zp;
        flagBits |= Scale;
    } else {
        m[0][0] *= vector.xp;
        m[0][1] *= vector.xp;
        m[0][2] *= vector.xp;
        m[0][3] *= vector.xp;
        m[1][0] *= vector.yp;
        m[1][1] *= vector.yp;
        m[1][2] *= vector.yp;
        m[1][3] *= vector.yp;
        m[2][0] *= vector.zp;
        m[2][1] *= vector.zp;
        m[2][2] *= vector.zp;
        m[2][3] *= vector.zp;
        flagBits = General;
    }
    return *this;
}
#endif

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, and \a y.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::scale(double x, double y)
{
    double vx(x);
    double vy(y);
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, \a y, and \a z.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::scale(double x, double y, double z)
{
    double vx(x);
    double vy(y);
    double vz(z);
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
        m[2][2] *= vz;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        m[2][0] *= vz;
        m[2][1] *= vz;
        m[2][2] *= vz;
        m[2][3] *= vz;
        flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    given \a factor.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::scale(double factor)
{
    if (flagBits == Identity) {
        m[0][0] = factor;
        m[1][1] = factor;
        m[2][2] = factor;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= factor;
        m[1][1] *= factor;
        m[2][2] *= factor;
    } else if (flagBits == Translation) {
        m[0][0] = factor;
        m[1][1] = factor;
        m[2][2] = factor;
        flagBits |= Scale;
    } else {
        m[0][0] *= factor;
        m[0][1] *= factor;
        m[0][2] *= factor;
        m[0][3] *= factor;
        m[1][0] *= factor;
        m[1][1] *= factor;
        m[1][2] *= factor;
        m[1][3] *= factor;
        m[2][0] *= factor;
        m[2][1] *= factor;
        m[2][2] *= factor;
        m[2][3] *= factor;
        flagBits = General;
    }
    return *this;
}

#ifndef QT_NO_VECTOR3D
/*!
    Multiplies this matrix by another that translates coordinates by
    the components of \a vector.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::translate(const QVector3D& vector)
{
    double vx = vector.x();
    double vy = vector.y();
    double vz = vector.z();
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        m[3][2] = vz;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
        m[3][2] += vz;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = m[2][2] * vz;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
        m[3][2] += m[2][2] * vz;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
        m[3][1] += m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
        m[3][2] += m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
        m[3][3] += m[0][3] * vx + m[1][3] * vy + m[2][3] * vz;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

#endif

#if 1
/*!
    Multiplies this matrix by another that translates coordinates by
    the components of \a vector.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::translate(const QVector3DD& vector)
{
    if (flagBits == Identity) {
        m[3][0] = vector.xp;
        m[3][1] = vector.yp;
        m[3][2] = vector.zp;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vector.xp;
        m[3][1] += vector.yp;
        m[3][2] += vector.zp;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vector.xp;
        m[3][1] = m[1][1] * vector.yp;
        m[3][2] = m[2][2] * vector.zp;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vector.xp;
        m[3][1] += m[1][1] * vector.yp;
        m[3][2] += m[2][2] * vector.zp;
    } else {
        m[3][0] += m[0][0] * vector.xp + m[1][0] * vector.yp + m[2][0] * vector.zp;
        m[3][1] += m[0][1] * vector.xp + m[1][1] * vector.yp + m[2][1] * vector.zp;
        m[3][2] += m[0][2] * vector.xp + m[1][2] * vector.yp + m[2][2] * vector.zp;
        m[3][3] += m[0][3] * vector.xp + m[1][3] * vector.yp + m[2][3] * vector.zp;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

#endif

/*!
    \overload

    Multiplies this matrix by another that translates coordinates
    by the components \a x, and \a y.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::translate(double x, double y)
{
    double vx(x);
    double vy(y);
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = 0.;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy;
        m[3][1] += m[0][1] * vx + m[1][1] * vy;
        m[3][2] += m[0][2] * vx + m[1][2] * vy;
        m[3][3] += m[0][3] * vx + m[1][3] * vy;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that translates coordinates
    by the components \a x, \a y, and \a z.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4D& QMatrix4x4D::translate(double x, double y, double z)
{
    double vx(x);
    double vy(y);
    double vz(z);
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        m[3][2] = vz;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
        m[3][2] += vz;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = m[2][2] * vz;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
        m[3][2] += m[2][2] * vz;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
        m[3][1] += m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
        m[3][2] += m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
        m[3][3] += m[0][3] * vx + m[1][3] * vy + m[2][3] * vz;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

#ifndef QT_NO_VECTOR3D

/*!
    Multiples this matrix by another that rotates coordinates through
    \a angle degrees about \a vector.  Returns this matrix.

    \sa scale(), translate()
*/
QMatrix4x4D& QMatrix4x4D::rotate(double angle, const QVector3D& vector)
{
    return rotate(angle, vector.x(), vector.y(), vector.z());
}

#endif

#if 1

/*!
    Multiples this matrix by another that rotates coordinates through
    \a angle degrees about \a vector.  Returns this matrix.

    \sa scale(), translate()
*/
QMatrix4x4D& QMatrix4x4D::rotate(double angle, const QVector3DD& vector)
{
    return rotate(angle, vector.xp, vector.yp, vector.zp);
}

#endif

/*!
    \overload

    Multiplies this matrix by another that rotates coordinates through
    \a angle degrees about the vector (\a x, \a y, \a z).  Returns this matrix.

    \sa scale(), translate()
*/
QMatrix4x4D& QMatrix4x4D::rotate(double angle, double x, double y, double z)
{
    QMatrix4x4D m(1); // The "1" says to not load the identity.
    double a = angle * M_PI / 180.0;
    double c = qCos(a);
    double s = qSin(a);
    double ic;
    bool quick = false;
    if (x == 0.0) {
        if (y == 0.0) {
            if (z != 0.0) {
                // Rotate around the Z axis.
                m.setToIdentity();
                m.m[0][0] = c;
                m.m[1][1] = c;
                if (z < 0.0) {
                    m.m[1][0] = s;
                    m.m[0][1] = -s;
                } else {
                    m.m[1][0] = -s;
                    m.m[0][1] = s;
                }
                m.flagBits = General;
                quick = true;
            }
        } else if (z == 0.0) {
            // Rotate around the Y axis.
            m.setToIdentity();
            m.m[0][0] = c;
            m.m[2][2] = c;
            if (y < 0.0) {
                m.m[2][0] = -s;
                m.m[0][2] = s;
            } else {
                m.m[2][0] = s;
                m.m[0][2] = -s;
            }
            m.flagBits = General;
            quick = true;
        }
    } else if (y == 0.0 && z == 0.0) {
        // Rotate around the X axis.
        m.setToIdentity();
        m.m[1][1] = c;
        m.m[2][2] = c;
        if (x < 0.0) {
            m.m[2][1] = s;
            m.m[1][2] = -s;
        } else {
            m.m[2][1] = -s;
            m.m[1][2] = s;
        }
        m.flagBits = General;
        quick = true;
    }
    if (!quick) {
        double len = x * x + y * y + z * z;
        if (!qFuzzyIsNull(len - 1.0) && !qFuzzyIsNull(len)) {
            len = qSqrt(len);
            x /= len;
            y /= len;
            z /= len;
        }
        ic = 1.0 - c;
        m.m[0][0] = x * x * ic + c;
        m.m[1][0] = x * y * ic - z * s;
        m.m[2][0] = x * z * ic + y * s;
        m.m[3][0] = 0.0;
        m.m[0][1] = y * x * ic + z * s;
        m.m[1][1] = y * y * ic + c;
        m.m[2][1] = y * z * ic - x * s;
        m.m[3][1] = 0.0;
        m.m[0][2] = x * z * ic - y * s;
        m.m[1][2] = y * z * ic + x * s;
        m.m[2][2] = z * z * ic + c;
        m.m[3][2] = 0.0;
        m.m[0][3] = 0.0;
        m.m[1][3] = 0.0;
        m.m[2][3] = 0.0;
        m.m[3][3] = 1.0;
    }
    int flags = flagBits;
    *this *= m;
    if (flags != Identity)
        flagBits = flags | Rotation;
    else
        flagBits = Rotation;
    return *this;
}

#ifndef QT_NO_VECTOR4D

/*!
    Multiples this matrix by another that rotates coordinates according
    to a specified \a quaternion.  The \a quaternion is assumed to have
    been normalized.  Returns this matrix.

    \sa scale(), translate(), QQuaternion
*/
QMatrix4x4D& QMatrix4x4D::rotate(const QQuaternion& quaternion)
{
    // Algorithm from:
    // http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q54
    QMatrix4x4D m(1);
    double xx = quaternion.x() * quaternion.x();
    double xy = quaternion.x() * quaternion.y();
    double xz = quaternion.x() * quaternion.z();
    double xw = quaternion.x() * quaternion.scalar();
    double yy = quaternion.y() * quaternion.y();
    double yz = quaternion.y() * quaternion.z();
    double yw = quaternion.y() * quaternion.scalar();
    double zz = quaternion.z() * quaternion.z();
    double zw = quaternion.z() * quaternion.scalar();
    m.m[0][0] = 1.0 - 2 * (yy + zz);
    m.m[1][0] =        2 * (xy - zw);
    m.m[2][0] =        2 * (xz + yw);
    m.m[3][0] = 0.0;
    m.m[0][1] =        2 * (xy + zw);
    m.m[1][1] = 1.0 - 2 * (xx + zz);
    m.m[2][1] =        2 * (yz - xw);
    m.m[3][1] = 0.0;
    m.m[0][2] =        2 * (xz - yw);
    m.m[1][2] =        2 * (yz + xw);
    m.m[2][2] = 1.0 - 2 * (xx + yy);
    m.m[3][2] = 0.0;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 1.0;
    int flags = flagBits;
    *this *= m;
    if (flags != Identity)
        flagBits = flags | Rotation;
    else
        flagBits = Rotation;
    return *this;
}

#endif

/*!
    \overload

    Multiplies this matrix by another that applies an orthographic
    projection for a window with boundaries specified by \a rect.
    The near and far clipping planes will be -1 and 1 respectively.
    Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4D& QMatrix4x4D::ortho(const QRect& rect)
{
    // Note: rect.right() and rect.bottom() subtract 1 in QRect,
    // which gives the location of a pixel within the rectangle,
    // instead of the extent of the rectangle.  We want the extent.
    // QRectF ex()resses the extent properly.
    return ortho(rect.x(), rect.x() + rect.width(), rect.y() + rect.height(), rect.y(), -1.0, 1.0);
}

/*!
    \overload

    Multiplies this matrix by another that applies an orthographic
    projection for a window with boundaries specified by \a rect.
    The near and far clipping planes will be -1 and 1 respectively.
    Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4D& QMatrix4x4D::ortho(const QRectF& rect)
{
    return ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), -1.0, 1.0);
}

/*!
    Multiplies this matrix by another that applies an orthographic
    projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.  Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4D& QMatrix4x4D::ortho(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (left == right || bottom == top || nearPlane == farPlane)
        return *this;

    // Construct the projection.
    double width = right - left;
    double invheight = top - bottom;
    double clip = farPlane - nearPlane;
#ifndef QT_NO_VECTOR3D
    if (clip == 2.0 && (nearPlane + farPlane) == 0.0) {
        // We can express this projection as a translate and scale
        // which will be more efficient to modify with further
        // transformations than producing a "General" matrix.
        translate(QVector3DD
            (-(left + right) / width,
             -(top + bottom) / invheight,
             0.0));
        scale(QVector3DD
            (2.0 / width,
             2.0 / invheight,
             -1.0));
        return *this;
    }
#endif
    QMatrix4x4D m(1);
    m.m[0][0] = 2.0 / width;
    m.m[1][0] = 0.0;
    m.m[2][0] = 0.0;
    m.m[3][0] = -(left + right) / width;
    m.m[0][1] = 0.0;
    m.m[1][1] = 2.0 / invheight;
    m.m[2][1] = 0.0;
    m.m[3][1] = -(top + bottom) / invheight;
    m.m[0][2] = 0.0;
    m.m[1][2] = 0.0;
    m.m[2][2] = -2.0 / clip;
    m.m[3][2] = -(nearPlane + farPlane) / clip;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 1.0;

    // Apply the projection.
    *this *= m;
    return *this;
}

/*!
    Multiplies this matrix by another that applies a perspective
    frustum projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.  Returns this matrix.

    \sa ortho(), perspective()
*/
QMatrix4x4D& QMatrix4x4D::frustum(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (left == right || bottom == top || nearPlane == farPlane)
        return *this;

    // Construct the projection.
    QMatrix4x4D m(1);
    double width = right - left;
    double invheight = top - bottom;
    double clip = farPlane - nearPlane;
    m.m[0][0] = 2.0 * nearPlane / width;
    m.m[1][0] = 0.0;
    m.m[2][0] = (left + right) / width;
    m.m[3][0] = 0.0;
    m.m[0][1] = 0.0;
    m.m[1][1] = 2.0 * nearPlane / invheight;
    m.m[2][1] = (top + bottom) / invheight;
    m.m[3][1] = 0.0;
    m.m[0][2] = 0.0;
    m.m[1][2] = 0.0;
    m.m[2][2] = -(nearPlane + farPlane) / clip;
    m.m[3][2] = -2.0 * nearPlane * farPlane / clip;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = -1.0;
    m.m[3][3] = 0.0;

    // Apply the projection.
    *this *= m;
    return *this;
}

/*!
    Multiplies this matrix by another that applies a perspective
    projection.  The field of view will be \a angle degrees within
    a window with a given \a aspect ratio.  The projection will
    have the specified \a nearPlane and \a farPlane clipping planes.
    Returns this matrix.

    \sa ortho(), frustum()
*/
QMatrix4x4D& QMatrix4x4D::perspective(double angle, double aspect, double nearPlane, double farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (nearPlane == farPlane || aspect == 0.0)
        return *this;

    // Construct the projection.
    QMatrix4x4D m(1);
    double radians = (angle / 2.0) * M_PI / 180.0;
    double sine = qSin(radians);
    if (sine == 0.0)
        return *this;
    double cotan = qCos(radians) / sine;
    double clip = farPlane - nearPlane;
    m.m[0][0] = cotan / aspect;
    m.m[1][0] = 0.0;
    m.m[2][0] = 0.0;
    m.m[3][0] = 0.0;
    m.m[0][1] = 0.0;
    m.m[1][1] = cotan;
    m.m[2][1] = 0.0;
    m.m[3][1] = 0.0;
    m.m[0][2] = 0.0;
    m.m[1][2] = 0.0;
    m.m[2][2] = -(nearPlane + farPlane) / clip;
    m.m[3][2] = -(2.0 * nearPlane * farPlane) / clip;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = -1.0;
    m.m[3][3] = 0.0;

    // Apply the projection.
    *this *= m;
    return *this;
}

#ifndef QT_NO_VECTOR3D

/*!
    Multiplies this matrix by another that applies an \a eye position
    transformation.  The \a center value indicates the center of the
    view that the \a eye is looking at.  The \a up value indicates
    which direction should be considered up with respect to the \a eye.
    Returns this matrix.
*/
QMatrix4x4D& QMatrix4x4D::lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up)
{
    QVector3D forward = (center - eye).normalized();
    QVector3D side = QVector3D::crossProduct(forward, up).normalized();
    QVector3D upVector = QVector3D::crossProduct(side, forward);

    QMatrix4x4D m(1);

    m.m[0][0] = side.x();
    m.m[1][0] = side.y();
    m.m[2][0] = side.z();
    m.m[3][0] = 0.0;
    m.m[0][1] = upVector.x();
    m.m[1][1] = upVector.y();
    m.m[2][1] = upVector.z();
    m.m[3][1] = 0.0;
    m.m[0][2] = -forward.x();
    m.m[1][2] = -forward.y();
    m.m[2][2] = -forward.z();
    m.m[3][2] = 0.0;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 1.0;

    *this *= m;
    return translate(-eye);
}

#endif

#if 1

/*!
    Multiplies this matrix by another that applies an \a eye position
    transformation.  The \a center value indicates the center of the
    view that the \a eye is looking at.  The \a up value indicates
    which direction should be considered up with respect to the \a eye.
    Returns this matrix.
*/
QMatrix4x4D& QMatrix4x4D::lookAt(const QVector3DD& eye, const QVector3DD& center, const QVector3DD& up)
{
    QVector3DD forward = (center - eye).normalized();
    QVector3DD side = QVector3DD::crossProduct(forward, up).normalized();
    QVector3DD upVector = QVector3DD::crossProduct(side, forward);

    QMatrix4x4D m(1);

    m.m[0][0] = side.xp;
    m.m[1][0] = side.yp;
    m.m[2][0] = side.zp;
    m.m[3][0] = 0.0;
    m.m[0][1] = upVector.xp;
    m.m[1][1] = upVector.yp;
    m.m[2][1] = upVector.zp;
    m.m[3][1] = 0.0;
    m.m[0][2] = -forward.xp;
    m.m[1][2] = -forward.yp;
    m.m[2][2] = -forward.zp;
    m.m[3][2] = 0.0;
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 1.0;

    *this *= m;
    return translate(-eye);
}

#endif


/*!
    Flips between right-handed and left-handed coordinate systems
    by multiplying the y and z co-ordinates by -1.  This is normally
    used to create a left-handed orthographic view without scaling
    the viewport as ortho() does.  Returns this matrix.

    \sa ortho()
*/
QMatrix4x4D& QMatrix4x4D::flipCoordinates()
{
    if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[1][1] = -m[1][1];
        m[2][2] = -m[2][2];
    } else if (flagBits == Translation) {
        m[1][1] = -m[1][1];
        m[2][2] = -m[2][2];
        flagBits |= Scale;
    } else if (flagBits == Identity) {
        m[1][1] = -1.0;
        m[2][2] = -1.0;
        flagBits = Scale;
    } else {
        m[1][0] = -m[1][0];
        m[1][1] = -m[1][1];
        m[1][2] = -m[1][2];
        m[1][3] = -m[1][3];
        m[2][0] = -m[2][0];
        m[2][1] = -m[2][1];
        m[2][2] = -m[2][2];
        m[2][3] = -m[2][3];
        flagBits = General;
    }
    return *this;
}

/*!
    Retrieves the 16 items in this matrix and writes them to \a values
    in row-major order.
*/
void QMatrix4x4D::toValueArray(double *values) const
{
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            values[row * 4 + col] = double(m[col][row]);
}

/*!
    Returns the conventional Qt 2D affine transformation matrix that
    corresponds to this matrix.  It is assumed that this matrix
    only contains 2D affine transformation elements.

    \sa toTransform()
*/
QMatrix QMatrix4x4D::toAffine() const
{
    return QMatrix(double(m[0][0]), double(m[0][1]),
                   double(m[1][0]), double(m[1][1]),
                   double(m[3][0]), double(m[3][1]));
}

/*!
    Returns the conventional Qt 2D transformation matrix that
    corresponds to this matrix.  It is assumed that this matrix
    only contains 2D transformation elements.

    \sa toAffine()
*/
QTransform QMatrix4x4D::toTransform() const
{
    return QTransform(double(m[0][0]), double(m[0][1]), double(m[0][3]),
                      double(m[1][0]), double(m[1][1]), double(m[1][3]),
                      double(m[3][0]), double(m[3][1]), double(m[3][3]));
}

/*!
    \fn QPoint QMatrix4x4D::map(const QPoint& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

/*!
    \fn QPointF QMatrix4x4D::map(const QPointF& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#ifndef QT_NO_VECTOR3D

/*!
    \fn QVector3D QMatrix4x4D::map(const QVector3D& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    \fn QVector4D QMatrix4x4D::map(const QVector4D& point) const;

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#endif

/*!
    Maps \a rect by multiplying this matrix by the corners
    of \a rect and then forming a new rectangle from the results.
    The returned rectangle will be an ordinary 2D rectangle
    with sides parallel to the horizontal and vertical axes.

    \sa map()
*/
QRect QMatrix4x4D::mapRect(const QRect& rect) const
{
    if (flagBits == (Translation | Scale) || flagBits == Scale) {
        double x = rect.x() * m[0][0] + m[3][0];
        double y = rect.y() * m[1][1] + m[3][1];
        double w = rect.width() * m[0][0];
        double h = rect.height() * m[1][1];
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        return QRect(qRound(x), qRound(y), qRound(w), qRound(h));
    } else if (flagBits == Translation) {
        return QRect(qRound(rect.x() + m[3][0]),
                     qRound(rect.y() + m[3][1]),
                     rect.width(), rect.height());
    }

    QPoint tl = map(rect.topLeft());
    QPoint tr = map(QPoint(rect.x() + rect.width(), rect.y()));
    QPoint bl = map(QPoint(rect.x(), rect.y() + rect.height()));
    QPoint br = map(QPoint(rect.x() + rect.width(),
                           rect.y() + rect.height()));

    int xmin = qMin(qMin(tl.x(), tr.x()), qMin(bl.x(), br.x()));
    int xmax = qMax(qMax(tl.x(), tr.x()), qMax(bl.x(), br.x()));
    int ymin = qMin(qMin(tl.y(), tr.y()), qMin(bl.y(), br.y()));
    int ymax = qMax(qMax(tl.y(), tr.y()), qMax(bl.y(), br.y()));

    return QRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

/*!
    Maps \a rect by multiplying this matrix by the corners
    of \a rect and then forming a new rectangle from the results.
    The returned rectangle will be an ordinary 2D rectangle
    with sides parallel to the horizontal and vertical axes.

    \sa map()
*/
QRectF QMatrix4x4D::mapRect(const QRectF& rect) const
{
    if (flagBits == (Translation | Scale) || flagBits == Scale) {
        double x = rect.x() * m[0][0] + m[3][0];
        double y = rect.y() * m[1][1] + m[3][1];
        double w = rect.width() * m[0][0];
        double h = rect.height() * m[1][1];
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        return QRectF(x, y, w, h);
    } else if (flagBits == Translation) {
        return rect.translated(m[3][0], m[3][1]);
    }

    QPointF tl = map(rect.topLeft()); QPointF tr = map(rect.topRight());
    QPointF bl = map(rect.bottomLeft()); QPointF br = map(rect.bottomRight());

    double xmin = qMin(qMin(tl.x(), tr.x()), qMin(bl.x(), br.x()));
    double xmax = qMax(qMax(tl.x(), tr.x()), qMax(bl.x(), br.x()));
    double ymin = qMin(qMin(tl.y(), tr.y()), qMin(bl.y(), br.y()));
    double ymax = qMax(qMax(tl.y(), tr.y()), qMax(bl.y(), br.y()));

    return QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

/*!
    \fn double *QMatrix4x4D::data()

    Returns a pointer to the raw data of this matrix.  This is intended
    for use with raw GL functions.

    \sa constData(), inferSpecialType()
*/

/*!
    \fn const double *QMatrix4x4D::data() const

    Returns a constant pointer to the raw data of this matrix.
    This is intended for use with raw GL functions.

    \sa constData()
*/

/*!
    \fn const double *QMatrix4x4D::constData() const

    Returns a constant pointer to the raw data of this matrix.
    This is intended for use with raw GL functions.

    \sa data()
*/

// Helper routine for inverting orthonormal matrices that consist
// of just rotations and translations.
QMatrix4x4D QMatrix4x4D::orthonormalInverse() const
{
    QMatrix4x4D result(1);  // The '1' says not to load identity

    result.m[0][0] = m[0][0];
    result.m[1][0] = m[0][1];
    result.m[2][0] = m[0][2];

    result.m[0][1] = m[1][0];
    result.m[1][1] = m[1][1];
    result.m[2][1] = m[1][2];

    result.m[0][2] = m[2][0];
    result.m[1][2] = m[2][1];
    result.m[2][2] = m[2][2];

    result.m[0][3] = 0.0;
    result.m[1][3] = 0.0;
    result.m[2][3] = 0.0;

    result.m[3][0] = -(result.m[0][0] * m[3][0] + result.m[1][0] * m[3][1] + result.m[2][0] * m[3][2]);
    result.m[3][1] = -(result.m[0][1] * m[3][0] + result.m[1][1] * m[3][1] + result.m[2][1] * m[3][2]);
    result.m[3][2] = -(result.m[0][2] * m[3][0] + result.m[1][2] * m[3][1] + result.m[2][2] * m[3][2]);
    result.m[3][3] = 1.0;

    return result;
}

/*!
    Infers the special type of this matrix from its current elements.

    Some operations such as translate(), scale(), and rotate() can be
    performed more efficiently if the matrix being modified is already
    known to be the identity, a previous translate(), a previous
    scale(), etc.

    Normally the QMatrix4x4D class keeps track of this special type internally
    as operations are performed.  However, if the matrix is modified
    directly with operator()() or data(), then QMatrix4x4D will lose track of
    the special type and will revert to the safest but least efficient
    operations thereafter.

    By calling inferSpecialType() after directly modifying the matrix,
    the programmer can force QMatrix4x4D to recover the special type if
    the elements appear to conform to one of the known optimized types.

    \sa operator()(), data(), translate()
*/
void QMatrix4x4D::inferSpecialType()
{
    // If the last element is not 1, then it can never be special.
    if (m[3][3] != 1.0) {
        flagBits = General;
        return;
    }

    // If the upper three elements m12, m13, and m21 are not all zero,
    // or the lower elements below the diagonal are not all zero, then
    // the matrix can never be special.
    if (m[1][0] != 0.0 || m[2][0] != 0.0 || m[2][1] != 0.0) {
        flagBits = General;
        return;
    }
    if (m[0][1] != 0.0 || m[0][2] != 0.0 || m[0][3] != 0.0 ||
        m[1][2] != 0.0 || m[1][3] != 0.0 || m[2][3] != 0.0) {
        flagBits = General;
        return;
    }

    // Determine what we have in the remaining regions of the matrix.
    bool identityAlongDiagonal
        = (m[0][0] == 1.0 && m[1][1] == 1.0 && m[2][2] == 1.0);
    bool translationPresent
        = (m[3][0] != 0.0 || m[3][1] != 0.0 || m[3][2] != 0.0);

    // Now determine the special matrix type.
    if (translationPresent && identityAlongDiagonal)
        flagBits = Translation;
    else if (translationPresent)
        flagBits = (Translation | Scale);
    else if (identityAlongDiagonal)
        flagBits = Identity;
    else
        flagBits = Scale;
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const QMatrix4x4D &m)
{
    // Create a string that represents the matrix type.
    QByteArray bits;
    if ((m.flagBits & QMatrix4x4D::Identity) != 0)
        bits += "Identity,";
    if ((m.flagBits & QMatrix4x4D::General) != 0)
        bits += "General,";
    if ((m.flagBits & QMatrix4x4D::Translation) != 0)
        bits += "Translation,";
    if ((m.flagBits & QMatrix4x4D::Scale) != 0)
        bits += "Scale,";
    if ((m.flagBits & QMatrix4x4D::Rotation) != 0)
        bits += "Rotation,";
    if (bits.size() > 0)
        bits = bits.left(bits.size() - 1);

    // Output in row-major order because it is more human-readable.
    dbg.nospace() << "QMatrix4x4D(type:" << bits.constData() << endl
        << qSetFieldWidth(10)
        << m(0, 0) << m(0, 1) << m(0, 2) << m(0, 3) << endl
        << m(1, 0) << m(1, 1) << m(1, 2) << m(1, 3) << endl
        << m(2, 0) << m(2, 1) << m(2, 2) << m(2, 3) << endl
        << m(3, 0) << m(3, 1) << m(3, 2) << m(3, 3) << endl
        << qSetFieldWidth(0) << ')';
    return dbg.space();
}

#endif
