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
			if (cells[sommet].color == 'X')
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

int checkAll(void)
{

	for (int i =0; i< listeCouleurs.size(); i++)
	{
		color * c = listeCouleurs[i];
		int res = checkSolutionColor(c);
		if (res == 0) return 0;
	}
	return 1;
}


struct generator
{

	//We need this data structureto explore a graph like a tree
	int visited[27]; //VLA array pls
	

	//Find the solution with DFS search
	//provide any starting node which is not a X node
	//Returns when the solution is found. The solution will be in the global state
	int find(int state)
	{

		//get the cell at this state
		cell * c = &cells[state];
		visited[state] = 1;

		//Check if the solution is valid
		int res = checkAll();
		if (res == 1) return 1;

		//if its not an original cell, it means we can change the color
		if (c->original = false) 
		{

			//Change the color of this cell and check if it works
			for (int i =0; i< listeCouleurs.size(); i++)
			{
				c->color = listeCouleurs[i]->couleur;

				for (int j = 0; j< c->voisins.size(); j++)
				{
					int next = c->voisins[j]->id;
					if (visited[next] == 1) continue; //eliminate the cycles
					if (find(next) == 1) return 1;
				}
			}
		}
		//original cell, just continue the search
		else 
		{
				for (int j = 0; j< c->voisins.size(); j++)
				{
					int next = c->voisins[j]->id;
					if (visited[next] == 1) continue; //eliminate the cycles
					if (find(next) == 1) return 1;
				}
		}

		return 0; //not found
	}

};


int main(void)
{
	
	//Read the puzzle
	readMatrix("puzzle.txt");

	//Read the graph
	readGraph("graph.txt");

	int res;
	//Check a solution for the first color
	color * first = listeCouleurs[0];
	// res = checkSolutionColor(first);

	//should work
	//cells[1].color = '1';
	//cells[4].color = '1';

	//res = checkSolutionColor(first);

	generator g;
	res = g.find(0);
	if (res == 1) printf("Succes\n");
	else printf("bouuu\n");



	return 0;


}

//Try everything. Stops when the  solution is found
//void tryAll()
//{
//	
//	for (int i = 0; i < cells.size() ; i++) 
//	{
//		cell * current = &cells[i];
//		if (current->color == 'X')
//			continue;  //idee : les cells "X" peuvent être enlevees, elles ne servent a rien quand on a notre graphe?
//
//		//skip quand cest une originale
//		if (current->original == true)
//			continue;
//
//		//Sinon essayer toutes les couleurs
//		for (int j =0; j< listeCouleurs.size(); j++)
//		{
//			current->color = listeCouleurs[j]->couleur;
//
//			int res = checkAll();
//			if (res == 1) {
//				printf("succes");
//				return;
//			}
//
//		}
//
//	}
//
//	printf("fail");
//
//
//}