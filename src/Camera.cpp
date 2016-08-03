#include <GL/glew.h>
#include "Camera.hpp"
#include <QFile>

//==============================================================================
// Camera
//==============================================================================

Camera::Camera(const QRectF &nvp, int dpr) :
devicePixelRatio(dpr),
visCurrent(Camera::VISIBILITY_FULL),
zoomLevel(0),
pixelScaling(1.0)
{
  this->setNormalizedViewport(nvp);
}

Camera::~Camera()
{
}

void Camera::setNormalizedViewport(const QRectF &nvp)
{
  this->normalizedViewport = nvp;
}

void Camera::setViewport(int width, int height, int dpr, float zf)
{
  this->zoomFactor     = zf;
  this->screenViewport = QRectF(0, 0, width/this->zoomFactor, height/this->zoomFactor);
  this->totalSize      = QSizeF(this->screenViewport.width()/this->normalizedViewport.width(),
                                this->screenViewport.height()/this->normalizedViewport.height());
  this->localViewport  = QRectF(this->totalSize.width()*(this->normalizedViewport.x()-0.5),
                                this->totalSize.height()*(this->normalizedViewport.y()-0.5),
                                this->totalSize.width()*this->normalizedViewport.width(),
                                this->totalSize.height()*this->normalizedViewport.height());
  this->devicePixelRatio = dpr;

  this->nativeViewport = QRect(this->screenViewport.x()*this->zoomFactor*this->devicePixelRatio,
                               this->screenViewport.y()*this->zoomFactor*this->devicePixelRatio,
                               ceil(this->screenViewport.width()*this->zoomFactor*this->devicePixelRatio),
                               ceil(this->screenViewport.height()*this->zoomFactor*this->devicePixelRatio));

    this->updateViewport();
}

void Camera::setWorldViewport(const QRectF &wvp)
{
  this->worldViewport  = wvp;
  this->worldBounds[0] = QLineF(this->worldViewport.topLeft(), this->worldViewport.topRight());
  this->worldBounds[1] = QLineF(this->worldViewport.topRight(), this->worldViewport.bottomRight());
  this->worldBounds[2] = QLineF(this->worldViewport.bottomRight(), this->worldViewport.bottomLeft());
  this->worldBounds[3] = QLineF(this->worldViewport.bottomLeft(), this->worldViewport.topLeft());


}

QPointF Camera::mapToLocal(const QPointF &p) const
{
  return (p-QPointF(this->totalSize.width(), this->totalSize.height())*0.5-this->localViewport.topLeft())*this->zoomFactor;
}

void Camera::resetVisibility()
{
  this->visRendering = this->visCurrent;
  this->visCurrent   = Camera::VISIBILITY_NONE;
}

void Camera::updateCurrentVisibility(VisibilityTest vis)
{
  switch (this->visCurrent) {

    case Camera::VISIBILITY_FULL:
      break;

    case Camera::VISIBILITY_MORE:
      if (vis==Camera::VISIBILITY_LESS || vis==Camera::VISIBILITY_FULL)
        vis = Camera::VISIBILITY_FULL;
      break;

    case Camera::VISIBILITY_SAME:
      if (vis!=Camera::VISIBILITY_FULL)
        this->visCurrent = vis;
      break;

    case Camera::VISIBILITY_LESS:
      if (vis==Camera::VISIBILITY_MORE || vis==Camera::VISIBILITY_FULL)
        vis = Camera::VISIBILITY_FULL;
      break;

    case Camera::VISIBILITY_NONE:
      this->visCurrent = vis;
      break;

    default:
      break;
  }
}

//==============================================================================
// Camera 2D
//==============================================================================

Camera2D::Camera2D(const QRectF &nvp, int dpr):
Camera(nvp, dpr)
{
}

Camera2D::Camera2D(const QPointF &center, double level, const QRectF &nvp, int dpr):
Camera(nvp, dpr)
{
  this->setCenter(center);
  this->setZoomLevel(level);
    this->zoomFactor = 1.0;
}

Camera2D::~Camera2D()
{
}

void Camera2D::setCenter(const QPointF &center)
{
  this->setWorldCenter(Camera::geo2world(center));
}

void Camera2D::setWorldCenter(const QPointF &center)
{
  this->worldCenter = center;
  this->geoCenter = Camera::world2geo(this->worldCenter);
  this->updateCurrentVisibility(Camera::VISIBILITY_SAME);
}

void Camera2D::setZoomLevel(float level)
{
  float newZoomLevel = std::max(0.f, std::min(WORLD_ZOOM_LEVEL, level));
  if (newZoomLevel!=this->zoomLevel) {
    this->updateCurrentVisibility(newZoomLevel>this->zoomLevel?
                                  Camera::VISIBILITY_MORE:
                                  Camera::VISIBILITY_LESS);
    this->zoomLevel = newZoomLevel;
  }
}

double Camera2D::getGroundResolution() const
{
  return Camera::getGroundResolution(this->geoCenter, this->zoomLevel);
}

QString Camera2D::info() const
{
  return QString("w:%1,%2   z:%3")
  .arg(this->geoCenter.x(), 0, 'f', 6)
  .arg(this->geoCenter.y(), 0, 'f', 6)
  .arg(this->zoomLevel, 0, 'f', 1);
}

void Camera2D::updateViewport()
{
  this->setWorldViewport(QRectF(this->zoom2world(this->localViewport.topLeft()),
                                this->zoom2world(this->localViewport.bottomRight())));

  this->matProjection.setToIdentity();
  this->matProjection.ortho(this->localViewport.left(), this->localViewport.right(),
                            this->localViewport.bottom(), this->localViewport.top(),
                            -2000, 1000);

  this->glTransform();
  this->matMapToScreen.setToIdentity();
  this->matMapToScreen.scale(this->localViewport.width()*0.5, -this->localViewport.height()*0.5);
  this->matMapToScreen.translate(1, -1);
  this->matMapToScreen *= this->matModelViewProjection;
  this->matScreenToMap  = this->matMapToScreen.inverted();
  this->matNormal = this->matModelView.normalMatrix();

  this->dataView.ground = this->worldViewport;
  this->dataView.height = QPointF(this->zoomLevel, WORLD_ZOOM_LEVEL);
}

void Camera2D::glTransform(float dataLevel, const QPointF &translation)
{
  double data2zoom = exp2(this->zoomLevel-dataLevel);
  double groundRes = this->getGroundResolution();
  this->matModelView.setToIdentity();
  this->matModelView.scale(data2zoom, data2zoom, 1.0/groundRes);

  double  world2data   = exp2(dataLevel-WORLD_ZOOM_LEVEL);
  QPointF translate    = translation-this->worldCenter*world2data;
  this->matModelView.translate(translate.x(), translate.y(), 0);

  this->matModelViewProjection = this->matProjection*this->matModelView;
}

void Camera2D::glTransformOverlay(bool useDepth)
{
  this->matModelView.setToIdentity();
  this->matModelView.translate(this->localViewport.left(), this->localViewport.top());
  if (!useDepth) {
    this->matModelView.translate(0, 0, 2000);
    this->matModelView.scale(1, 1, 0);
  }
  this->matModelViewProjection = this->matProjection*this->matModelView;
}

void Camera2D::pan(const QPointF &anchor, const QPointF &screenDiff)
{
  qWarning()<< "2dPan = " << anchor; 
  double zoom2world = exp2(WORLD_ZOOM_LEVEL-this->zoomLevel);
  this->setWorldCenter(this->worldCenter - screenDiff*zoom2world);
}

void Camera2D::zoom(const QPointF &anchor, double diff)
{
  double ratio     = exp2(diff*1e-3);
  double newLevel  = std::max((double)0, std::min(this->zoomLevel*ratio, (double)WORLD_ZOOM_LEVEL));
  double zoom2zoom = exp2(this->zoomLevel-newLevel);
  QPointF zoomPos  = this->screen2world(anchor);
  this->setWorldCenter(zoomPos+(this->worldCenter-zoomPos)*zoom2zoom);
  this->setZoomLevel(newLevel);
}

//==============================================================================
// Camera 3D
//==============================================================================
// Focal Length (mm)      0       2      12      14      16     20      24      35      50      70      85     105     200     300     400     500     600     700     800    1200
// Diagonal (deg)     180.0   169.4   122.0   114.2   107.1   94.5    84.1    63.4    46.8    34.4    28.6    23.3    12.4    8.25    6.19    4.96    4.13    5.72    3.10    2.07
// Vertical (deg)     180.0   161.1    90.0    81.2    73.9   61.9    53.1    37.8    27.0    19.5    16.1    13.0     6.87   4.58    3.44    2.75    2.29    1.96    1.72    1.15
// Horizontal (deg)   180.0   166.9   111.1   102.7    95.1   82.4    73.7    54.4    39.6    28.8    23.9    19.5    10.3    6.87    5.15    4.12    3.44    2.86    2.58    1.72
//==============================================================================

Camera3D::Camera3D(const QRectF &nvp, int dpr):
Camera(nvp, dpr), pUp(0, 1, 0)
{
  this->setParameters(37.8f, 10, 3*8192);
  this->isOrtho = false;
}

void Camera3D::setParameters(float vAngle, float nearPlane, float farPlane)
{
  this->setVerticalAngle(vAngle);
  this->setClippingRange(nearPlane, farPlane);
}

void Camera3D::setVerticalAngle(float vAngle)
{
  this->verticalAngle = vAngle;
}

void Camera3D::setClippingRange(float nearPlane, float farPlane)
{
  this->nearPlane = nearPlane;
  this->farPlane  = farPlane;
}

void Camera3D::setEye(const QVector3DD &eye)
{
  this->pEye = eye;
}


void Camera3D::setLookAt(const QVector3DD &lookat)
{
  this->pLookAt = lookat;
}

void Camera3D::setUp(const QVector3DD &up)
{
  this->pUp = up;
}

void Camera3D::setOrtho(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
  this->wLeft = left;
  this->wRight = right;
  this->wBottom = bottom;
  this->wTop = top;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
}

void Camera3D::setIsOrtho(bool isOrtho)
{
    this->isOrtho = isOrtho;
}


void Camera3D::setPositions(const QVector3DD &eye, const QVector3DD &lookat, const QVector3DD &up)
{
  this->setEye(eye);
  this->setLookAt(lookat);
  this->setUp(up);
}

bool Camera3D::isInFrustum(const QVector3DD &p) const
{
  qreal dis = p.distanceToPlane(this->wEye, this->wEyeDir);
  return dis>=this->wNear*10 && dis<=this->wFar;
}

void Camera3D::updateWorldPositions()
{
  this->groundRes  = 1.0;//this->getGroundResolution();
  this->wNear      = this->nearPlane/this->groundRes;
  this->wFar       = this->farPlane/this->groundRes;

  this->wEye       = this->geo2cam(this->pEye);
  this->wLookAt    = this->geo2cam(this->pLookAt);
  this->wEyeDir    = this->wLookAt-this->wEye;
  this->wEyeLength = this->wEyeDir.length();
  this->wEyeDir.normalize();

  this->wUp        = QVector3DD::crossProduct(this->wEyeDir, QVector3DD::crossProduct(this->pUp, this->wEyeDir)).normalized();
  this->pUp        = this->wUp;
  this->wHeadLight = this->wEye + this->wUp*this->wEyeLength*0.5;
  this->wHeadLight.setZ(this->wHeadLight.z()*this->groundRes);

//    qDebug() << "start";
//    qDebug() << this->groundRes << this->wNear << this->wFar
//             << this->wEye << this->wLookAt << this->wEyeDir << this->wEyeLength
//             << this->wUp << this->pUp << this->wHeadLight;
//    qDebug() << "end";
}

double Camera3D::getGroundResolution() const
{
  return Camera::getGroundResolution(this->pLookAt.toPointF(), WORLD_ZOOM_LEVEL);
}

QString Camera3D::info() const
{
  return QString("w:%1,%2   z:%3   e:%4m")
  .arg(this->pLookAt.x(), 0, 'f', 6)
  .arg(this->pLookAt.y(), 0, 'f', 6)
  .arg(this->zoomLevel, 0, 'f', 1)
  .arg(this->getElevation(), 0, 'f', 1);
}

void Camera3D::updateViewport()
{
//    this->wNear = 0;
//    this->wFar = 0;
//    this->zoomFactor = 1.0;

  this->updateWorldPositions();
  double aspect  = this->totalSize.width()/this->totalSize.height();
  double radians = (this->verticalAngle*0.5) * M_PI / 180.0;
  double worldHeight = tan(radians)*this->wEye.z()*2;
  double worldWidth  = worldHeight*aspect;
  double localZoom = this->totalSize.height()/this->localViewport.height();
  QPointF localOffset = QPointF(-this->localViewport.center().x()*2/this->totalSize.width(),
                                this->localViewport.center().y()*2/this->totalSize.height())*localZoom;

  this->matProjection.setToIdentity();
  this->matProjection.translate(localOffset.x(), localOffset.y());
  this->matProjection.scale(localZoom, localZoom);
  this->matProjection.scale(1, -1, 1);
  this->matProjection.perspective(this->verticalAngle, aspect, this->wNear, this->wFar);

  double scaling = (2.0*this->wNear/this->totalSize.height())/this->matProjection(1, 1);
  {
    this->matOverlay.setToIdentity();
    this->matOverlay.scale(-scaling, -scaling, -1);
    this->matOverlay.translate(1, -1);
    this->matOverlay.scale(localZoom, localZoom);
    this->pixelScaling = fabs(4.0*this->zoomFactor/this->matProjection(1, 1)/this->groundRes);
  }

  this->glTransform();
  this->matMapToScreen.setToIdentity();
  this->matMapToScreen.scale(this->localViewport.width()*0.5, -this->localViewport.height()*0.5, 1);
  this->matMapToScreen.translate(1, -1);
  this->matMapToScreen *= this->matModelViewProjection;
  this->matScreenToMap.setToIdentity();
  this->matScreenToMap.translate(0, 0, this->nearPlane/this->farPlane+1.5e-3-1.0);
  this->matScreenToMap.scale(1, 1, 2);
  this->matScreenToMap = this->matMapToScreen.inverted()*this->matScreenToMap;
  {
    QMatrix4x4D invertedY;
    invertedY.setToIdentity();
    invertedY.scale(1, -1, 1);
    this->matNormal = invertedY*this->matModelView.normalMatrix();
  }

  this->zoomLevel = Camera::getLevelFromResolution(this->pEye.toPointF(),
                                                   this->groundRes*worldHeight/this->totalSize.height());
  this->dataView.height = QPointF(this->zoomLevel, WORLD_ZOOM_LEVEL);
  {
    QPolygonF ground;
    scaling = -scaling*this->wFar/this->wNear;
    QVector3DD X = QVector3DD::crossProduct(this->wEyeDir, this->wUp).normalized()*this->screenViewport.width()*0.5*scaling;
    QVector3DD Y = this->wUp*this->screenViewport.height()*0.5*scaling;
    QVector3DD O = this->wEye+this->wEyeDir*this->wFar;
    const qreal dir[4][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    for (int i=0; i<4; ++i) {
      QVector3DD f = O + dir[i][0]*X + dir[i][1]*Y;
      if (f.z()<0) {
        f -= this->wEye;
        f = this->wEye - f*this->wEye.z()/f.z();
      }
      ground << f.toPointF();
    }
    this->dataView.ground = ground.boundingRect();
  }
  this->setWorldViewport(this->dataView.ground);

  if(this->isOrtho) {
    this->matProjection.setToIdentity();
    this->matProjection.translate(localOffset.x(), localOffset.y());
    this->matProjection.scale(localZoom, localZoom);
    this->matProjection.scale(1, -1, 1);
//    this->matProjection.ortho(-worldWidth, worldWidth,
//                              -worldHeight, worldHeight,
//                              this->wNear / 2, this->wFar * 2);
      this->matProjection.ortho(this->wLeft, this->wRight,
                                this->wBottom, this->wTop,
                                this->nearPlane, this->farPlane);

//      qDebug() << localZoom << localOffset;
//      qDebug() << this->wLeft << this->wRight << this->wBottom << this->wTop << this->nearPlane << this->farPlane;
//      qDebug() << this->matProjection;

  }
  this->matModelViewProjection = this->matProjection * this->matModelView;

}

void Camera3D::glTransform(float dataLevel, const QPointF &translation, const QPointF &scale)
{
  {
        this->dataLevel = dataLevel;
        this->translation = QPointF(translation);

        double data2zoom = exp2(WORLD_ZOOM_LEVEL-dataLevel);
        this->matModelView.setToIdentity();
        this->matModelView.lookAt(this->wEye, this->wLookAt, this->wUp);
        this->matModelView.scale(data2zoom, data2zoom, 1.0/this->groundRes);
        this->matModelView.translate(translation.x(), translation.y(), 0);
//        this->matModelView.scale(scale.x()/2.0,scale.y()/2.0, 1);
//        this->matModelView.translate(-translation.x(), -translation.y(), 0);
        this->matModelViewProjection = this->matProjection*this->matModelView;
        this->wHeadLightTransformed = this->matModelView*(this->wHeadLight-QVector3DD(translation,0));
  }
}

void Camera3D::glTransformOverlay(bool useDepth)
{
  this->matModelView.setToIdentity();
  if (!useDepth) {
    this->matModelView.translate(this->localViewport.left(), this->localViewport.top(), this->nearPlane/this->groundRes);
    this->matModelView.scale(1, 1, 0);
  } else {
    this->matModelView.translate(this->localViewport.left(), this->localViewport.top());
  }
  this->matModelView = this->matOverlay*this->matModelView;
  this->matModelViewProjection = this->matProjection*this->matModelView;
}

void Camera3D::pan(const QPointF &, const QPointF &diff)
{

  QVector3DD X = QVector3DD::crossProduct(this->wEyeDir, this->wUp).normalized();
  qWarning() << "Hello " ;
  double scale = (this->wEyeLength*this->groundRes*this->matOverlay(0, 0))/this->nearPlane;
 // scale *= 1e6;
  QVector3DD D = (diff.x()*X + diff.y()*this->wUp)*scale;
  this->wEye = this->wEye - D;
  if (this->wEye.z()<1000) return;
  this->wLookAt = this->wEye + this->wEyeDir*this->wEyeLength;
  this->pEye = this->cam2geo(this->wEye);
  this->pLookAt = this->cam2geo(this->wLookAt);

//  qDebug() << ">>>>>>" << qSetRealNumberPrecision(10) << pEye << pLookAt;

  this->updateWorldPositions();

}

void Camera3D::zoom(const QPointF &, double diff)
{
  double ratio = diff*1e6;
  if (ratio+this->wNear>this->wEyeLength)
    return;

  this->wEye += this->wEyeDir*ratio;
  this->pEye = this->cam2geo(this->wEye);
  this->wLookAt = this->wEye+this->wEyeDir*this->wEyeLength;
  this->pLookAt = this->cam2geo(this->wLookAt);

  this->updateWorldPositions();
}

void Camera3D::yaw(const QPointF &, double diff)
{
  QVector3DD X  = QVector3DD::crossProduct(this->wEyeDir, this->wUp).normalized();
  double rad    = (diff/20.0)/180.0*M_PI;
  QVector3DD D  = X*sin(rad) + this->wEyeDir*cos(rad);
  this->wLookAt = this->wEye + D*this->wEyeLength;
  this->pLookAt = this->cam2geo(this->wLookAt);
  QVector3DD T  = X*cos(rad) + this->wEyeDir*tan(rad);
  this->wUp     = QVector3DD::crossProduct(T, D).normalized();
  this->pUp     = this->wUp;
  this->updateWorldPositions();
}

void Camera3D::reversedYaw(float focus, double diff)
{
  QVector3DD X = QVector3DD::crossProduct(-this->wEyeDir, this->wUp).normalized();
  double rad   = (diff/5.0)/180.0*M_PI;
  QVector3DD D = X*sin(rad) - this->wEyeDir*cos(rad);
  double length = this->wEyeLength*focus;
  QVector3DD pivot = this->wEye + this->wEyeDir*length;
  this->wEye   = pivot + D*length;
  this->pEye   = this->cam2geo(this->wEye);

  this->wEyeDir = (pivot-this->wEye).normalized();
  this->wLookAt = this->wEye + this->wEyeDir*this->wEyeLength;
  this->pLookAt = this->cam2geo(this->wLookAt);

  QVector3DD T = X*cos(rad) - this->wEyeDir*tan(rad);
  this->wUp    = QVector3DD::crossProduct(T, D).normalized();
  this->pUp    = this->wUp;
  this->updateWorldPositions();
}

void Camera3D::pitch(const QPointF &, double diff)
{
  double rad    = (diff/20.0)/180.0*M_PI;
  QVector3DD D  = this->wUp*sin(rad) + this->wEyeDir*cos(rad);
  this->wLookAt = this->wEye + D*this->wEyeLength;
  this->pLookAt = this->cam2geo(this->wLookAt);
  this->updateWorldPositions();
}

void Camera3D::reversedPitch(float focus, double diff)
{
  double rad   = (-diff/5.0)/180.0*M_PI;
  QVector3DD D = (this->wUp)*sin(rad) - this->wEyeDir*cos(rad);
  double length = this->wEyeLength*focus;
  QVector3DD pivot = this->wEye + this->wEyeDir*length;
  this->wEye   = pivot + D*length;
  this->wEyeDir = (pivot-this->wEye).normalized();
  this->wLookAt= this->wEye + this->wEyeDir*this->wEyeLength;
  if (D.z()<0.01 || this->wEye.z()<1000) return;
  this->pEye   = this->cam2geo(this->wEye);
  this->pLookAt   = this->cam2geo(this->wLookAt);
  this->updateWorldPositions();
}

void Camera3D::copyFromCamera3D(const Camera3D *camera)
{

    QVector3DD lookat = camera->cam2geo(camera->getWorldLookAt());
    QVector3DD eye = camera->cam2geo(camera->getWorldCenterOld());

//    this->setClippingRange(camera->getNearPlane(),camera->getFarPlane());
    this->setEye(eye);
    this->setLookAt(lookat);
    this->setUp(camera->pUp);
    this->setIsOrtho(camera->isOrtho);
    this->setOrtho(camera->wLeft, camera->wRight, camera->wBottom, camera->wTop, camera->getNearPlane(), camera->getFarPlane());

    this->verticalAngle = camera->verticalAngle;
    this->localViewport = QRectF(camera->localViewport);
    this->normalizedViewport = QRectF(camera->normalizedViewport);
    this->zoomFactor = camera->zoomFactor;
    this->zoomLevel = camera->zoomLevel;
    this->dataLevel = camera->dataLevel;
    this->translation = QPointF(camera->translation);

    this->screenViewport = QRectF(camera->screenViewport);
    this->totalSize = QSizeF(camera->totalSize);
    this->devicePixelRatio = camera->devicePixelRatio;
    this->nativeViewport = camera->nativeViewport;

//    this->resetVisibility();
    this->updateViewport();
}

void Camera3D::copyFromCamera2D(const Camera2D *cam2)
{
  double res = Camera::getGroundResolution(cam2->getCenter(), cam2->getZoomLevel());
  double wh  = res*cam2->getTotalSize().height();
  double radians = (this->getVerticalAngle()*0.5) * M_PI / 180.0;
  double elv = wh/2/tan(radians);
  this->setLookAt(QVector3DD(cam2->getCenter()));
  this->setEye(QVector3DD(cam2->getCenter(), elv));
  this->setUp(QVector3DD(0, 1, 0));
}

void Camera3D::copyToCamera2D(Camera2D *cam2) const
{
  cam2->setZoomLevel(this->getZoomLevel());
  cam2->setWorldCenter(this->getWorldLookAt().toPointF());
}

float Camera3D::getGroundFocus() const
{
  float d = this->wEye.z()-this->wLookAt.z();
  if (fabs(d)<1e-3)
    return 1.f;
  return this->wEye.z()/d;
}

float Camera3D::getDepthFocus(float depth) const
{
  float distance;
  if (depth+1e-6<1) {
    depth = 2*depth-1.0;
    distance = 2.0*this->wNear*this->wFar/(this->wFar+this->wNear-depth*(this->wFar-this->wNear))/this->wEyeLength;
  }
  else {
    distance = std::min(1.f, this->getGroundFocus());
  }
  return distance;
}

void Camera3D::loadFromFile(const QString &filename)
{
  cameraFile = filename;
  this->loadDefaults();
}

void Camera3D::loadDefaults()
{
  QFile fi(cameraFile);
  fi.open(QFile::ReadOnly | QIODevice::Text);
  QTextStream input(&fi);
  double x, y, z;

  input >> x >> y >> z;
  this->setEye(QVector3DD(x, y, z));

  input >> x >> y >> z;
  this->setLookAt(QVector3DD(x, y, z));

  input >> x >> y >> z;
  this->setUp(QVector3DD(x, y, z));
  fi.close();
}

void Camera3D::print() const
{
  fprintf(stderr, "%.6lf %.6lf %.6lf\n%.6lf %.6lf %.6lf\n%g %g %g\n",
          this->pEye.x(), this->pEye.y(), this->pEye.z(),
          this->pLookAt.x(), this->pLookAt.y(), this->pLookAt.z(),
          this->pUp.x(), this->pUp.y(), this->pUp.z());
}

//==============================================================================
// Camera 3D with a fixed horizontal plane
//==============================================================================
Camera3DGround::Camera3DGround(const QRectF &nvp, int dpr):
Camera3D(nvp, dpr)
{
}

void Camera3DGround::updateWorldPositions()
{
  bool vertical = (this->pEye.toPointF()-this->pLookAt.toPointF()).manhattanLength()<1e-3;
  if (vertical)
    this->pUp = QVector3DD(0, 1, 0);
  else
    this->pUp = QVector3DD(0, 0, -1);
  Camera3D::updateWorldPositions();
}

//==============================================================================
// Camera 3D with function to draw frustum
//==============================================================================
Camera3DRenderer::Camera3DRenderer(const QRectF &nvp, int dpr):
Camera3DGround(nvp, dpr)
{
    this->shaderInitialized  = false;
    this->initialized = false;
}


void Camera3DRenderer::initShader()
{
    this->shader = new QOpenGLShaderProgram();
    this->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/camera3D.vert");
    this->shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/camera3D.frag");
    this->shader->link();

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->colorbuffer);

    this->shaderInitialized = true;

}

 // If no parameter, use frustum of Camera3DRenderer itself
void Camera3DRenderer::setup(Camera3D* camera)
{

    if(!this->shaderInitialized)
        this->initShader();


    // If no parameter, use frustum of Camera3DRenderer itself
    if(camera == nullptr)
        camera = this;


    float points[] = {
        //near plane
        -1.0f,-1.0f, -1.0f,
        1.0f,-1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f,-1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        //far plane
        -1.0f,-1.0f,  1.0f,
        1.0f,-1.0f,  1.0f,
        1.0f, 1.0f,  1.0f,
        -1.0f,-1.0f,  1.0f,
        1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f,  1.0f,
        //top
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f,  1.0f,
        //bottom
        -1.0f,-1.0f, -1.0f,
        1.0f,-1.0f,  1.0f,
        -1.0f,-1.0f,  1.0f,
        -1.0f,-1.0f, -1.0f,
        1.0f,-1.0f, -1.0f,
        1.0f,-1.0f,  1.0f,
        //left
        -1.0f,-1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f,  1.0f,
        -1.0f,-1.0f,  1.0f,
        -1.0f,-1.0f, -1.0f,
        //right
        1.0f,-1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f,  1.0f,
        1.0f, 1.0f,  1.0f,
        1.0f,-1.0f,  1.0f,
        1.0f,-1.0f, -1.0f,
    };

    float colors[] = {
        //near plane
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        //far plane
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        //top
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        //bottom
        0,1,1,
        0,1,1,
        0,1,1,
        0,1,1,
        0,1,1,
        0,1,1,
        //left
        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,
        //right
        1,1,0,
        1,1,0,
        1,1,0,
        1,1,0,
        1,1,0,
        1,1,0
    };

    for(int i=0; i<36; i++) {
        QVector4D point = QVector4D(points[3*i], points[3*i+1], points[3*i+2], 1.0);
        QVector4D newpoint = camera->getModelViewProjectionMatrix().inverted() * point;
        newpoint = newpoint / newpoint.w();
        points[3*i]   = newpoint.x();
        points[3*i+1] = newpoint.y();
        points[3*i+2] = newpoint.z();
//        qDebug() << newpoint;
    }


    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->initialized = true;
}


// Receives as a parameter another Camera3D (the actual real camera used to render),
void Camera3DRenderer::render(Camera3D* camera)
{

    if(!this->initialized)
        return;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    this->shader->bind();
    glUniformMatrix4dv(shader->uniformLocation("mvp"),
                       1, GL_FALSE, camera->getModelViewProjectionMatrix().constData());
    //    glUniformMatrix4dv(shader->uniformLocation("invmvp"),
    //                       1, GL_FALSE, this->getModelViewProjectionMatrix().inverted().constData());

    //    qDebug() << mvp;
//        qDebug() << this->getModelViewProjectionMatrix();

    glBindVertexArray( this->vao );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawArrays(GL_TRIANGLES, 0, 36*3);
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    this->shader->release();


}

