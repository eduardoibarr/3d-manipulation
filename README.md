# Projeto: Renderização de um Modelo 3D da Terra

Este projeto é uma aplicação C++ que carrega e renderiza um modelo 3D da Terra, utilizando OpenGL moderno. Ele permite ao usuário explorar o modelo interativamente, movimentando-o e rotacionando-o, enquanto a Terra gira automaticamente para simular o ciclo de dia e noite.

## Tecnologias Utilizadas

- **OpenGL**: Biblioteca para renderização gráfica em 3D.
- **GLFW**: Usado para criação de janelas e captura de eventos de entrada (teclado e mouse).
- **GLEW**: Gerenciamento de extensões do OpenGL, permitindo o uso de funcionalidades modernas.
- **GLM**: Biblioteca para realizar operações matemáticas com vetores e matrizes.
- **Assimp**: Utilizada para carregar o modelo 3D da Terra em formato `.obj`.
- **stb_image**: Biblioteca para carregar a textura da Terra.

## Funcionalidades do Projeto

- **Carregamento do Modelo 3D**: Utiliza a biblioteca Assimp para carregar um arquivo `.obj` que representa a Terra.
- **Aplicar Textura**: Usa `stb_image` para carregar a textura do planeta, adicionando realismo ao modelo.
- **Controle de Movimentação**: O modelo pode ser movimentado no espaço 3D utilizando as teclas de seta:
  - **Seta para cima**: Move o modelo para mais longe da câmera.
  - **Seta para baixo**: Move o modelo para mais perto da câmera.
  - **Seta para esquerda** e **direita**: Move lateralmente o modelo.
- **Rotacionar com o Mouse**: O usuário pode rotacionar o modelo com o movimento do mouse, permitindo uma exploração livre do planeta.
- **Rotacionar Automaticamente**: A Terra também gira automaticamente em torno de seu eixo, simulando o movimento de rotação real do planeta.

### Estrutura de Arquivos
- **main.cpp**: Arquivo principal contendo a lógica para carregar e renderizar o modelo 3D.
- **/shaders**: Contém os arquivos de shader (`vertex_shader.glsl` e `fragment_shader.glsl`) usados para aplicar transformações aos vértices e definir a cor dos pixels.
- **/earth**: Pasta contendo os arquivos `.obj` e `.jpg` que representam o modelo da Terra e sua textura.

## Controles do Usuário
- **Mouse**: Move o mouse para rotacionar o modelo.
- **Teclas de Seta**:
  - **Cima/Baixo**: Aproximar ou afastar o modelo da câmera.
  - **Esquerda/Direita**: Mover o modelo lateralmente.
- **ESC**: Fechar a aplicação.

## Estrutura e Funcionalidade

A aplicação é composta por três etapas principais:
1. **Carregamento do Modelo e Textura**: O modelo 3D é carregado do arquivo `.obj` usando Assimp, e a textura é aplicada usando stb_image.
2. **Inicialização do OpenGL**: Configura o contexto OpenGL, define os shaders e aplica as transformações necessárias para a renderização.
3. **Loop de Renderização**: Realiza a renderização do modelo enquanto o programa está em execução, atualizando a cada frame as matrizes de transformação para movimentar e rotacionar o modelo.

## Observações
- **Configuração de Caminhos**: Certifique-se de que os caminhos para os arquivos de textura, modelo, e shaders estejam corretos ao configurar o ambiente.
- **Shaders**: Os shaders estão escritos em GLSL (OpenGL Shading Language). Você precisará do vertex shader (`vertex_shader.glsl`) e fragment shader (`fragment_shader.glsl`) para que o modelo seja renderizado corretamente.

## Possíveis Melhorias Futuras
- **Iluminação**: Adicionar fontes de luz para aumentar o realismo da cena.
- **Controles Adicionais**: Implementar zoom usando a roda do mouse.
- **Textura Atmosférica**: Adicionar uma camada de atmosfera para tornar a visualização mais realista.

## Licença
Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para mais detalhes.

## Autor
Desenvolvido por Eduardo Ibarr de Paula. Qualquer dúvida ou sugestão, sinta-se à vontade para entrar em contato!

