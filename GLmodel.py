from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import time
import sys
global ang
ang = [0, 0, 0]


def drawGround(size, color, M, N):     # Отрисовка земли
    glPushMatrix()
    i = 0
    #N = 10
    j = 0
    #M = 10
    glColor3f(color[0], color[1], color[2])
    while i < N:
        while j < M:
            glBegin(GL_QUADS)
            glVertex3f(-size, 0.0, -size)
            glVertex3f(-size, 0.0,  size)
            glVertex3f(size, 0.0,  size)
            glVertex3f(size, 0.0, -size)
            glEnd()
            glTranslate(0.0, 0.0, -(2*size+1))
            j = j+1
        glTranslate((2*size+1), 0.0, (2*size+1)*N)
        j = 0
        i = i+1
    glPopMatrix()


def drawStick(size, color):
    glPushMatrix()
    glColor3f(color[0], color[1], color[2])
    glBegin(GL_QUADS)
    glVertex3f(size, -size, -size)
    glVertex3f(size, size, -size)
    glVertex3f(-size, size, -size)
    glVertex3f(-size, -size, -size)
    glEnd()

    glColor3f(color[0] - 0.05, color[1] - 0.05, color[2] - 0.05)
    glBegin(GL_QUADS)
    glVertex3f(size, -size, size)
    glVertex3f(size, size, size)
    glVertex3f(-size, size, size)
    glVertex3f(-size, -size, size)
    glEnd()

    glColor3f(color[0] + 0.05, color[1] + 0.05, color[2] + 0.05)
    glBegin(GL_QUADS)
    glVertex3f(size, -size, -size)
    glVertex3f(size, size, -size)
    glVertex3f(size, size, size)
    glVertex3f(size, -size, size)
    glEnd()

    glColor3f(color[0] - 0.1, color[1] - 0.1, color[2] - 0.1)
    glBegin(GL_QUADS)
    glVertex3f(-size, -size, size)
    glVertex3f(-size, size, size)
    glVertex3f(-size, size, -size)
    glVertex3f(-size, -size, -size)
    glEnd()

    glColor3f(color[0] + 0.1, color[1] + 0.1, color[2] + 0.1)
    glBegin(GL_QUADS)
    glVertex3f(size, size, size)
    glVertex3f(size, size, -size)
    glVertex3f(-size, size, -size)
    glVertex3f(-size, size, size)
    glEnd()

    glBegin(GL_QUADS)
    glVertex3f(size, -size, size)
    glVertex3f(size, -size, -size)
    glVertex3f(-size, -size, -size)
    glVertex3f(-size, -size, size)
    glEnd()

    glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, 2 * size, 0.0)
    glEnd()

    glColor3f(0.0, 1.0, 0.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(2 * size, 0.0, 0.0)
    glEnd()

    glColor3f(0.0, 0.0, 1.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, 0.0, 2 * size)
    glEnd()

    glTranslate(size / 2, size / 4, size)
    glColor3f(0.2, 0.2, 0.2)
    glutSolidCylinder(0.3, 0.15, 20, 20)

    glTranslate(-size, 0.0, 0.0)
    glutSolidCylinder(0.3, 0.15, 20, 20)
    glPopMatrix()


def drawGlobalCoord(size):
    glPushMatrix()
    glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, size, 0.0)
    glEnd()

    glColor3f(0.0, 1.0, 0.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(size, 0.0, 0.0)
    glEnd()

    glColor3f(0.0, 0.0, 1.0)
    glBegin(GL_LINES)
    glVertex3f(0.0, 0.0, 0.0)
    glVertex3f(0.0, 0.0,  size)
    glEnd()
    glPopMatrix()


def renderScene():
    global ang
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60.0, 1.0, 1.0, 200.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    # glTranslate(5*5.0, -1.0, 0.0)
    glPushMatrix()
    glTranslate(1.5, 1.5, -3.0)
    glRotatef(-ang[0], 0.0, 1.0, 0.0)
    glRotatef(-ang[1], 1.0, 0.0, 0.0)
    glRotatef(ang[2], 0.0, 0.0, 1.0)
    drawGlobalCoord(0.2)
    glPopMatrix()

    glPushMatrix()
    gluLookAt(50.0, 10, -50.0, 50, 10, -60, 0, 1, 0)
    drawGround(5.0, [0.5, 0.5, 0.5])
    glTranslate(50.0, 10.0, -60.0)
    glRotatef(-ang[0], 0.0, 1.0, 0.0)
    glRotatef(-ang[1], 1.0, 0.0, 0.0)
    glRotatef(ang[2], 0.0, 0.0, 1.0)
    drawStick(1.0, [0.5, 0.5, 1.0])
    glPopMatrix()

    glutSwapBuffers()


def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA)
    glutInitWindowPosition(100, 100)
    glutInitWindowSize(900, 900)
    glutCreateWindow("OpenGL")
    while True:
        renderScene()
        time.sleep(0.05)
    #glutMainLoop()


def read_handler(angle):
    global ang
    ang = angle


def start_handler():
    print("\nI started\n")


def stop_handler():
    pass


main()