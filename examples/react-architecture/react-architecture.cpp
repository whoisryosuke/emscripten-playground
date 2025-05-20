// emcc react-architecture.cpp -o react-architecture-es6.js --emit-tsd react-architecture-es6.d.ts -s USE_SDL=2 -s FULL_ES2=1 -s MODULARIZE -s SINGLE_FILE=1 -O2 -lembind -s EXPORT_ES6=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s -sEXPORTED_FUNCTIONS="_render_gl,_init_gl,_change_color,_add_ui" -s EXPORT_NAME="render_module"
// emcc react-architecture.cpp -o react-architecture-es6.js --emit-tsd your_module.d.ts -s USE_SDL=2 -s FULL_ES2=1 -s MODULARIZE -s SINGLE_FILE=1 -O2 -lembind -s EXPORT_ES6=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s -sEXPORTED_FUNCTIONS="_render_gl,_init_gl,_change_color,_add_ui"
// emcc react-architecture.cpp -o react-architecture-es6.js -s USE_SDL=2 -s FULL_ES2=1 -s MODULARIZE -s SINGLE_FILE=1 -O2 -lembind -s EXPORT_ES6=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s -sEXPORTED_FUNCTIONS="_render_gl,_init_gl,_change_color,_add_ui"
// emcc -o react-architecture.html react-architecture.cpp -s USE_SDL=2 -s FULL_ES2=1 -s -sEXPORTED_FUNCTIONS="_render_gl,_init_gl,_change_color,_add_ui" -sEXPORTED_RUNTIME_METHODS="ccall,cwrap"
// emcc -o react-architecture-asyncify.html react-architecture.cpp -s USE_SDL=2 -s FULL_ES2=1 -s ASYNCIFY=1 -s SINGLE_FILE=1

#include <exception>
#include <functional>

#define GL_GLEXT_PROTOTYPES 1
#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL_opengles2.h>
#include "glm.hpp"

extern "C" {
    
static SDL_Window *window = NULL;

// Shader sources
const GLchar* vertexSource =
    "attribute vec4 position;                      \n"
    "varying vec3 color;                           \n"
    "attribute vec2 aOffset;                           \n"
    "void main()                                   \n"
    "{                                             \n"
    "    gl_Position = vec4(position.xyz + aOffset, 1.0);    \n"
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


// static std::vector<unsigned int> indices{0, 1, 2};
// static GLuint index_buffer;

// UI Elements
// Ideally here we'd defer to a higher level library
typedef struct UIElement {
    GLuint vbo;
    GLuint vao;
} UIElement;
static std::vector<UIElement> ui_elements;

UIElement create_ui_element(int offset) {
    UIElement ui_element;
    // Create Vertex Array Object
    // GLuint vao;
    glGenVertexArraysOES(1, &ui_element.vao);
    glBindVertexArrayOES(ui_element.vao);
    // ui_element.vao = vao;

    // Create a Vertex Buffer Object and copy the vertex data to it
    // GLuint vbo;
    glGenBuffers(1, &ui_element.vbo);
    // ui_element.vbo = vbo;

    // Vertices
    float vertex_offset = offset * 0.1f;
    float top_left_x = vertex_offset * 0.1f + 0.5f;
    // GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
    GLfloat vertices[] = {vertex_offset, top_left_x, top_left_x, -top_left_x, -top_left_x, -top_left_x};

    glBindBuffer(GL_ARRAY_BUFFER, ui_element.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    printf("Creating UI element \n");

    return ui_element;
}

// Global state for renderer
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

            // Draw UI elements
            // for (auto & element : ui_elements) {
            //     printf("Rendering UI element %d \n", element.vao);
            //     glBindVertexArrayOES(element.vao);
            //     glBindBuffer(GL_ARRAY_BUFFER, element.vbo);
            //     glDrawArrays(GL_TRIANGLES, 0, 3);
            // }
            
            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

            // Draw a triangle from the 3 vertices
            // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

            // Instanced drawing
            glDrawArraysInstancedNV(GL_TRIANGLES, 0, 3, 100);


            SDL_GL_SwapWindow(window);
    }

    
    void add_ui(int offset)
    {
        // Create a "UI Element" (aka triangle)
        UIElement ui_element = create_ui_element(offset);
        ui_elements.push_back(ui_element);
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

        add_ui(5);

        glm::vec2 translations[100];
        int index = 0;
        float offset = 0.1f;
        for(int y = -10; y < 10; y += 2)
        {
            for(int x = -10; x < 10; x += 2)
            {
                glm::vec2 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translations[index++] = translation;
            }
        }  
        printf("generated instances %.6f \n", translations[0].x);

        unsigned int instanceVBO;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisorNV(2, 1);  

        // Index
        // glGenBuffers(1, &index_buffer);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
