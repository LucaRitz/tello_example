#include <iostream>
#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "camera.h"

using tello::LoggerSettings;
using tello::Logger;
using tello::Network;
using tello::Tello;
using tello::Response;
using tello::Status;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const int window_width = 800;
const int window_height = 600;

struct RemoteControl {
    float leftRight;
    float forwardBackward;
    float upDown;
    float yaw;
};

struct RemoteControl speed {
        20.0, 20.0, 20.0, 20.0
};

int initGLFW() {
    if (glfwInit() == GLFW_FALSE) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processKeys(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void processMovementKeys(GLFWwindow *window, struct RemoteControl &rc) {

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        rc.leftRight = -speed.leftRight;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rc.leftRight = +speed.leftRight;
    } else {
        rc.leftRight = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        rc.forwardBackward = +speed.forwardBackward;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        rc.forwardBackward = -speed.forwardBackward;
    } else {
        rc.forwardBackward = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rc.yaw = -speed.yaw;
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rc.yaw = +speed.yaw;
    } else {
        rc.yaw = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        rc.upDown = +5;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        rc.upDown = -5;
    } else {
        rc.upDown = 0;
    }
}

void move(Tello &tello, struct RemoteControl &rc) {

    int x = (int) rc.leftRight;
    int y = (int) rc.forwardBackward;
    int z = (int) rc.upDown;
    int r = (int) rc.yaw;

    std::cout << "RC: " << x << " " << y << " " << z << " " << r << std::endl;
    future<Response> commandResponse = tello.rc_control(x, y, z, r);
}

int controller(tello::Tello &tello) {

    if (initGLFW() != 0) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float droneColor[] = { 0.5f, 0.4f, 0.2f };

    Shader shader("./shaders/shader.vs", "./shaders/shader.fs");
    shader.use();
    shader.setFloatVector3("droneColor", droneColor[0], droneColor[1], droneColor[2]);

    Camera camera {};

//    tello.setStatusHandler([](const StatusResponse& status) {
//
//        glm::vec3 newPosition ((float) status.get_x(), (float) status.get_y(), (float) status.get_z());
//        camera.setPosition(newPosition);
//    });

    float vertices[] = {
            // positions
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
    };
    glm::vec3 position(0.0f,  0.0f,  0.0f);

    VertexBuffer* vbo = VertexBuffer::of(sizeof(vertices), vertices);
    VertexArray* vao = VertexArray::create();
    vao->bind();
    vao->layout({
                        Attribute::of(3, AttributeType::FLOAT, Normalization::OFF)
                });

    struct RemoteControl rc {};

    glm::mat4 viewTransform = camera.transform();
    glm::mat4 projectionTransform = glm::perspective(glm::radians(camera.fov()), (float)window_width / window_height, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeys(window);
        processMovementKeys(window, rc);
        move(tello, rc);

        shader.use();

        glm::mat4 localTransform = glm::mat4(1.0f);

        glm::mat4 modelTransform = glm::mat4(1.0f);
        modelTransform = glm::translate(modelTransform, position);

        glm::mat4 totalTransform = projectionTransform * viewTransform * modelTransform * localTransform;

        shader.setFloatMatrix4("vTransform", glm::value_ptr(totalTransform));

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao->bind();

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}