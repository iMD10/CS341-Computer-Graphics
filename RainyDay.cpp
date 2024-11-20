#include <GL/glut.h>
#include <cstdlib>
#include <vector>

// Particle structure
struct Particle {
    float x, y;     // Position
    float speed;    // Speed of falling
    float alpha;    // Transparency level
};

// Global variables
std::vector<Particle> particles;
float rainX = 100.0f;       // Default rain position (controlled by mouse)
bool isRaining = false;     // Controls whether rain is active
float waterHeight = 2.0f;   // Height of the water (initially at ground level)
float wind = 0.5f;          // Wind strength and direction
float cloudOffset = 0.0f;   // Offset for moving clouds
float boatX = 60.0f; // Initial horizontal position of the boat
float boatOscillation = 0.0f; // Oscillation effect


// Initialize particles for water effect
void addParticles(int count) {
    for (int i = 0; i < count; ++i) {
        particles.push_back({
            rainX + static_cast<float>(rand() % 50 - 25), // X position
            200.0f,                                       // Y position
            static_cast<float>((rand() % 5) + 1),         // Speed
            0.5f + static_cast<float>(rand() % 50) / 100.0f // Random transparency (0.5 to 1.0)
            });
    }
}
void drawMountain(float baseX, float baseY, float width, float height) {
    glColor3f(0.5f, 0.35f, 0.05f); // Brownish mountain
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX, baseY);                  // Left base
    glVertex2f(baseX + width, baseY);          // Right base
    glVertex2f(baseX + width / 2, baseY + height); // Peak
    glEnd();

    // Snowcap at the top of the mountain
    glColor3f(1.0f, 1.0f, 1.0f); // White snow
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX + width * 0.4f, baseY + height * 0.8f); // Left snow
    glVertex2f(baseX + width * 0.6f, baseY + height * 0.8f); // Right snow
    glVertex2f(baseX + width / 2, baseY + height);           // Peak
    glEnd();
}


// Draw a simple house
void drawHouse() {
    glColor3f(1.0f, 0.5f, 0.0f); // Orange base
    glBegin(GL_QUADS);
    glVertex2f(70, 20);
    glVertex2f(130, 20);
    glVertex2f(130, 70);
    glVertex2f(70, 70);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.0f); // Dark red roof
    glBegin(GL_TRIANGLES);
    glVertex2f(65, 70);
    glVertex2f(135, 70);
    glVertex2f(100, 100);
    glEnd();

    glColor3f(0.3f, 0.2f, 0.1f); // Brown door
    glBegin(GL_QUADS);
    glVertex2f(90, 20);
    glVertex2f(110, 20);
    glVertex2f(110, 50);
    glVertex2f(90, 50);
    glEnd();
}

// Draw clouds with dynamic movement
void drawCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f); // White clouds
    for (int i = 0; i < 3; ++i) {
        glBegin(GL_POLYGON);
        for (float angle = 0; angle < 2 * 3.14159; angle += 0.1f) {
            glVertex2f(x + cloudOffset + i * 10 + 10 * cos(angle), y + 10 * sin(angle));
        }
        glEnd();
    }
}
void drawBoat(float boatX, float boatY) {
    // Draw the boat base
    glColor3f(0.55f, 0.27f, 0.07f); // Brown color
    glBegin(GL_POLYGON);
    glVertex2f(boatX - 10, boatY);      // Bottom-left corner
    glVertex2f(boatX + 10, boatY);      // Bottom-right corner
    glVertex2f(boatX + 7, boatY + 5);   // Top-right corner
    glVertex2f(boatX - 7, boatY + 5);   // Top-left corner
    glEnd();

    // Draw the boat sail
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the sail
    glBegin(GL_TRIANGLES);
    glVertex2f(boatX, boatY + 5);       // Sail base
    glVertex2f(boatX, boatY + 15);      // Sail tip
    glVertex2f(boatX + 5, boatY + 5);   // Sail corner
    glEnd();
}


// Draw the sun
void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow sun
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 2 * 3.14159; angle += 0.1f) {
        glVertex2f(180 + 15 * cos(angle), 170 + 15 * sin(angle));
    }
    glEnd();
}

// Draw a tree
void drawTree(float baseX, float baseY) {
    glColor3f(0.6f, 0.3f, 0.0f); // Brown trunk
    glBegin(GL_QUADS);
    glVertex2f(baseX - 5, baseY);
    glVertex2f(baseX + 5, baseY);
    glVertex2f(baseX + 5, baseY + 50);
    glVertex2f(baseX - 5, baseY + 50);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); // Green leaves
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

// Draw the ground with grass
void drawGrass() {
    glColor3f(0.0f, 0.8f, 0.0f); // Green grass
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(200, 0);
    glVertex2f(200, 20);
    glVertex2f(0, 20);
    glEnd();
}

// Draw water
void drawWater() {
    static float waveOffset = 0.0f; // Keeps track of the wave movement
    waveOffset += 0.1f;            // Increment the wave offset to create smooth horizontal motion

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

// Display function
void display() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Sky blue background
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

    // Draw the floating boat
    float boatY = waterHeight  + sin(boatOscillation) * 1.0f; // Float above water with oscillation
    drawBoat(boatX, boatY);

    if (isRaining) {
        for (const auto& particle : particles) {
            glColor4f(0.0f, 0.0f, 1.0f, particle.alpha);
            glPointSize(6.0f);
            glBegin(GL_POINTS);
            glVertex2f(particle.x, particle.y);
            glEnd();
        }
    }

    glFlush();
}


// Update function
// Update function
void update(int value) {
    cloudOffset += 0.1f;
    if (cloudOffset > 200) {
        cloudOffset = -160.0f;
    }

    // Oscillate the boat
    

    if (isRaining) {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i].y -= particles[i].speed;
            particles[i].x += wind; // Wind effect
            if (particles[i].y < waterHeight) {
                particles.erase(particles.begin() + i);
                --i;
                waterHeight += 0.02f;
                if (waterHeight > 25) waterHeight = 25;
            }
        }
        boatOscillation += 0.1f;
        if (boatOscillation > 2 * 3.14159f) { // Keep oscillation within 0 to 2π
            boatOscillation -= 2 * 3.14159f;
        }
    }
    else {
        // Decrease waterHeight when there's no rain
        if (waterHeight > 0.0f) {
            waterHeight -= 0.02f;
            if (waterHeight < 2.0f) waterHeight = 2.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Mouse drag function
void mouseDrag(int x, int y) {
    rainX = static_cast<float>(x) * 200.0f / glutGet(GLUT_WINDOW_WIDTH);
    isRaining = true;
    addParticles(5);
}

// Mouse release function
void mouseRelease(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isRaining = false;
    }
}

// Main function
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
