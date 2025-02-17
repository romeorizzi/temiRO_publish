#include<cassert>
#include<iostream>
#include<fstream>
#include <string.h>
#include <cstdio>
#include <iomanip>  // libreria per formattare output

using namespace std;

const char *LATEX_FILE = "lcs.tex";
const char *LATEX_INPUT = "lcs.in.tex";

const int Max_L = 20;
char s[Max_L +2], t[Max_L +2];  int len_s, len_t;

int Max_Len[Max_L +2][Max_L +2];
int num_opt_free[Max_L +2][Max_L +2];
int num_opt_taking_last[Max_L +2][Max_L +2];

bool suffix, num_sol_in_t;

int max(int a, int b) { return (a>b) ? a : b; }

int first_index_s() { if( suffix ) return len_s; return 1; }
int after_last_index_s() { if( suffix ) return 0; return len_s+1; }
int next_index_s(int i) { if( suffix ) return i-1; return i+1; }
int prev_index_s(int i) { if( suffix ) return i+1; return i-1; }

int first_index_t() { if( suffix ) return len_t; return 1; }
int after_last_index_t() { if( suffix ) return 0; return len_t+1; }
int next_index_t(int i) { if( suffix ) return i-1; return i+1; }
int prev_index_t(int i) { if( suffix ) return i+1; return i-1; }

void ComSub() {
  for(int i = 0; i <= Max_L +1; i++)
    for(int j = 0; j <= Max_L +1; j++) {
      Max_Len[i][j] = 0;
      num_opt_free[i][j] = 1;
      num_opt_taking_last[i][j] = 0;
  }
  for(int i = first_index_s(); i!=after_last_index_s(); i = next_index_s(i) )
    for(int j = first_index_t(); j!=after_last_index_t(); j = next_index_t(j) ) {
      if( s[i] == t[j] ) {
         Max_Len[i][j] = 1 + Max_Len[ prev_index_s(i) ][ prev_index_t(j) ];
         num_opt_taking_last[i][j] = num_opt_free[ prev_index_s(i) ][ prev_index_t(j) ];
         num_opt_free[i][j] = num_opt_taking_last[i][j]; 
         if( num_sol_in_t ) {               
           if( Max_Len[i][ prev_index_t(j) ] == Max_Len[i][j] )
             num_opt_free[i][j] += num_opt_free[i][ prev_index_t(j) ];               
         }
         else { //numero di sottosequenze di S
           if( Max_Len[ prev_index_s(i) ][j] == Max_Len[i][j] )
             num_opt_free[i][j] += num_opt_free[ prev_index_s(i) ][j];
         }
      }
      else {
	Max_Len[i][j] = max (Max_Len[ i ][ prev_index_t(j) ], Max_Len[ prev_index_s(i) ][ j ]);
        num_opt_taking_last[i][j] = num_opt_free[i][j] = 0;
        if( Max_Len[ prev_index_s(i) ][j] == Max_Len[i][j] ) {
          if( num_sol_in_t )               
            num_opt_taking_last[i][j] = num_opt_taking_last[ prev_index_s(i) ][j];
          num_opt_free[i][j] += num_opt_free[ prev_index_s(i) ][j];
        }               
        if( Max_Len[i][ prev_index_t(j) ] == Max_Len[i][j] ) {
          if( !num_sol_in_t )               
            num_opt_taking_last[i][j] = num_opt_taking_last[i][ prev_index_t(j) ];
          num_opt_free[i][j] += num_opt_free[i][ prev_index_t(j) ];
        }               
        if( Max_Len[ prev_index_s(i) ][ prev_index_t(j) ] == Max_Len[i][j] ) {
          num_opt_free[i][j] -= num_opt_free[ prev_index_s(i) ][ prev_index_t(j) ];
        }               
      }
    }
}

void view_matrix() {
  cout << "s\\t ";
  for(int j = (suffix) ? 1 : 0; j<=len_t + (suffix) ? 1 : 0; j++)
    cout << "  " << t[j];
  cout << endl;

  for(int i = (suffix) ? 1 : 0; i<=len_s + (suffix) ? 1 : 0; i++) {
    cout << s[i] << "   ";
    for(int j = (suffix) ? 1 : 0; j<=len_t +1; j++)
      cout << setw(3) << Max_Len[i][j];  //allineo output
    cout << endl;
  }
}

void latex_matrix() {
  ofstream fout(LATEX_FILE); assert( fout );
  fout << "\\begin{table}[h!]" << endl;
  fout << "\\begin{center}" << endl;
  fout << "\\begin{tabular}{l||";
  for(int j = 0; j<=len_t; j++) 
    fout << "r|";
  fout << "}" << endl;
  fout << "\\hline" << endl << endl;

  fout << "   $s$|$t$ ";
  for(int j = (suffix) ? 1 : 0; j<=len_t + (suffix) ? 1 : 0; j++) 
    fout << "& " << t[j] << " ";
  fout << "\\\\" << endl << endl;
  fout << "\\hline" << endl;

  for(int i = (suffix) ? 1 : 0; i<=len_s + (suffix) ? 1 : 0; i++) {
    fout << "\\hline" << endl;
    fout << s[i] << " ";
    for(int j = (suffix) ? 1 : 0; j<=len_t + (suffix) ? 1 : 0; j++)
      fout << "& $" << Max_Len[i][j] << "_{" << num_opt_free[i][j] << "}^{" << num_opt_taking_last[i][j] << "}$ ";
    fout << " \\\\" << endl;
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl << endl;
  fout << "\\end{center}" << endl << endl;
  fout << "\\end{table}" << endl << endl;
  fout.close();
}


int main() {
  cin >> suffix >> num_sol_in_t;
  cin >> (s+1);   len_s = strlen(s+1);
  cin >> (t+1);   len_t = strlen(t+1);

  cout << "affronto l'istanza:\n   s = " << (s+1) << "\n   t = " << (t+1) << endl << endl;

  ofstream fot(LATEX_INPUT); assert( fot );
  fot << "$s= ";
  for(int i = 1; i < strlen(s+1); i++)
    fot << s[i] << "\\" << ", ";
  fot << s[strlen(s+1)] << "$ e $t= ";
  for(int i = 1; i < strlen(t+1); i++)
    fot << t[i] << "\\" << ", ";
  fot << t[strlen(t+1)] << "$.";
  
  s[0] = t[0] = s[len_s +1] = t[len_t +1] = '-';
  s[len_s +2] = t[len_t +2] = '\n';

  cout << "strighe estese:\n   s = " << s << "   t = " << t << endl << endl;

  ComSub();

  view_matrix();
  latex_matrix();

  return 0;
}
