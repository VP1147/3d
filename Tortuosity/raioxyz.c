/*	raio3d1.c Aplica o metodo monte carlo para  gerar canais tortuosos*/
/*calcula o modulo do campo eletrico Et(x,y,z)=Ex(x,y,z)+Ey(x,y,z)+Ez(x,y,z)*/
/* em forna de um tripolo com cargas Qp, Qn, Q1p em Hp, Hn e H1p imagens */
/* calcula a orientacao do vetor densidade de correntes (j) versao beta mar/2000 */
/* Autor modulo 2d: MOACIR LACERDA; Modulo 3d:EDUARDO A.CURVO*/
/* CSV parser: Vinicius Pavao */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// O x vai da origem ao y.

// Definindo os `prototipos` das funcoes
static int entradas();
static int calcepz();
static int calce1pz();
static int saidas();
static int calcjt();
static int calcenz();
static int calcet();


// Definindo as variaveis
double Epz,E1pz,Enz,Ez;
double Epx,E1px,Enx,Ex;
double Epy,E1py,Eny,Ey;
double Et,jx,jy,jz,jt,sigx,sigy,sigz,sigo;
float x,y,z,cp,minnz,unitx,unity,unitz,xEx,yEy,zEz,unitxEx,unityEy,unitzEz,Hp,Hn,H1p,Qp,Qn,Q1p,xo,yo,zo;
int nx,ny,nz,sx,sy,sz,i,j,m1,m2,m3,g;

#define MAX_LINE_LENGTH 	2048		// Comprimento max. do arquivo CSV
#define NUM_ITERACOES 		30   		// Numero de iteracoes
#define CLEAR_DAT					1				// 1 - Limpa o arquivo .dat ao iniciar

// Funcao principal
int main() {
	FILE *coord1;
	srand(time(NULL));

	printf("raioxyz.c - Aplica o metodo Monte Carlo para gerar canais tortuosos.\n");
	printf("Calcula o modulo do campo eletrico Et(x,y,z) = Ex(x,y,z) + Ey(x,y,z) + Ez(x,y,z)\n");
	printf("em forma de um tripolo com cargas Qp, Qn, Q1p em Hp, Hn e H1p.\n");
	printf("Calcula a orientacao do vetor densidade de correntes (j). Versao beta (mar. 2000)\n");
	printf("Modulo 2D: MOACIR LACERDA; Modulo 3D: EDUARDO AUGUSTO CURVO\n");
	printf("CSV parser: VINICIUS PAVAO");
	
 	if(CLEAR_DAT == 1) {
 		// Limpa o arquivo .dat
 		fclose(fopen("raio3d1.dat", "w"));
 	}
	entradas();

	g=0;
 	do {
 			coord1 = fopen("raio3d1.dat","at");
  		printf ("\n");
  		printf (" INICIANDO O PROGRAMA, rodada %i\n",g+1);
  		printf ("\n");
  		x=xo;
  		y=yo;
  		z=zo;
  		xEx=xo;
  		yEy=yo;
  		zEz=xo;
 			i = 0;
 		do {
 			calcepz();
 			calce1pz();
 			calcenz();
 			calcet();
 			m1 = 0;
 			m2 = 0;
 			m3 = 0;

  		do {
				sx = rand() % nx;
				m1 = m1 + 1;
			} while (m1 < 6000);

  		do {
				sy = rand() % ny;
				m2 = m2 + 1;
	 		} while (m2 < 6000);

  		do {
				sz = rand() % nz;
				m3 = m3 + 1;
	 		} while (m3 < 6000);

  		calcjt();

  		// Novas coordenadas
  		x = x + cp * unitx;
  		y = y + cp * unity;
  		z = z + cp * unitz;
  		xEx = xEx + cp * unitxEx;
  		yEy = yEy + cp * unityEy;
  		zEz = zEz + cp * unitzEz;

  		fprintf(coord1, "\n");
  		fprintf(coord1,"%f %f %f %f %f %f %d %d %d %f %f %f %f %f ",x,y,z,xEx,yEy,zEz,sx,sy,sz,unitz,unitzEz,Ex,Ey,Ez);
  		i = i + 1;
  	} while (z > 0);

  	fprintf(coord1, "\n");
		fclose(coord1);

		saidas();

 		g = g + 1;
 	} while (g < NUM_ITERACOES);

  return (0);
  }

// Valores definidos pelo cliente
float x,y,z,Hp,Qp,H1p,Q1p,Hn,Qn,xEx,xEy,zEz,xo,yo,zo,minnz;
int nx,ny,nz;

// Function to parse numbers from a CSV file
double *parse_csv_numbers(const char *filename, int *count) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    double *numbers = NULL;
    int capacity = 10; // Initial capacity
    int size = 0;

    // Open the CSV file
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate initial memory for the numbers array
    numbers = (double *)malloc(capacity * sizeof(double));
    if (numbers == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token;

        // Split the line into tokens based on commas
        token = strtok(line, ",");
        while (token != NULL) {
            // Convert the token to a number and store it in the array
            numbers[size] = atof(token);
            size++;

            // Resize the array if necessary
            if (size >= capacity) {
                capacity *= 2;
                numbers = (double *)realloc(numbers, capacity * sizeof(double));
                if (numbers == NULL) {
                    perror("Problema ao alocar memoria!\n");
                    fclose(file);
                    return NULL;
                }
            }

            // Get the next token
            token = strtok(NULL, ",");
        }
    }

    // Close the file
    fclose(file);

    // Resize the array to the actual number of elements
    numbers = (double *)realloc(numbers, size * sizeof(double));
    if (numbers == NULL && size > 0) {
        perror("Problema ao realocar memoria!\n");
        return NULL;
    }

    // Set the count of numbers
    *count = size;
    return numbers;
}

// Recebe os valores do arquivo CSV definidos pelo usuario
int entradas() {
	FILE *coord2;
	coord2 = fopen("raioen3d1.dat","at");

	const char *filename = "entrada.csv";
  int count;
  double *numbers;
	printf("Recebendo os parametros do arquivo 'entrada.csv'\n");
	numbers = parse_csv_numbers(filename, &count);
	if (numbers == NULL) {
		fprintf(stderr, "Falha ao abrir o arquivo!\n");
		return EXIT_FAILURE;
	}
	printf("%d valores lidos em 'entrada.csv'\n", count);
	x = numbers[0];
	printf("x = %fm\n", x);
	xo = x;

	y = numbers[1];
	printf("y = %fm\n", y);
	yo = y;

	z = numbers[2];
	printf("z = %fm\n", z);
	zo = z;

	xEx = x;
	yEy = y;
	zEz = z;

	cp = numbers[3];
	printf("cp = %fm\n", cp);

	Hp = numbers[4];
	printf("Hp = %fm\n", Hp);

	Qp = numbers[5];
	printf("Qp = %fC\n", Qp);

	H1p = numbers[6];
	printf("H1p = %fm\n", H1p);

	Q1p = numbers[7];
	printf("Q1p = %fC\n", Q1p);

	Hn = numbers[8];
	printf("Hn = %fm\n", Hn);

	Qn = numbers[9];
	printf("Qn = %fC\n", Qn);

	// nx (largura do randomico sx 0-nx):
	nx = numbers[10];
	printf("nx = %d\n", nx);

	// ny (largura do randomico sy 0-ny):
	ny = numbers[11];
	printf("ny = %d\n", ny);

	// nz (largura do randomico sz 0-nz):
	nz = numbers[12];
	printf("nz = %d\n", nz);

	// minnz -minimo valor percentual p/ sz=minnz+(0-nz):
	minnz = numbers[13];
	printf("minnz = %f\n", minnz);

	fprintf (coord2,"\n");
	fprintf (coord2,"%f %f %f %f %f %f %f %f %f %i %i %i",x,y,z,Hp,H1p,Hn,Qp,Q1p,Qn,nx,ny,nz);
	fclose(coord2);

}

float x, y, z, Hp, Qp, Qnim;
double Ka, Epz, Epx, Epy;

int calcepz() {
	Ka=-9*pow(10,9);
	Qnim=-Qp;
	Epz=Ka*(Qp*(z-Hp))/(pow(pow(Hp-z,2)+pow(x,2),1.5))+
		 Ka*(Qnim*(z+Hp))/(pow(pow(Hp+z,2)+pow(x,2),1.5));
	Epx=Ka*(Qp*x)/(pow(pow(Hp-z,2)+pow(x,2),1.5))+
		 Ka*(Qnim*x)/(pow(pow(Hp+z,2)+pow(x,2),1.5));
	Epy=Ka*(Qp*y)/(pow(pow(Hp-z,2)+pow(y,2),1.5))+
		 Ka*(Qnim*y)/(pow(pow(Hp+z,2)+pow(y,2),1.5));
	  return (0);
}

float x, y, z, H1p, Q1p, Q1nim;
double Ka, E1pz, E1px, E1py;

int calce1pz() {
	Ka=-9*pow(10,9);
	Q1nim=-Q1p;
	E1pz=Ka*(Q1p*(z-H1p))/(pow(pow(H1p-z,2)+pow(x,2),1.5)) +
		  Ka*(Q1nim*(z+H1p))/(pow(pow(H1p+z,2)+pow(x,2),1.5));
	E1px=Ka*(Q1p*x)/(pow(pow(H1p-z,2)+pow(x,2),1.5)) +
		  Ka*(Q1nim*x)/(pow(pow(H1p+z,2)+pow(x,2),1.5));
	E1py=Ka*(Q1p*y)/(pow(pow(H1p-z,2)+pow(y,2),1.5)) +   //Se der algum problema talvez seja aqui.
		  Ka*(Q1nim*y)/(pow(pow(H1p+z,2)+pow(y,2),1.5));
	  return (0);
}

float x, y, z, Hn, Qn, Qpim;
double Ka, Enz, Enx, Eny;

int calcenz() {
	 Ka=-9*pow(10,9);
	 Qpim=-Qn;
	Enz=Ka*(Qn*(z-Hn))/(pow(pow(Hn-z,2)+pow(x,2),1.5))+
		 Ka*(Qpim*(z+Hn))/(pow(pow(Hn+z,2)+pow(x,2),1.5));
	Enx=Ka*(Qn*x)/(pow(pow(Hn-z,2)+pow(x,2),1.5))+
		 Ka*(Qpim*x)/(pow(pow(Hn+z,2)+pow(x,2),1.5));
	Enx=Ka*(Qn*y)/(pow(pow(Hn-z,2)+pow(y,2),1.5))+
		 Ka*(Qpim*y)/(pow(pow(Hn+z,2)+pow(y,2),1.5));
	  return (0);
}

double Enz,Enx,Eny,E1pz,E1px,E1py,Enz,Enx,Eny,Ez,Ex,Ey,Et,jz,jx,jy,jt;
float x,y,z,Hp,Qp,H1p,Q1p,Hn,Qn,minnz,unitx,unity,unitz;
int sx,sy,sz;
int saidas() {
	printf ("\n");
	printf (" O valor de Enz  e':%f \n", Enz);
	printf (" O valor de Enx  e':%f \n", Enx);
	printf (" O valor de Eny  e':%f \n", Eny);
	printf ("\n");

	printf (" O valor de E1pz  e':%f \n", E1pz);
	printf (" O valor de E1px  e':%f \n", E1px);
	printf (" O valor de E1py  e':%f \n", E1py);
	printf ("\n");

 	printf ("\n");
	printf (" O valor de Epz  e':%f \n", Epz);
	printf (" O valor de Epx  e':%f \n", Epx);
	printf (" O valor de Epy  e':%f \n", Epy);
	printf ("\n");

	printf (" O valor de sx (numero randomico) e':%i \n", sx);
  printf (" O valor de sy (numero randomico) e':%i \n", sy);
	printf (" O valor de sz (numero randomico) e':%i \n", sz);
	printf ("\n");

	printf ("\n");
	printf (" minnz -minimo valor percentual p/ sz=minnz+(0-nz):%f \n ", minnz);
	printf ("\n");

 	printf (" O valor de Ez (total) e':%f \n", Ez);
  printf ("\n");
  printf (" O valor de Ex (total) e':%f \n", Ex);
  printf ("\n");
  printf (" O valor de Ey (total) e':%f \n", Ey);
  printf ("\n");
  printf (" O valor de Et (total) e':%f \n", Et);
  printf ("\n");
  printf (" O valor de jz  e':%f \n", jz);
  printf ("\n");
  printf (" O valor de jx e':%f \n", jx);
  printf ("\n");
  printf (" O valor de jy e':%f \n", jy);
  printf ("\n");
  printf (" O valor de jt (total) e':%f \n", jt);
  printf ("\n");
  printf (" O valor de unitx e':%f \n", unitx);
  printf ("\n");
  printf (" O valor de unity e':%f \n", unity);
  printf ("\n");
  printf (" O valor de unitz (total) e':%f \n", unitz);
  printf ("\n");
}


double Ez,Epz,E1pz,Enpz,Ex,Epx,E1px,Enx,Ey,Epy,E1py,Eny,Et;
int calcet() {
  Ez = Epz + E1pz + Enz;
  Ex = Epx + E1px + Enx;
  Ey = Epy + E1py + Eny;
  Et = pow(pow(Ex, 2) + pow(Ez, 2) + pow(Ey, 2), 0.5);
}

double sigx,sigy,sigz,sigo,Ex,Ey,Ez,jx,jy,jz,jt;
float unitx,unity,unitz,unitxEx,unityEy,unitzEz;

int calcjt() {
  sigo = 1;
  sigx = (sigo * sx) / 100;
  sigy = (sigo * sy) / 100;
  sigz = (sigo * (minnz + sz)) / 100;

  jx=sigx*Ex;
  jy=sigy*Ey;
  jz=sigz*Ez;
  jt=pow(jx*jx+jz*jz+jy*jy,0.5);
  unitx=jx/jt;
  unity=jy/jt;
  unitz=jz/jt;
  unitxEx=Ex/Et;
  unityEy=Ey/Et;
  unitzEz=Ez/Et;
  }