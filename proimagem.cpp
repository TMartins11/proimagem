#include <iostream>
#include <fstream>

using namespace std;

// Struct para armazenar uma imagem em formato PGM
typedef struct {
    string tipo;
    int colunas;
    int linhas;
    int valormax;
    int** pixels;
} imagemPGM;

// Função para ler a imagem original em formato .pgm
void lerImagem(imagemPGM &imagem, ifstream &arquivoleitura) {
    arquivoleitura >> imagem.tipo;
    arquivoleitura >> imagem.colunas;
    arquivoleitura >> imagem.linhas;
    arquivoleitura >> imagem.valormax;

    // Alocando Memória para a variável pixels conforme as colunas e linhas da matriz presente na imagem.
    imagem.pixels = new int*[imagem.linhas];
    for (int i = 0; i < imagem.linhas; i++) {
        imagem.pixels[i] = new int[imagem.colunas];
        for (int j = 0; j < imagem.colunas; j++) {
            arquivoleitura >> imagem.pixels[i][j];
        }
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

    while (opdesejada != 7) {
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
                    while (brilho == 0 || brilho > 100) {
                        cout << endl << "Você escolheu uma % inválida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho /= 100;
                    /* alterarbrilho(imagem, opbrilho, brilho); */
                } else if (opbrilho == 2) {
                    cout << endl << "Digite a % de brilho que você deseja retirar da imagem: ";
                    cin >> brilho;
                    while (brilho == 0 || brilho > 100) {
                        cout << endl << "Você escolheu uma % inválida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho /= 100;
                    /* alterarbrilho(imagem, opbrilho, brilho); */
                }
                break;
            case 2:
                encontrarNegativa(imagem);
                break;
            case 3:
                cout << "Digite o valor do limiar para binarizar a imagem: ";
                cin >> limiar;
                binarizarImagem(imagem, limiar);
                break;
            case 4:
                iconizarImagem(imagem, tamanhoBloco);
                cout << endl << "Pixelização concluída." << endl << endl;
                break;
        }
    }

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
    }
    delete[] imagem.pixels;
    
    return 0;
    
}

