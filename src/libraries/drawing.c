#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define PI 3.14159f

float radians(int degrees)
{
    return degrees * (PI / 180);
}

void render_circle(double x, double y, double z, double radius, int segments)
{
    double angleIncrement = M_PI * 2.0f / segments;
    double vertices[(segments + 1) * 3];

    for (int i = 0; i < segments; i++)
    {
        double angle = angleIncrement * i - 1;
        int index = i * 3;
        vertices[index] = sin(angle);
        vertices[index + 1] = cos(angle);
        vertices[index + 2] = 0;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertices);

    glPushMatrix();
    glTranslatef(x, y, z);
    glScaled(radius, radius, radius);
    // glScaled(0.2, 1, 1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 1);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
}
