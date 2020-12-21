#include "mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define posicao(I, J, COLUNAS) ((I)*(COLUNAS) + (J))
#define TAMANHO 8
#define MASTER 0
#define WORKER 1


/**
 *
 *  - TRABALHO DE PAD - Código puro sem ás devidas otimizações
 *  TELEPROG
 *    - Raphael Lira dos Santos 223865
 *    - Elziele Da Rocha 196396
 *
**/





/***
 *
 *
 *
 * - CABEÇALHO DAS FUNÇÕES
 *
 *
**/
float random_number();
float * alocar(int dimensaoA,int dimensaoB);
float * gerarMatriz(char * path,int dimensaoA,int dimensaoB);
float * lerArquivo(char * path,int dimensaoA,int dimensaoB);
float * calculaMatriz(float * matrizA, float * matrizB,int dimensaoA,int dimensaoB,int dimensaoC);
double reducaoMatriz(float * matriz, int dimensaoA, int dimensaoB);


/***
 *
 *
 *
 * - Declaração das variaveisVariavies 
 *
 *
**/

int 
    y,w,v, //variavel que guardará os valores da coluna
	i,j,k; // variaveis de controle


/***
 *
 *
 *
 * - FUNÇÕES
 *
 *
**/
//gerador de numeros aleatórios
float random_number(){
	return ((float) (rand() % 2000)/100)  - 10;
}


// aloca dinamicamente como proposto pelo professor
float * alocar(int dimensaoA,int dimensaoB){
	float * ponteiro;
	ponteiro = calloc(sizeof(float), dimensaoA * dimensaoB);	
	return ponteiro;
}


// gera a matriz de forma aleatória
float * gerarMatriz(char * path,int dimensaoA,int dimensaoB){
	
  	// manipula o arquivo para escrita
	FILE * arquivo;
	arquivo = fopen(path,"w");
	
  	//aloca e define o tamanho total
	float * matriz = alocar(dimensaoA,dimensaoB);	
	int MAX = dimensaoA * dimensaoB;
	
	int i = 0;
		
  	//faz o loop, atribuindo valor aleatório e salva o arquivo
	for(i = 0;i<MAX;i++){
		matriz[i] = random_number();
		fprintf(arquivo,"%.2f\n",matriz[i]);
	}
	
	fclose(arquivo);
	
	return matriz;
	
}


// le o arquivo 
float * lerArquivo(char * path,int dimensaoA,int dimensaoB){
	
  	//abre o arquivo para leitura
	FILE * arquivo;
	arquivo = fopen(path,"r");
	
  	//caso o arquivo não exista, gera um novo
	if(!arquivo){		
		return NULL;
	}
		
		
  	// aloca a matriz dinamicamente
	float * matriz = alocar(dimensaoA,dimensaoB);	
	int MAX = dimensaoA * dimensaoB;
	
	int i = 0;
	
 
   	// faz o loop de leitura
	for(i = 0;i<MAX;i++){
		fscanf(arquivo,"%f\n", &matriz[i]);	
	}
	
	fclose(arquivo);
	
	return matriz;
	
}



/**
*
*
* - Calcula a matriz (A x B)
*
*/
float * calculaMatriz(float * matrizA, float * matrizB,int dimensaoA,int dimensaoB,int dimensaoC){
	
	float
		* matriz = alocar(dimensaoA,dimensaoC);
   
	for(i=0;i<dimensaoA;i++){	       							
		for(j=0;j<dimensaoB;j++){	         						
			for(k=0;k<dimensaoC;k++){	
				matriz[posicao(i,k,dimensaoC)] += (matrizA[posicao(i,j,dimensaoB)] * matrizB[posicao(j,k,dimensaoC)]) ;		 								
			}      	
		}			
	}
	
	return matriz;
	
}

double reducaoMatriz(float * matriz, int dimensaoA, int dimensaoB){
    
    int
    	MAX = dimensaoA * dimensaoB;
    
	double
		resultado = 0;
    
	for(i=0;i<MAX;i++){             
		resultado += matriz[i];     
	}

	return resultado;
 
}


/***
 *
 *
 *
 * - MAIN
 *
 *
**/
int main(int argc,char ** argv){

  	// verifica se todos os argumentos estão
	if(argc < 8){
		printf("argumentos invalidos!\n");
		return 1;
	}	
	
	//aloca e le os arquivos do vetor
	float 
		* matrizA, 
		* matrizB, 
		* matrizC, 
		* matrizD,
		* matrizAB,
		* bufferRecvA;
	
	
	double
	  	reducao;	//salvara o resultado da redução
	  	
	int 
		quantProcs, rank; //mpi
 
	clock_t 
		tIni,tFim;  
		
	// atribui os valores de dimensão da matriz
   	y = atoi(argv[1]);
  	w = atoi(argv[2]);
  	v = atoi(argv[3]);	
  	
  	bufferRecvA = alocar((w*v)/TAMANHO,1);
  	
  	// aloca os dados nas matrizes  	
	matrizA = lerArquivo(argv[4],y,w); 
	matrizB = lerArquivo(argv[5],w,v); 
	matrizC = lerArquivo(argv[6],v,1); 
		
	MPI_Status Stat;

	// Inicializa o MPI
	MPI_Init(&argc,&argv);
	
	// Obtem a quantidade de processos
	MPI_Comm_size(MPI_COMM_WORLD, &quantProcs);
	
	// Obtem o rank (identificador) do processo
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//mestre
	if(rank == MASTER){
		
	 		
		if(y == 0 || w == 0 || v == 0){
			printf("Valor(es) y,w e/ou v invalido(s)!\n");
			return 1;
		}
		
	  	// caso falhe algum arquivo
		if(matrizA == NULL || matrizB == NULL || matrizC == NULL){	
			printf("Matriz(es) não carregada(s)!\n");	
			return 1;
		}
		
		//grava o tempo incial
   		tIni = clock();
		
	}  

	//broadcast da matriz C
	MPI_Bcast(matrizC,v, MPI_FLOAT, MASTER , MPI_COMM_WORLD);	   
 
	//broadcast da matriz B
	MPI_Bcast(matrizB,(w*v), MPI_FLOAT, MASTER , MPI_COMM_WORLD);		
 

	//scatter da matriz A    
    MPI_Scatter(matrizA, y * w/TAMANHO, MPI_FLOAT, bufferRecvA, y * w/TAMANHO, MPI_FLOAT,MASTER,MPI_COMM_WORLD);

	
	matrizAB = calculaMatriz(bufferRecvA,matrizB,y/TAMANHO,w,v);
	matrizD = calculaMatriz(matrizAB,matrizC,y/TAMANHO,v,1); 
	
	//MPI_Reduce(matrizD ,&reducao, v, MPI_FLOAT, MPI_SUM, MASTER,MPI_COMM_WORLD);
 
	//reducao = reducaoMatriz(matrizD,y/TAMANHO,1);	
	  
	//mestre
	if (rank == MASTER){
		//grava o tempo final
		tFim = clock();
		
	  	// printa a redução e o tempo
		printf("o resultado da reducao foi: %f - o tempo exercido foi de %f segundos\n",reducao,(double) (tFim - tIni)/CLOCKS_PER_SEC);
		
	  	// abre o arquivo para gravação da matriz D
		FILE * arquivo;
		arquivo = fopen(argv[7],"w");
		
	  	//faz o loop de gravação
		/*for(i=0;i<y;i++){
			fprintf(arquivo,"%.2f\n",matrizD[i]);
		}*/
		
		fclose(arquivo);
	}
		
		
	// Finaliza o MPI  
 	MPI_Finalize();
	
	return 0;  
   
}
