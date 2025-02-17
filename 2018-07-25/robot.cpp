// Romeo Rizzi, 2016-07-22

#include<cassert>
#include<iostream>
#include<fstream>
#include <string.h>
#include <cstdio>
#include <iomanip>  // for info on use see : http://faculty.cs.niu.edu/~mcmahon/CS241/c241man/node83.html 

using namespace std;

const char *LATEX_INPUT = "robot.in.tex";
const char *LATEX_ANY_PATHS_UP = "robot.num.paths.up.tex";
const char *LATEX_ANY_PATHS_DOWN = "robot.num.paths.down.tex";
const char *LATEX_OPT_PATHS_UP = "robot.opt.paths.up.tex";
const char *LATEX_OPT_PATHS_DOWN = "robot.opt.paths.down.tex";

const int WALL = -1;

const int MAX_M = 20, MAX_N = 20;
typedef int borded_mn_mat[MAX_M +2][MAX_N +2];

int m, n;
bool maximize;
borded_mn_mat cell; // se true cerchiamo cammini di max valore invece che min costo.

borded_mn_mat num_any_paths, opt, num_opt_paths;

int my_max(int a, int b) { return (a>b) ? a : b; }
int my_min(int a, int b) { return (a<b) ? a : b; }

bool up;
int first_index_row() { if( up ) return m; return 1; }
int after_last_index_row() { if( up ) return 0; return m+1; }
int next_index_row(int i) { if( up ) return i-1; return i+1; }
int prev_index_row(int i) { if( up ) return i+1; return i-1; }

int first_index_col() { if( up ) return n; return 1; }
int after_last_index_col() { if( up ) return 0; return n+1; }
int next_index_col(int i) { if( up ) return i-1; return i+1; }
int prev_index_col(int i) { if( up ) return i+1; return i-1; }

void DP_any_paths() {
  for(int i = 0; i <= m +1; i++)
    for(int j = 0; j <= n +1; j++) {
      num_any_paths[i][j] = 0;   // solo per inizializzare ogni possibile bordo
  }
  if(up) num_any_paths[m][n] = 1;
  else num_any_paths[1][1] = 1;

  for(int i = first_index_row(); i!=after_last_index_row(); i = next_index_row(i) )
    for(int j = first_index_col(); j!=after_last_index_col(); j = next_index_col(j) ) {
      if( (i == first_index_row()) && (j == first_index_col()) )  continue;
      if( cell[i][j] != WALL )
         num_any_paths[i][j] = num_any_paths[i][ prev_index_col(j) ] + num_any_paths[ prev_index_row(i) ][j];
    }
}

void DP_opt_paths() {
  for(int i = 0; i <= m +1; i++)
    for(int j = 0; j <= n +1; j++) {
      num_opt_paths[i][j] = 0;   // solo per inizializzare ogni possibile bordo
      if( maximize )
         opt[i][j] = -9999;
      else
         opt[i][j] = 9999;
  }
  if(up) { opt[m][n] = 0; num_opt_paths[m][n] = 1; }
  else { opt[1][1] = 0; num_opt_paths[1][1] = 1; }

  for(int i = first_index_row(); i!=after_last_index_row(); i = next_index_row(i) )
    for(int j = first_index_col(); j!=after_last_index_col(); j = next_index_col(j) ) {
      if( (i == first_index_row()) && (j == first_index_col()) )  continue;
      if( cell[i][j] != WALL ) {
	opt[i][j] = cell[i][j] + ( (maximize)?
	                              my_max( opt[i][ prev_index_col(j) ], opt[ prev_index_row(i) ][j] ) :
                                      my_min( opt[i][ prev_index_col(j) ], opt[ prev_index_row(i) ][j] )
			         );
        if(opt[i][j] == cell[i][j] + opt[i][ prev_index_col(j) ])
	  num_opt_paths[i][j] += num_opt_paths[i][ prev_index_col(j) ];
        if(opt[i][j] == cell[i][j] + opt[ prev_index_row(i) ][j])
	  num_opt_paths[i][j] += num_opt_paths[ prev_index_row(i) ][j]; 
      }
      else opt[i][j] = -9999;
    }
}

void display_matrix(borded_mn_mat mat) {
  cout << endl;
  for(int i = 1; i <= m; i++) {
    for(int j = 1; j <= n; j++)
      cout << setw(4) << mat[i][j];
    cout << endl;
  }
}

void latex_matrix(const char *file_name, bool display_robot_and_home, borded_mn_mat mat, borded_mn_mat mat_sub = NULL, borded_mn_mat mat_sup = NULL) {
  ofstream fout(file_name); assert( fout );
  fout << "\\begin{tabular}{||c||";
  for(int j = 1; j<=n; j++) 
    fout << "c|";
  fout << "|}" << endl;
  fout << "\\hline\n\\hline" << endl;

  for(int j = 1; j<=n; j++) 
    fout << "  & $" << j << "$";
  fout << "\\\\" << endl;
  fout << "\\hline" << endl << endl;

  for(int i = 1; i<=m; i++) {
    fout << "\\hline" << endl;
    fout << "$" << char('A'+i-1) << "$ ";
    for(int j = 1; j<=n; j++) {
      fout << "& $";
      if( cell[i][j] == WALL )
        fout << "\\bullet";
      else if( (i==1) && (j==1) && (display_robot_and_home) )
        fout << 'R';
      else if( (i==m) && (j==n) && (display_robot_and_home) )
        fout << 'H';
      else {
	if(mat[i][j] >= 0) {
	  fout << mat[i][j];
	  if(mat_sub != NULL)
	    fout << "_{" << mat_sub[i][j] << "}";
	  if(mat_sup != NULL)
	    fout << "^{" << mat_sup[i][j] << "}";
	}
	else fout << " - ";
      }
      fout << "$ ";
    }
    fout << "\\\\\n\\hline" << endl;
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl << endl;
  fout.close();
}



int main() {
  cin >> m >> n >> maximize;
  for(int i = 1; i <= m; i++)
    for(int j = 1; j <= n; j++)
      cin >> cell[i][j];

  cout << "affronto l'istanza:\n   m = " << m << "\n   n = " << n << "\n   maximize = " << maximize << endl << endl;
  for(int i = 1; i <= m; i++) {
    for(int j = 1; j <= n; j++)
      cout << cell[i][j] << " ";
    cout << endl;
  }
  assert( cell[1][1] == 0 );
  assert( cell[m][n] == 0 );

  latex_matrix(LATEX_INPUT, true, cell);

  up = 1;
  DP_any_paths();
  display_matrix(num_any_paths);
  latex_matrix(LATEX_ANY_PATHS_UP, false, num_any_paths);

  up = 0;
  DP_any_paths();
  display_matrix(num_any_paths);
  latex_matrix(LATEX_ANY_PATHS_DOWN, false, num_any_paths);

  DP_opt_paths();
  latex_matrix(LATEX_OPT_PATHS_DOWN, false, opt, num_opt_paths);

  up = 1;
  DP_opt_paths();
  latex_matrix(LATEX_OPT_PATHS_UP, false, opt, num_opt_paths);
  
  return 0;
}
