#define STB_IMAGE_IMPLEMENTATION

// Inclusão das bibliotecas necessárias
#include <GL/glew.h>          // Gerenciamento de extensões OpenGL
#include <GLFW/glfw3.h>       // Criação de janelas e contexto OpenGL

#include <assimp/Importer.hpp>    // Importação de modelos 3D
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>             // Biblioteca matemática para gráficos 3D
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <stb_image.h> // Biblioteca para carregar imagens (texturas)

const unsigned int WIDTH = 800;   // Largura da janela
const unsigned int HEIGHT = 600;  // Altura da janela

// Variáveis globais para controlar a posição e rotação do modelo
float modelXPosition = 0.0f;
float modelYPosition = 0.0f;
float modelZPosition = -500.0f; // Posição inicial do modelo no eixo Z

float cameraSpeed = 50.0f; // Velocidade de movimento do modelo

// Variáveis para controle de rotação com o mouse
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
float yaw = 0.0f;    // Rotação em torno do eixo Y (horizontal)
float pitch = 0.0f;  // Rotação em torno do eixo X (vertical)
bool firstMouse = true;

// Sensibilidade do mouse
float sensitivity = 0.1f;

// Variável para controlar a rotação automática (simulação da rotação da Terra)
float autoRotateAngle = 0.0f;

// Estrutura para armazenar os dados dos vértices
struct Vertex {
    glm::vec3 position;   // Posição do vértice
    glm::vec2 texCoords;  // Coordenadas de textura
};

// Vetores para armazenar os vértices e índices do modelo
std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

// Função de callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Ajusta a área de renderização
}

// Função de callback para o movimento do mouse
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;   // Inicializa a posição anterior do mouse
        lastY = ypos;
        firstMouse = false;
    }

    // Calcula o deslocamento do mouse
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverte o eixo Y para corresponder à direção do movimento
    lastX = xpos;
    lastY = ypos;

    // Aplica a sensibilidade
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Atualiza os ângulos de rotação
    yaw += xoffset;
    pitch += yoffset;

    // Limita o ângulo de inclinação para evitar "flip" da câmera
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

// Função para processar a entrada do teclado
void processInput(GLFWwindow* window, float deltaTime) {
    // Fecha a janela se a tecla ESC for pressionada
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movimentação do modelo com as teclas de seta
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        modelZPosition -= cameraSpeed * deltaTime; // Move para longe da câmera

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        modelZPosition += cameraSpeed * deltaTime; // Move para perto da câmera

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        modelXPosition -= cameraSpeed * deltaTime; // Move para a esquerda

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        modelXPosition += cameraSpeed * deltaTime; // Move para a direita
}

// Função para carregar o código-fonte dos shaders a partir de arquivos
std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo de shader: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); // Lê todo o conteúdo do arquivo
    return buffer.str();
}

// Função para compilar um shader
GLuint compileShader(const char* filepath, GLenum shaderType) {
    std::string shaderCode = loadShaderSource(filepath);
    if (shaderCode.empty()) {
        std::cerr << "Código de shader vazio: " << filepath << std::endl;
        return 0;
    }
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(shaderType); // Cria um objeto shader
    glShaderSource(shader, 1, &shaderSource, NULL); // Define o código-fonte do shader
    glCompileShader(shader); // Compila o shader

    // Verifica erros de compilação
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar shader " << filepath << ":\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

// Função para criar o shader program
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    GLuint vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    if (vertexShader == 0) return 0;

    GLuint fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) return 0;

    GLuint shaderProgram = glCreateProgram(); // Cria o shader program
    glAttachShader(shaderProgram, vertexShader);   // Anexa o vertex shader
    glAttachShader(shaderProgram, fragmentShader); // Anexa o fragment shader
    glLinkProgram(shaderProgram); // Linka os shaders

    // Verifica erros de linkagem
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro ao linkar shader program:\n" << infoLog << std::endl;
        return 0;
    }

    // Deleta os shaders intermediários, pois já estão linkados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Função para carregar o modelo 3D usando Assimp
void loadModel(const std::string& path) {
    Assimp::Importer importer;
    // Carrega o modelo com processamento de triangulação e inversão de UVs
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Erro ao carregar modelo com Assimp: "
            << importer.GetErrorString() << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0]; // Carrega apenas a primeira malha

    // Processa os vértices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // Posições dos vértices
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Coordenadas de textura
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f); // Coordenadas padrão se não houver
        }

        vertices.push_back(vertex);
    }

    // Processa os índices (faces)
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // Cada face pode ter vários índices (triângulos)
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Imprime informações do modelo
    std::cout << "Número de vértices: " << vertices.size() << std::endl;
    std::cout << "Número de índices: " << indices.size() << std::endl;
}

// Função para carregar uma textura a partir de um arquivo
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID); // Gera um ID para a textura

    int width, height, nrComponents;

    // Carrega a imagem usando stb_image
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        // Determina o formato da imagem
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = GL_RGB; // Padrão

        // Vincula e configura a textura
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
            format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Configura os parâmetros de textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repetição em S
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repetição em T
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Filtro minificação
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro magnificação

        stbi_image_free(data); // Libera a imagem da memória
        std::cout << "Textura carregada com ID: " << textureID << std::endl;
    }
    else {
        std::cerr << "Falha ao carregar a textura no caminho: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID; // Retorna o ID da textura
}

int main() {
    // Inicialização do GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }
    // Configuração da versão do OpenGL (3.3 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelo da Terra com Controle de Rotação", NULL, NULL);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Define o contexto OpenGL

    // Configuração de callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Redimensionamento
    glfwSetCursorPosCallback(window, mouse_callback);                  // Movimento do mouse

    // Oculta o cursor e captura o movimento do mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicialização do GLEW
    glewExperimental = GL_TRUE; // Ativa funcionalidades modernas do OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar GLEW" << std::endl;
        return -1;
    }

    // Configurações OpenGL
    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade

    // Carrega os shaders
    GLuint shaderProgram = createShaderProgram(
        "C:/Users/Eduardo/Downloads/final/shaders/vertex_shader.glsl",    // Caminho do vertex shader
        "C:/Users/Eduardo/Downloads/final/shaders/fragment_shader.glsl"   // Caminho do fragment shader
    );
    if (shaderProgram == 0) {
        std::cerr << "Erro ao criar shader program" << std::endl;
        return -1;
    }

    // Carrega o modelo 3D da Terra
    loadModel("C:/Users/Eduardo/Downloads/final/earth/13902_Earth_v1_l3.obj");

    // Verifica se o modelo foi carregado corretamente
    if (vertices.empty() || indices.empty()) {
        std::cerr << "Modelo não carregado corretamente." << std::endl;
        return -1;
    }

    // Carrega a textura da Terra
    unsigned int textureID = loadTexture("C:/Users/Eduardo/Downloads/final/earth/Earth_diff.jpg");

    // Configuração do VAO (Vertex Array Object), VBO (Vertex Buffer Object) e EBO (Element Buffer Object)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // Gera um VAO

    glGenBuffers(1, &VBO); // Gera um VBO
    glGenBuffers(1, &EBO); // Gera um EBO

    // Vincula o VAO
    glBindVertexArray(VAO);

    // Configura o VBO com os dados dos vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Configura o EBO com os índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Configuração dos atributos dos vértices
    // Posições
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Atributo 0: posição
    glEnableVertexAttribArray(0);
    // Coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)); // Atributo 1: texCoords
    glEnableVertexAttribArray(1);

    // Desvincula o VAO
    glBindVertexArray(0);

    // Variáveis para calcular o deltaTime (tempo entre quadros)
    float deltaTime = 0.0f; // Tempo entre quadros atuais e anteriores
    float lastFrame = 0.0f; // Tempo do último quadro

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        // Calcular deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Processa a entrada do usuário
        processInput(window, deltaTime);

        // Atualiza o ângulo de rotação automática
        autoRotateAngle += 10.0f * deltaTime; // Rotaciona 10 graus por segundo

        // Renderização
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Define a cor de fundo (preto)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e profundidade

        // Ativa o shader program
        glUseProgram(shaderProgram);

        // Configura as transformações
        glm::mat4 model = glm::mat4(1.0f); // Matriz identidade para o modelo

        // Aplica a translação do modelo (posição no espaço)
        model = glm::translate(model, glm::vec3(modelXPosition, modelYPosition, modelZPosition));
        model = glm::scale(model, glm::vec3(0.5f)); // Aplica uma escala ao modelo

        // Rotação controlada pelo mouse (aplicada ao modelo)
        model = glm::rotate(model, glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotação no eixo X
        model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));   // Rotação no eixo Y

        // Rotação automática (simulação da rotação da Terra)
        model = glm::rotate(model, glm::radians(autoRotateAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotação no eixo Y

        glm::mat4 view = glm::mat4(1.0f); // Matriz de visão (câmera na origem)

        // Matriz de projeção (perspectiva)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);

        // Envia as matrizes para o shader
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));       // Matriz modelo
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));         // Matriz visão
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));   // Matriz projeção

        // Vincula a textura
        glActiveTexture(GL_TEXTURE0); // Ativa a unidade de textura 0
        glBindTexture(GL_TEXTURE_2D, textureID); // Vincula a textura carregada
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // Define o sampler no shader

        // Renderiza o modelo
        glBindVertexArray(VAO); // Vincula o VAO
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0); // Desenha os elementos

        // Troca os buffers e processa eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza: deleta os buffers e arrays
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Encerra o GLFW
    glfwTerminate();
    return 0;
}
