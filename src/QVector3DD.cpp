/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "QVector3DD.hpp"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>
#include <QDataStream>

#ifndef QT_NO_VECTOR3D

/*!
    \class QVector3DD
    \brief The QVector3DD class represents a vector or vertex in 3D space.
    \since 4.6
    \ingroup painting-3D

    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.

    The QVector3DD class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \bold{Note:} By design values in the QVector3DD instance are stored as \c float.
    This means that on platforms where the \c double arguments to QVector3DD
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa QVector2D, QVector4D, QQuaternion
*/

/*!
    \fn QVector3DD::QVector3DD()

    Constructs a null vector, i.e. with coordinates (0, 0, 0).
*/

/*!
    \fn QVector3DD::QVector3DD(double xpos, double ypos, double zpos)

    Constructs a vector with coordinates (\a xpos, \a ypos, \a zpos).
*/

/*!
    \fn QVector3DD::QVector3DD(const QPoint& point)

    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/

/*!
    \fn QVector3DD::QVector3DD(const QPointF& point)

    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/

#ifndef QT_NO_VECTOR2D

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to zero.

    \sa toVector2D()
*/
QVector3DD::QVector3DD(const QVector2D& vector)
{
    xp = vector.x();
    yp = vector.y();
    zp = 0.0f;
}

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to \a zpos.

    \sa toVector2D()
*/
QVector3DD::QVector3DD(const QVector2D& vector, double zpos)
{
    xp = vector.x();
    yp = vector.y();
    zp = zpos;
}

#endif


#ifndef QT_NO_VECTOR3D

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to zero.

    \sa toVector2D()
*/
QVector3DD::QVector3DD(const QVector3D& vector)
{
    xp = vector.x();
    yp = vector.y();
    zp = vector.z();
}

#endif


#ifndef QT_NO_VECTOR4D

/*!
    Constructs a 3D vector from the specified 4D \a vector.  The w
    coordinate is dropped.

    \sa toVector4D()
*/
QVector3DD::QVector3DD(const QVector4D& vector)
{
    xp = vector.x();
    yp = vector.y();
    zp = vector.z();
}

#endif

/*!
    \fn bool QVector3DD::isNull() const

    Returns true if the x, y, and z coordinates are set to 0.0,
    otherwise returns false.
*/

/*!
    \fn double QVector3DD::x() const

    Returns the x coordinate of this point.

    \sa setX(), y(), z()
*/

/*!
    \fn double QVector3DD::y() const

    Returns the y coordinate of this point.

    \sa setY(), x(), z()
*/

/*!
    \fn double QVector3DD::z() const

    Returns the z coordinate of this point.

    \sa setZ(), x(), y()
*/

/*!
    \fn void QVector3DD::setX(double x)

    Sets the x coordinate of this point to the given \a x coordinate.

    \sa x(), setY(), setZ()
*/

/*!
    \fn void QVector3DD::setY(double y)

    Sets the y coordinate of this point to the given \a y coordinate.

    \sa y(), setX(), setZ()
*/

/*!
    \fn void QVector3DD::setZ(double z)

    Sets the z coordinate of this point to the given \a z coordinate.

    \sa z(), setX(), setY()
*/

/*!
    Returns the normalized unit vector form of this vector.

    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.

    \sa length(), normalize()
*/
QVector3DD QVector3DD::normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp);
    if (qFuzzyIsNull(len - 1.0f))
        return *this;
    else if (!qFuzzyIsNull(len))
        return *this / qSqrt(len);
    else
        return QVector3DD();
}

/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.

    \sa length(), normalized()
*/
void QVector3DD::normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp);
    if (qFuzzyIsNull(len - 1.0f) || qFuzzyIsNull(len))
        return;

    len = qSqrt(len);

    xp /= len;
    yp /= len;
    zp /= len;
}

/*!
    \fn QVector3DD &QVector3DD::operator+=(const QVector3DD &vector)

    Adds the given \a vector to this vector and returns a reference to
    this vector.

    \sa operator-=()
*/

/*!
    \fn QVector3DD &QVector3DD::operator-=(const QVector3DD &vector)

    Subtracts the given \a vector from this vector and returns a reference to
    this vector.

    \sa operator+=()
*/

/*!
    \fn QVector3DD &QVector3DD::operator*=(double factor)

    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.

    \sa operator/=()
*/

/*!
    \fn QVector3DD &QVector3DD::operator*=(const QVector3DD& vector)
    \overload

    Multiplies the components of this vector by the corresponding
    components in \a vector.

    Note: this is not the same as the crossProduct() of this
    vector and \a vector.

    \sa crossProduct()
*/

/*!
    \fn QVector3DD &QVector3DD::operator/=(double divisor)

    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.

    \sa operator*=()
*/

/*!
    Returns the dot product of \a v1 and \a v2.
*/
double QVector3DD::dotProduct(const QVector3DD& v1, const QVector3DD& v2)
{
    return v1.xp * v2.xp + v1.yp * v2.yp + v1.zp * v2.zp;
}

/*!
    Returns the cross-product of vectors \a v1 and \a v2, which corresponds
    to the normal vector of a plane defined by \a v1 and \a v2.

    \sa normal()
*/
QVector3DD QVector3DD::crossProduct(const QVector3DD& v1, const QVector3DD& v2)
{
    return QVector3DD(v1.yp * v2.zp - v1.zp * v2.yp,
                      v1.zp * v2.xp - v1.xp * v2.zp,
                      v1.xp * v2.yp - v1.yp * v2.xp, 1);
}

/*!
    Returns the normal vector of a plane defined by vectors \a v1 and \a v2,
    normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v1 and \a v2 if you
    do not need the result to be normalized to a unit vector.

    \sa crossProduct(), distanceToPlane()
*/
QVector3DD QVector3DD::normal(const QVector3DD& v1, const QVector3DD& v2)
{
    return crossProduct(v1, v2).normalized();
}

/*!
    \overload

    Returns the normal vector of a plane defined by vectors
    \a v2 - \a v1 and \a v3 - \a v1, normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v2 - \a v1 and
    \a v3 - \a v1 if you do not need the result to be normalized to a
    unit vector.

    \sa crossProduct(), distanceToPlane()
*/
QVector3DD QVector3DD::normal
        (const QVector3DD& v1, const QVector3DD& v2, const QVector3DD& v3)
{
    return crossProduct((v2 - v1), (v3 - v1)).normalized();
}

/*!
    Returns the distance from this vertex to a plane defined by
    the vertex \a plane and a \a normal unit vector.  The \a normal
    parameter is assumed to have been normalized to a unit vector.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    \sa normal(), distanceToLine()
*/
double QVector3DD::distanceToPlane
        (const QVector3DD& plane, const QVector3DD& normal) const
{
    return dotProduct(*this - plane, normal);
}

/*!
    \overload

    Returns the distance from this vertex a plane defined by
    the vertices \a plane1, \a plane2 and \a plane3.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    The two vectors that define the plane are \a plane2 - \a plane1
    and \a plane3 - \a plane1.

    \sa normal(), distanceToLine()
*/
double QVector3DD::distanceToPlane
    (const QVector3DD& plane1, const QVector3DD& plane2, const QVector3DD& plane3) const
{
    QVector3DD n = normal(plane2 - plane1, plane3 - plane1);
    return dotProduct(*this - plane1, n);
}

/*!
    Returns the distance that this vertex is from a line defined
    by \a point and the unit vector \a direction.

    If \a direction is a null vector, then it does not define a line.
    In that case, the distance from \a point to this vertex is returned.

    \sa distanceToPlane()
*/
double QVector3DD::distanceToLine
        (const QVector3DD& point, const QVector3DD& direction) const
{
    if (direction.isNull())
        return (*this - point).length();
    QVector3DD p = point + dotProduct(*this - point, direction) * direction;
    return (*this - p).length();
}

/*!
    \fn bool operator==(const QVector3DD &v1, const QVector3DD &v2)
    \relates QVector3DD

    Returns true if \a v1 is equal to \a v2; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn bool operator!=(const QVector3DD &v1, const QVector3DD &v2)
    \relates QVector3DD

    Returns true if \a v1 is not equal to \a v2; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn const QVector3DD operator+(const QVector3DD &v1, const QVector3DD &v2)
    \relates QVector3DD

    Returns a QVector3DD object that is the sum of the given vectors, \a v1
    and \a v2; each component is added separately.

    \sa QVector3DD::operator+=()
*/

/*!
    \fn const QVector3DD operator-(const QVector3DD &v1, const QVector3DD &v2)
    \relates QVector3DD

    Returns a QVector3DD object that is formed by subtracting \a v2 from \a v1;
    each component is subtracted separately.

    \sa QVector3DD::operator-=()
*/

/*!
    \fn const QVector3DD operator*(double factor, const QVector3DD &vector)
    \relates QVector3DD

    Returns a copy of the given \a vector,  multiplied by the given \a factor.

    \sa QVector3DD::operator*=()
*/

/*!
    \fn const QVector3DD operator*(const QVector3DD &vector, double factor)
    \relates QVector3DD

    Returns a copy of the given \a vector,  multiplied by the given \a factor.

    \sa QVector3DD::operator*=()
*/

/*!
    \fn const QVector3DD operator*(const QVector3DD &v1, const QVector3DD& v2)
    \relates QVector3DD

    Multiplies the components of \a v1 by the corresponding components in \a v2.

    Note: this is not the same as the crossProduct() of \a v1 and \a v2.

    \sa QVector3DD::crossProduct()
*/

/*!
    \fn const QVector3DD operator-(const QVector3DD &vector)
    \relates QVector3DD
    \overload

    Returns a QVector3DD object that is formed by changing the sign of
    all three components of the given \a vector.

    Equivalent to \c {QVector3DD(0,0,0) - vector}.
*/

/*!
    \fn const QVector3DD operator/(const QVector3DD &vector, double divisor)
    \relates QVector3DD

    Returns the QVector3DD object formed by dividing all three components of
    the given \a vector by the given \a divisor.

    \sa QVector3DD::operator/=()
*/

/*!
    \fn bool qFuzzyCompare(const QVector3DD& v1, const QVector3DD& v2)
    \relates QVector3DD

    Returns true if \a v1 and \a v2 are equal, allowing for a small
    fuzziness factor for floating-point comparisons; false otherwise.
*/

#ifndef QT_NO_VECTOR2D

/*!
    Returns the 2D vector form of this 3D vector, dropping the z coordinate.

    \sa toVector4D(), toPoint()
*/
QVector2D QVector3DD::toVector2D() const
{
    return QVector2D(xp, yp);
}

#endif

#ifndef QT_NO_VECTOR3D
QVector3D QVector3DD::toVector3D() const
{
    return QVector3D(xp, yp, zp);
}
#endif


#ifndef QT_NO_VECTOR4D

/*!
    Returns the 4D form of this 3D vector, with the w coordinate set to zero.

    \sa toVector2D(), toPoint()
*/
QVector4D QVector3DD::toVector4D() const
{
    return QVector4D(xp, yp, zp, 0.0f);
}

#endif

/*!
    \fn QPoint QVector3DD::toPoint() const

    Returns the QPoint form of this 3D vector. The z coordinate
    is dropped.

    \sa toPointF(), toVector2D()
*/

/*!
    \fn QPointF QVector3DD::toPointF() const

    Returns the QPointF form of this 3D vector. The z coordinate
    is dropped.

    \sa toPoint(), toVector2D()
*/

/*!
    Returns the 3D vector as a QVariant.
*/
QVector3DD::operator QVariant() const
{
    return QVariant(QVariant::Vector3D, this);
}

/*!
    Returns the length of the vector from the origin.

    \sa lengthSquared(), normalized()
*/
double QVector3DD::length() const
{
    return qSqrt(xp * xp + yp * yp + zp * zp);
}

/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.

    \sa length(), dotProduct()
*/
double QVector3DD::lengthSquared() const
{
    return xp * xp + yp * yp + zp * zp;
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const QVector3DD &vector)
{
    dbg.nospace() << "QVector3DD("
        << vector.x() << ", " << vector.y() << ", " << vector.z() << ')';
    return dbg.space();
}

#endif

#ifndef QT_NO_DATASTREAM

/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QVector3DD &vector)
    \relates QVector3DD

    Writes the given \a vector to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const QVector3DD &vector)
{
    stream << double(vector.x()) << double(vector.y())
           << double(vector.z());
    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QVector3DD &vector)
    \relates QVector3DD

    Reads a 3D vector from the given \a stream into the given \a vector
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, QVector3DD &vector)
{
    double x, y, z;
    stream >> x;
    stream >> y;
    stream >> z;
    vector.setX(double(x));
    vector.setY(double(y));
    vector.setZ(double(z));
    return stream;
}

#endif // QT_NO_DATASTREAM

#endif // QT_NO_VECTOR3D

// result = phi, theta
QPointF QVector3DD::toSpherical() const
{
  return QPointF(atan2(this->yp, this->xp), acos(this->zp/this->length()));
}

// p = phi, theta
QVector3DD QVector3DD::fromSpherical(const QPointF &p)
{
  return QVector3DD(QPointF(cos(p.x()), sin(p.x()))*sin(p.y()), cos(p.y()));
}

inline uint32_t f2u16(float f)
{
  return (uint32_t)((f*65535.0)+0.5);
}

inline uint32_t packUnorm2x16(float lowerF, float higherF)
{
  return f2u16(lowerF) + (f2u16(higherF)<<16);
}

inline uint32_t f2s16(float f)
{
  return (uint32_t)((f*32767.0)+0.5);
}

inline uint32_t packSnorm2x16(float lowerF, float higherF)
{
  return f2s16(lowerF) + (f2s16(higherF)<<16);
}

uint32_t QVector3DD::toPackedNormal() const
{
  QPointF sn = (this->toSpherical()/M_PI*0.5)+QPointF(0.5, 0.5);
  return packUnorm2x16(sn.x(), sn.y());
}
