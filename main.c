#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>
#include "inicjacja.h"
#include "wypisywanie.h"
#include "ruch.h"

int main( int argc, char **argv){
	int i, j, pom;
	int opt;
	char pom_d='x';
	char *o=NULL;
	char *l=NULL;
	int it=0;
	int spr=0;
	mrowka ant;
	pole_m pole;
	pole.p=0;
	pole.m=pole.n=0;
	setlocale(LC_ALL, "C.UTF-8");

	//Przetwarzanie parametrów
	while ((opt=getopt(argc, argv, "m:n:i:o:d:l:p:")) != -1){
		switch (opt){
			case 'm':
				pole.m=atoi(optarg);
				break;
			case 'n':
				pole.n=atoi(optarg);
				break;
			case 'i':
				it=atoi(optarg);
				break;
			case 'o':
				o=optarg;
				break;
			case 'd':
				pom_d=optarg[0];
				break;
			case 'l':
				l=optarg;
				break;
			case 'p':
				pole.p=atof(optarg);
				spr=1;
				break;
			default:
				fprintf(stderr, "Użycie ./a.out -m <wiersze> -n <kolumn> -i <iteracje> -o <plik> -d <kierunek> -l(opcjonalny) <mapa do wczytania> -r(opcjonalny) <procentowe zapełnienie>\n");
				return EXIT_FAILURE;
		}
	}

	//Obsługa błędów
	if (pole.m<=0 || pole.n<=0){
		fprintf(stderr, "Należy podać dodatnią liczbę wierszy i kolumn.\n");
		return 1;
	}
	if (it<=0){
		fprintf(stderr, "Należy podać dodatnią liczbę iteracji.\n");
		return 1;
	}
	if (spr==1 && (pole.p<=0 || pole.p>100)){
		fprintf(stderr, "Niepoprawna wartość procentowego zapełnienia planszy.\n");
		return 1;
	}
	if (l!=NULL && pole.p>0){
		fprintf(stderr, "Proszę wybrać jedną z dwóch dodatkowych opcji.\n");
		return 1;
	}
	switch (pom_d){
		case 'N':
			ant.d=0;
			break;
		case 'E':
			ant.d=1;
			break;
		case 'S':
			ant.d=2;
			break;
		case 'W':
			ant.d=3;
			break;
		default:
			fprintf(stderr, "Podaj kierunek mrówki jako N E S lub W.\n");
			return 1;
	}

	//Inicjacja siatki
	pole.s=malloc(pole.m*sizeof(int*));
	for (i=0; i<pole.m; i++)
		pole.s[i]=malloc(pole.n*sizeof(int));
	
	//Obsługa wczytwania z pliku
	if (l!=NULL){
		pom=wczytaj(&ant, &pole, l);
		if (pom==1)
			return 1;
	}else{
		ant.y=(pole.m-1)/2;
		ant.x=(pole.n-1)/2;
		for (i=0; i<pole.m; i++)
			for (j=0; j<pole.n; j++)
				pole.s[i][j]=0;
		if (pole.p>0){
			los(&pole);
		}
	}

	//Obliczanie i wypisywanie etapów
	pom=wypisz(&ant, &pole, 0, o);
	if (pom==1)
		return 1;
	for (i=1; i<=it; i++){
		move(&ant, &pole);
		pom=wypisz(&ant, &pole, i, o);
		if (pom==1)
			return 1;
	}

	return 0;
}
