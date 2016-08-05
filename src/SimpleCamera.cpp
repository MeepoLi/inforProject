#include "SimpleCamera.hpp"

#include <algorithm>

SimpleCamera::SimpleCamera(const QRectF &nvp, int dpr, QPointF p)
{
    this->setNormalizedViewport(nvp);
    this->devicePixelRatio = dpr;
    this->pos = p ;
	this->mousePos =QPointF(0.5,0.5);
    this->lookAt = p;
    this->zoomLevel = 1.0;
}
void SimpleCamera::setWorldCenter( const QPointF &center)
{
	this->worldCenter = center;

}

void SimpleCamera::updateMousePosition(const QPointF mousePosition)
{
	this->mousePos = this->screen2world(mousePosition);
	return;
}
void SimpleCamera::pan(const QPointF &anchor, const QPointF &screenDiff)
{
//   	qWarning()<<"from :"<< pos<<"    >>>>";
	double diff = this->zoomLevel;
    this->pos.setX(this->pos.x() - screenDiff.x() / (this->screenViewport.width() * this->zoomLevel));
    this->pos.setY(this->pos.y() + screenDiff.y() / (this->screenViewport.height() * this->zoomLevel));	
	this->mousePos =QPointF(0.5,0.5);
//	qWarning()<< pos<<"    <<<<    ";
	this->lookAt = pos;
    this->update();
	return;
}

void SimpleCamera::zoom(const QPointF &a, double diff)
{
    diff = -0.01*diff;
	double oldZoom = this->zoomLevel;
	this->zoomLevel = this->zoomLevel - diff;
	this->zoomLevel = this->zoomLevel>0.1? this->zoomLevel:0.1;
	if (oldZoom!=this->zoomLevel)
	{
		QPointF windowMid = QPointF(0.5,0.5);
		this->mousePos =  screen2world(a);
		QPointF screenMidTransfer = (oldZoom / this->zoomLevel) * ( windowMid - this->mousePos) + this->mousePos - windowMid;
	//	qWarning()<<"pos = "<< pos<<"  screenMidTransfer = " << screenMidTransfer;
   	 	this->pos = this->pos + screenMidTransfer/oldZoom;	
	    this->update();
	}

}

const QPointF SimpleCamera::screen2world(const QPointF &screen)
{
	
	return QPointF( screen.x()/this->screenViewport.width(), 1- screen.y()/this->screenViewport.height());
}

const QMatrix4x4D &SimpleCamera::getModelViewProjectionMatrix()
{
    return this->matProjection * this->matModelView;
}

void SimpleCamera::updateViewport()
{
    this->matProjection.setToIdentity();
    this->matProjection.ortho(this->normalizedViewport.left(), this->normalizedViewport.right(),
                        this->normalizedViewport.top(), this->normalizedViewport.bottom(),
                        0, 1);
//    this->matProj.frustum(this->normalizedViewport.left(), this->normalizedViewport.right(),
//                          this->normalizedViewport.top(), this->normalizedViewport.bottom(),
//                          0, 10);
    this->update();
}
void SimpleCamera::update()
{
    QVector2D direction = QVector2D(this->lookAt - this->pos);//.normalized();
    double dirFactor = this->zoomLevel;
//-----------glTransform
//
    this->matModelView.setToIdentity();
	double diff = 1 - this->zoomLevel;
	QPointF translate =-this-> pos + (QPointF(0.5,0.5)  + this->pos) * diff;
    
//	this->matModelView.translate(-this->pos.x() + (0.5 + this->pos.x()) * diff , -this->pos.y() + (0.5 + this->pos.y()) * diff,0);
    this->matModelView.translate(translate.x(), translate.y(), 0);
	this->matModelView.scale(this->zoomLevel);
	
    
 //   this->matModelView.translate(0.5,0.5);
//    this->matModelView.translate(0.5*direction.x()*dirFactor, -0.5*direction.y()*dirFactor);
//    this->matModelView.scale(1.0+this->zoomLevel);
//    this->matModelView.translate(this->anchor.x()*0.5, -this->anchor.y()*0.5);
//    this->matModelView.translate(-this->pos.x(), -this->pos.y(),0);
//    this->pos = this->zoomLevel * direction.toPointF();

}

void SimpleCamera::setNormalizedViewport(const QRectF &nvp)
{
    this->normalizedViewport = nvp;
}

void SimpleCamera::setViewport(int width, int height, int dpr)
{
    this->screenViewport = QRectF(0, 0, width, height);
    this->nativeViewport = QRectF(this->screenViewport.x()*this->devicePixelRatio,
                                 this->screenViewport.y()*this->devicePixelRatio,
                                 ceil(this->screenViewport.width()*this->devicePixelRatio),
                                 ceil(this->screenViewport.height()*this->devicePixelRatio));
    this->updateViewport();
}

const QRectF &SimpleCamera::getNativeViewport() const
{
    return this->nativeViewport;
}
//    this->pos = this->pos - 0.001*screenDiff;

