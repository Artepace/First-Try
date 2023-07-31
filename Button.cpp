#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Переменные для отслеживания состояния кнопки
bool isButtonPressed = false;
float buttonColor = 0.5f;

// Коллбэк для обработки событий клавиш мыши
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isButtonPressed = true;
        } else if (action == GLFW_RELEASE) {
            isButtonPressed = false;
        }
    }
}

// Функция для анимации изменения цвета кнопки
void animateButtonColor() {
    if (isButtonPressed) {
        buttonColor = 0.3f; // Цвет кнопки при нажатии
    } else {
        buttonColor = 0.5f; // Исходный цвет кнопки
    }
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка при инициализации GLFW" << std::endl;
        return -1;
    }

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Простая кнопка", nullptr, nullptr);
    if (!window) {
        std::cerr << "Ошибка при создании окна GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Устанавливаем контекст окна для GLEW
    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Ошибка при инициализации GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Устанавливаем коллбэк для обработки событий клавиш мыши
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Главный цикл приложения
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Анимация изменения цвета кнопки
        animateButtonColor();

        // Рисуем кнопку
        glBegin(GL_QUADS);
        glColor3f(buttonColor, buttonColor, buttonColor);  // Используем измененный цвет кнопки
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(-0.5f, 1.0f);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(-1.0f, 0.5f);
        glEnd();

        // Проверяем и обрабатываем события
        glfwPollEvents();

        // Обмен буферов и вывод на экран
        glfwSwapBuffers(window);
    }

    // Завершение работы
    glfwTerminate();
    return 0;
}