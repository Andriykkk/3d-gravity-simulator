#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libraries/drawing.h"

#define N 30
#define G 6.67430e-11
#define DT 1

typedef struct
{
    double x, y, z;    // Position
    double vx, vy, vz; // Velocity
    double mass;       // Mass
} Particle;

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
        particles[i].x = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
        particles[i].y = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
        particles[i].z = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
        particles[i].vx = 0;
        particles[i].vy = 0;
        particles[i].vz = 0;
        particles[i].mass = 1;
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
        render_circle( particles[i].x, particles[i].y, particles[i].z);
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
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window\n");
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        compute_forces();
        update_positions();
        print_particles();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
