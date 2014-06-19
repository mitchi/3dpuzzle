#include <iostream>
#include <vector>
#include <string>
using namespace std;


// globals
int dimension = 0; //3,4,5 etc
int cpt=0;

struct color
{
	char couleur;
	int count;
	int states[2];
};

color colors[255]; //This puts the max of colors at an arbitrary 10.

vector<color*> listeCouleurs;

struct cell
{
	int id; //pour debug
	vector<cell*> voisins;
	char color;

	//char originalState

};


vector<cell> cells;


string itos(int integer)
{
	char ascii[100];
	sprintf(ascii, "%d", integer);
	string s = ascii;
	return s;
}

void readMatrixLine(char* line)
{

	char *pc = line;
	while (1)
	{
		//end of line
		if (*pc == 10) break;

		if (*pc == ' ') {
			pc++;
			continue;
		}

		cell t;
		t.color = *pc;
		t.id = cpt++;
		cells.push_back(t);
		pc++;

		//Structure globale des couleurs
		if (t.color != 0 && t.color != 'X') {
			int count = colors[t.color].count; //on va chercher le count
			colors[t.color].couleur = t.color; //on set la couleur (debug surtout)
			colors[t.color].states[ count ] =  t.id; //on set le state
			colors[t.color].count++;

			//add dans le vecteur
			listeCouleurs.push_back( &colors[t.color] );
		}

	}


}

void readMatrix(char * filename)
{

	FILE * fichier = fopen(filename, "r");
	static char line[200];

	char * pc; //iterator

	//Read the dimension

	while (fgets(line, 200, fichier) != NULL) 
	{

		//Commentary, skip
		if (line[0] == ';') continue;

		//Empty line, skip
		if (line[0] == 10) continue;

		//Special case for the dimension
		if (line[0] == 'd') {
			dimension = line[1] + '0';
			continue;
		}

		pc = line;

		//Read a line of the matrix
		//readMatrixLine(pc);

		readMatrixLine(pc);

	}
}


int readIntegerBeforeX(char X, char  * &  pc)
{

	char buffer[200];
	memset(buffer, 0, 200);
	int i = 0;
		
	while (1)
	{
		//test#1, fin de la string
		if (*pc == 0) return -1;
		//caract de fin de ligne
		if (*pc == 10) break;
		//test#2, l'espace
		if (*pc == X) break;

		//On enregistre dans buffer
		buffer[i++] = *pc;
		pc++;

	}
	//On va pointer apres lespace
	pc++;
	int value = atoi(buffer);
	return value;

}


//This will crash if the matrix hasnt been read before (ofc)
void readGraph(char* filename)
{

	FILE * fichier = fopen(filename, "r");
	static char line[200];

	char * pc; //iterator

	//Read the dimension

	while (fgets(line, 200, fichier) != NULL) 
	{

		//Commentary, skip
		if (line[0] == ';') continue;

		//Empty line, skip
		if (line[0] == 10) continue;

		pc = line;

		//Read a line of the matrix

		//10 9,11,13,23
		int sommet = readIntegerBeforeX(' ', pc);

		while (1)
		{
			int s = readIntegerBeforeX(',', pc);
			if (s == -1 ) break; //fin des connexions
			//ajouter la connexion
			cells[sommet].voisins.push_back( &cells[s] );
		}




	} //fin de la boucle de lecture du fichier


}

//Fonction Recursive qui verifie si une couleur est valide
int checkSolutionColor(int startCell, int endCell)
{
	//Parcourir recursivement en profondeur


	return 1;


}


int main(void)
{
	
	//Read the puzzle
	readMatrix("puzzle.txt");

	//Read the graph
	readGraph("graph.txt");

	//Check a solution


	return 0;


}