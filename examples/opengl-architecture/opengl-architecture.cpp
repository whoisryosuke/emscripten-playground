// emcc -o opengl-architecture.html opengl-architecture.cpp -s USE_SDL=2 -s FULL_ES2=1 -s -sEXPORTED_FUNCTIONS="_render_gl,_init_gl,_change_color" -sEXPORTED_RUNTIME_METHODS="ccall,cwrap"
// emcc -o opengl-architecture-asyncify.html opengl-architecture.cpp -s USE_SDL=2 -s FULL_ES2=1 -s ASYNCIFY=1 -s SINGLE_FILE=1

#include <exception>
#include <functional>

#define GL_GLEXT_PROTOTYPES 1

#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL_opengles2.h>

extern "C" {
    
static SDL_Window *window = NULL;

// Shader sources
const GLchar* vertexSource =
    "attribute vec4 position;                      \n"
    "varying vec3 color;                           \n"
    "void main()                                   \n"
    "{                                             \n"
    "    gl_Position = vec4(position.xyz, 1.0);    \n"
    "    color = gl_Position.xyz + vec3(0.5);      \n"
    "}                                             \n";

// Fragment/pixel shader
const GLchar* fragmentSource =
    "precision mediump float;                     \n"
    "varying vec3 color;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    "    gl_FragColor = vec4 ( color, 1.0 );      \n"
    "}                                            \n";


typedef struct render_params {
  int color;
} render_params;

static render_params global_params = {};

    void main_loop() { 
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) std::terminate();
            }

            // Clear the screen to black
            printf("color in renderer %d\n", global_params.color);
            if(global_params.color == 0) {
                glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            } else {
                glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            }
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw a triangle from the 3 vertices
            glDrawArrays(GL_TRIANGLES, 0, 3);

            SDL_GL_SwapWindow(window);
    }

    void init_gl() {

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = (
            SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

        auto glc = SDL_GL_CreateContext(window);

        SDL_GL_SetSwapInterval(0);

        auto rdr = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED);

        // Create Vertex Array Object
        GLuint vao;
        glGenVertexArraysOES(1, &vao);
        glBindVertexArrayOES(vao);

        // Create a Vertex Buffer Object and copy the vertex data to it
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Create and compile the vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        // Create and compile the fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        // Link the vertex and fragment shader into a shader program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        // Specify the layout of the vertex data
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void change_color(int color)
    {
        global_params.color = color;
        printf("changing color %d\n", color);
    }

    void render_gl()
    {
        emscripten_set_main_loop(main_loop, 0, 1);
    }
}
