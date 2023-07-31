#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 920

int g = 7; //*10^-11
int r_sun = 140; //*5000
int r_er = 7; //*10^6 m
int r_mar = 3; //*10^6 m
int m_sun = 2000000; //*10^24
int m_er = 6; //*10^24
int m_mar = 1; //*10^24
int v_er = 30; //*1000 м/с
int v_mar = 24; //*1000 м/с





void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
    int numberOfVertices = numberOfSides + 2;
    
    GLfloat twicePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;
    
    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[(numberOfVertices) * 3];
    
    for (int i = 0; i < numberOfVertices; i++)
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// Функция обратного вызова для обработки событий клавиатуры
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main()
{
    GLFWwindow *window;
    
    // Инициализация библиотеки GLFW
    if (!glfwInit())
    {
        return -1;
    }
    
    // Создание окна с OpenGL контекстом
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "First Version", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    // Устанавливаем контекст окна текущим
    glfwMakeContextCurrent(window);

    // Устанавливаем функцию обратного вызова для обработки событий клавиатуры
    glfwSetKeyCallback(window, keyCallback);
    
    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // Задаем область вывода OpenGL (в пикселях)
    glMatrixMode(GL_PROJECTION); // Матрица проекции определяет свойства камеры, которая видит объекты в мировой системе координат. Здесь обычно устанавливают масштаб, соотношение сторон и плоскости отсечения.
    glLoadIdentity(); // Заменяем текущую матрицу на единичную матрицу, чтобы начать с чистого листа, так как матричные преобразования, такие как glOrtho и glRotate, накапливаются, просто ставим нас в (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // Устанавливаем систему координат
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    int nachx = SCREEN_WIDTH / 2;
    int nachy = SCREEN_HEIGHT / 2;
    int i = 0, c = 0;
    int x_er, y_er, x_mar, y_mar;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовываем sun
        glColor3f(1.0f, 0.9f, 0.6f);
        drawCircle(nachx, nachy, 0, r_sun, 360);
        
        //F = G*M*m/s^2 = m*v^2/s
        //s = G*M/v^2

        int s = g*m_sun/(v_er*v_er*50); //  (*2*10^9 m)

        //std::cerr << s << std::endl;

        //float otn = v_er/v_mar;

        x_er = s*cosf(i * 1.25f * M_PI/180) + nachx; // 1.25 = v_er/v_mar
        y_er = s*sinf(i * 1.25f * M_PI/180) + nachy;
        glColor3f(0.0f, 0.0f, 1.0f);
        drawCircle(x_er, y_er, 0, r_er, 360);


        s = g*m_sun/(v_mar*v_mar*50); //  (*2*10^9 m)

        //std::cerr << s << std::endl;

        x_mar = s*cosf(c * M_PI/180) + nachx;
        y_mar = s*sinf(c * M_PI/180) + nachy;
        glColor3f(0.9f, 0.0f, 0.0f);
        drawCircle(x_mar, y_mar, 0, r_mar, 360);
        

        i++;
        c++;
        
        if (i * 1.25 > 360) {
            i = 0;
        }
        if (c > 360) {
            c = 0;
        }
        
        // Переключаем передний и задний буферы
        glfwSwapBuffers(window);
        
        // Проверяем и обрабатываем события
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}