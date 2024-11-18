#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

// Constants for the grid and window
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
const int CELL_SIZE = 2;
const int GRID_COLS = WINDOW_WIDTH / CELL_SIZE;
const int GRID_ROWS = WINDOW_HEIGHT / CELL_SIZE;

int updateCounter = 0;     // Counter to control particle updates
const int updateInterval = 2;  // Update grid every 2 frames


// Grids for current and next states
int grid[GRID_COLS][GRID_ROWS] = { 0 };
int nextGrid[GRID_COLS][GRID_ROWS] = { 0 };

// Sand color (hue value)
int hueValue = 200;

// Function to check if a column is within bounds
inline bool withinCols(int i) {
    return i >= 0 && i < GRID_COLS;
}

// Function to check if a row is within bounds
inline bool withinRows(int j) {
    return j >= 0 && j < GRID_ROWS;
}

// Convert HSV to RGB
void hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
    }
}

// Mouse drag function to add sand particles
void mouseDragged(int x, int y) {
    int mouseCol = x / CELL_SIZE;
    int mouseRow = (y) / CELL_SIZE;  // Flip Y-axis here

    int matrix = 5;
    int extent = matrix / 2;

    for (int i = -extent; i <= extent; i++) {
        for (int j = -extent; j <= extent; j++) {
            if ((float)rand() / RAND_MAX < 0.75) {  // 75% chance to add particle
                int col = mouseCol + i;
                int row = mouseRow + j;
                if (withinCols(col) && withinRows(row)) {
                    grid[col][row] = hueValue;
                }
            }
        }
    }

    // Cycle hue value
    hueValue += 1;
    if (hueValue > 360) {
        hueValue = 1;
    }
}

// Update the grid for the next frame
void updateGrid() {
    // Reset the next state grid
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            nextGrid[i][j] = 0;
        }
    }

    // Loop through all cells
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            int state = grid[i][j];

            // If it's a piece of sand
            if (state > 0) {
                int below = j + 1 < GRID_ROWS ? grid[i][j + 1] : -1;

                int dir = (rand() % 2 == 0) ? 1 : -1;  // Random left/right direction
                int belowA = withinCols(i + dir) && j + 1 < GRID_ROWS ? grid[i + dir][j + 1] : -1;
                int belowB = withinCols(i - dir) && j + 1 < GRID_ROWS ? grid[i - dir][j + 1] : -1;

                // Check where the sand can fall
                if (below == 0) {
                    nextGrid[i][j + 1] = state;
                }
                else if (belowA == 0) {
                    nextGrid[i + dir][j + 1] = state;
                }
                else if (belowB == 0) {
                    nextGrid[i - dir][j + 1] = state;
                }
                else {
                    nextGrid[i][j] = state;  // Stay put
                }
            }
        }
    }

    // Copy the next state grid into the current grid
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            grid[i][j] = nextGrid[i][j];
        }
    }
}

// Display function to render the sand particles
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            if (grid[i][j] > 0) {
                float r, g, b;
                float h = grid[i][j] / 360.0f;
                hsvToRgb(h, 1.0f, 1.0f, r, g, b);

                glColor3f(r, g, b);
                float x = i * CELL_SIZE;
                float y = WINDOW_HEIGHT - j * CELL_SIZE;  // Correct Y-coordinate here
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + CELL_SIZE, y);
                glVertex2f(x + CELL_SIZE, y - CELL_SIZE);
                glVertex2f(x, y - CELL_SIZE);
                glEnd();
            }
        }
    }

    glutSwapBuffers();
}

// Idle function for continuous updates

// Idle function for continuous updates
void idle() {
    updateCounter++;
    if (updateCounter >= updateInterval) {
        updateGrid();      // Update particles only at the defined interval
        updateCounter = 0; // Reset the counter
    }
    glutPostRedisplay();   // Redraw screen at each frame
}


// Mouse function to handle dragging
void mouseMotion(int x, int y) {
    mouseDragged(x, y);
}

// Initialize OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Sand Simulation");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
