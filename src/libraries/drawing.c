#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159f
#define R 0.03f

float radians(int degrees)
{
    return degrees * (PI / 180);
}

void render_circle(float center_x, float center_y, float center_z)
{
    float scale_factor = 1.0f / (1.0f + center_z);
    float radius = R * scale_factor;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(center_x, center_y);

    for (int i = 0; i <= 360; i++)
    {
        float angle = radians(i);

        float x = radius * cos(angle) + center_x;
        float y = radius * sin(angle) + center_y;

        glVertex2f(x, y);
    }

    glEnd();
}
