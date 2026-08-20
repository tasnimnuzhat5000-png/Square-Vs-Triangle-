// NAFISHA TASNIM NUZAT ID-41230301557
//MIM AKTER DOLA ID-41230301539

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>

int windowWidth = 800;
int windowHeight = 600;

bool isGameOver = false;
int gameTimeSeconds = 30;
int gameOverDelay = 0;

float triX = 400.0f;
float triY = 300.0f;
float triSize = 20.0f;

float triColorR = 1.0f;
float triColorG = 0.5f;
float triColorB = 0.0f;

float sqX = 100.0f;
float sqY = 100.0f;
float sqSize = 40.0f;
float sqSpeed = 6.0f;

float sqColorR = 0.5f;
float sqColorG = 0.5f;
float sqColorB = 0.5f;

bool keys[256] = { false };
bool specialKeys[256] = { false };

void resetGame() {
    isGameOver = false;
    gameTimeSeconds = 30;
    gameOverDelay = 0;

    triX = 400.0f;
    triY = 300.0f;

    sqX = 100.0f;
    sqY = 100.0f;
}

void renderText(float x, float y, const std::string& text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawTriangle() {
    glColor3f(triColorR, triColorG, triColorB);
    glBegin(GL_TRIANGLES);
        glVertex2f(triX, triY + triSize);
        glVertex2f(triX - triSize, triY - triSize);
        glVertex2f(triX + triSize, triY - triSize);
    glEnd();
}

void drawSquare() {
    glColor3f(sqColorR, sqColorG, sqColorB);
    float half = sqSize / 2.0f;
    glBegin(GL_QUADS);
        glVertex2f(sqX - half, sqY - half);
        glVertex2f(sqX + half, sqY - half);
        glVertex2f(sqX + half, sqY + half);
        glVertex2f(sqX - half, sqY + half);
    glEnd();
}

bool checkCollision() {
    float halfSq = sqSize / 2.0f;

    bool collideX = (sqX + halfSq >= triX - triSize) && (triX + triSize >= sqX - halfSq);
    bool collideY = (sqY + halfSq >= triY - triSize) && (triY + triSize >= sqY - halfSq);

    return collideX && collideY;
}

void passiveMouseMotion(int x, int y) {
    if (isGameOver) return;

    triX = (float)x;
    triY = (float)(windowHeight - y);
}

void updateSquarePosition() {
    if (isGameOver) return;

    if (specialKeys[GLUT_KEY_UP] && sqY + sqSize/2 < windowHeight) {
        sqY += sqSpeed;
    }
    if (specialKeys[GLUT_KEY_DOWN] && sqY - sqSize/2 > 0) {
        sqY -= sqSpeed;
    }
    if (specialKeys[GLUT_KEY_LEFT] && sqX - sqSize/2 > 0) {
        sqX -= sqSpeed;
    }
    if (specialKeys[GLUT_KEY_RIGHT] && sqX + sqSize/2 < windowWidth) {
        sqX += sqSpeed;
    }
}

void specialKeyDown(int key, int x, int y) { specialKeys[key] = true; }
void specialKeyUp(int key, int x, int y)   { specialKeys[key] = false; }

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isGameOver) {
        drawTriangle();
        drawSquare();

        std::string timerStr = "Time Left: " + std::to_string(gameTimeSeconds) + "s";
        renderText(20.0f, windowHeight - 30.0f, timerStr, 1.0f, 1.0f, 1.0f);

        if (checkCollision()) {
            isGameOver = true;
        }
    } else {
        glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderText(windowWidth / 2.0f - 120.0f, windowHeight / 2.0f, "GAME OVER - CAUGHT!", 1.0f, 1.0f, 1.0f);
    }

    glutSwapBuffers();
}

void timer(int value) {
    if (!isGameOver) {
        updateSquarePosition();
    } else {
        gameOverDelay++;
        if (gameOverDelay > 120) {
            resetGame();
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void countdownClock(int value) {
    if (!isGameOver) {
        gameTimeSeconds--;

        if (gameTimeSeconds <= 0) {
            resetGame();
        }
    }

    glutTimerFunc(1000, countdownClock, 0);
}

void initOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Catch the Triangle - GLUT 2D Game");

    initOpenGL();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMouseMotion);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);

    glutTimerFunc(0, timer, 0);
    glutTimerFunc(1000, countdownClock, 0);

    resetGame();
    glutMainLoop();
    return 0;
}
