#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;

    uniform mat4 model;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * model * vec4(position, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 fragColor;

    void main() {
        fragColor = vec4(1.0, 1.0, 0.0, 1.0); // Желтый цвет
    }
)";

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void render(GLFWwindow* window, GLuint shaderProgram, float radius, float angularVelocity) {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.0f));

    float time = static_cast<float>(glfwGetTime());
    float angle = angularVelocity * time;

    glm::mat4 rotation = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(rotation));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    const int numSegments = 100; // Количество сегментов окружности
    const float segmentAngle = 2.0f * glm::pi<float>() / numSegments;

    for (int i = 0; i < numSegments; ++i) {
        float x = radius * glm::cos(i * segmentAngle);
        float y = radius * glm::sin(i * segmentAngle);

        float nextX = radius * glm::cos((i + 1) * segmentAngle);
        float nextY = radius * glm::sin((i + 1) * segmentAngle);

        float vertices[] = {
            x, y,
            nextX, nextY,
            0.0f, 0.0f
        };

        GLuint vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_LINE_LOOP, 0, 3);

        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //расскажем как работать с яблоками
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    gladLoadGL();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float radius = 200.0f;
    float angularVelocity = 0.5f; // Угловая скорость (рад/с)

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render(window, shaderProgram, radius, angularVelocity);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}