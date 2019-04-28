#include <SFML/Graphics.hpp>
#include <string>

////////////////////
// FORMA

class Forma {
public:
    // Construtores

    // Construtor Padrão
    Forma() : x{ 0.f }, y{ 0.f }, largura{ 0 }, altura{ 0 }, cor{ sf::Color(0, 0, 0, 0) } {}

    // Construtor sobrecarregado para outros parâmetros
    Forma(float _x, float _y, float _largura, float _altura, sf::Color _cor) {
        x = _y;
        y = _y;

        largura = _largura;
        altura = _altura;

        cor = _cor;
    }

    // Getters
    float getXPos() { return x; }
    float getYPos() { return y; }

    unsigned int getLargura() { return largura; }
    unsigned int getAltura() { return altura; }

    // Setters
    void setXPos(float _x) { x = _x; forma.setPosition(_x, getYPos()); }
    void setYPos(float _y) { y = _y; forma.setPosition(getXPos(), _y); }

    void setLargura(unsigned int _largura) { largura = _largura; }
    void setAltura(unsigned int _altura) { altura = _altura; }

    sf::RectangleShape getForma() { return forma; }

protected:
    // Propriedades
    float x;
    float y;

    unsigned int largura;
    unsigned int altura;

    sf::Color cor;
    sf::RectangleShape forma;
};

///////////////////////
// RAQUETE

class Raquete : public Forma
{
public:
    Raquete() {
        x = 0.f;
        y = 0.f;

        largura = 0;
        altura = 0;

        cor = sf::Color(0, 0, 0);

        // Configura depois do construtor
        forma.setPosition(sf::Vector2<float>(this->x, this->y));            // Posição
        forma.setSize(sf::Vector2<float>(this->largura, this->altura));     // Tamanho
        forma.setFillColor(this->cor);                                      // Cor
    }

    Raquete(float _x, float _y, unsigned int _largura, unsigned int _altura, sf::Color _cor) {
        x = _x;
        y = _y;

        largura = _largura;
        altura = _altura;

        cor = _cor;

        // Configura depois do construtor
        forma.setPosition(sf::Vector2<float>(x, y));            // Posição
        forma.setSize(sf::Vector2<float>(largura, altura));     // Tamanho
        forma.setFillColor(cor);                                // Cor
    }
};

///////////////////////
// PONTO

class Ponto : public Forma
{
public:
    Ponto(float _x, float _y, unsigned int _size, sf::Color _cor) {
        x = _x;
        y = _y;

        largura = _size;
        altura = _size;

        cor = _cor;

        // Configura depois do construtor
        forma.setPosition(sf::Vector2<float>(x, y));            // Posição
        forma.setSize(sf::Vector2<float>(largura, altura));     // Tamanho
        forma.setFillColor(cor);                                // Cor
    }
};

///////////////////////
// APPLICATION

class Application
{
public:
    Application(unsigned int largura, unsigned int altura, std::string titulo, bool telaCheia = false) {
        // Simplesmente cria uma janela com as proporções dadas por parâmetro
        janela.create(sf::VideoMode(largura, altura), titulo, (telaCheia) ? sf::Style::Fullscreen : sf::Style::Default);
        janela.setFramerateLimit(60);

        // Carrega a fonte
        fonte.loadFromFile("Resources/fonts/ARCADECLASSIC.TTF");
    }

    // Processa os eventos em loop
    void processaEventos() {
        while (janela.pollEvent(evento))
        {
            if (evento.type == sf::Event::Closed)
                janela.close();
        }
    }

    bool estaAberta() {
        return (janela.isOpen());
    }

    void limpaTela(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255) {
        janela.clear(sf::Color(r, g, b, a));
    }

    void desenha(Forma forma) {
        janela.draw(forma.getForma());
    }

    void trocaBuffers() {
        janela.display();
    }

    void destroiJanela() {
        janela.close();
    }

    bool checaColisao(Forma f1, Forma f2) {
        sf::FloatRect r1 = f1.getForma().getGlobalBounds();
        sf::FloatRect r2 = f2.getForma().getGlobalBounds();

        bool colisao = r1.intersects(r2);
        return colisao;
    }

    void mostraTexto(float _x, float _y, std::string _string, float _escala)
    {
        sf::Text _txt;
        _txt.setFont(fonte);
        _txt.setString(_string);
        _txt.setOutlineThickness(2.f);
        _txt.setFillColor(sf::Color::White);
        _txt.setPosition(_x, _y);
        _txt.setOutlineColor(sf::Color(60, 60, 60));
        _txt.setScale(_escala, _escala);

        janela.draw(_txt);
    }

private:
    sf::RenderWindow janela;
    sf::Event evento;
    sf::Font fonte;
};

///////////////////
// MAIN

int main()
{
    Application app(800, 600, "Pong");

    // Raquetes
    Raquete pad1(30, 250, 30, 100, sf::Color::White);
    Raquete pad2(740, 250, 30, 100, sf::Color::White);

    // "Bola"
    Ponto ball(380, 280, 20, sf::Color::White);
    float deslocamentoX = 320.f;
    float deslocamentoY = 320.f;

    // Gatilhos booleanos
    bool estaEmJogo = false;
    bool estaNoMenu = true;
    bool player1Win = false;
    bool player2Win = false;

    sf::Clock clock;

    // Game Loop
    while (app.estaAberta())
    {
        float deltaTimer = clock.restart().asSeconds();

        app.processaEventos();
        app.limpaTela(0, 0, 0);

        if (estaNoMenu)
        {
            app.mostraTexto(290, 150, "PONG", 3.f);
            app.mostraTexto(215, 350, "APERTE  ENTER  PARA  JOGAR!", 1.0f);

            // Checa pela tecla ENTER
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                estaEmJogo = true;
                estaNoMenu = false;
            }
        }

        else if (estaEmJogo)
        {
            // Interações do teclado (Raquete 1)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && pad1.getYPos() > 0.f)
                pad1.setYPos(pad1.getYPos() - 500.f * deltaTimer);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && pad1.getYPos() < 600.f - pad1.getAltura())
                pad1.setYPos(pad1.getYPos() + 500.f * deltaTimer);

            // Interações do teclado (Raquete 2)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && pad2.getYPos() > 0.f)
                pad2.setYPos(pad2.getYPos() - 500.f * deltaTimer);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && pad2.getYPos() < 600.f - pad2.getAltura())
                pad2.setYPos(pad2.getYPos() + 500.f * deltaTimer);

            // Checagem de colisão

            if (ball.getXPos() < 0.f)
            {
                player2Win = true;
                estaEmJogo = false;
            }

            if (ball.getXPos() > 800.f - ball.getLargura())
            {
                player1Win = true;
                estaEmJogo = false;
            }

            // Bola com teto
            if (ball.getYPos() < 0.f)
                deslocamentoY = -deslocamentoY;

            // Bola com chão
            if (ball.getYPos() > 600.f - ball.getAltura())
                deslocamentoY = -deslocamentoY;

            // Bola com raquete 1
            if (app.checaColisao(ball, pad1))
                deslocamentoX = -deslocamentoX;

            // Bola com raquete 2
            if (app.checaColisao(ball, pad2))
                deslocamentoX = -deslocamentoX;

            // Move a bola
            ball.setXPos(ball.getXPos() - deslocamentoX * deltaTimer);
            ball.setYPos(ball.getYPos() - deslocamentoY * deltaTimer);

            // Usa o polimorfismo para com uma classe base por parâmetro, poder passar todas que herdam da classe
            app.desenha(pad1);
            app.desenha(pad2);
            app.desenha(ball);
        }

        else if (player1Win)
        {
            app.mostraTexto(260, 250, "JOGADOR  1  VENCEU!", 1.f);
            app.mostraTexto(100, 270, "APERTE  ENTER  PARA  JOGAR  NOVAMENTE  OU", 1.f);
            app.mostraTexto(180, 290, "APERTE  ESC  PARA  SAIR  DO  JOGO", 1.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                ball.setXPos(380.f);
                ball.setYPos(280.f);

                pad1.setYPos(250.f);
                pad2.setYPos(250.f);

                estaEmJogo = true;
                player1Win = false;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                app.destroiJanela();
            }
        }

        else if (player2Win)
        {
            app.mostraTexto(260, 250, "JOGADOR  2  VENCEU!", 1.f);
            app.mostraTexto(100, 270, "APERTE  ENTER  PARA  JOGAR  NOVAMENTE  OU", 1.f);
            app.mostraTexto(180, 290, "APERTE  ESC  PARA  SAIR  DO  JOGO", 1.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                ball.setXPos(380);
                ball.setYPos(280);

                pad1.setYPos(250.f);
                pad2.setYPos(250.f);

                estaEmJogo = true;
                player2Win = false;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                app.destroiJanela();
            }
        }

        app.trocaBuffers();
    }

    return 0;
}
