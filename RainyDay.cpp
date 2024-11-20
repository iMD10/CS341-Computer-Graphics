#include <GL/glut.h>
#include <cstdlib>
#include <vector>

struct Particle {
    float x, y;
    float speed;
    float alpha;
};

std::vector<Particle> particles;
float rainX = 100.0f;
bool isRaining = false;
float waterHeight = 2.0f;
float wind = 0.5f;
float cloudOffset = 0.0f;
float boatX = 60.0f;
float boatOscillation = 0.0f;

void addParticles(int count) {
    for (int i = 0; i < count; ++i) {
        particles.push_back({
            rainX + static_cast<float>(rand() % 50 - 25),
            200.0f,
            static_cast<float>((rand() % 5) + 1),
            0.5f + static_cast<float>(rand() % 50) / 100.0f
            });
    }
}

void drawMountain(float baseX, float baseY, float width, float height) {
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX, baseY);
    glVertex2f(baseX + width, baseY);
    glVertex2f(baseX + width / 2, baseY + height);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX + width * 0.4f, baseY + height * 0.8f);
    glVertex2f(baseX + width * 0.6f, baseY + height * 0.8f);
    glVertex2f(baseX + width / 2, baseY + height);
    glEnd();
}

void drawHouse() {
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(70, 20);
    glVertex2f(130, 20);
    glVertex2f(130, 70);
    glVertex2f(70, 70);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(65, 70);
    glVertex2f(135, 70);
    glVertex2f(100, 100);
    glEnd();

    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(90, 20);
    glVertex2f(110, 20);
    glVertex2f(110, 50);
    glVertex2f(90, 50);
    glEnd();
}

void drawCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; ++i) {
        glBegin(GL_POLYGON);
        for (float angle = 0; angle < 2 * 3.14159; angle += 0.1f) {
            glVertex2f(x + cloudOffset + i * 10 + 10 * cos(angle), y + 10 * sin(angle));
        }
        glEnd();
    }
}

void drawBoat(float boatX, float boatY) {
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_POLYGON);
    glVertex2f(boatX - 10, boatY);
    glVertex2f(boatX + 10, boatY);
    glVertex2f(boatX + 7, boatY + 5);
    glVertex2f(boatX - 7, boatY + 5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(boatX, boatY + 5);
    glVertex2f(boatX, boatY + 15);
    glVertex2f(boatX + 5, boatY + 5);
    glEnd();
}

void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 2 * 3.14159; angle += 0.1f) {
        glVertex2f(180 + 15 * cos(angle), 170 + 15 * sin(angle));
    }
    glEnd();
}

void drawTree(float baseX, float baseY) {
    glColor3f(0.6f, 0.3f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(baseX - 5, baseY);
    glVertex2f(baseX + 5, baseY);
    glVertex2f(baseX + 5, baseY + 50);
    glVertex2f(baseX - 5, baseY + 50);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX - 30, baseY + 50);
    glVertex2f(baseX + 30, baseY + 50);
    glVertex2f(baseX, baseY + 80);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(baseX - 25, baseY + 65);
    glVertex2f(baseX + 25, baseY + 65);
    glVertex2f(baseX, baseY + 95);
    glEnd();
}

void drawGrass() {
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(200, 0);
    glVertex2f(200, 20);
    glVertex2f(0, 20);
    glEnd();
}

void drawWater() {
    static float waveOffset = 0.0f;
    waveOffset += 0.1f;

    if (waterHeight > 2.0f) {
        glColor4f(0.0f, 0.0f, 1.0f, 0.7f);
        glBegin(GL_TRIANGLE_STRIP);
        for (float x = 0; x <= 200; x += 1.0f) {
            float waveHeight = waterHeight + 2.0f * sin((x - waveOffset) * 0.1f);
            glVertex2f(x, 0.0f);
            glVertex2f(x, waveHeight);
        }
        glEnd();
    }
    else {
        glColor4f(0.0f, 0.0f, 1.0f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(200, 0);
        glVertex2f(200, waterHeight);
        glVertex2f(0, waterHeight);
        glEnd();
    }
}

void display() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawSun();
    drawMountain(100, 20, 100, 120);
    drawCloud(130, 160);
    drawCloud(140, 170);
    drawCloud(150, 160);
    drawCloud(50, 160);
    drawCloud(60, 170);
    drawCloud(70, 160);
    drawCloud(0, 170);
    drawCloud(0, 160);
    drawGrass();
    drawTree(30, 20);
    drawTree(180, 20);
    drawHouse();
    drawWater();

    float boatY = waterHeight + sin(boatOscillation) * 1.0f;
    drawBoat(boatX, boatY);

    
    for (const auto& particle : particles) {
        glColor4f(0.0f, 0.0f, 1.0f, particle.alpha);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        glVertex2f(particle.x, particle.y);
        glEnd();
    }
    

    glFlush();
}

void update(int value) {
    cloudOffset += 0.1f;
    if (cloudOffset > 200) {
        cloudOffset = -190.0f;
    }

    
    for (size_t i = 0; i < particles.size(); ++i) {
        particles[i].y -= particles[i].speed;
        particles[i].x += wind;
        if (particles[i].y < waterHeight) {
            particles.erase(particles.begin() + i);
            --i;
            waterHeight += 0.02f;
            if (waterHeight > 25) waterHeight = 25;
        }
    }
    boatOscillation += 0.1f;
    if (boatOscillation > 2 * 3.14159f) {
        boatOscillation -= 2 * 3.14159f;
    }
    
    if (waterHeight > 0.0f) {
        waterHeight -= 0.02f;
        if (waterHeight < 2.0f) {
            waterHeight = 2.0f;
            boatOscillation = 0;
        }
    }
    

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void mouseDrag(int x, int y) {
    rainX = static_cast<float>(x) * 200.0f / glutGet(GLUT_WINDOW_WIDTH);
    isRaining = true;
    addParticles(5);
}

void mouseRelease(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isRaining = false;
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(150, 50);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rain Sim");
    gluOrtho2D(0, 200, 0, 190);
    glutDisplayFunc(display);
    glutMotionFunc(mouseDrag);
    glutMouseFunc(mouseRelease);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
