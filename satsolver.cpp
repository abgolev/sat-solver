/*
To run: 
g++ satsolver.cpp
./a.out < filename.cnf
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>	//rand()
#include <ctime>	//srand()
#include <time.h>	//timer
#include <vector>

using namespace std;

int MAX_RUNS = 10000;

//Prints out two-dimensional array
void printMatrix(int c, int matrix[][3]){
	for(int i=0; i<c; i++){
		for(int j=0; j<3; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

//Output: 1 if the boolean formula is satisfied; 0 if not
int isSolved(int c, int matrix[][3]){
	for(int i=0; i<c; i++)
		if( !(matrix[i][0] || matrix[i][1] || matrix[i][2]) )
			return 0;
	return 1;	
}

//Output: Number of clauses in the boolean formula which are currently true
int clausesCorrect(int c, int matrix[][3]){
	int correct = c;
	for(int i=0; i<c; i++)
		if( !(matrix[i][0] || matrix[i][1] || matrix[i][2]) )
			correct = correct-1;
	return correct;	
}

//Output: Number of clauses correct if a certain variable is flipped in the variable list
int clausesCorrectFlippyVar(int c, int mBool[][3], int mVar[][3], int flippyVar){
	int correct = c;

	//We start by assuming all clauses are correct then subtract anytime we flip the variable and a clause is all 0's
	for(int i=0; i<c; i++){
		if(flippyVar==abs(mVar[i][0]))
			mBool[i][0]=(!mBool[i][0]);
		if(flippyVar==abs(mVar[i][1]))
			mBool[i][1]=(!mBool[i][1]);
		if(flippyVar==abs(mVar[i][2]))
			mBool[i][2]=(!mBool[i][2]);
		if(!(mBool[i][0] || mBool[i][1] || mBool[i][2]) )
			correct = correct-1;
		if(flippyVar==abs(mVar[i][0]))
			mBool[i][0]=(!mBool[i][0]);
		if(flippyVar==abs(mVar[i][1]))
			mBool[i][1]=(!mBool[i][1]);
		if(flippyVar==abs(mVar[i][2]))
			mBool[i][2]=(!mBool[i][2]);
	}
	return correct;	
}

//Prints correct variable assignment
void printSolution(int vnum, int cnum, int mat[][3], int var[], int truthmat[][3]){
        //output correct solution
        cout<<"Original Matrix here:"<<endl;
        printMatrix(cnum,mat);
	cout<<endl<<"Solution:"<<endl;
	for(int i=0; i<vnum; i++)
		cout<<i+1<<": "<<var[i+1]<<endl;
        cout<<endl<<"Truth Values of Matrix Here:"<<endl;
        	printMatrix(cnum,truthmat);
	return;
}

/*
For random clause, flip the bit that makes the most clauses be good
Resetting the truth matrix could be made more efficient here
Input: 
	v is number of variables
	c is number of clauses
	matrix contains each clause as a row and the three variables of each clause as columns
	vars is the assignments of variables as a 0 or 1
Output: Maximum number of clauses that are True
*/
int walkSat(int v, int c, int matrix[][3], int vars[]){
	int m[c][3]; //the truth matrix	
	int var;

	vector<int> varLocations;

	//cout<<"List of vars here:"<<endl;
	//for(int i=0; i<v; i++){
	//	cout<<vars[i+1]<<endl;
	//}

	//setting the truth matrix
	for(int i=0; i<c; i++){
		for(int j=0; j<3; j++){
			var = matrix[i][j];
			if(var>0)
				m[i][j]=(vars[abs(var)]==1);
			else
				m[i][j]=(vars[abs(var)]==0);
		}
	}

	int randClause;
	int prevCorrect = clausesCorrect(c, m);	//the previous # of clauses that are True
	int maxClauseCorrectVar = -1;

	if(prevCorrect==c){
		cout<<"WalkSat Solution:"<<endl;
		printSolution(v, c, matrix, vars, m);
		return c;
	}

	int newCorrect;	
	int changeVar;
	for(int i=0; i<MAX_RUNS; i++){
		randClause=rand()%c;
		for(int j=0; j<3; j++){
			changeVar = abs(matrix[randClause][j]);
			vars[changeVar] = !vars[changeVar];			
			newCorrect = clausesCorrectFlippyVar(c,m,matrix,changeVar);
			if(prevCorrect<=newCorrect){
				maxClauseCorrectVar = changeVar;
				prevCorrect = newCorrect;
			}
			vars[changeVar] = !vars[changeVar];
		}		

		if(maxClauseCorrectVar!=-1)
			vars[maxClauseCorrectVar] = !vars[maxClauseCorrectVar];

		//resetting the truth matrix
		for(int j=0; j<c; j++){
			for(int k=0; k<3; k++){
				var = matrix[j][k];
				if(var>0)
					m[j][k]=(vars[abs(var)]==1);
				else
					m[j][k]=(vars[abs(var)]==0);
			}
		}		

		prevCorrect = clausesCorrect(c,m);

		if(prevCorrect==c){
			cout<<"WalkSat Solution:"<<endl;
			printSolution(v, c, matrix, vars, m);
			return prevCorrect;
		}

		maxClauseCorrectVar=-1;
	}

	return prevCorrect;
}

//Local Search flips a random variable and keeps if flipped if this causes and improvement
//Returns # of clauses correct after final variable assignment
int localSearch(int v, int c, int matrix[][3], int vars[]){
	int m[c][3]; //the truth matrix	
	int var; //which variable in the var list

	//setting the truth matrix
	for(int i=0; i<c; i++){
		for(int j=0; j<3; j++){
			var = matrix[i][j];
			if(var>0)
				m[i][j]=(vars[abs(var)]==1);
			else
				m[i][j]=(vars[abs(var)]==0);
		}
	}

	if(isSolved(c,m)){
		cout<<"Local Search Solution:"<<endl;
		printSolution(v, c, matrix, vars, m);
		return c;
	}
	
	int walkSideways=0;	//walkSideways variable used if it runs into a wall
	int prevCorrect, newCorrect;
	prevCorrect = clausesCorrect(c,m);

	for(int i=1; i<MAX_RUNS; i++){
		for(int i=1; i<v*2+1; i++){
			if(i==v*2){
				if(walkSideways)
					return prevCorrect;
				else{
					walkSideways=1;
					break;
				}
			}

			int randVar = (rand()%v)+1;	
			vars[randVar] = !vars[randVar];

			//resetting the truth matrix
			for(int j=0; j<c; j++){
				for(int k=0; k<3; k++){
					var = matrix[j][k];
					if(abs(var)==randVar)
						m[j][k]=!m[j][k];
				}
			}

			if(isSolved(c,m)){
				cout<<"Local Search Solution:"<<endl;
				printSolution(v, c, matrix, vars, m);
				return c;
			}

			newCorrect = clausesCorrect(c,m);
			if((newCorrect<=prevCorrect&&!walkSideways) || (newCorrect<prevCorrect&&walkSideways)){
				//reverting back to original
				vars[randVar]=(!vars[randVar]);

				if(prevCorrect==newCorrect){
					walkSideways=0;
					break;
				}
	
				for(int j=0; j<c; j++){
					for(int k=0; k<3; k++){
						var = matrix[j][k];
						if(abs(var)==randVar)
							m[j][k]=!m[j][k];
					}
				}
			}
			else{
				if(newCorrect>prevCorrect){
					prevCorrect=newCorrect;
					break;
				}
				walkSideways=0;
			}
		}//end randVar for loop
	}

	if(isSolved(c,m)){
		cout<<"Local Search Solution:"<<endl;
		printSolution(v, c, matrix, vars, m);
		return c;
	}

	return prevCorrect;
}

/*
isItLiteral:
	-2 means no
	-1 means yes, all negated
	0 means we dont know
	1 means yes, all positive
*/

vector<int> pureLiteralCheck(int vars, int columns, int m[][3]){
	vector<int> literals;
	int isItLiteral[vars+1];
	int thisVar, sign;
	for(int i=0; i<=vars+1; i++)
		isItLiteral[i]=0;

	for(int i=0; i<columns; i++){
		for(int j=0; j<3; j++){
			thisVar = abs(m[i][j]);
			sign = (thisVar>0);	
			if(isItLiteral[thisVar]==0)
				isItLiteral[thisVar] = sign;
			else if(isItLiteral[thisVar]==-1){
				if(sign>0)
					isItLiteral[thisVar]=-2;
				else
					isItLiteral[thisVar]=-1;
			}
			else if(isItLiteral[abs(thisVar)]==1){
				if(sign>0)
					isItLiteral[thisVar]=1;
				else
					isItLiteral[thisVar]=-2;
			}
		}
	}

	for(int i=0; i<=vars+1; i++)
		if(isItLiteral[i]==-1 || isItLiteral[i]==1)
			literals.push_back(i*isItLiteral[i]);
	
	return literals;
}

int DPLL(int v, int c, int matrix[][3], int vars[]){
	//pure literal check
	vector<int> pureLiterals;
	pureLiteralCheck(v, c, matrix);
	//loop
	//unit clause setting
	//check for fail/success

	//splitter
}

int main(){
	string line;
	string firstVal = "";
	string secondVal = "";
	int nvar, nclause;	
	int stopper=0;
	clock_t start, start2, start3;
	double duration, duration2, duration3;

	srand(time(NULL));

	while(getline(cin,line)){
		if(line[11]=='p')
			line = line.substr(11);

		//Begin parsing the file at the first 'p' character
		if(line[0]=='p'){
			for(int i=2; i<line.length(); i++){
				if(line[i]==' '){
					for(int j=i+1; j<line.length(); j++){
						if(line[j]==' '){
							secondVal=line.substr(j+1, line.length());
							break;
						}
						firstVal+=line[j];
					}
				break;
				}
			}
			nvar = stoi(firstVal);
			nclause = stoi(secondVal);
			break;
		}	
	}

	int count=0;
	int varMatrix[nclause][3];	

	while(getline(cin,line)){
		if(count>nclause)
			cout<<"Error: number of clauses possible in the cnf file exceeded."<<endl;

		if(line[0]=='%' || line[0]=='c')
			break;

		int counter=0;
		string newVar="";
		int newVarInt;
		int firstChar=0;
		if(line[0]==' ')
			firstChar=1;

		//Parsing the file 
		//Making varMatrix[clause][variable]
		for(int i=firstChar; i<line.length(); i++){
			if(counter==3)
				break;

			if(line[i]==' '){
				newVarInt=stoi(newVar);	
				varMatrix[count][counter]=newVarInt;  //count is clause #; counter is var position in clause
				newVar="";	
				counter=counter+1;
				}
			else
				newVar+=line[i];
		}
		count=count+1;
	}
	
	int maxSolve, maxSolve2;

	double sumSolvedLocal = 0;
	double sumTimeLocal = 0;
	double propSolvedLocal = 0;

	double sumSolvedWalkSat = 0;
	double sumTimeWalkSat = 0;
	double propSolvedWalkSat = 0;

	int vars[nvar+1]; //the list of variables (+1 because we start at 0 but we don't use it)

	DPLL(nvar, nclause, varMatrix, vars);

	for(int i=0; i<10; i++){
		cout<<endl<<"Trial #"<<i<<endl;

		//The same list of random variable assignments goes into both local search and WalkSAT
		for(int i=1; i<nvar+1; i++)
			vars[i]=rand()%2;

		start = clock();
		maxSolve = localSearch(nvar, nclause, varMatrix, vars);
		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		sumSolvedLocal += maxSolve;
		sumTimeLocal += duration;
		propSolvedLocal += (maxSolve==nclause);

		start2 = clock();
		maxSolve2 = walkSat(nvar, nclause, varMatrix, vars);
		duration2 = (clock() - start2) / (double) CLOCKS_PER_SEC;
		sumSolvedWalkSat += maxSolve2;
		sumTimeWalkSat += duration2;
		propSolvedWalkSat += (maxSolve2==nclause);

	}

	cout<<"Number of Variables: "<<nvar<<endl;
	cout<<"Number of Clauses: "<<nclause<<endl<<endl;

	cout<<"Results After 10 Trials:"<<endl<<endl;

	cout<<"Local Search:"<<endl;
	cout<<"Average Duration: "<<duration/10<<endl;
	cout<<"Average # Clauses Solved: "<<1.0*sumSolvedLocal/10<<endl;
	cout<<"Percentage of Instances Found Satisfiable: "<<propSolvedLocal/10*100<<endl;

	cout<<endl<<"WalkSAT:"<<endl;
	cout<<"Average Duration: "<<duration2/10<<endl;
	cout<<"Average # Clauses Solved: "<<1.0*sumSolvedWalkSat/10<<endl;
	cout<<"Percentage of Instances Found Satisfiable: "<<propSolvedWalkSat/10*100<<endl;
	cout<<endl;

	return 0;
}
