#include "OpenGLFuncs.hpp"

#include <QString>
#include <QDebug>

void createTexImage2D(int width, int height, GLenum iformat, GLenum format, GLenum type, GLuint *tex, void* data)
{
    GLenum err = glGetError();
/*    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "createTexImage2D error 1: " << strError;
    }
*/
    if((*tex) > 0)
        glDeleteTextures(1, tex);

    glGenTextures(1, tex);

    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexImage2D(GL_TEXTURE_2D, 0,iformat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    err = glGetError();
    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "createTexImage2D error 2: " << strError;
    }
}

void createTextureBuffer(int size, GLenum format, GLuint *tex, GLuint *bo, void* data)
{
    GLenum err;

    if( (*bo) > 0 )
        glDeleteBuffers( 1, bo );  //delete previously created tbo

    glGenBuffers( 1, bo );

    glBindBuffer( GL_TEXTURE_BUFFER, *bo );
    glBufferData( GL_TEXTURE_BUFFER, size, data, GL_STATIC_DRAW );

    err = glGetError();
    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "createTextureBuffer error 1: " << strError;
    }

    if( (*tex) > 0 )
        glDeleteTextures( 1, tex ); //delete previously created texture

    glGenTextures( 1, tex );
    glBindTexture( GL_TEXTURE_BUFFER, *tex );
    glTexBuffer( GL_TEXTURE_BUFFER, format,  *bo );
    glBindBuffer( GL_TEXTURE_BUFFER, 0 );

    err = glGetError();
    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "createTextureBuffer error 2: " << strError;
    }

}

void createUniformBuffer(int size, GLuint *bo, void* data)
{
    GLenum err;

    if( (*bo) > 0 )
        glDeleteBuffers( 1, bo );  //delete previously created tbo

    glGenBuffers( 1, bo );

    glBindBuffer( GL_UNIFORM_BUFFER, *bo );
    glBufferData( GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW );
    err = glGetError();

    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "createUniformBuffer error 1: " << strError;
    }

}
