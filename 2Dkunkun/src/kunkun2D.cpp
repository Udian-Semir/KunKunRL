#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <list>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctime>
#include <unistd.h>

const int window_width = 800;
const int window_height = 600;

struct KunKun2DEngine
{
    GLFWwindow *windows;

    KunKun2DEngine()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        windows = glfwCreateWindow(window_width, window_height, "KunKun2D", nullptr, nullptr);
        if (!windows)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        glfwMakeContextCurrent(windows);
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(windows, &framebufferWidth, &framebufferHeight); 
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }

    void run()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, 0, window_height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
    }

    float crossProduct2D(const Eigen::Vector2f &a, const Eigen::Vector2f &b)
    {
        return a.x() * b.y() - a.y() * b.x();
    }

    void drawCircle(Eigen::Vector2f position, float radius, Eigen::Vector3f colour)
    {
        glColor3f(colour.x(), colour.y(), colour.z());
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(position.x(), position.y());
        for (int i = 0; i <= 100; ++i)
        {
            float angle = 2.0f * M_PI * i / 100;
            float x = position.x() + radius * cos(angle);
            float y = position.y() + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }

    Eigen::Vector2<float> rotate(Eigen::Vector2f vector, float angle)
    {
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        return Eigen::Vector2f(vector.x() * cosAngle - vector.y() * sinAngle,
                               vector.x() * sinAngle + vector.y() * cosAngle);
    }

    Eigen::Vector2<float> cross(float scalar, Eigen::Vector2f vector)
    {
        return Eigen::Vector2f(-scalar * vector.y(), scalar * vector.x());
    }
};

struct Bone2D
{
    Eigen::Vector2f position, velocity;
    float angle, angleVelocity;
    float halflength, radius, mass, inertia;

    Eigen::Vector2f force = Eigen::Vector2f(0, 0);
    float torque = 0.0f;
    bool dragged = false;

    Bone2D(Eigen::Vector2f position, float angle, float halflength, float radius, float mass)
        : position(position), angle(angle), halflength(halflength), radius(radius), mass(mass)
    {
        float length = 2 * halflength;
        float width = 2 * radius;
        inertia = (1.0f / 12.0f) * mass * (length * length + width * width);
    }

    Eigen::Vector2f wolrdPoint(Eigen::Vector2f localPoint) const
    {
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        Eigen::Vector2f rotatedPoint(cosAngle * localPoint.x() - sinAngle * localPoint.y(),
                                     sinAngle * localPoint.x() + cosAngle * localPoint.y());
        return position + rotatedPoint;
    }


    void draw()
    {
        Eigen::Vector2f direction(cos(angle), sin(angle));
        Eigen::Vector2f positionleft = position - direction * halflength;
        Eigen::Vector2f positionright = position + direction * halflength;

        Eigen::Vector2f unit_direction = direction.normalized();
        Eigen::Vector2f perpendicular(-unit_direction.y(), unit_direction.x());

        Eigen::Vector2f circleleft = positionleft + unit_direction * radius;
        Eigen::Vector2f circleright = positionright - unit_direction * radius;

        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f((circleright + perpendicular * radius).x(), (circleright + perpendicular * radius).y());
        glVertex2f((circleright - perpendicular * radius).x(), (circleright - perpendicular * radius).y());
        glVertex2f((circleleft - perpendicular * radius).x(), (circleleft - perpendicular * radius).y());
        glVertex2f((circleleft + perpendicular * radius).x(), (circleleft + perpendicular * radius).y());
        glEnd();

        for (Eigen::Vector2f circlePos : {circleleft, circleright})
        {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(circlePos.x(), circlePos.y());
            for (int i = 0; i <= 100; ++i)
            {
                float angle = 2.0f * M_PI * i / 100;
                float x = circlePos.x() + radius * cos(angle);
                float y = circlePos.y() + radius * sin(angle);
                glVertex2f(x, y);
            }
            glEnd();
        }

    }

};

struct Joint2D
{
    Bone2D *boneA, *boneB;
    Eigen::Vector2f anchorA_local, anchorB_local;
    float maxTorque = std::numeric_limits<float>::infinity();

    float targetAngle = 0.0f;
    float stiffness = 0.0f;

    




};


struct Skeleton2D
{
    std::vector<Bone2D*> bones;

};

int timer = 0;
int frameInterval = 250;

int main()
{

    KunKun2DEngine engine;
    glfwSwapBuffers(engine.windows);
    // engine.run();
    while (!glfwWindowShouldClose(engine.windows))
    {
        engine.run();
        // engine.drawCircle(Eigen::Vector2f(400, 300), 50, Eigen::Vector3f(1, 0, 0));

        if (timer % frameInterval == 0)
        {
            // std::cout << "Frame: " << timer / frameInterval << std::endl;
            glfwSwapBuffers(engine.windows);
        }
        timer++;
        // glfwSwapBuffers(engine.windows)
        glfwPollEvents();
           
    }


}