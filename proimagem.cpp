#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

// Struct para armazenar uma imagem em formato PGM
typedef struct {
    string tipo;
    int colunas;
    int linhas;
    int valormax;
    int** pixels;
    int** pixelsOriginal; // Adicionando matriz para armazenar a imagem original (Aplicar filtro posteriormente)
} imagemPGM;

// Função para ler a imagem original em formato .pgm
void lerImagem(imagemPGM &imagem, ifstream &arquivoleitura) {
    arquivoleitura >> imagem.tipo;
    arquivoleitura >> imagem.colunas;
    arquivoleitura >> imagem.linhas;
    arquivoleitura >> imagem.valormax;

    // Alocando Memória para a variável pixels conforme as colunas e linhas da matriz presente na imagem
    imagem.pixels = new int*[imagem.linhas];
    imagem.pixelsOriginal = new int*[imagem.linhas]; 
    for (int i = 0; i < imagem.linhas; i++) {
        imagem.pixels[i] = new int[imagem.colunas];
        imagem.pixelsOriginal[i] = new int[imagem.colunas]; 
        for (int j = 0; j < imagem.colunas; j++) {
            arquivoleitura >> imagem.pixels[i][j];
            imagem.pixelsOriginal[i][j] = imagem.pixels[i][j];
        }
    }
}

// Função para alterar o brilho da imagem
void alterarbrilho(imagemPGM &imagem, int opbrilho, int brilho) {
    // Clarear a imagem
    if (opbrilho == 1) { 
        for (int i = 0; i < imagem.linhas; i++) {
            for (int j = 0; j < imagem.colunas; j++) {
                int novoPixel = imagem.pixels[i][j] + brilho;
                imagem.pixels[i][j] = min(imagem.valormax, novoPixel); // Garante que o valor não ultrapasse o máximo
            }
        }
        cout << endl << "Brilho aumentado com sucesso!" << endl << endl;
    } else if (opbrilho == 2) { // Escurecer a imagem
        for (int i = 0; i < imagem.linhas; i++) {
            for (int j = 0; j < imagem.colunas; j++) {
                int novoPixel = imagem.pixels[i][j] - brilho;
                imagem.pixels[i][j] = max(0, novoPixel); // Garante que o valor não seja menor que 0
            }
        }
        cout << endl << "Brilho diminuído com sucesso!" << endl << endl;
    }
}

// Função para encontrar a imagem negativa
void encontrarNegativa(imagemPGM &imagem) {
    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            imagem.pixels[i][j] = imagem.valormax - imagem.pixels[i][j];
        }
    }
}

// Função para binarizar a imagem
void binarizarImagem(imagemPGM &imagem, int limiar) {
    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            if (imagem.pixels[i][j] > limiar) {
                imagem.pixels[i][j] = imagem.valormax;
            } else {
                imagem.pixels[i][j] = 0;
            }
        }
    }
}

// Função para iconizar a imagem para 64x64 pixels
void iconizarImagem(imagemPGM &imagem, int tamanhoBloco) {
    int novaLargura = 64;
    int novaAltura = 64;
    
    // Calcula o tamanho do bloco em termos do tamanho original da imagem
    int blocoLargura = imagem.colunas / novaLargura;
    int blocoAltura = imagem.linhas / novaAltura;

    // Itera sobre a nova imagem de 64x64 pixels
    for (int i = 0; i < novaAltura; i++) {
        for (int j = 0; j < novaLargura; j++) {
            int soma = 0;
            int contador = 0;

            // Calcula a média dos valores dentro do bloco
            for (int x = i * blocoAltura; x < (i + 1) * blocoAltura; x++) {
                for (int y = j * blocoLargura; y < (j + 1) * blocoLargura; y++) {
                    soma += imagem.pixels[x][y];
                    contador++;
                }
            }

            // Calcula a média
            int media = soma / contador;

            // Preenche o bloco com a média
            for (int x = i * blocoAltura; x < (i + 1) * blocoAltura; x++) {
                for (int y = j * blocoLargura; y < (j + 1) * blocoLargura; y++) {
                    imagem.pixels[x][y] = media;
                }
            }
        }
    }

    // Ajusta as dimensões da imagem para 64x64 pixels
    imagem.linhas = novaAltura;
    imagem.colunas = novaLargura;
}

// Função para gerar ruídos na imagem
void gerarRuidos(imagemPGM &imagem, int nivelRuido) {
    srand(time(NULL));

    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            int ruido = rand() % (nivelRuido + 1) - nivelRuido / 2;
            imagem.pixels[i][j] = min(imagem.valormax, max(0, imagem.pixels[i][j] + ruido));
        }
    }

    cout << endl << "Ruído adicionado à imagem com sucesso!" << endl << endl;
}

// Função para aplicar filtro médio na imagem
void aplicarFiltro(imagemPGM &imagem, int tamanhoFiltro) {
    // Matriz temporária para armazenar os pixels modificados
    int** tempPixels = new int*[imagem.linhas];
    for (int i = 0; i < imagem.linhas; i++) {
        tempPixels[i] = new int[imagem.colunas];
    }

    // Aplicando o filtro de média
    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            int soma = 0;
            int contador = 0;

            // Calculando a média dos pixels ao redor
            for (int x = max(0, i - tamanhoFiltro); x <= min(imagem.linhas - 1, i + tamanhoFiltro); x++) {
                for (int y = max(0, j - tamanhoFiltro); y <= min(imagem.colunas - 1, j + tamanhoFiltro); y++) {
                    soma += imagem.pixelsOriginal[x][y]; // Utiliza a matriz original
                    contador++;
                }
            }

            // Calculando a média
            int media = soma / contador;
            tempPixels[i][j] = media;
        }
    }

    // Copiando os pixels filtrados de volta para a matriz principal
    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            imagem.pixels[i][j] = tempPixels[i][j];
        }
    }

    // Liberando memória da matriz temporária
    for (int i = 0; i < imagem.linhas; i++) {
        delete[] tempPixels[i];
    }
    delete[] tempPixels;

    cout << endl << "Filtro aplicado com sucesso!" << endl << endl;
}

// Função para reescrever a imagem modificada em um novo arquivo .pgm
void finalizarImagem(imagemPGM &imagem, ofstream &arquivoescrita) {
    arquivoescrita << imagem.tipo << endl;
    arquivoescrita << imagem.colunas << " ";
    arquivoescrita << imagem.linhas << endl;
    arquivoescrita << imagem.valormax << endl;

    for (int i = 0; i < imagem.linhas; i++) {
        for (int j = 0; j < imagem.colunas; j++) {
            arquivoescrita << imagem.pixels[i][j] << " ";
        }
        arquivoescrita << endl;
    }
}

// Função implementada somente para melhorar a experiência do usuário, adicionar um delay entre as reaprições do menu.
void delay(int seconds) {
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait) {}
}

int main() {
    imagemPGM imagem;

    ifstream arquivoleitura("stanford.pgm");
    if (!arquivoleitura.is_open()) {
        cout << endl << "O arquivo selecionado não existe!";
        return 1;
    }

    lerImagem(imagem, arquivoleitura);
    arquivoleitura.close();

    int opdesejada = 0;
    int opbrilho, brilho;
    int limiar;
    int tamanhoBloco = 8; // Tamanho padrão do bloco para iconizar

    do {
        cout << "Processamento de Imagem" << endl;
        cout << endl << "Opções Disponíveis: " << endl;
        cout << endl << "1 - Alterar o Brilho da Imagem" << endl << "2 - Encontrar a Imagem Negativa" << endl << "3 - Binarizar a Imagem" << endl << "4 - Iconizar a Imagem";
        cout << endl << "5 - Criar Ruídos na Imagem" << endl << "6 - Aplicar Filtro" << endl << "7 - Sair" << endl;

        cout << endl << "Escolha a opção desejada: ";
        cin >> opdesejada;
        while (opdesejada < 1 || opdesejada > 7) {
            cout << endl << "Opção Inválida!!! Insira Novamente: ";
            cin >> opdesejada;
        }

        switch (opdesejada) {
            case 1:
                cout << endl << "Você deseja escurecer ou clarear a imagem?" << endl << endl << "1 - Clarear" << endl << "2 - Escurecer" << endl;
                cout << endl << "Digite a Opção desejada: ";
                cin >> opbrilho;
                while (opbrilho < 1 || opbrilho > 2) {
                    cout << endl << "Opção Inválida!!! Digite novamente: ";
                    cin >> opbrilho;
                }
                if (opbrilho == 1) {
                    cout << endl << "Digite a % de brilho que você deseja adicionar a imagem: ";
                    cin >> brilho;
                    while (brilho <= 0 || brilho > 100) {
                        cout << endl << "Você escolheu uma % inválida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho = brilho * 255 / 100;
                    alterarbrilho(imagem, opbrilho, brilho); 
                } else if (opbrilho == 2) {
                    cout << endl << "Digite a % de brilho que você deseja retirar da imagem: ";
                    cin >> brilho;
                    while (brilho <= 0 || brilho > 100) {
                        cout << endl << "Você escolheu uma % inválida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho = brilho * 255 / 100;
                    alterarbrilho(imagem, opbrilho, brilho);
                }
                break;
            case 2:
                encontrarNegativa(imagem);
                cout << endl << "Negativando imagem..." << endl << endl;
                cout << endl << "Imagem negativada com sucesso!" << endl << endl;
                break;
            case 3:
                cout << endl << "Digite o valor do limiar para binarizar a imagem: ";
                cin >> limiar;
                binarizarImagem(imagem, limiar);
                cout << endl << "Imagem binarizada com sucesso!" << endl << endl;
                break;
            case 4:
                iconizarImagem(imagem, tamanhoBloco);
                cout << endl << "Pixelização concluída!" << endl << endl;
                break;
            case 5:
                int nivelRuido;
                cout <<  endl << "Digite o nível de ruído desejado: ";
                cin >> nivelRuido;
                gerarRuidos(imagem, nivelRuido);
                break;
            case 6:
                int tamanhoFiltro;
                cout << endl << "Digite o tamanho do filtro (em pixels) para aplicar na imagem: ";
                cin >> tamanhoFiltro;
                int tamanhoMaximoFiltro = min(imagem.colunas, imagem.linhas) / 2;
                aplicarFiltro(imagem, tamanhoFiltro);
                break;
        }
        delay(1);
    } while (opdesejada != 7);

    cout << endl << "Finalizando o programa..." << endl;

    ofstream arquivoescrita("imgmod.pgm");
    if (!arquivoescrita.is_open()) {
        cout << endl << "O arquivo selecionado não existe!";
        return 1;
    }

    finalizarImagem(imagem, arquivoescrita);
    arquivoescrita.close();

    // Liberação da memória alocada para a matriz de pixels
    for (int i = 0; i < imagem.linhas; i++) {
        delete[] imagem.pixels[i];
        delete[] imagem.pixelsOriginal[i];
    }
    delete[] imagem.pixels;
    delete[] imagem.pixelsOriginal;

    return 0;
}