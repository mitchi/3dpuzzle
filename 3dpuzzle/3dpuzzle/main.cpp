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
	bool original; //cant change this value
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
		t.original = false;
		t.color = *pc;
		t.id = cpt++;


		//Structure globale des couleurs
		if (t.color != '0' && t.color != 'X') {
			int count = colors[t.color].count; //on va chercher le count
			colors[t.color].couleur = t.color; //on set la couleur (debug surtout)
			colors[t.color].states[ count ] =  t.id; //on set le state
			//set the cell as an original
			t.original = true;

			colors[t.color].count++;

			//add dans le vecteur
			if (colors[t.color].count == 1)
				listeCouleurs.push_back( &colors[t.color] );
		}

		cells.push_back(t);
		pc++;

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
//One early optimisation we do here is that we remove the X cells
//Since they are not needed in the graph
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
		//skip these nodes
		if (cells[sommet].color == 'X')
			continue;


		while (1)
		{
			int s = readIntegerBeforeX(',', pc);
			if (s == -1 ) break; //fin des connexions
			//ajouter la connexion
			//skip these nodes
			if (cells[s].color == 'X')
			continue;

			cells[sommet].voisins.push_back( &cells[s] );
		}




	} //fin de la boucle de lecture du fichier


}

struct colorExplorer
{
	char currentColor;
	int startState;
	int endState;

	int visited[27]; //VLA array pls

	//Fuck you C si j'ai pas mes higher order functions je vais faire ça comme ça
	//cette fonction part du currentState et cherche endState recursivement
	int check(int state)
	{

		visited[state] = 1;

		if (state == endState) return 1; //succes

		//Pour toutes les connexions de cette cell
		for (int i=0; i< cells[state].voisins.size(); i++) 
		{
			cell* voisin = cells[state].voisins[i];
			if (voisin->color != currentColor) continue;
			if (visited[voisin->id] == 1) continue;

			int res = check(voisin->id);
			if (res == 1) return 1;
			else return 0;

		}

		return 0; //failure

	}


};

//Fonction Recursive qui verifie si une couleur est valide
int checkSolutionColor(color * c)
{
	//Parcourir recursivement en profondeur
	colorExplorer e;

	e.currentColor = c->couleur;
	e.startState = c->states[0];
	e.endState = c->states[1];

	int res = e.check( e.startState);
	if (res == 1) return 1;
	
	
	return 0;


}

void print_cells(void)
{

	for (int i = 0; i < cells.size(); i++) {

		//if (cells[i].original == true) printf("o%c ", cells[i].color);
		//else printf("r%c ", cells[i].color);

		printf("%c ", cells[i].color);

		if ( (i+1) % 3 == 0) printf("\n");

		if ( (i+1) % 9 == 0) printf("\n");

	}

	int u = 2+2;

}
int ddd = 0;
int checkAll(void)
{

	//printf("essai : %d\n", ++ddd);
	//print_cells(); //debug

	for (int i =0; i< listeCouleurs.size(); i++)
	{
		color * c = listeCouleurs[i];
		int res = checkSolutionColor(c);
		if (res == 0) return 0;
	}
	return -1;
}

struct bruteforce
{

	vector<cell*> liste;

	void init()
	{

		for (int i=0; i<cells.size(); i++)
		{
			cell * c = &cells[i];
			if (c->color == 'X' || c->original == true) continue;
			liste.push_back(c);
			
		}
	}

	int gen(int index)
	{

		if (index == liste.size() ) return 0;

			cell* c = liste[index];

			for(int i = 0; i < listeCouleurs.size(); i++)
			{
				c->color = listeCouleurs[i]->couleur;
				int res = gen(index+1); if (res == -1) return -1;
				if (checkAll() == -1)  {
					print_cells();
					return -1;
				}
			}

		return 0;
	}

};


int main(void)
{
	
	//freopen ("myfile.txt","w",stdout);

	//Read the puzzle
	readMatrix("puzzle.txt");

	//Read the graph
	readGraph("graph.txt");

	bruteforce b;
	b.init();
	int res = b.gen(0);
	if (res == -1) printf("succes");
	else printf("fail");


	 fclose (stdout);

	return 0;


}
