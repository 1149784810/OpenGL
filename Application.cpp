
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

static uint32_t CompileShader(uint32_t type, const string& source)
{
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();

    //第二参数指定的1代表我们只需要指定一段源码
    //第四参数代表每段代码的长度，若为nullptr，则代表自动以nullptr结尾
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //异常处理
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        //alloca是c语言中一个可以为栈区内存动态分配大小的函数（前提是栈区内存仍然足够）, _malloca是安全版本，但需要调用_freea进行释放
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile " << (type== GL_VERTEX_SHADER?"GL_VERTEX_SHADER":"GL_FRAGMENT_SHADER") << " shader !" << endl;
        cout << message << endl;
        _freea(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static uint32_t CreateShader(const string& vertexShader, const string& fragmentShader)
{
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //在上面已经得到程序，因此可以删除中间文件
    glDeleteShader(vs);
    glDeleteShader(fs);
    //不使用detach的原因是会删除shader代码，而保留代码的shader很有可能在后面有用处

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        cout << "error!" << endl;
    }
    cout << glewGetString(GLEW_VERSION) << endl;

   
    unsigned int buffer;
    float positions[] =
    {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        glDeleteShader(shader);
    }

    glfwTerminate();
    return 0;
}