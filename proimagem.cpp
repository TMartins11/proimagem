#include <iostream>
#include <fstream>

using namespace std;

// Struct para armazenar uma imagem em formato PGM
typedef struct{
    string tipo;
    int colunas;
    int linhas;
    int valormax;
    int** pixels;
}imagemPGM;

// Implementada Função para ler a imagem original em formato .pgm
void lerImagem(imagemPGM &imagem, ifstream &arquivoleitura){
    arquivoleitura >> imagem.tipo;
    arquivoleitura >> imagem.colunas;
    arquivoleitura >> imagem.linhas;
    arquivoleitura >> imagem.valormax;

    // Alocando Memória para a variável pixels conforme as colunas e linhas da matriz presente na imagem.
    imagem.pixels = new int*[imagem.linhas];
    for(int i = 0; i < imagem.linhas; i++){
        imagem.pixels[i] = new int[imagem.colunas];
        for(int j = 0; j < imagem.colunas; j++){
            arquivoleitura >> imagem.pixels[i][j];
        }
    }
    
}

/*
// Função para Alterar o Brilho da Imagem conforme a vontade do usuário, a Clareando ou Escurecendo.
void alterarbrilho(imagemPGM &imagem, int opbrilho, int brilho){
    if(opbrilho == 1){
        for(int i = 0; i < imagem.linhas; i++){
            for(int j = 0; j < imagem.colunas; j++){
                imagem.pixels[i][j] = imagem.pixels[i][j] + (imagem.pixels[i][j] * brilho);
            }
        }
    }else if(opbrilho == 2){
        for(int i = 0; i < imagem.linhas; i++){
            for(int j = 0; j < imagem.colunas; j++){
                imagem.pixels[i][j] = imagem.pixels[i][j] - (imagem.pixels[i][j] * brilho);
            }
        }
    }
}
*/


// Implementada Função para reescrever a imagem modificada em um novo arquivo .pgm
void finalizarImagem(imagemPGM &imagem, ofstream &arquivoescrita){
    arquivoescrita << imagem.tipo << endl;
    arquivoescrita << imagem.colunas << " ";
    arquivoescrita << imagem.linhas << endl;
    arquivoescrita << imagem.valormax << endl;

    for(int i = 0; i < imagem.linhas; i++){
        for(int j = 0; j < imagem.colunas;j++){
            arquivoescrita << imagem.pixels[i][j] << " ";
        }
        arquivoescrita << endl;
    }
}

int main(){
    imagemPGM imagem;
    
    ifstream arquivoleitura("stanford.pgm");
    if(!arquivoleitura.is_open()){
        cout << endl << "O arquivo selecionado não existe!";
        return 1;
    }

    int opdesejada = 0;

    int opbrilho, brilho;

    while(opdesejada != 7){
        cout << "Processamento de Imagem" << endl;
        cout << endl << "Opções Disponíveis: " << endl;
        cout << endl << "1 - Alterar o Brilho da Imagem" << endl << "2 - Encontrar a Imagem Negativa" << endl << "3 - Binarizar a Imagem" << endl << "4 - Iconizar a Imagem";
        cout << endl << "5 - Criar Ruídos na Imagem" << endl << "6 - Aplicar Filtro" << endl << "7 - Sair" << endl;

        cout << endl << "Escolha a opção desejada: ";
        cin >> opdesejada;
        while(opdesejada < 1 || opdesejada > 7){
            cout << endl << "Opção Inválida!!! Insira Novamente: ";
            cin >> opdesejada;
        }

        switch(opdesejada){
            case 1:
                cout << endl << "Você deseja escurecer ou clarear a imagem?" << endl << endl << "1 - Clarear" << endl << "2 - Escurecer" << endl;
                cout << endl << "Digite a Opção desejada: ";
                cin >> opbrilho;
                while(opbrilho < 1 || opbrilho > 2){
                    cout << endl << "Opção Inválida!!! Digite novamente: ";
                    cin >> opbrilho;
                }
                if(opbrilho == 1){
                    cout << endl << "Digite a % de brilho que você deseja adicionar a imagem: ";
                    cin >> brilho;
                    while(brilho == 0 || brilho > 100){
                        cout << endl << "Você escolheu uma % invalida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho /= 100;
                }else if(opbrilho == 2){
                    cout << endl << "Digite a % de brilho que você deseja retirar da imagem: ";
                    cin >> brilho;
                    while(brilho == 0 || brilho > 100){
                        cout << endl << "Você escolheu uma % invalida!!! Digite novamente: ";
                        cin >> brilho;
                    }
                    brilho /= 100;
                }
                /* alterarbrilho(imagem, opbrilho, brilho); */
                break;
            case 2:

                break;
        }
    }
    lerImagem(imagem, arquivoleitura);
    
    arquivoleitura.close();

    
    
    
    
    
    
    
    ofstream arquivoescrita("imgmod.pgm");
    if(!arquivoescrita.is_open()){
        cout << endl << "O arquivo selecionado não existe!";
    }

    finalizarImagem(imagem, arquivoescrita);


    arquivoescrita.close();


    return 0;
}