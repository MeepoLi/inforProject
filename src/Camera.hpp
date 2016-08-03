#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QRectF>
#include <QLineF>
#include <QPolygonF>
#include <QOpenGLShaderProgram>
#include <cmath>
#include "QMatrix4x4D.hpp"

#define WORLD_ZOOM_LEVEL 22.f

#define CAMERA_MATRIX(Name)                       \
public:                                           \
const QMatrix4x4D & get##Name##Matrix() const {  \
return this->mat##Name;                        \
}                                                \
protected:                                        \
QMatrix4x4D mat##Name;

//==============================================================================
// Camera
//==============================================================================
class Camera
{
public:
  Camera(const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
  virtual ~Camera();
  
  void  setViewport(int width, int height, int dpr, float zoomFactor=1.0);
  void  setNormalizedViewport(const QRectF & viewport);
  void  setWorldViewport(const QRectF &wvp);
  
public:
  const QRect  & getNativeViewport() const;
  const QRectF & getScreenViewport() const;
  const QRectF & getLocalViewport() const;
  const QRectF & getNormalizedViewport() const;
  const QRectF & getWorldViewport() const;
  const QSizeF & getTotalSize() const;
  
  float   getZoomFactor() const;
  int     getDevicePixelRatio() const;
  float   getZoomLevel() const;
  float   getPixelScaling() const;
  bool    isVisible(const QPolygonF &p) const;
  
  QPointF mapToLocal(const QPointF &p) const;
  
public:
  virtual bool isInFrustum(const QVector3DD &p) const { return true; }
  bool isInFrustum(const QPolygonF &poly) const;
  
public:
  struct DataView {
    QRectF  ground;
    QPointF height;
  };
  const DataView * getDataView() const;
  
public:
  static QPointF   geo2world(const QPointF &);
  static QPointF   world2geo(const QPointF &);
  static QPointF   geo2level(const QPointF &, float level);
  static double    getGroundResolution(const QPointF &, float level);
  static float     getLevelFromResolution(const QPointF &geo, double res);
  
public:
  virtual void    updateViewport() = 0;
  virtual void    glTransform(float dataLevel=WORLD_ZOOM_LEVEL, const QPointF &translation=QPointF(0,0), const QPointF &scale=QPointF(1,1)) = 0;
  virtual double  getGroundResolution() const = 0;
  virtual void    glTransformOverlay(bool useDepth) = 0;
  virtual QString info() const { return QString(); }
  virtual void    print() const {}
  
public:
  virtual void pan(const QPointF &anchor, const QPointF &screenDiff) {};
  virtual void zoom(const QPointF &anchor, double diff) {};
  
  virtual void roll(const QPointF &anchor, double diff) {};
  virtual void pitch(const QPointF &anchor, double diff) {};
  virtual void yaw(const QPointF &anchor, double diff) {};
  
  virtual void reversedRoll(float focus, double diff) {};
  virtual void reversedPitch(float focus, double diff) {};
  virtual void reversedYaw(float focus, double diff) {};
  
public:
  CAMERA_MATRIX(Projection);
  CAMERA_MATRIX(ModelView);
  CAMERA_MATRIX(ModelViewProjection);
  CAMERA_MATRIX(MapToScreen);
  CAMERA_MATRIX(ScreenToMap);
  CAMERA_MATRIX(Normal);
  
public:
  QVector3DD world2viewport(const QVector3DD &p) const;
  QPointF    screen2world(const QPointF &) const;
  QVector3DD screen2world(const QPointF &, double) const;
  
protected:
  QSizeF    totalSize;
  QRectF    normalizedViewport;
  QRectF    localViewport;
  QRectF    screenViewport;
  QRect     nativeViewport;
  int       devicePixelRatio;
  float     zoomFactor;
  float     pixelScaling;

  
protected:
  QRectF    worldViewport;
  QLineF    worldBounds[4];
  float     horizonCulling;
  DataView  dataView;
  float     zoomLevel;
  
public:
  enum VisibilityTest {
    VISIBILITY_FULL, // all visibility tests to be redone
    VISIBILITY_MORE, // hidden objects may become visible
    VISIBILITY_SAME, // only perform tests on new objects
    VISIBILITY_LESS, // visible objects may become hidden
    VISIBILITY_NONE = -1
  };
  
  void             resetVisibility();
  VisibilityTest   getRenderingVisibility() const;
  float     dataLevel;
  QPointF   translation;
  
protected:
  VisibilityTest   visRendering;
  VisibilityTest   visCurrent;
  
  void             updateCurrentVisibility(VisibilityTest vis);
};

//==============================================================================
// Camera 2D
//==============================================================================
class Camera2D : public Camera
{
public:
  Camera2D(const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
  Camera2D(const QPointF &center, double level, const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
  virtual ~Camera2D();
  
  void            setZoomLevel(float level);
  void            setCenter(const QPointF &center);
  void            setWorldCenter(const QPointF &p);
  const QPointF & getWorldCenter() const;
  const QPointF & getCenter() const;
  
  QPointF         zoom2world(const QPointF &) const;
  QPointF         world2zoom(const QPointF &) const;
  QPointF         world2screen(const QPointF &) const;
  
  virtual QString info() const;
  virtual double  getGroundResolution() const;
  virtual void    updateViewport();
  virtual void    glTransform(float dataLevel=WORLD_ZOOM_LEVEL, const QPointF &translation=QPointF(0,0));
  virtual void    glTransformOverlay(bool useDepth);
  
public:
  void pan(const QPointF &anchor, const QPointF &screenDiff);
  void zoom(const QPointF &anchor, double diff);
  
protected:
  QPointF         geoCenter;
  QPointF         worldCenter;
};

//==============================================================================
// Camera 3D
//==============================================================================
class Camera3D : public Camera
{
public:
  Camera3D(const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
  
  void setParameters(float verticalAngle, float nearPlane, float farPlane);
  void setClippingRange(float nearPlane, float farPlane);
  void setVerticalAngle(float verticalAngle);
  void setEye(const QVector3DD &eye);
  void setLookAt(const QVector3DD &look);
  void setUp(const QVector3DD &up);
  void setOrtho(double left, double right, double bottom, double top, double nearPlane, double farPlane);
  void setIsOrtho(bool isOrtho);
  void setPositions(const QVector3DD &eye, const QVector3DD &look, const QVector3DD &up);

  const QVector3DD & headLight() const;
  QVector3DD headLightTransformed() const;
  
  float      getVerticalAngle() const;
  QVector3DD getWorldLookAt() const;
  QVector3DD getWorldCenter() const;
  QVector3DD getWorldCenterOld() const;
  QVector3DD getWorldUp() const;
  float      getElevation() const;
  float      getNearPlane() const;
  float      getFarPlane() const;
  
  QVector3DD cam2geo(const QVector3DD &world) const;
  QVector3DD geo2cam(const QVector3DD &lat_lon_z) const;
  
public:
  float      getGroundFocus() const;
  float      getDepthFocus(float depth) const;
  
public:
  virtual QString info() const;
  virtual void    print() const;
  virtual double  getGroundResolution() const;
  virtual void    updateViewport();
  virtual void    glTransform(float dataLevel=WORLD_ZOOM_LEVEL, const QPointF &translation=QPointF(0,0), const QPointF &scale=QPointF(1,1));
  virtual void    glTransformOverlay(bool useDepth);
  
public:
  void pan(const QPointF &anchor, const QPointF &screenDiff);
  void zoom(const QPointF &anchor, double diff);
  void pitch(const QPointF &anchor, double diff);
  void yaw(const QPointF &anchor, double diff);
  void reversedPitch(float focus, double diff);
  void reversedYaw(float focus, double diff);
  
public:
  bool isInFrustum(const QVector3DD &p) const;
  bool isBehindCamera(const QVector3DD &p) const;
  bool getIsOrtho() const;
  
public:
  void copyFromCamera3D(const Camera3D *cam3);
  void copyFromCamera2D(const Camera2D *cam2);
  void copyToCamera2D(Camera2D *cam2) const;
  void loadFromFile(const QString &filename);
  void loadDefaults();
  
protected:
  virtual void updateWorldPositions();
  QVector3DD wEye;
  QVector3DD wLookAt;
  QVector3DD wUp;
  QVector3DD wHeadLight;
  QVector3DD wHeadLightTransformed;
  double     groundRes;
  QVector3DD wEyeDir;
  double     wEyeLength;
  double     wNear;
  double     wFar;
  double     wLeft;
  double     wRight;
  double     wTop;
  double     wBottom;

public:
  bool       isOrtho;

  CAMERA_MATRIX(Overlay);
  
protected:
  QVector3DD pEye;
  QVector3DD pLookAt;
  QVector3DD pUp;
  float      verticalAngle;
  float      nearPlane;
  float      farPlane;


public:
  QString cameraFile;
};


//==============================================================================
// Camera 3D with a fixed ground interaction
//==============================================================================

class Camera3DGround : public Camera3D
{
public:
  Camera3DGround(const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
  virtual void updateWorldPositions();
};

//==============================================================================
// Camera 3D with function to render a camera frustum
//==============================================================================

class Camera3DRenderer : public Camera3DGround
{
public:
    Camera3DRenderer(const QRectF &normalizedViewport=QRectF(0,0,1,1), int dpr=1);
    void render(Camera3D* camera);
    void setup(Camera3D* camera = nullptr);  // If no parameter, use frustum of Camera3DRenderer itself
    
    bool initialized;

protected:
    bool shaderInitialized;
    void initShader();
    
    QOpenGLShaderProgram *shader;
    GLuint                vao;
    GLuint                vbo;
    GLuint                colorbuffer;

};


//==============================================================================
// Inline functions
//==============================================================================


inline const Camera::DataView * Camera::getDataView() const
{
  return &this->dataView;
}

inline Camera::VisibilityTest Camera::getRenderingVisibility() const
{
  return this->visRendering;
}

inline const QRect & Camera::getNativeViewport() const
{
  return this->nativeViewport;
}

inline const QRectF & Camera::getScreenViewport() const
{
  return this->screenViewport;
}

inline const QRectF & Camera::getLocalViewport() const
{
  return this->localViewport;
}

inline const QRectF & Camera::getNormalizedViewport() const
{
  return this->normalizedViewport;
}

inline const QRectF & Camera::getWorldViewport() const
{
  return this->worldViewport;
}

inline const QSizeF & Camera::getTotalSize() const
{
  return this->totalSize;
}

inline float Camera::getZoomFactor() const
{
  return this->zoomFactor;
}

inline int Camera::getDevicePixelRatio() const
{
  return this->devicePixelRatio;
}

inline float Camera::getZoomLevel() const
{
  return this->zoomLevel;
}

inline float Camera::getPixelScaling() const
{
  return this->pixelScaling;
}

inline bool Camera::isInFrustum(const QPolygonF &poly) const
{
  return (this->isInFrustum(QVector3DD(poly.front())) &&
          this->isInFrustum(QVector3DD(poly.back())));
}

inline bool Camera::isVisible(const QPolygonF &p) const {
  for (int k=1; k<p.count(); k++) {
    {
      bool inside = false;
      const QPointF & pk = p.at(k);
      for (int i=0, j=3; i<4; j=i++) {
        const QPointF & wj = this->worldBounds[j].p1();
        const QPointF & wi = this->worldBounds[i].p1();
        if (((wi.y()>pk.y()) != (wj.y()>pk.y())) &&
            (pk.x() < (wj.x()-wi.x())*(pk.y()-wi.y())/(wj.y()-wi.y()) + wi.x()))
          inside = !inside;
      }
      if (inside) return true;
    }
    // if (this->worldViewport.contains(p.at(k)))
    //   return true;
    
    QLineF segment(p.at(k-1), p.at(k));
    for (int j=0; j<4; j++)
      if (this->worldBounds[j].intersect(segment, 0)==QLineF::BoundedIntersection)
        return true;
  }
  return false;
}

inline QPointF Camera::geo2world(const QPointF &geo)
{
  double y;
  if (geo.x()==90.0)
    y = 256;
  else if (geo.x()==-90.0)
    y = 0;
  else
    y = (M_PI-atanh(sin(geo.x()*M_PI/180)))/M_PI*128;
  return QPointF((geo.y()+180)/360.0*256, y)*exp2(WORLD_ZOOM_LEVEL);
}

inline QPointF Camera::geo2level(const QPointF &geo, float level)
{
  double y;
  if (geo.x()==90.0)
    y = 256;
  else if (geo.x()==-90.0)
    y = 0;
  else
    y = (M_PI-atanh(sin(geo.x()*M_PI/180)))/M_PI*128;
  return QPointF((geo.y()+180)/360.0*256, y)*exp2(level);
}

inline QPointF Camera::world2geo(const QPointF &world)
{
  double s = exp2(WORLD_ZOOM_LEVEL);
  return QPointF(atan(sinh(M_PI*(1-world.y()/s/128)))*180/M_PI, world.x()*360/s/256-180);
}

inline double Camera::getGroundResolution(const QPointF &geo, float level)
{
  return cos(geo.x() * M_PI/180) * 6378137 * 2 * M_PI / exp2(8+level);
}

inline float Camera::getLevelFromResolution(const QPointF &geo, double res)
{
  return log2(cos(geo.x() * M_PI/180) * 6378137 * 2 * M_PI / res) - 8;
}

inline QVector3DD Camera::world2viewport(const QVector3DD &p) const
{
  return this->matModelViewProjection*p;
}

inline QPointF Camera::screen2world(const QPointF &screen) const
{
  return this->matScreenToMap*(screen/this->zoomFactor);
}

inline QVector3DD Camera::screen2world(const QPointF &screen, double depth) const
{
  return this->matScreenToMap*(QVector3DD(screen/this->zoomFactor, depth));
}

inline QPointF Camera2D::zoom2world(const QPointF &zoom) const
{
  double s = exp2(WORLD_ZOOM_LEVEL-this->zoomLevel);
  return zoom*s + this->worldCenter;
}

inline QPointF Camera2D::world2zoom(const QPointF &world) const
{
  double s = exp2(this->zoomLevel-WORLD_ZOOM_LEVEL);
  return (world-this->worldCenter)*s;
}

inline QPointF Camera2D::world2screen(const QPointF &world) const
{
  double s = exp2(this->zoomLevel-WORLD_ZOOM_LEVEL);
  return (world-this->worldViewport.topLeft())*s*this->zoomFactor;
}

inline const QPointF & Camera2D::getWorldCenter() const
{
  return this->worldCenter;
}

inline const QPointF & Camera2D::getCenter() const
{
  return this->geoCenter;
}

inline QVector3DD Camera3D::geo2cam(const QVector3DD &lat_lon_z) const
{
  return QVector3DD(Camera::geo2world(lat_lon_z.toPointF()), lat_lon_z.z()/this->groundRes);
}

inline QVector3DD Camera3D::cam2geo(const QVector3DD &world) const
{
  return QVector3DD(Camera::world2geo(world.toPointF()), world.z()*this->groundRes);
}

inline const QVector3DD & Camera3D::headLight() const
{
  return this->wHeadLight;
}

inline QVector3DD Camera3D::headLightTransformed() const
{
  return this->wHeadLightTransformed;
}

inline float Camera3D::getVerticalAngle() const
{
  return this->verticalAngle;
}

inline QVector3DD Camera3D::getWorldLookAt() const
{
  return this->wLookAt;
}

inline QVector3DD Camera3D::getWorldUp() const
{
  return this->wUp;
}

inline QVector3DD Camera3D::getWorldCenter() const
{
  return this->getWorldLookAt();
}

inline QVector3DD Camera3D::getWorldCenterOld() const
{
  return this->wEye;
}

inline float Camera3D::getElevation() const
{
  return this->wEye.z()*this->groundRes;
}

inline float Camera3D::getNearPlane() const
{
  return this->nearPlane;
}

inline float Camera3D::getFarPlane() const
{
  return this->farPlane;
}

inline bool Camera3D::isBehindCamera(const QVector3DD &p) const
{
  return QVector3DD::dotProduct(this->wEyeDir, p)<0;
}
#endif
