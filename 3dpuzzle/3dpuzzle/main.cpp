#include <iostream>
#include <vector>
#include <string>
using namespace std;


// globals
int dimension = 0; //3,4,5 etc

struct cell
{
	vector<cell*> voisins;
	char color;

};


vector<cell> all_cells;


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
		all_cells.push_back(t);
		pc++;

	}


}

void readGraphFile(char * filename)
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

void convertMatrixToGraph(void)
{
	
	//La matrice est remplie de gauche a droite
	//18 19 20
	//21 22 23
	//24 25 26

	//(les indices, les connexions) 

 //24 25 26      20 23 26
	//0 1 2      9 10 11
	//3 4 5      12 13 14
	//6 7 8      15 16 17

	
	//Etape #1, faire les connexions de base
	//Exception : quand la matrice contient un X

	//Faire les connexions horizontales
	//for (int i = 0; i < all_cells.size(); i++)
	//{
	//	all_cells[i].voisins.push_back( &all_cells[i+1]);

	//}


	if (all_cells[0].color != 'X') {
		all_cells[0].voisins.push_back(all_cells[0+1]; //gauche






}

int main(void)
{
	
	//Read the puzzle
	readGraphFile("puzzle.txt");

	convertMatrixToGraph();




	return 0;


}