#include <QtGui/QWidget>
#include <QtGui/QColorDialog>
#include <QtOpenGL/QGLFormat>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>


#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    _width=0;
    _height=0;

    zz=0.0f;
    checkImageXY=0;

    yy=0.0f;
    checkImageXZ=0;

    xx=0.0f;
    checkImageYZ=0;

}

GLWidget::~GLWidget(){}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}


void GLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

}

void GLWidget::paintGL()
{
    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -2.5f);
	glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_LINES);
    // draw line for x axis
    //glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    // draw line for y axis
    //glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    // draw line for Z axis
    //glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    if (checkImageXY!=0){
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, 1.0f, zz);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, zz);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, zz);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, zz);
		glEnd();
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, zz);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, zz);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, zz);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.0f, 1.0f, zz);
		glEnd();
    }
    if (checkImageYZ!=0){
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(xx, 0.0f, 1.0f);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(xx, 0.0f, 0.0f);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(xx, 1.0f, 0.0f);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(xx, 1.0f, 1.0f);
		glEnd();
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(xx, 1.0f, 0.0f);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(xx, 1.0f, 1.0f);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(xx, 0.0f, 1.0f);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(xx, 0.0f, 0.0f);
		glEnd();
    }
    if (checkImageXZ!=0){
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, yy, 1.0f);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, yy, 0.0f);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, yy, 0.0f);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, yy, 1.0f);
		glEnd();
		glBegin(GL_QUADS);
			// top left vertex
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, yy, 1.0f);
			// bottom left vertex
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, yy, 0.0f);
			// bottom right vertex
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.0f, yy, 0.0f);
			// top right vertex
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.0f, yy, 1.0f);
		glEnd();
	}

}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes

	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::makeObjectYZ()
{
	textures[1] = bindTexture(QPixmap::fromImage(QImage(checkImageYZ,_width,_height,QImage::Format_RGB888)),GL_TEXTURE_2D);
}

void GLWidget::makeObjectXZ()
{
	textures[2] = bindTexture(QPixmap::fromImage(QImage(checkImageXZ,_width,_height,QImage::Format_RGB888)),GL_TEXTURE_2D);
}

void GLWidget::makeObjectXY()
{
	textures[0] = bindTexture(QPixmap::fromImage(QImage(checkImageXY,_width,_height,QImage::Format_RGB888)),GL_TEXTURE_2D);
}

