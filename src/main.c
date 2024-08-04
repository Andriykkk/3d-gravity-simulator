#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libraries/drawing.h"

#define N 10
#define G 6.67430e-11
#define DT 4

typedef struct
{
    double x, y, z;    // Position
    double vx, vy, vz; // Velocity
    double mass;
    double radius; // Mass
} Particle;

double posX = 0, posY = 0, posZ = -3;

Particle particles[N];

// Error callback function
void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

void init_particles()
{
    for (int i = 0; i < N; i++)
    {
        particles[i].x = (((float)rand() / RAND_MAX) * 2.0 - 1.0) * 3;
        particles[i].y = (((float)rand() / RAND_MAX) * 2.0 - 1.0) * 3;
        particles[i].z = (((float)rand() / RAND_MAX) * 2.0 - 1.0) * 3 - 3;
        particles[i].vx = 0;
        particles[i].vy = 0;
        particles[i].vz = 0;
        particles[i].mass = 1;
        particles[i].radius = 0.2;
    }
}

void compute_forces()
{
    for (int i = 0; i < N; i++)
    {
        double force_x = 0, force_y = 0, force_z = 0;
        for (int j = 0; j < N; j++)
        {
            if (i == j)
                continue;
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double dz = particles[j].z - particles[i].z;
            double distance = sqrt(dx * dx + dy * dy + dz * dz);
            double force = G * particles[i].mass * particles[j].mass / (distance * distance);
            force_x += force * dx / distance;
            force_y += force * dy / distance;
            force_z += force * dz / distance;
        }
        particles[i].vx += force_x / particles[i].mass * DT;
        particles[i].vy += force_y / particles[i].mass * DT;
        particles[i].vz += force_z / particles[i].mass * DT;
    }
}

void update_positions()
{
    for (int i = 0; i < N; i++)
    {
        particles[i].x += particles[i].vx * DT;
        particles[i].y += particles[i].vy * DT;
        particles[i].z += particles[i].vz * DT;
    }
}

void print_particles()
{
    for (int i = 0; i < N; i++)
    {
        render_circle(particles[i].x, particles[i].y, particles[i].z, particles[i].radius, 36);
    }
}

char moveForward = 0, moveBackward = 0, moveLeft = 0, moveRight = 0;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            moveForward = 1;
            break;
        case GLFW_KEY_S:
            moveBackward = 1;
            break;
        case GLFW_KEY_A:
            moveLeft = 1;
            break;
        case GLFW_KEY_D:
            moveRight = 1;
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            moveForward = 0;
            break;
        case GLFW_KEY_S:
            moveBackward = 0;
            break;
        case GLFW_KEY_A:
            moveLeft = 0;
            break;
        case GLFW_KEY_D:
            moveRight = 0;
            break;
        }
    }
}

void process_input()
{
    if (moveForward)
    {
        posZ += 0.01;
    }
    if (moveBackward)
    {
        posZ -= 0.01;
    }
    if (moveLeft)
    {
        posX += 0.01;
    }
    if (moveRight)
    {
        posX -= 0.01;
    }
}

int main(void)
{
    srand(time(NULL));
    init_particles();

    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set the error callback
    glfwSetErrorCallback(error_callback);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(640, 640, "Particle Life", NULL, NULL);
    // GLFWwindow *window2 = glfwCreateWindow(500, 500, "Additional Window", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input();

        glPushMatrix();
        // Move the camera back so we can see the objects
        glTranslatef(posX, posY, posZ);
        compute_forces();
        update_positions();
        print_particles();
        glPopMatrix();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
