#include <GL/glut.h>
#include <vector>

// Constants for window and grid
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 5;  // Each cell will be 5x5 pixels

const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

// Struct for particles
struct Particle {
    bool occupied = false;
    float r = 1.0f, g = 1.0f, b = 0.0f;  // Initial yellow color
    bool inAir = true;  // Track if the particle is falling
};

// Grid to represent the sand particles
std::vector<std::vector<Particle>> grid(GRID_WIDTH, std::vector<Particle>(GRID_HEIGHT));

// Track if the mouse is being held down
bool mouseHeld = false;

// Timer to control color changes for falling particles
float colorTimer = 0.0f;

// Initialize OpenGL settings
void init() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);  // Background: dark blue-gray
}

// Create a particle at grid coordinates (x, y)
void createParticle(int x, int y) {
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
        grid[x][y].occupied = true;
        grid[x][y].inAir = true;  // Mark particle as falling
        grid[x][y].r = 1.0f;      // Initial color: bright yellow
        grid[x][y].g = 1.0f;
        grid[x][y].b = 0.0f;
    }
}

// Update particles to simulate gravity, stacking, and sliding
void updateParticles() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 1; y < GRID_HEIGHT; y++) {  // Start from bottom row
            if (grid[x][y].occupied) {
                if (grid[x][y].inAir) {
                    // Change color while in air
                    grid[x][y].r = 1.0f - (colorTimer * 0.5f);
                    grid[x][y].g = 1.0f - (colorTimer * 0.3f);
                    grid[x][y].b = 0.5f + (colorTimer * 0.2f);

                    // Apply gravity: move particle down if the cell below is empty
                    if (!grid[x][y - 1].occupied) {
                        grid[x][y - 1] = grid[x][y];
                        grid[x][y].occupied = false;
                    }
                    // Try to slide left if the below-left cell is empty
                    else if (x > 0 && !grid[x - 1][y - 1].occupied) {
                        grid[x - 1][y - 1] = grid[x][y];
                        grid[x][y].occupied = false;
                    }
                    // Try to slide right if the below-right cell is empty
                    else if (x < GRID_WIDTH - 1 && !grid[x + 1][y - 1].occupied) {
                        grid[x + 1][y - 1] = grid[x][y];
                        grid[x][y].occupied = false;
                    }
                    // If no movement is possible, particle has landed
                    else {
                        grid[x][y].inAir = false;  // Mark as landed
                    }
                }
            }
        }
    }

    // Update color timer for falling particles
    if (mouseHeld) {
        colorTimer += 0.01f;
        if (colorTimer > 1.0f) colorTimer = 0.0f;  // Reset timer
    }
}

// Render all particles
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (grid[x][y].occupied) {
                glColor3f(grid[x][y].r, grid[x][y].g, grid[x][y].b);
                float screenX = (x * GRID_SIZE) / (float)WINDOW_WIDTH * 2.0f - 1.0f;
                float screenY = (y * GRID_SIZE) / (float)WINDOW_HEIGHT * 2.0f - 1.0f;
                glVertex2f(screenX, screenY);
                glVertex2f(screenX + GRID_SIZE * 2.0f / WINDOW_WIDTH, screenY);
                glVertex2f(screenX + GRID_SIZE * 2.0f / WINDOW_WIDTH, screenY + GRID_SIZE * 2.0f / WINDOW_HEIGHT);
                glVertex2f(screenX, screenY + GRID_SIZE * 2.0f / WINDOW_HEIGHT);
            }
        }
    }
    glEnd();

    glutSwapBuffers();
}

// Idle function to continuously update the screen
void idle() {
    updateParticles();
    glutPostRedisplay();
}

// Handle mouse click events
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseHeld = true;
            int gridX = x / GRID_SIZE;
            int gridY = (WINDOW_HEIGHT - y) / GRID_SIZE;
            createParticle(gridX, gridY);
        }
        else if (state == GLUT_UP) {
            mouseHeld = false;
            colorTimer = 0.0f;  // Reset color timer when mouse is released
        }
    }
}

// Handle mouse motion for dragging
void motion(int x, int y) {
    if (mouseHeld) {
        int gridX = x / GRID_SIZE;
        int gridY = (WINDOW_HEIGHT - y) / GRID_SIZE;
        createParticle(gridX, gridY);
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Dynamic Sand Simulation");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
