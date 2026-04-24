#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

// Global μεταβλητές για την κίνηση και το mode σχεδίασης
float offsetX = 0.0f;
float offsetY = 0.0f;
bool wireframeMode = false;

// Συνάρτηση χειρισμού πληκτρολογίου
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float speed = 0.05f; // Ταχύτητα κίνησης
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP)    offsetY += speed;
        if (key == GLFW_KEY_DOWN)  offsetY -= speed;
        if (key == GLFW_KEY_LEFT)  offsetX -= speed;
        if (key == GLFW_KEY_RIGHT) offsetX += speed;

        // Εναλλαγή Solid/Wireframe με το Tab
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
            wireframeMode = !wireframeMode;
    }
}

int main() {
    // 1. Αρχικοποίηση GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 2. Δημιουργία Παραθύρου
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ergasia 1 - AM: 59065", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // 3. Αρχικοποίηση GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // 4. Φόρτωση Shaders από τον φάκελο res/shaders/
    Shader ourShader("res/shaders/59065_vertexshader.glsl", "res/shaders/59065_fragmentshader.glsl");

    // 5. Δεδομένα Γεωμετρίας (ΑΜ 59065 -> Περιττός -> Σχήμα 1β)
    // Απόσταση d = 0.1 + 5/30 = 0.26
    // Χρώματα: Τ1(0.9, 0, 0.6) και Τ2(0.5, 0.9, 0)
    GLfloat tri1[] = {
        // Θέσεις (x, y, z)      // Χρώματα (R, G, B)
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

    // Ρύθμιση 1ου Τριγώνου
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri1), tri1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Ρύθμιση 2ου Τριγώνου
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri2), tri2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 6. Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        // Καθαρισμός οθόνης
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Χρήση του Shader
        ourShader.use();

        // --- ΔΙΟΡΘΩΣΗ ΚΙΝΗΣΗΣ ---
        // Βρίσκουμε το location του uniform "offset" και στέλνουμε τις τιμές
        // Χρησιμοποιούμε ourShader.ID (ή το όνομα που έχει η κλάση σου για το ID του shader)
        int offsetLoc = glGetUniformLocation(ourShader.ID, "offset");
        glUniform2f(offsetLoc, offsetX, offsetY);

        // Εναλλαγή Solid/Wireframe
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Σχεδίαση Τριγώνου 1
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Σχεδίαση Τριγώνου 2
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