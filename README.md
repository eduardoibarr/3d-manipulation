# Projeto: Renderiza��o de um Modelo 3D da Terra

Este projeto � uma aplica��o C++ que carrega e renderiza um modelo 3D da Terra, utilizando OpenGL moderno. Ele permite ao usu�rio explorar o modelo interativamente, movimentando-o e rotacionando-o, enquanto a Terra gira automaticamente para simular o ciclo de dia e noite.

## Tecnologias Utilizadas

- **OpenGL**: Biblioteca para renderiza��o gr�fica em 3D.
- **GLFW**: Usado para cria��o de janelas e captura de eventos de entrada (teclado e mouse).
- **GLEW**: Gerenciamento de extens�es do OpenGL, permitindo o uso de funcionalidades modernas.
- **GLM**: Biblioteca para realizar opera��es matem�ticas com vetores e matrizes.
- **Assimp**: Utilizada para carregar o modelo 3D da Terra em formato `.obj`.
- **stb_image**: Biblioteca para carregar a textura da Terra.

## Funcionalidades do Projeto

- **Carregamento do Modelo 3D**: Utiliza a biblioteca Assimp para carregar um arquivo `.obj` que representa a Terra.
- **Aplicar Textura**: Usa `stb_image` para carregar a textura do planeta, adicionando realismo ao modelo.
- **Controle de Movimenta��o**: O modelo pode ser movimentado no espa�o 3D utilizando as teclas de seta:
  - **Seta para cima**: Move o modelo para mais longe da c�mera.
  - **Seta para baixo**: Move o modelo para mais perto da c�mera.
  - **Seta para esquerda** e **direita**: Move lateralmente o modelo.
- **Rotacionar com o Mouse**: O usu�rio pode rotacionar o modelo com o movimento do mouse, permitindo uma explora��o livre do planeta.
- **Rotacionar Automaticamente**: A Terra tamb�m gira automaticamente em torno de seu eixo, simulando o movimento de rota��o real do planeta.

### Estrutura de Arquivos
- **main.cpp**: Arquivo principal contendo a l�gica para carregar e renderizar o modelo 3D.
- **/shaders**: Cont�m os arquivos de shader (`vertex_shader.glsl` e `fragment_shader.glsl`) usados para aplicar transforma��es aos v�rtices e definir a cor dos pixels.
- **/earth**: Pasta contendo os arquivos `.obj` e `.jpg` que representam o modelo da Terra e sua textura.

## Controles do Usu�rio
- **Mouse**: Move o mouse para rotacionar o modelo.
- **Teclas de Seta**:
  - **Cima/Baixo**: Aproximar ou afastar o modelo da c�mera.
  - **Esquerda/Direita**: Mover o modelo lateralmente.
- **ESC**: Fechar a aplica��o.

## Estrutura e Funcionalidade

A aplica��o � composta por tr�s etapas principais:
1. **Carregamento do Modelo e Textura**: O modelo 3D � carregado do arquivo `.obj` usando Assimp, e a textura � aplicada usando stb_image.
2. **Inicializa��o do OpenGL**: Configura o contexto OpenGL, define os shaders e aplica as transforma��es necess�rias para a renderiza��o.
3. **Loop de Renderiza��o**: Realiza a renderiza��o do modelo enquanto o programa est� em execu��o, atualizando a cada frame as matrizes de transforma��o para movimentar e rotacionar o modelo.

## Observa��es
- **Configura��o de Caminhos**: Certifique-se de que os caminhos para os arquivos de textura, modelo, e shaders estejam corretos ao configurar o ambiente.
- **Shaders**: Os shaders est�o escritos em GLSL (OpenGL Shading Language). Voc� precisar� do vertex shader (`vertex_shader.glsl`) e fragment shader (`fragment_shader.glsl`) para que o modelo seja renderizado corretamente.

## Poss�veis Melhorias Futuras
- **Ilumina��o**: Adicionar fontes de luz para aumentar o realismo da cena.
- **Controles Adicionais**: Implementar zoom usando a roda do mouse.
- **Textura Atmosf�rica**: Adicionar uma camada de atmosfera para tornar a visualiza��o mais realista.

## Licen�a
Este projeto est� licenciado sob a Licen�a MIT - veja o arquivo LICENSE para mais detalhes.

## Autor
Desenvolvido por Eduardo Ibarr de Paula. Qualquer d�vida ou sugest�o, sinta-se � vontade para entrar em contato!

