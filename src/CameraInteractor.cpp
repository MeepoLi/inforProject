#include <math.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include "CameraInteractor.hpp"
//#include "Camera.hpp"
#include "NcQuickView.hpp"

//==============================================================================
// CameraInteractor
//==============================================================================
CameraInteractor::CameraInteractor(NcMapView *mv, SimpleCamera *cam) :
    view(mv), baseCamera(cam), action(-1)
{
	this->rectangle_flag = false;
	this->rectangle_idx = 0;

}

CameraInteractor::~CameraInteractor()
{

}

void CameraInteractor::setCamera(SimpleCamera *cam)
{
    this->baseCamera = cam;
}

void CameraInteractor::setRectangle(std::vector<float> *rectangle[3])
{
	for (int i = 0; i < 3; i++)
		this->rect[i] = rectangle[i];	
}

void CameraInteractor::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Shift:
			this->rectangle_flag = false;
//			qDebug()<<"KeyRealse";
			break;
	}

}
void CameraInteractor::keyPressEvent(QKeyEvent *event)
{
    // double dist = exp2((double)(8-this->camera->getZoomLevel()));
    // double dlat = 0, dlng=0;
    // event->accept();
     switch ( event->key() ) {
    // case Qt::Key_Left:
    // case Qt::Key_Right:
    // case Qt::Key_Up:
		case Qt::Key_1:
				this->rectangle_idx = 0;
				break;
		case Qt::Key_2:
				this->rectangle_idx = 1;
				break;
		case Qt::Key_3:
				this->rectangle_idx = 2;
				break;
		case Qt::Key_Shift:{	
				this->rectangle_flag = true;
				QPointF pos = this->baseCamera->getworldCoordinate(this->lastPos);
				(*this->rect[this->rectangle_idx])[0] = (*this->rect[this->rectangle_idx])[2] = pos.x();
				(*this->rect[this->rectangle_idx])[1] = (*this->rect[this->rectangle_idx])[3] = pos.y();
		//		qDebug()<<"set Cood";
				break;
				}
		
	 case Qt::Key_S:	
			break;
	//   if (key==Qt::Key_Down) dlat = -dist;

    //   this->camera->setCenter(this->camera->getCenter() + QPointF(dlat, dlng));
    //   this->view->updateView();
    //   break;

    // case Qt::Key_Minus:
    // case Qt::Key_Equal:
    // case Qt::Key_Plus:
    //   if (key==Qt::Key_Minus)
    //     this->camera->setZoomLevel(ceil(this->camera->getZoomLevel())-1);
    //   else
    //     this->camera->setZoomLevel(floor(this->camera->getZoomLevel())+1);
    //   this->view->updateView();
    //   break;

     default:
       break;
     }
}

void CameraInteractor::mousePressEvent(QMouseEvent *event)
{
    Qt::KeyboardModifiers mods = event->modifiers();
    this->screenPos = event->pos();
    switch (event->button()) {
    case Qt::LeftButton:
        if (mods & Qt::AltModifier) {
            this->action = 3;
//            this->focusDistance = this->view->getFocusDistance();
        }
        else if (mods & Qt::ShiftModifier) {
            this->action = 4;
        }
        else
        {
			this->action = 0;
       // 	this->baseCamera->updateMousePosition(event->pos());
		}
	//qWarning() << event->pos().x();
		//qWarning() << event->pos().y();


        break;
    case Qt::RightButton:
        this->action = 1;
		this->mouseClickPos = event->pos();
		break;
    case Qt::MiddleButton:
        this->action = 2;
        break;
    default:
        this->action = -1;
        break;
    }
    this->lastPos = event->pos();
}

void CameraInteractor::mouseReleaseEvent(QMouseEvent *event)
{
    this->action = -1;
    this->view->updateView();
}

void CameraInteractor::mouseMoveEvent(QMouseEvent *event)
{
    QPointF diff = (event->pos()-this->lastPos);
	
	this->lastPos = event->pos();
	if (this->rectangle_flag)
	{
		QPointF pos = this->baseCamera->getworldCoordinate(event->pos());
		(*this->rect[this->rectangle_idx])[2] = pos.x();
		(*this->rect[this->rectangle_idx])[3] = pos.y();
//		qDebug()<<"setting idx = "<<this->rectangle_idx;
	}
//    qDebug()<< "mouse moving == "<<event->pos();
    // Pan
    if (this->action==0) {
       this->baseCamera->pan(this->lastPos, diff);
//	qWarning() << event->pos();
//		qWarning() << this->lastPos;
	    //this->lastPos = event->pos();
	}

    // Zoom
    else if (this->action==1) {
   		 qDebug() << "zoomMId = " << this->mouseClickPos;
		 this->baseCamera->zoom(this->mouseClickPos, diff.y() * 2.0);  
   }

    // Middle button, unused
    else if (this->action==2) {
    }

    // Rotating around the screen pivot
    else if (this->action==3) {
//        this->baseCamera->reversedYaw(this->focusDistance, diff.x());
//        this->baseCamera->reversedPitch(this->focusDistance, diff.y());
    }

    // Rotating around the camera's eye
    else if (this->action==4) {
//        this->baseCamera->yaw(this->screenPos, diff.x());
//        this->baseCamera->pitch(this->screenPos, diff.y());
    } 
    //this->view->updateView();
}

void CameraInteractor::mouseDoubleClickEvent(QMouseEvent *event)
{
//    this->view->setSelectedPosToUnder(event->pos());
    this->view->updateView();
}


bool CameraInteractor::isInteracting() const
{
    return this->action!=-1;
}
