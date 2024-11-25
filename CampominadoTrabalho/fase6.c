#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

#define MAX_NAME 50
#define STATS_FILE "estatisticas.bin"

int linhas, colunas;
int dificuldade;

typedef struct {
    char nome[MAX_NAME];
    int pontuacao;
    char fase[10];
} Estatisticas;

typedef struct {
    int **minas;
    int **visivel;
} Tabuleiro;

void inicializarTabuleiro(Tabuleiro *t);
void imprimirTabuleiro(Tabuleiro *t, int revelar);
void colocarMinas(Tabuleiro *t, int *minas);
void jogar(Tabuleiro *tabuleiro, const char* jogadorNome);
int revelarCelula(Tabuleiro *t, int linha, int coluna);
int verificarVitoria(Tabuleiro *t);
int contarMinasVizinhas(Tabuleiro *t, int linha, int coluna);
char* traduzir(int num);
void salvarEstatisticas(const char* nome, int pontuacao);
void exibirEstatisticas();

int main() {
    int opcao;
    char nome[MAX_NAME];
    Tabuleiro tabuleiro;
	setlocale(LC_ALL, "Portuguese");

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Iniciar Jogo\n");
        printf("2. Estatisticas\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite seu nome: ");
                scanf("%s", nome);
                jogar(&tabuleiro, nome);
                break;
            case 2:
               exibirEstatisticas();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção invalida!\n");
        }
    } while (opcao != 3);
    return 0;
    
}

void exibirEstatisticas() {
    FILE* file = fopen(STATS_FILE, "rb");
    if (!file) {
        printf("Nenhuma estatistica encontrada.\n");
        return;
    }

    Estatisticas est;
    printf("\nEstatisticas dos Jogadores:\n");
    while (fread(&est, sizeof(Estatisticas), 1, file)) {
        printf("%s Nome: %s, Pontuacao: %d\n ", est.fase, est.nome, est.pontuacao);
    }
    fclose(file);
}

void jogar(Tabuleiro *tabuleiro, const char* jogadorNome){
	int minas;
    inicializarTabuleiro(tabuleiro);
    colocarMinas(tabuleiro, &minas);
    int linha, coluna, resultado, pontuacao = 0;
	int verificar = 0;
    while (1) {
    	do{
	        imprimirTabuleiro(tabuleiro, 0);
    		if(verificar){
    			printf("\nErro, digite um valor valido\n");
			}
	        printf("Para desistir coloque -1 em ambos\n");
	        printf("Digite a linha: ");
	        scanf("%d", &linha);
	        
	        printf("Digite a coluna: ");
	        scanf("%d", &coluna);
			
	        if (linha == -1 && coluna == -1) {
	            printf("Você desistiu!\n");
	            system("pause");
	            break;
	        }
	        verificar = 1;
		} while(linha >= linhas || linhas < -1 || coluna >= colunas || colunas < -1);
		verificar = 0;
		
		
        resultado = revelarCelula(tabuleiro, linha, coluna);
	
        if (resultado == -1) {
            imprimirTabuleiro(tabuleiro, 1);
            printf("Bateu em uma mina!\n");
            system("pause");
            break;
            
        } else if (verificarVitoria(tabuleiro)) {
        	
            imprimirTabuleiro(tabuleiro, 1);
            printf("Você venceu!\n");
            pontuacao = 50;
            system("pause");
            break;
        }
    }
    
    int i, j;
    
	
	int cont = 0;
	for(i = 0 ; i < linhas ; i++){
		for( j = 0 ; j < colunas ; j++){
			if(tabuleiro->visivel[i][j] == 1){
				cont++;
			}
		}
	}
	if(dificuldade == 1){
		pontuacao += 55;
	}
	else{
		pontuacao += 60;	
	}
	pontuacao += cont + minas;
	cont = 0;
    salvarEstatisticas(jogadorNome, pontuacao);
}

void salvarEstatisticas(const char* nome, int pontuacao) {
    Estatisticas est = {0};
    strncpy(est.fase, "Fase 6: ", 10);
    strncpy(est.nome, nome, MAX_NAME - 1);
    est.pontuacao = pontuacao;

    FILE* file = fopen(STATS_FILE, "ab");
    if (file) {
        fwrite(&est, sizeof(Estatisticas), 1, file);
        fclose(file);
    } else {
        printf("Erro ao salvar estatisticas!\n");
    }
}


int revelarCelula(Tabuleiro *t, int linha, int coluna) {	
    t->visivel[linha][coluna] = 1;
    
    if (t->minas[linha][coluna]) {
        return -1; 
    }
    return 0;
    
}

void inicializarTabuleiro(Tabuleiro *t){
	int i, j, verificar = 0;
	do{
		if(verificar){
			printf("\nErro, digite valores validos: \n");	
		}
		printf("Digite a quantidade de linhas (10 MIN) (10 MAX): ");
		scanf("%d", &linhas);
		printf("Digite a quantidade de colunas (10 MIN) (10 MAX): ");
		scanf("%d", &colunas);
		system("cls");
		while(getchar() != '\n');
		
		verificar = 1;
	} while(!(linhas == 10 && colunas == 10 ));
	verificar = 0;
	t->visivel = (int **) malloc(sizeof(int**) * linhas);
	for(i = 0 ; i < linhas ; i++){
		t->visivel[i] = (int*) malloc(sizeof(int*) * colunas);
	}
	
	if(t->visivel == NULL){
		printf("Erro ao alocar memoria");
		exit(1);
	}
	
	t->minas = (int **) malloc(sizeof(int**) * linhas);
	for(i = 0 ; i < linhas ; i++){
		t->minas[i] = (int*) malloc(sizeof(int*) * colunas);
	}
	
	if(t->minas == NULL){
		printf("Erro ao alocar memoria");
		exit(1);
	}
	
    for ( i = 0; i < linhas; i++) {
        for ( j = 0; j < colunas; j++) {
            t->minas[i][j] = 0;
            t->visivel[i][j] = 0;
        }
    }
    system("cls");
}

void imprimirTabuleiro(Tabuleiro *t, int revelar) {
	system("cls");
	printf("Dificuldade: %s\n", traduzir(dificuldade));
    printf("\n   ");
    int j , i;
    for ( j = 0; j < colunas ; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for ( i = 0; i < linhas; i++) {
        printf("%2d ", i);
        for ( j = 0; j < colunas; j++) {
            if (revelar || t->visivel[i][j]) {
                if (t->minas[i][j]) {
                    printf(" * ");
                } else {
                    printf(" %d ", contarMinasVizinhas(t, i, j));
                }
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}
char* traduzir(int num){
	switch(num){
		case 1: return "Impossivel (95%)";
		case 2: return "Inferno Infinito (99%)";
		default: NULL;
	}
}

void colocarMinas(Tabuleiro *t, int *minas){
	srand(time(NULL));
	int quant =  linhas * colunas;
    int minas_colocadas = 0;
    int opcao;
    do{
	    printf("Escolha a dificuldade: (Quantidade de bombas)\n");
	    
	    printf("[1] Impossivel (95%%)\n");
	    printf("[2] Inferno Infinito (99%%)\n");
	    scanf("%d", &opcao);
	    
	    switch(opcao){
	    	case 1:
	    		quant = quant * 0.95;
	    		break;
	    	case 2:
	    		quant = quant * 0.99;
	    		break;
	    	default:
	    		printf("Digite uma opcao valida\n");
				break;
		}
	}while(opcao <= 0 || opcao > 5);
	dificuldade = opcao;	
    *minas = (int) quant;
    while (minas_colocadas < *minas) {
        int lin = rand() % linhas;
        int cols = rand() % colunas;
        if (t->minas[lin][cols] == 0) {
            t->minas[lin][cols] = 1;
            minas_colocadas++;
        }
    }
}

int verificarVitoria(Tabuleiro *t) {
	int i, j;
    for ( i = 0; i < linhas; i++) {
        for ( j = 0; j < colunas; j++) {
            if (!t->minas[i][j] && !t->visivel[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}
int contarMinasVizinhas(Tabuleiro *t, int linha, int coluna) {
    int count = 0, i, j;
    for ( i = -1; i <= 1; i++) {
        for ( j = -1; j <= 1; j++) {
            int ni = linha + i;
            int nj = coluna + j;
            if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas && t->minas[ni][nj]) {
                count++;
            }
        }
    }
    return count;
}

