#include <iostream>
#include <fstream>

using namespace std;

// Struct para armazenar uma imagem em formato PGM
typedef struct{
    string tipo;
    int largura;
    int altura;
    int valormax;
    int** pixels;
}imagemPGM;

// Implementada Função para ler a imagem original em formato .pgm
void lerImagem(imagemPGM &imagem, ifstream &arquivoleitura){
    arquivoleitura >> imagem.tipo;
    arquivoleitura >> imagem.largura;
    arquivoleitura >> imagem.altura;
    arquivoleitura >> imagem.valormax;

    // Alocando Memória para a variável pixels conforme a largura e altura da matriz presente na imagem.
    imagem.pixels = new int*[imagem.altura];
    for(int i = 0; i < imagem.altura; i++){
        imagem.pixels[i] = new int[imagem.largura];
        for(int j = 0; j < imagem.largura; j++){
            arquivoleitura >> imagem.pixels[i][j];
        }
    }
    
}

// Implementada Função para reescrever a imagem modificada em um novo arquivo .pgm
void finalizarImagem(imagemPGM &imagem, ofstream &arquivoescrita){
    arquivoescrita << imagem.tipo << endl;
    arquivoescrita << imagem.largura << " ";
    arquivoescrita << imagem.altura << endl;
    arquivoescrita << imagem.valormax << endl;

    for(int i = 0; i < imagem.altura; i++){
        for(int j = 0; j < imagem.largura;j++){
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