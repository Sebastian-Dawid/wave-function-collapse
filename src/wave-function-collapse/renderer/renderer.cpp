#include "renderer.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

void renderer_t::prep_mesh(std::vector<wfc::mesh_tile_t::mesh_t> meshes)
{
    std::size_t off_index = 0;
    std::size_t base_index = 0;
    float xoff = -1.f, yoff = 1.f;

    for (wfc::mesh_tile_t::mesh_t mesh : meshes)
    {
        if (off_index % this->w == 0)
        {
            xoff = -1.f;
            yoff -= 2.f/this->h;
        }
        for (glm::vec3 vert : mesh.vertices)
        {
            vertices.push_back(vert.x * 2.f/this->w + xoff);
            vertices.push_back(vert.y * 2.f/this->h + yoff);
            vertices.push_back(vert.z);
        }

        xoff += 2.f/this->w;

        for (std::size_t idx : mesh.indices)
            this->indices.push_back(idx + base_index);
        off_index++;
        base_index += mesh.vertices.size();
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

bool renderer_t::compile_shader()
{
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "\033[1;31m[ERROR VERTEX SHADER COMPILATION]:\033[0m\n %s\n", infoLog);
        return false;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "\033[1;31m[ERROR FRAGMENT SHADER COMPILATION]:\033[0m\n %s\n", infoLog);
        return false;
    }
    // link shaders
    this->shader = glCreateProgram();
    glAttachShader(this->shader, vertex_shader);
    glAttachShader(this->shader, fragmentShader);
    glLinkProgram(this->shader);
    // check for linking errors
    glGetProgramiv(this->shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shader, 512, NULL, infoLog);
        fprintf(stderr, "\033[1;31m[ERROR SHADER LINKING]:\033[0m\n %s\n", infoLog);
        return false;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragmentShader);

    return true;
}

renderer_t::renderer_t(std::size_t w, std::size_t h)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(100 * w, 100 * h, "wfc", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "\033[0;31m[ERROR]:\033[0m Failed to create window!\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        fprintf(stderr, "\033[0;31m[ERROR]:\033[0m Failed to initialize GLAD!\n");
        glfwTerminate();
    }
    if (!compile_shader())
    {
        glfwTerminate();
		return;
    }
    this->window = window;
    this->w = w;
    this->h = h;
    this->init = true;
}

std::int32_t renderer_t::render()
{
    if (!this->init) return -1;

    glViewport(0, 0, 100 * w, 100 * h);
    while (!glfwWindowShouldClose(this->window))
    {
        if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(this->window, GLFW_TRUE);

        // rendering code
        glClearColor(1.0f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(this->shader);
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
