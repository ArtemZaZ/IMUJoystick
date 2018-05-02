from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import time
import sys
import interface
import threading
global ang
ang = [0, 0, 0]
buttons = [0, 0, 0]
EXIT = False


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


def drawButtons():
    global buttons
    size = 0.2
    glPushMatrix()
    glTranslatef(-0.4, 0.0, 0.0)
    if buttons[0]:
        glColor3f(0.0, 1.0, 0.0)
    else:
        glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glVertex2f(0.0, 0.0)
    glVertex2f(size, 0.0)
    glVertex2f(size, size)
    glVertex2f(0.0, size)
    glEnd()

    glTranslatef(0.3, 0.0, 0.0)
    if buttons[1]:
        glColor3f(0.0, 1.0, 0.0)
    else:
        glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glVertex2f(0.0, 0.0)
    glVertex2f(size, 0.0)
    glVertex2f(size, size)
    glVertex2f(0.0, size)
    glEnd()

    glTranslatef(0.3, 0.0, 0.0)
    if buttons[2]:
        glColor3f(0.0, 1.0, 0.0)
    else:
        glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glVertex2f(0.0, 0.0)
    glVertex2f(size, 0.0)
    glVertex2f(size, size)
    glVertex2f(0.0, size)
    glEnd()

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
    glTranslatef(0.0, -0.9, -2.0)
    drawButtons()
    glPopMatrix()

    glPushMatrix()
    glTranslate(1.3, 0.7, -3.0)
    glRotatef(-ang[0], 0.0, 1.0, 0.0)
    glRotatef(-ang[1], 1.0, 0.0, 0.0)
    glRotatef(ang[2], 0.0, 0.0, 1.0)
    drawGlobalCoord(0.2)
    glPopMatrix()

    glPushMatrix()
    gluLookAt(50.0, 10, -50.0, 50, 10, -60, 0, 1, 0)
    drawGround(5.0, [0.5, 0.5, 0.5], 20, 20)
    glTranslate(50.0, 10.0, -60.0)
    glRotatef(-ang[0], 0.0, 1.0, 0.0)
    glRotatef(-ang[1], 1.0, 0.0, 0.0)
    glRotatef(ang[2], 0.0, 0.0, 1.0)
    drawStick(1.0, [0.5, 0.5, 1.0])
    glPopMatrix()

    glutSwapBuffers()


def main():
    global EXIT
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA)
    glutInitWindowPosition(100, 100)
    glutInitWindowSize(900, 900)
    glutCreateWindow("OpenGL")
    while not EXIT:
        renderScene()
        time.sleep(0.05)


def PRHandler(data):
    global ang
    if data[0] == "PRD":
        print(data[1])
        ang[1] = data[1]
        #ang[2] = data[2]


def startHandler(data):
    print("I started\n")


def stopHandler(data):
    print("I stopped\n")


def errorHandler(data):
    print("It's a trap\n")


def buttonHandler(data):
    global buttons
    if data[0] == "BUT":
        print(data)
        buttons[data[1]-1] = data[2]


t = threading.Thread(target=main)
t.start()
p = interface.Interface()
p.ISP.connectFun("START", startHandler)
p.ISP.connectFun("STOP", stopHandler)
p.ISP.connectFun("ERROR", errorHandler)
p.ISP.connectFun("PRD", PRHandler)
p.ISP.connectFun("BUT", buttonHandler)
p.start()

EXIT = True
