#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QtGui/QRadialGradient>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <GL/gl.h>
#include <GL/glu.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void rotateBy(int xAngle, int yAngle, int zAngle);
    void makeObjectXY();
    void makeObjectYZ();
    void makeObjectXZ();

    GLubyte *checkImageXY;
    float zz;

    GLubyte *checkImageYZ;
    float xx;

    GLubyte *checkImageXZ;
    float yy;

    int _width,_height;


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:

    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    GLuint textures[3];
};

#endif
