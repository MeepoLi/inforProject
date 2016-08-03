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

#ifndef QVECTOR3DD_HPP
#define QVECTOR3DD_HPP

#include <QtCore/qmath.h>
#include <QPointF>
#include <stdint.h>
class QMatrix4x4D;
class QVector2D;
class QVector3D;
class QVector4D;
class QVariant;

class QVector3DD
{
public:
    QVector3DD();
    QVector3DD(double xpos, double ypos, double zpos);
    explicit QVector3DD(const QPoint& point);
    explicit QVector3DD(const QPointF& point);
    explicit QVector3DD(const QPointF& point, double zpos);
#ifndef QT_NO_VECTOR2D
    QVector3DD(const QVector2D& vector);
    QVector3DD(const QVector2D& vector, double zpos);
#endif
#ifndef QT_NO_VECTOR3D
    explicit QVector3DD(const QVector3D& vector);
#endif
#ifndef QT_NO_VECTOR4D
    explicit QVector3DD(const QVector4D& vector);
#endif

    bool isNull() const;

    double x() const;
    double y() const;
    double z() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    double length() const;
    double lengthSquared() const;

    QVector3DD normalized() const;
    void normalize();

    QVector3DD &operator+=(const QVector3DD &vector);
    QVector3DD &operator-=(const QVector3DD &vector);
    QVector3DD &operator*=(double factor);
    QVector3DD &operator*=(const QVector3DD& vector);
    QVector3DD &operator/=(double divisor);

    static double dotProduct(const QVector3DD& v1, const QVector3DD& v2);
    static QVector3DD crossProduct(const QVector3DD& v1, const QVector3DD& v2);
    static QVector3DD normal(const QVector3DD& v1, const QVector3DD& v2);
    static QVector3DD normal
        (const QVector3DD& v1, const QVector3DD& v2, const QVector3DD& v3);

    double distanceToPlane(const QVector3DD& plane, const QVector3DD& normal) const;
    double distanceToPlane(const QVector3DD& plane1, const QVector3DD& plane2, const QVector3DD& plane3) const;
    double distanceToLine(const QVector3DD& point, const QVector3DD& direction) const;

    friend inline bool operator==(const QVector3DD &v1, const QVector3DD &v2);
    friend inline bool operator!=(const QVector3DD &v1, const QVector3DD &v2);
    friend inline const QVector3DD operator+(const QVector3DD &v1, const QVector3DD &v2);
    friend inline const QVector3DD operator-(const QVector3DD &v1, const QVector3DD &v2);
    friend inline const QVector3DD operator*(double factor, const QVector3DD &vector);
    friend inline const QVector3DD operator*(const QVector3DD &vector, double factor);
    friend const QVector3DD operator*(const QVector3DD &v1, const QVector3DD& v2);
    friend inline const QVector3DD operator-(const QVector3DD &vector);
    friend inline const QVector3DD operator/(const QVector3DD &vector, double divisor);

    friend inline bool qFuzzyCompare(const QVector3DD& v1, const QVector3DD& v2);

#ifndef QT_NO_VECTOR2D
    QVector2D toVector2D() const;
#endif
#ifndef QT_NO_VECTOR3D
    QVector3D toVector3D() const;
#endif
#ifndef QT_NO_VECTOR4D
    QVector4D toVector4D() const;
#endif

    QPoint toPoint() const;
    QPointF toPointF() const;

    operator QVariant() const;

public:
    static   QVector3DD fromSpherical(const QPointF &p);
    QPointF  toSpherical() const;
    uint32_t toPackedNormal() const;

private:
    double xp, yp, zp;

    QVector3DD(double xpos, double ypos, double zpos, int dummy);

    friend class QMatrix4x4D;
#ifndef QT_NO_MATRIX4X4
    friend QVector3DD operator*(const QVector3DD& vector, const QMatrix4x4D& matrix);
    friend QVector3DD operator*(const QMatrix4x4D& matrix, const QVector3DD& vector);
#endif
};

Q_DECLARE_TYPEINFO(QVector3DD, Q_MOVABLE_TYPE);

inline QVector3DD::QVector3DD() : xp(0.0f), yp(0.0f), zp(0.0f) {}

inline QVector3DD::QVector3DD(double xpos, double ypos, double zpos) : xp(xpos), yp(ypos), zp(zpos) {}

inline QVector3DD::QVector3DD(double xpos, double ypos, double zpos, int) : xp(xpos), yp(ypos), zp(zpos) {}

inline QVector3DD::QVector3DD(const QPoint& point) : xp(point.x()), yp(point.y()), zp(0.0f) {}

inline QVector3DD::QVector3DD(const QPointF& point) : xp(point.x()), yp(point.y()), zp(0.0f) {}

inline QVector3DD::QVector3DD(const QPointF& point, double zpos) : xp(point.x()), yp(point.y()), zp(zpos) {}

inline bool QVector3DD::isNull() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp);
}

inline double QVector3DD::x() const { return double(xp); }
inline double QVector3DD::y() const { return double(yp); }
inline double QVector3DD::z() const { return double(zp); }

inline void QVector3DD::setX(double aX) { xp = aX; }
inline void QVector3DD::setY(double aY) { yp = aY; }
inline void QVector3DD::setZ(double aZ) { zp = aZ; }

inline QVector3DD &QVector3DD::operator+=(const QVector3DD &vector)
{
    xp += vector.xp;
    yp += vector.yp;
    zp += vector.zp;
    return *this;
}

inline QVector3DD &QVector3DD::operator-=(const QVector3DD &vector)
{
    xp -= vector.xp;
    yp -= vector.yp;
    zp -= vector.zp;
    return *this;
}

inline QVector3DD &QVector3DD::operator*=(double factor)
{
    xp *= factor;
    yp *= factor;
    zp *= factor;
    return *this;
}

inline QVector3DD &QVector3DD::operator*=(const QVector3DD& vector)
{
    xp *= vector.xp;
    yp *= vector.yp;
    zp *= vector.zp;
    return *this;
}

inline QVector3DD &QVector3DD::operator/=(double divisor)
{
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    return *this;
}

inline bool operator==(const QVector3DD &v1, const QVector3DD &v2)
{
    return v1.xp == v2.xp && v1.yp == v2.yp && v1.zp == v2.zp;
}

inline bool operator!=(const QVector3DD &v1, const QVector3DD &v2)
{
    return v1.xp != v2.xp || v1.yp != v2.yp || v1.zp != v2.zp;
}

inline const QVector3DD operator+(const QVector3DD &v1, const QVector3DD &v2)
{
    return QVector3DD(v1.xp + v2.xp, v1.yp + v2.yp, v1.zp + v2.zp, 1);
}

inline const QVector3DD operator-(const QVector3DD &v1, const QVector3DD &v2)
{
    return QVector3DD(v1.xp - v2.xp, v1.yp - v2.yp, v1.zp - v2.zp, 1);
}

inline const QVector3DD operator*(double factor, const QVector3DD &vector)
{
    return QVector3DD(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const QVector3DD operator*(const QVector3DD &vector, double factor)
{
    return QVector3DD(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const QVector3DD operator*(const QVector3DD &v1, const QVector3DD& v2)
{
    return QVector3DD(v1.xp * v2.xp, v1.yp * v2.yp, v1.zp * v2.zp, 1);
}

inline const QVector3DD operator-(const QVector3DD &vector)
{
    return QVector3DD(-vector.xp, -vector.yp, -vector.zp, 1);
}

inline const QVector3DD operator/(const QVector3DD &vector, double divisor)
{
    return QVector3DD(vector.xp / divisor, vector.yp / divisor, vector.zp / divisor, 1);
}

inline bool qFuzzyCompare(const QVector3DD& v1, const QVector3DD& v2)
{
    return qFuzzyCompare(v1.xp, v2.xp) &&
           qFuzzyCompare(v1.yp, v2.yp) &&
           qFuzzyCompare(v1.zp, v2.zp);
}

inline QPoint QVector3DD::toPoint() const
{
    return QPoint(qRound(xp), qRound(yp));
}

inline QPointF QVector3DD::toPointF() const
{
    return QPointF(double(xp), double(yp));
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QVector3DD &vector);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &, const QVector3DD &);
QDataStream &operator>>(QDataStream &, QVector3DD &);
#endif

#endif
