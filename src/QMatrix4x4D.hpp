/****************************************************************************
**
** Copyright) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QMATRIX4X4D_HPP
#define QMATRIX4X4D_HPP

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>
#include <QGenericMatrix>
#include <QRect>
#include <QRectF>
#include "QVector3DD.hpp"

class QMatrix;
class QTransform;

class QMatrix4x4D
{
public:
    inline QMatrix4x4D() { setToIdentity(); }
    explicit QMatrix4x4D(const double *values);
    inline QMatrix4x4D(double m11, double m12, double m13, double m14,
                      double m21, double m22, double m23, double m24,
                      double m31, double m32, double m33, double m34,
                      double m41, double m42, double m43, double m44);
#if !defined(QT_NO_MEMBER_TEMPLATES) || defined(Q_QDOC)
    template <int N, int M, typename Real>
    explicit QMatrix4x4D(const QGenericMatrix<N, M, Real>& matrix);
#endif
    QMatrix4x4D(const double *values, int cols, int rows);
    QMatrix4x4D(const QTransform& transform);
    QMatrix4x4D(const QMatrix& matrix);

    inline double operator()(int row, int column) const;
    inline double& operator()(int row, int column);

    inline QVector4D column(int index) const;
    inline void setColumn(int index, const QVector4D& value);

    inline QVector4D row(int index) const;
    inline void setRow(int index, const QVector4D& value);

    inline bool isIdentity() const;
    inline void setToIdentity();

    inline void fill(double value);

    double determinant() const;
    QMatrix4x4D inverted(bool *invertible = 0) const;
    QMatrix4x4D transposed() const;
    QMatrix4x4D normalMatrix() const;

    inline QMatrix4x4D& operator+=(const QMatrix4x4D& other);
    inline QMatrix4x4D& operator-=(const QMatrix4x4D& other);
    inline QMatrix4x4D& operator*=(const QMatrix4x4D& other);
    inline QMatrix4x4D& operator*=(double factor);
    QMatrix4x4D& operator/=(double divisor);
    inline bool operator==(const QMatrix4x4D& other) const;
    inline bool operator!=(const QMatrix4x4D& other) const;

    friend QMatrix4x4D operator+(const QMatrix4x4D& m1, const QMatrix4x4D& m2);
    friend QMatrix4x4D operator-(const QMatrix4x4D& m1, const QMatrix4x4D& m2);
    friend QMatrix4x4D operator*(const QMatrix4x4D& m1, const QMatrix4x4D& m2);
#ifndef QT_NO_VECTOR3D
    friend QVector3D operator*(const QMatrix4x4D& matrix, const QVector3D& vector);
    friend QVector3D operator*(const QVector3D& vector, const QMatrix4x4D& matrix);
#endif
#if 1
    friend QVector3DD operator*(const QMatrix4x4D& matrix, const QVector3DD& vector);
    friend QVector3DD operator*(const QVector3DD& vector, const QMatrix4x4D& matrix);
#endif
#ifndef QT_NO_VECTOR4D
    friend QVector4D operator*(const QVector4D& vector, const QMatrix4x4D& matrix);
    friend QVector4D operator*(const QMatrix4x4D& matrix, const QVector4D& vector);
#endif
    friend QPoint operator*(const QPoint& point, const QMatrix4x4D& matrix);
    friend QPointF operator*(const QPointF& point, const QMatrix4x4D& matrix);
    friend QVector2D operator*(const QVector2D& point, const QMatrix4x4D& matrix);
    friend QMatrix4x4D operator-(const QMatrix4x4D& matrix);
    friend QPoint operator*(const QMatrix4x4D& matrix, const QPoint& point);
    friend QPointF operator*(const QMatrix4x4D& matrix, const QPointF& point);
    friend QVector2D operator*(const QMatrix4x4D& matrix, const QVector2D& point);
    friend QMatrix4x4D operator*(double factor, const QMatrix4x4D& matrix);
    friend QMatrix4x4D operator*(const QMatrix4x4D& matrix, double factor);
    friend QMatrix4x4D operator/(const QMatrix4x4D& matrix, double divisor);

    friend inline bool qFuzzyCompare(const QMatrix4x4D& m1, const QMatrix4x4D& m2);

#ifndef QT_NO_VECTOR3D
    QMatrix4x4D& scale(const QVector3D& vector);
    QMatrix4x4D& translate(const QVector3D& vector);
    QMatrix4x4D& rotate(double angle, const QVector3D& vector);
#endif
#if 1
    QMatrix4x4D& scale(const QVector3DD& vector);
    QMatrix4x4D& translate(const QVector3DD& vector);
    QMatrix4x4D& rotate(double angle, const QVector3DD& vector);
#endif
    QMatrix4x4D& scale(double x, double y);
    QMatrix4x4D& scale(double x, double y, double z);
    QMatrix4x4D& scale(double factor);
    QMatrix4x4D& translate(double x, double y);
    QMatrix4x4D& translate(double x, double y, double z);
    QMatrix4x4D& rotate(double angle, double x, double y, double z = 0.0f);
#ifndef QT_NO_QUATERNION
    QMatrix4x4D& rotate(const QQuaternion& quaternion);
#endif

    QMatrix4x4D& ortho(const QRect& rect);
    QMatrix4x4D& ortho(const QRectF& rect);
    QMatrix4x4D& ortho(double left, double right, double bottom, double top, double nearPlane, double farPlane);
    QMatrix4x4D& frustum(double left, double right, double bottom, double top, double nearPlane, double farPlane);
    QMatrix4x4D& perspective(double angle, double aspect, double nearPlane, double farPlane);
#ifndef QT_NO_VECTOR3D
    QMatrix4x4D& lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up);
#endif
#if 1
    QMatrix4x4D& lookAt(const QVector3DD& eye, const QVector3DD& center, const QVector3DD& up);
#endif
    QMatrix4x4D& flipCoordinates();

    void toValueArray(double *values) const;

    QMatrix toAffine() const;
    QTransform toTransform() const;

    QPoint map(const QPoint& point) const;
    QPointF map(const QPointF& point) const;
#ifndef QT_NO_VECTOR3D
    QVector3D map(const QVector3D& point) const;
#endif
#if 1
    QVector3DD map(const QVector3DD& point) const;
#endif
#ifndef QT_NO_VECTOR4D
    QVector4D map(const QVector4D& point) const;
#endif
    QRect mapRect(const QRect& rect) const;
    QRectF mapRect(const QRectF& rect) const;

#if !defined(QT_NO_MEMBER_TEMPLATES) || defined(Q_QDOC)
    template <int N, int M, typename Real>
    QGenericMatrix<N, M, Real> toGenericMatrix() const;
#endif

    inline double *data();
    inline const double *data() const { return m[0]; }
    inline const double *constData() const { return m[0]; }

    void inferSpecialType();

#ifndef QT_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug dbg, const QMatrix4x4D &m);
#endif

private:
    double m[4][4];          // Column-major order to match OpenGL.
    int flagBits;           // Flag bits from the enum below.

    enum {
        Identity        = 0x0001,   // Identity matrix
        General         = 0x0002,   // General matrix, unknown contents
        Translation     = 0x0004,   // Contains a simple translation
        Scale           = 0x0008,   // Contains a simple scale
        Rotation        = 0x0010    // Contains a simple rotation
    };

    // Construct without initializing identity matrix.
    QMatrix4x4D(int) { flagBits = General; }

    QMatrix4x4D orthonormalInverse() const;
};

inline QMatrix4x4D::QMatrix4x4D
        (double m11, double m12, double m13, double m14,
         double m21, double m22, double m23, double m24,
         double m31, double m32, double m33, double m34,
         double m41, double m42, double m43, double m44)
{
    m[0][0] = m11; m[0][1] = m21; m[0][2] = m31; m[0][3] = m41;
    m[1][0] = m12; m[1][1] = m22; m[1][2] = m32; m[1][3] = m42;
    m[2][0] = m13; m[2][1] = m23; m[2][2] = m33; m[2][3] = m43;
    m[3][0] = m14; m[3][1] = m24; m[3][2] = m34; m[3][3] = m44;
    flagBits = General;
}

#if !defined(QT_NO_MEMBER_TEMPLATES)

template <int N, int M, typename Real>
Q_INLINE_TEMPLATE QMatrix4x4D::QMatrix4x4D
    (const QGenericMatrix<N, M, Real>& matrix)
{
    const double *values = matrix.constData();
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            if (col < N && row < M)
                m[col][row] = values[col * M + row];
            else if (col == row)
                m[col][row] = 1.0f;
            else
                m[col][row] = 0.0f;
        }
    }
    flagBits = General;
}

template <int N, int M, typename Real>
QGenericMatrix<N, M, Real> QMatrix4x4D::toGenericMatrix() const
{
    QGenericMatrix<N, M, Real> result;
    Real *values = result.data();
    for (int col = 0; col < N; ++col) {
        for (int row = 0; row < M; ++row) {
            if (col < 4 && row < 4)
                values[col * M + row] = m[col][row];
            else if (col == row)
                values[col * M + row] = 1.0f;
            else
                values[col * M + row] = 0.0f;
        }
    }
    return result;
}

#endif

inline double QMatrix4x4D::operator()(int row, int column) const
{
    Q_ASSERT(row >= 0 && row < 4 && column >= 0 && column < 4);
    return double(m[column][row]);
}

inline double& QMatrix4x4D::operator()(int row, int column)
{
    Q_ASSERT(row >= 0 && row < 4 && column >= 0 && column < 4);
    flagBits = General;
    return m[column][row];
}

inline QVector4D QMatrix4x4D::column(int index) const
{
    Q_ASSERT(index >= 0 && index < 4);
    return QVector4D(m[index][0], m[index][1], m[index][2], m[index][3]);
}

inline void QMatrix4x4D::setColumn(int index, const QVector4D& value)
{
    Q_ASSERT(index >= 0 && index < 4);
    m[index][0] = value.x();
    m[index][1] = value.y();
    m[index][2] = value.z();
    m[index][3] = value.w();
    flagBits = General;
}

inline QVector4D QMatrix4x4D::row(int index) const
{
    Q_ASSERT(index >= 0 && index < 4);
    return QVector4D(m[0][index], m[1][index], m[2][index], m[3][index]);
}

inline void QMatrix4x4D::setRow(int index, const QVector4D& value)
{
    Q_ASSERT(index >= 0 && index < 4);
    m[0][index] = value.x();
    m[1][index] = value.y();
    m[2][index] = value.z();
    m[3][index] = value.w();
    flagBits = General;
}

QMatrix4x4D operator/(const QMatrix4x4D& matrix, double divisor);

inline bool QMatrix4x4D::isIdentity() const
{
    if (flagBits == Identity)
        return true;
    if (m[0][0] != 1.0f || m[0][1] != 0.0f || m[0][2] != 0.0f)
        return false;
    if (m[0][3] != 0.0f || m[1][0] != 0.0f || m[1][1] != 1.0f)
        return false;
    if (m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][0] != 0.0f)
        return false;
    if (m[2][1] != 0.0f || m[2][2] != 1.0f || m[2][3] != 0.0f)
        return false;
    if (m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f)
        return false;
    return (m[3][3] == 1.0f);
}

inline void QMatrix4x4D::setToIdentity()
{
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    flagBits = Identity;
}

inline void QMatrix4x4D::fill(double value)
{
    m[0][0] = value;
    m[0][1] = value;
    m[0][2] = value;
    m[0][3] = value;
    m[1][0] = value;
    m[1][1] = value;
    m[1][2] = value;
    m[1][3] = value;
    m[2][0] = value;
    m[2][1] = value;
    m[2][2] = value;
    m[2][3] = value;
    m[3][0] = value;
    m[3][1] = value;
    m[3][2] = value;
    m[3][3] = value;
    flagBits = General;
}

inline QMatrix4x4D& QMatrix4x4D::operator+=(const QMatrix4x4D& other)
{
    m[0][0] += other.m[0][0];
    m[0][1] += other.m[0][1];
    m[0][2] += other.m[0][2];
    m[0][3] += other.m[0][3];
    m[1][0] += other.m[1][0];
    m[1][1] += other.m[1][1];
    m[1][2] += other.m[1][2];
    m[1][3] += other.m[1][3];
    m[2][0] += other.m[2][0];
    m[2][1] += other.m[2][1];
    m[2][2] += other.m[2][2];
    m[2][3] += other.m[2][3];
    m[3][0] += other.m[3][0];
    m[3][1] += other.m[3][1];
    m[3][2] += other.m[3][2];
    m[3][3] += other.m[3][3];
    flagBits = General;
    return *this;
}

inline QMatrix4x4D& QMatrix4x4D::operator-=(const QMatrix4x4D& other)
{
    m[0][0] -= other.m[0][0];
    m[0][1] -= other.m[0][1];
    m[0][2] -= other.m[0][2];
    m[0][3] -= other.m[0][3];
    m[1][0] -= other.m[1][0];
    m[1][1] -= other.m[1][1];
    m[1][2] -= other.m[1][2];
    m[1][3] -= other.m[1][3];
    m[2][0] -= other.m[2][0];
    m[2][1] -= other.m[2][1];
    m[2][2] -= other.m[2][2];
    m[2][3] -= other.m[2][3];
    m[3][0] -= other.m[3][0];
    m[3][1] -= other.m[3][1];
    m[3][2] -= other.m[3][2];
    m[3][3] -= other.m[3][3];
    flagBits = General;
    return *this;
}

inline QMatrix4x4D& QMatrix4x4D::operator*=(const QMatrix4x4D& other)
{
    if (flagBits == Identity) {
        *this = other;
        return *this;
    } else if (other.flagBits == Identity) {
        return *this;
    } else {
        *this = *this * other;
        return *this;
    }
}

inline QMatrix4x4D& QMatrix4x4D::operator*=(double factor)
{
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
    m[3][0] *= factor;
    m[3][1] *= factor;
    m[3][2] *= factor;
    m[3][3] *= factor;
    flagBits = General;
    return *this;
}

inline bool QMatrix4x4D::operator==(const QMatrix4x4D& other) const
{
    return m[0][0] == other.m[0][0] &&
           m[0][1] == other.m[0][1] &&
           m[0][2] == other.m[0][2] &&
           m[0][3] == other.m[0][3] &&
           m[1][0] == other.m[1][0] &&
           m[1][1] == other.m[1][1] &&
           m[1][2] == other.m[1][2] &&
           m[1][3] == other.m[1][3] &&
           m[2][0] == other.m[2][0] &&
           m[2][1] == other.m[2][1] &&
           m[2][2] == other.m[2][2] &&
           m[2][3] == other.m[2][3] &&
           m[3][0] == other.m[3][0] &&
           m[3][1] == other.m[3][1] &&
           m[3][2] == other.m[3][2] &&
           m[3][3] == other.m[3][3];
}

inline bool QMatrix4x4D::operator!=(const QMatrix4x4D& other) const
{
    return m[0][0] != other.m[0][0] ||
           m[0][1] != other.m[0][1] ||
           m[0][2] != other.m[0][2] ||
           m[0][3] != other.m[0][3] ||
           m[1][0] != other.m[1][0] ||
           m[1][1] != other.m[1][1] ||
           m[1][2] != other.m[1][2] ||
           m[1][3] != other.m[1][3] ||
           m[2][0] != other.m[2][0] ||
           m[2][1] != other.m[2][1] ||
           m[2][2] != other.m[2][2] ||
           m[2][3] != other.m[2][3] ||
           m[3][0] != other.m[3][0] ||
           m[3][1] != other.m[3][1] ||
           m[3][2] != other.m[3][2] ||
           m[3][3] != other.m[3][3];
}

inline QMatrix4x4D operator+(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
{
    QMatrix4x4D m(1);
    m.m[0][0] = m1.m[0][0] + m2.m[0][0];
    m.m[0][1] = m1.m[0][1] + m2.m[0][1];
    m.m[0][2] = m1.m[0][2] + m2.m[0][2];
    m.m[0][3] = m1.m[0][3] + m2.m[0][3];
    m.m[1][0] = m1.m[1][0] + m2.m[1][0];
    m.m[1][1] = m1.m[1][1] + m2.m[1][1];
    m.m[1][2] = m1.m[1][2] + m2.m[1][2];
    m.m[1][3] = m1.m[1][3] + m2.m[1][3];
    m.m[2][0] = m1.m[2][0] + m2.m[2][0];
    m.m[2][1] = m1.m[2][1] + m2.m[2][1];
    m.m[2][2] = m1.m[2][2] + m2.m[2][2];
    m.m[2][3] = m1.m[2][3] + m2.m[2][3];
    m.m[3][0] = m1.m[3][0] + m2.m[3][0];
    m.m[3][1] = m1.m[3][1] + m2.m[3][1];
    m.m[3][2] = m1.m[3][2] + m2.m[3][2];
    m.m[3][3] = m1.m[3][3] + m2.m[3][3];
    return m;
}

inline QMatrix4x4D operator-(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
{
    QMatrix4x4D m(1);
    m.m[0][0] = m1.m[0][0] - m2.m[0][0];
    m.m[0][1] = m1.m[0][1] - m2.m[0][1];
    m.m[0][2] = m1.m[0][2] - m2.m[0][2];
    m.m[0][3] = m1.m[0][3] - m2.m[0][3];
    m.m[1][0] = m1.m[1][0] - m2.m[1][0];
    m.m[1][1] = m1.m[1][1] - m2.m[1][1];
    m.m[1][2] = m1.m[1][2] - m2.m[1][2];
    m.m[1][3] = m1.m[1][3] - m2.m[1][3];
    m.m[2][0] = m1.m[2][0] - m2.m[2][0];
    m.m[2][1] = m1.m[2][1] - m2.m[2][1];
    m.m[2][2] = m1.m[2][2] - m2.m[2][2];
    m.m[2][3] = m1.m[2][3] - m2.m[2][3];
    m.m[3][0] = m1.m[3][0] - m2.m[3][0];
    m.m[3][1] = m1.m[3][1] - m2.m[3][1];
    m.m[3][2] = m1.m[3][2] - m2.m[3][2];
    m.m[3][3] = m1.m[3][3] - m2.m[3][3];
    return m;
}

inline QMatrix4x4D operator*(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
{
    if (m1.flagBits == QMatrix4x4D::Identity)
        return m2;
    else if (m2.flagBits == QMatrix4x4D::Identity)
        return m1;

    QMatrix4x4D m(1);
    m.m[0][0] = m1.m[0][0] * m2.m[0][0] +
                m1.m[1][0] * m2.m[0][1] +
                m1.m[2][0] * m2.m[0][2] +
                m1.m[3][0] * m2.m[0][3];
    m.m[0][1] = m1.m[0][1] * m2.m[0][0] +
                m1.m[1][1] * m2.m[0][1] +
                m1.m[2][1] * m2.m[0][2] +
                m1.m[3][1] * m2.m[0][3];
    m.m[0][2] = m1.m[0][2] * m2.m[0][0] +
                m1.m[1][2] * m2.m[0][1] +
                m1.m[2][2] * m2.m[0][2] +
                m1.m[3][2] * m2.m[0][3];
    m.m[0][3] = m1.m[0][3] * m2.m[0][0] +
                m1.m[1][3] * m2.m[0][1] +
                m1.m[2][3] * m2.m[0][2] +
                m1.m[3][3] * m2.m[0][3];
    m.m[1][0] = m1.m[0][0] * m2.m[1][0] +
                m1.m[1][0] * m2.m[1][1] +
                m1.m[2][0] * m2.m[1][2] +
                m1.m[3][0] * m2.m[1][3];
    m.m[1][1] = m1.m[0][1] * m2.m[1][0] +
                m1.m[1][1] * m2.m[1][1] +
                m1.m[2][1] * m2.m[1][2] +
                m1.m[3][1] * m2.m[1][3];
    m.m[1][2] = m1.m[0][2] * m2.m[1][0] +
                m1.m[1][2] * m2.m[1][1] +
                m1.m[2][2] * m2.m[1][2] +
                m1.m[3][2] * m2.m[1][3];
    m.m[1][3] = m1.m[0][3] * m2.m[1][0] +
                m1.m[1][3] * m2.m[1][1] +
                m1.m[2][3] * m2.m[1][2] +
                m1.m[3][3] * m2.m[1][3];
    m.m[2][0] = m1.m[0][0] * m2.m[2][0] +
                m1.m[1][0] * m2.m[2][1] +
                m1.m[2][0] * m2.m[2][2] +
                m1.m[3][0] * m2.m[2][3];
    m.m[2][1] = m1.m[0][1] * m2.m[2][0] +
                m1.m[1][1] * m2.m[2][1] +
                m1.m[2][1] * m2.m[2][2] +
                m1.m[3][1] * m2.m[2][3];
    m.m[2][2] = m1.m[0][2] * m2.m[2][0] +
                m1.m[1][2] * m2.m[2][1] +
                m1.m[2][2] * m2.m[2][2] +
                m1.m[3][2] * m2.m[2][3];
    m.m[2][3] = m1.m[0][3] * m2.m[2][0] +
                m1.m[1][3] * m2.m[2][1] +
                m1.m[2][3] * m2.m[2][2] +
                m1.m[3][3] * m2.m[2][3];
    m.m[3][0] = m1.m[0][0] * m2.m[3][0] +
                m1.m[1][0] * m2.m[3][1] +
                m1.m[2][0] * m2.m[3][2] +
                m1.m[3][0] * m2.m[3][3];
    m.m[3][1] = m1.m[0][1] * m2.m[3][0] +
                m1.m[1][1] * m2.m[3][1] +
                m1.m[2][1] * m2.m[3][2] +
                m1.m[3][1] * m2.m[3][3];
    m.m[3][2] = m1.m[0][2] * m2.m[3][0] +
                m1.m[1][2] * m2.m[3][1] +
                m1.m[2][2] * m2.m[3][2] +
                m1.m[3][2] * m2.m[3][3];
    m.m[3][3] = m1.m[0][3] * m2.m[3][0] +
                m1.m[1][3] * m2.m[3][1] +
                m1.m[2][3] * m2.m[3][2] +
                m1.m[3][3] * m2.m[3][3];
    return m;
}

#ifndef QT_NO_VECTOR3D

inline QVector3D operator*(const QVector3D& vector, const QMatrix4x4D& matrix)
{
    double x, y, z, w;
    x = vector.x() * matrix.m[0][0] +
        vector.y() * matrix.m[0][1] +
        vector.z() * matrix.m[0][2] +
        matrix.m[0][3];
    y = vector.x() * matrix.m[1][0] +
        vector.y() * matrix.m[1][1] +
        vector.z() * matrix.m[1][2] +
        matrix.m[1][3];
    z = vector.x() * matrix.m[2][0] +
        vector.y() * matrix.m[2][1] +
        vector.z() * matrix.m[2][2] +
        matrix.m[2][3];
    w = vector.x() * matrix.m[3][0] +
        vector.y() * matrix.m[3][1] +
        vector.z() * matrix.m[3][2] +
        matrix.m[3][3];
    if (w == 1.0f)
        return QVector3D(x, y, z);
    else
        return QVector3D(x / w, y / w, z / w);
}

inline QVector3D operator*(const QMatrix4x4D& matrix, const QVector3D& vector)
{
    double x, y, z, w;
    if (matrix.flagBits == QMatrix4x4D::Identity) {
        return vector;
    } else if (matrix.flagBits == QMatrix4x4D::Translation) {
        return QVector3D(vector.x() + matrix.m[3][0],
                         vector.y() + matrix.m[3][1],
                         vector.z() + matrix.m[3][2]);
    } else if (matrix.flagBits ==
                    (QMatrix4x4D::Translation | QMatrix4x4D::Scale)) {
        return QVector3D(vector.x() * matrix.m[0][0] + matrix.m[3][0],
                         vector.y() * matrix.m[1][1] + matrix.m[3][1],
                         vector.z() * matrix.m[2][2] + matrix.m[3][2]);
    } else if (matrix.flagBits == QMatrix4x4D::Scale) {
        return QVector3D(vector.x() * matrix.m[0][0],
                         vector.y() * matrix.m[1][1],
                         vector.z() * matrix.m[2][2]);
    } else {
        x = vector.x() * matrix.m[0][0] +
            vector.y() * matrix.m[1][0] +
            vector.z() * matrix.m[2][0] +
            matrix.m[3][0];
        y = vector.x() * matrix.m[0][1] +
            vector.y() * matrix.m[1][1] +
            vector.z() * matrix.m[2][1] +
            matrix.m[3][1];
        z = vector.x() * matrix.m[0][2] +
            vector.y() * matrix.m[1][2] +
            vector.z() * matrix.m[2][2] +
            matrix.m[3][2];
        w = vector.x() * matrix.m[0][3] +
            vector.y() * matrix.m[1][3] +
            vector.z() * matrix.m[2][3] +
            matrix.m[3][3];
        if (w == 1.0f)
            return QVector3D(x, y, z);
        else
            return QVector3D(x / w, y / w, z / w);
    }
}

#endif

#if 1

inline QVector3DD operator*(const QVector3DD& vector, const QMatrix4x4D& matrix)
{
    double x, y, z, w;
    x = vector.xp * matrix.m[0][0] +
        vector.yp * matrix.m[0][1] +
        vector.zp * matrix.m[0][2] +
        matrix.m[0][3];
    y = vector.xp * matrix.m[1][0] +
        vector.yp * matrix.m[1][1] +
        vector.zp * matrix.m[1][2] +
        matrix.m[1][3];
    z = vector.xp * matrix.m[2][0] +
        vector.yp * matrix.m[2][1] +
        vector.zp * matrix.m[2][2] +
        matrix.m[2][3];
    w = vector.xp * matrix.m[3][0] +
        vector.yp * matrix.m[3][1] +
        vector.zp * matrix.m[3][2] +
        matrix.m[3][3];
    if (w == 1.0f)
        return QVector3DD(x, y, z);
    else
        return QVector3DD(x / w, y / w, z / w);
}

inline QVector3DD operator*(const QMatrix4x4D& matrix, const QVector3DD& vector)
{
    double x, y, z, w;
    if (matrix.flagBits == QMatrix4x4D::Identity) {
        return vector;
    } else if (matrix.flagBits == QMatrix4x4D::Translation) {
        return QVector3DD(vector.xp + matrix.m[3][0],
                         vector.yp + matrix.m[3][1],
                         vector.zp + matrix.m[3][2]);
    } else if (matrix.flagBits ==
                    (QMatrix4x4D::Translation | QMatrix4x4D::Scale)) {
        return QVector3DD(vector.xp * matrix.m[0][0] + matrix.m[3][0],
                         vector.yp * matrix.m[1][1] + matrix.m[3][1],
                         vector.zp * matrix.m[2][2] + matrix.m[3][2]);
    } else if (matrix.flagBits == QMatrix4x4D::Scale) {
        return QVector3DD(vector.xp * matrix.m[0][0],
                         vector.yp * matrix.m[1][1],
                         vector.zp * matrix.m[2][2]);
    } else {
        x = vector.xp * matrix.m[0][0] +
            vector.yp * matrix.m[1][0] +
            vector.zp * matrix.m[2][0] +
            matrix.m[3][0];
        y = vector.xp * matrix.m[0][1] +
            vector.yp * matrix.m[1][1] +
            vector.zp * matrix.m[2][1] +
            matrix.m[3][1];
        z = vector.xp * matrix.m[0][2] +
            vector.yp * matrix.m[1][2] +
            vector.zp * matrix.m[2][2] +
            matrix.m[3][2];
        w = vector.xp * matrix.m[0][3] +
            vector.yp * matrix.m[1][3] +
            vector.zp * matrix.m[2][3] +
            matrix.m[3][3];
        if (w == 1.0f)
            return QVector3DD(x, y, z);
        else
            return QVector3DD(x / w, y / w, z / w);
    }
}
#endif

#ifndef QT_NO_VECTOR4D

inline QVector4D operator*(const QVector4D& vector, const QMatrix4x4D& matrix)
{
    double x, y, z, w;
    x = vector.x() * matrix.m[0][0] +
        vector.y() * matrix.m[0][1] +
        vector.z() * matrix.m[0][2] +
        vector.w() * matrix.m[0][3];
    y = vector.x() * matrix.m[1][0] +
        vector.y() * matrix.m[1][1] +
        vector.z() * matrix.m[1][2] +
        vector.w() * matrix.m[1][3];
    z = vector.x() * matrix.m[2][0] +
        vector.y() * matrix.m[2][1] +
        vector.z() * matrix.m[2][2] +
        vector.w() * matrix.m[2][3];
    w = vector.x() * matrix.m[3][0] +
        vector.y() * matrix.m[3][1] +
        vector.z() * matrix.m[3][2] +
        vector.w() * matrix.m[3][3];
    return QVector4D(x, y, z, w);
}

inline QVector4D operator*(const QMatrix4x4D& matrix, const QVector4D& vector)
{
    double x, y, z, w;
    x = vector.x() * matrix.m[0][0] +
        vector.y() * matrix.m[1][0] +
        vector.z() * matrix.m[2][0] +
        vector.w() * matrix.m[3][0];
    y = vector.x() * matrix.m[0][1] +
        vector.y() * matrix.m[1][1] +
        vector.z() * matrix.m[2][1] +
        vector.w() * matrix.m[3][1];
    z = vector.x() * matrix.m[0][2] +
        vector.y() * matrix.m[1][2] +
        vector.z() * matrix.m[2][2] +
        vector.w() * matrix.m[3][2];
    w = vector.x() * matrix.m[0][3] +
        vector.y() * matrix.m[1][3] +
        vector.z() * matrix.m[2][3] +
        vector.w() * matrix.m[3][3];
    return QVector4D(x, y, z, w);
}

#endif

inline QPoint operator*(const QPoint& point, const QMatrix4x4D& matrix)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    x = xin * matrix.m[0][0] +
        yin * matrix.m[0][1] +
        matrix.m[0][3];
    y = xin * matrix.m[1][0] +
        yin * matrix.m[1][1] +
        matrix.m[1][3];
    w = xin * matrix.m[3][0] +
        yin * matrix.m[3][1] +
        matrix.m[3][3];
    if (w == 1.0f)
        return QPoint(qRound(x), qRound(y));
    else
        return QPoint(qRound(x / w), qRound(y / w));
}

inline QPointF operator*(const QPointF& point, const QMatrix4x4D& matrix)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    x = xin * matrix.m[0][0] +
        yin * matrix.m[0][1] +
        matrix.m[0][3];
    y = xin * matrix.m[1][0] +
        yin * matrix.m[1][1] +
        matrix.m[1][3];
    w = xin * matrix.m[3][0] +
        yin * matrix.m[3][1] +
        matrix.m[3][3];
    if (w == 1.0f) {
        return QPointF(double(x), double(y));
    } else {
        return QPointF(double(x / w), double(y / w));
    }
}

inline QVector2D operator*(const QVector2D& point, const QMatrix4x4D& matrix)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    x = xin * matrix.m[0][0] +
        yin * matrix.m[0][1] +
        matrix.m[0][3];
    y = xin * matrix.m[1][0] +
        yin * matrix.m[1][1] +
        matrix.m[1][3];
    w = xin * matrix.m[3][0] +
        yin * matrix.m[3][1] +
        matrix.m[3][3];
    if (w == 1.0f) {
        return QVector2D(x, y);
    } else {
        return QVector2D(x/w, y/w);
    }
}

inline QPoint operator*(const QMatrix4x4D& matrix, const QPoint& point)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    if (matrix.flagBits == QMatrix4x4D::Identity) {
        return point;
    } else if (matrix.flagBits == QMatrix4x4D::Translation) {
        return QPoint(qRound(xin + matrix.m[3][0]),
                      qRound(yin + matrix.m[3][1]));
    } else if (matrix.flagBits ==
                    (QMatrix4x4D::Translation | QMatrix4x4D::Scale)) {
        return QPoint(qRound(xin * matrix.m[0][0] + matrix.m[3][0]),
                      qRound(yin * matrix.m[1][1] + matrix.m[3][1]));
    } else if (matrix.flagBits == QMatrix4x4D::Scale) {
        return QPoint(qRound(xin * matrix.m[0][0]),
                      qRound(yin * matrix.m[1][1]));
    } else {
        x = xin * matrix.m[0][0] +
            yin * matrix.m[1][0] +
            matrix.m[3][0];
        y = xin * matrix.m[0][1] +
            yin * matrix.m[1][1] +
            matrix.m[3][1];
        w = xin * matrix.m[0][3] +
            yin * matrix.m[1][3] +
            matrix.m[3][3];
        if (w == 1.0f)
            return QPoint(qRound(x), qRound(y));
        else
            return QPoint(qRound(x / w), qRound(y / w));
    }
}

inline QPointF operator*(const QMatrix4x4D& matrix, const QPointF& point)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    if (matrix.flagBits == QMatrix4x4D::Identity) {
        return point;
    } else if (matrix.flagBits == QMatrix4x4D::Translation) {
        return QPointF(xin + matrix.m[3][0],
                       yin + matrix.m[3][1]);
    } else if (matrix.flagBits ==
                    (QMatrix4x4D::Translation | QMatrix4x4D::Scale)) {
        return QPointF(xin * matrix.m[0][0] + matrix.m[3][0],
                       yin * matrix.m[1][1] + matrix.m[3][1]);
    } else if (matrix.flagBits == QMatrix4x4D::Scale) {
        return QPointF(xin * matrix.m[0][0],
                       yin * matrix.m[1][1]);
    } else {
        x = xin * matrix.m[0][0] +
            yin * matrix.m[1][0] +
            matrix.m[3][0];
        y = xin * matrix.m[0][1] +
            yin * matrix.m[1][1] +
            matrix.m[3][1];
        w = xin * matrix.m[0][3] +
            yin * matrix.m[1][3] +
            matrix.m[3][3];
        if (w == 1.0f) {
            return QPointF(double(x), double(y));
        } else {
            return QPointF(double(x / w), double(y / w));
        }
    }
}

inline QVector2D operator*(const QMatrix4x4D& matrix, const QVector2D& point)
{
    double xin, yin;
    double x, y, w;
    xin = point.x();
    yin = point.y();
    if (matrix.flagBits == QMatrix4x4D::Identity) {
        return point;
    } else if (matrix.flagBits == QMatrix4x4D::Translation) {
        return QVector2D(xin + matrix.m[3][0],
                         yin + matrix.m[3][1]);
    } else if (matrix.flagBits ==
                    (QMatrix4x4D::Translation | QMatrix4x4D::Scale)) {
        return QVector2D(xin * matrix.m[0][0] + matrix.m[3][0],
                         yin * matrix.m[1][1] + matrix.m[3][1]);
    } else if (matrix.flagBits == QMatrix4x4D::Scale) {
        return QVector2D(xin * matrix.m[0][0],
                         yin * matrix.m[1][1]);
    } else {
        x = xin * matrix.m[0][0] +
            yin * matrix.m[1][0] +
            matrix.m[3][0];
        y = xin * matrix.m[0][1] +
            yin * matrix.m[1][1] +
            matrix.m[3][1];
        w = xin * matrix.m[0][3] +
            yin * matrix.m[1][3] +
            matrix.m[3][3];
        if (w == 1.0f) {
            return QVector2D(x, y);
        } else {
            return QVector2D(x/w, y/w);
        }
    }
}

inline QMatrix4x4D operator-(const QMatrix4x4D& matrix)
{
    QMatrix4x4D m(1);
    m.m[0][0] = -matrix.m[0][0];
    m.m[0][1] = -matrix.m[0][1];
    m.m[0][2] = -matrix.m[0][2];
    m.m[0][3] = -matrix.m[0][3];
    m.m[1][0] = -matrix.m[1][0];
    m.m[1][1] = -matrix.m[1][1];
    m.m[1][2] = -matrix.m[1][2];
    m.m[1][3] = -matrix.m[1][3];
    m.m[2][0] = -matrix.m[2][0];
    m.m[2][1] = -matrix.m[2][1];
    m.m[2][2] = -matrix.m[2][2];
    m.m[2][3] = -matrix.m[2][3];
    m.m[3][0] = -matrix.m[3][0];
    m.m[3][1] = -matrix.m[3][1];
    m.m[3][2] = -matrix.m[3][2];
    m.m[3][3] = -matrix.m[3][3];
    return m;
}

inline QMatrix4x4D operator*(double factor, const QMatrix4x4D& matrix)
{
    QMatrix4x4D m(1);
    m.m[0][0] = matrix.m[0][0] * factor;
    m.m[0][1] = matrix.m[0][1] * factor;
    m.m[0][2] = matrix.m[0][2] * factor;
    m.m[0][3] = matrix.m[0][3] * factor;
    m.m[1][0] = matrix.m[1][0] * factor;
    m.m[1][1] = matrix.m[1][1] * factor;
    m.m[1][2] = matrix.m[1][2] * factor;
    m.m[1][3] = matrix.m[1][3] * factor;
    m.m[2][0] = matrix.m[2][0] * factor;
    m.m[2][1] = matrix.m[2][1] * factor;
    m.m[2][2] = matrix.m[2][2] * factor;
    m.m[2][3] = matrix.m[2][3] * factor;
    m.m[3][0] = matrix.m[3][0] * factor;
    m.m[3][1] = matrix.m[3][1] * factor;
    m.m[3][2] = matrix.m[3][2] * factor;
    m.m[3][3] = matrix.m[3][3] * factor;
    return m;
}

inline QMatrix4x4D operator*(const QMatrix4x4D& matrix, double factor)
{
    QMatrix4x4D m(1);
    m.m[0][0] = matrix.m[0][0] * factor;
    m.m[0][1] = matrix.m[0][1] * factor;
    m.m[0][2] = matrix.m[0][2] * factor;
    m.m[0][3] = matrix.m[0][3] * factor;
    m.m[1][0] = matrix.m[1][0] * factor;
    m.m[1][1] = matrix.m[1][1] * factor;
    m.m[1][2] = matrix.m[1][2] * factor;
    m.m[1][3] = matrix.m[1][3] * factor;
    m.m[2][0] = matrix.m[2][0] * factor;
    m.m[2][1] = matrix.m[2][1] * factor;
    m.m[2][2] = matrix.m[2][2] * factor;
    m.m[2][3] = matrix.m[2][3] * factor;
    m.m[3][0] = matrix.m[3][0] * factor;
    m.m[3][1] = matrix.m[3][1] * factor;
    m.m[3][2] = matrix.m[3][2] * factor;
    m.m[3][3] = matrix.m[3][3] * factor;
    return m;
}

inline bool qFuzzyCompare(const QMatrix4x4D& m1, const QMatrix4x4D& m2)
{
    return qFuzzyCompare(m1.m[0][0], m2.m[0][0]) &&
           qFuzzyCompare(m1.m[0][1], m2.m[0][1]) &&
           qFuzzyCompare(m1.m[0][2], m2.m[0][2]) &&
           qFuzzyCompare(m1.m[0][3], m2.m[0][3]) &&
           qFuzzyCompare(m1.m[1][0], m2.m[1][0]) &&
           qFuzzyCompare(m1.m[1][1], m2.m[1][1]) &&
           qFuzzyCompare(m1.m[1][2], m2.m[1][2]) &&
           qFuzzyCompare(m1.m[1][3], m2.m[1][3]) &&
           qFuzzyCompare(m1.m[2][0], m2.m[2][0]) &&
           qFuzzyCompare(m1.m[2][1], m2.m[2][1]) &&
           qFuzzyCompare(m1.m[2][2], m2.m[2][2]) &&
           qFuzzyCompare(m1.m[2][3], m2.m[2][3]) &&
           qFuzzyCompare(m1.m[3][0], m2.m[3][0]) &&
           qFuzzyCompare(m1.m[3][1], m2.m[3][1]) &&
           qFuzzyCompare(m1.m[3][2], m2.m[3][2]) &&
           qFuzzyCompare(m1.m[3][3], m2.m[3][3]);
}

inline QPoint QMatrix4x4D::map(const QPoint& point) const
{
    return *this * point;
}

inline QPointF QMatrix4x4D::map(const QPointF& point) const
{
    return *this * point;
}

#ifndef QT_NO_VECTOR3D

inline QVector3D QMatrix4x4D::map(const QVector3D& point) const
{
    return *this * point;
}

#endif

#ifndef QT_NO_VECTOR4D

inline QVector4D QMatrix4x4D::map(const QVector4D& point) const
{
    return *this * point;
}

#endif

inline double *QMatrix4x4D::data()
{
    // We have to assume that the caller will modify the matrix elements,
    // so we flip it over to "General" mode.
    flagBits = General;
    return m[0];
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QMatrix4x4D &m);
#endif

template <int N, int M>
QMatrix4x4D qGenericMatrixToMatrix4x4(const QGenericMatrix<N, M, double>& matrix)
{
    return QMatrix4x4D(matrix.constData(), N, M);
}

template <int N, int M>
QGenericMatrix<N, M, double> qGenericMatrixFromMatrix4x4(const QMatrix4x4D& matrix)
{
    QGenericMatrix<N, M, double> result;
    const double *m = matrix.constData();
    double *values = result.data();
    for (int col = 0; col < N; ++col) {
        for (int row = 0; row < M; ++row) {
            if (col < 4 && row < 4)
                values[col * M + row] = m[col * 4 + row];
            else if (col == row)
                values[col * M + row] = 1.0f;
            else
                values[col * M + row] = 0.0f;
        }
    }
    return result;
}

#endif
