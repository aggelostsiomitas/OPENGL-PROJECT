#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

// Global variables for mvement and drawing mode
float offsetX = 0.0f;
float offsetY = 0.0f;
bool wireframeMode = false;

// function to handle imput for the keyboard
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float speed = 0.05f; // Ταχύτητα κίνησης
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP)    offsetY += speed;
        if (key == GLFW_KEY_DOWN)  offsetY -= speed;
        if (key == GLFW_KEY_LEFT)  offsetX -= speed;
        if (key == GLFW_KEY_RIGHT) offsetX += speed;


        // Switch between Solid/Wireframe using  Tab
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
            wireframeMode = !wireframeMode;
    }
}

int main() {
    // initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Initialize window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ergasia 1 - AM: 59065", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // load shader form res/shaders/
    Shader ourShader("res/shaders/vertexshader.glsl", "res/shaders/fragmentshader.glsl");

  
    GLfloat tri1[] = {
        // potitions (x, y, z)      // colors (R, G, B)
         0.6f, -0.6f, 0.0f,      0.9f, 0.0f, 0.6f,
         0.1f, -0.6f, 0.0f,      0.9f, 0.0f, 0.6f,
         0.6f, -0.1f, 0.0f,      0.9f, 0.0f, 0.6f
    };

    GLfloat tri2[] = {
        -0.6f,  0.6f, 0.0f,      0.5f, 0.9f, 0.0f,
        -0.1f,  0.6f, 0.0f,      0.5f, 0.9f, 0.0f,
        -0.6f,  0.1f, 0.0f,      0.5f, 0.9f, 0.0f
    };

    unsigned int VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //handle first triangle
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri1), tri1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // handle second triangle
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri2), tri2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        // clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //use the shader
        ourShader.use();

    //find the location of the id of the shader and send the values back
        int offsetLoc = glGetUniformLocation(ourShader.ID, "offset");
        glUniform2f(offsetLoc, offsetX, offsetY);

        // switch between Solid/Wireframe
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw first triangle
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw second triangle
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers και Polling events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glfwTerminate();
    return 0;
}
