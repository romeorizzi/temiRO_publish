// Romeo Rizzi, 2016-06-06

#include<cassert>
#include<iostream>
#include<fstream>
#include <string.h>
#include <cstdio>
#include <iomanip>  // for info on use see : http://faculty.cs.niu.edu/~mcmahon/CS241/c241man/node83.html 

using namespace std;

const char *LATEX_FILE = "common_subsequence.tex";
const int MAX_L = 20;
char s[MAX_L +2], t[MAX_L +2];  int len_s, len_t;

int max_len[MAX_L +2][MAX_L +2];
int num_opt_taking_last[MAX_L +2][MAX_L +2];
int num_opt_free[MAX_L +2][MAX_L +2];

bool suffixes, max_num_sol_in_t;

int my_max(int a, int b) { return (a>b) ? a : b; }

int first_index_s() { if( suffixes ) return len_s; return 1; }
int after_last_index_s() { if( suffixes ) return 0; return len_s+1; }
int next_index_s(int i) { if( suffixes ) return i-1; return i+1; }
int prev_index_s(int i) { if( suffixes ) return i+1; return i-1; }

int first_index_t() { if( suffixes ) return len_t; return 1; }
int after_last_index_t() { if( suffixes ) return 0; return len_t+1; }
int next_index_t(int i) { if( suffixes ) return i-1; return i+1; }
int prev_index_t(int i) { if( suffixes ) return i+1; return i-1; }

void DP() {
  for(int i = 0; i <= MAX_L +1; i++)
    for(int j = 0; j <= MAX_L +1; j++) {
      max_len[i][j] = 0;   // solo per inizializzare ogni possibile bordo
      num_opt_free[i][j] = 1;
      num_opt_taking_last[i][j] = 0;
  }
  for(int i = first_index_s(); i!=after_last_index_s(); i = next_index_s(i) )
    for(int j = first_index_t(); j!=after_last_index_t(); j = next_index_t(j) ) {
      if( s[i] == t[j] ) {
         max_len[i][j] = 1+max_len[ prev_index_s(i) ][ prev_index_t(j) ];
         num_opt_taking_last[i][j] = num_opt_free[ prev_index_s(i) ][ prev_index_t(j) ];
         num_opt_free[i][j] = num_opt_taking_last[i][j]; // quantomeno questo ...
         if( max_num_sol_in_t ) {               
           if( max_len[i][ prev_index_t(j) ] == max_len[i][j] )
             num_opt_free[i][j] += num_opt_free[i][ prev_index_t(j) ];               
         }
         else { // stiamo conteggiando il numero di sottosequenze di s
           if( max_len[ prev_index_s(i) ][j] == max_len[i][j] )
             num_opt_free[i][j] += num_opt_free[ prev_index_s(i) ][j];
         }
      }
      else {
	max_len[i][j] = my_max (max_len[ i ][ prev_index_t(j) ], max_len[ prev_index_s(i) ][ j ]);
        num_opt_taking_last[i][j] = num_opt_free[i][j] = 0;
        if( max_len[ prev_index_s(i) ][j] == max_len[i][j] ) {
          if( max_num_sol_in_t )               
            num_opt_taking_last[i][j] = num_opt_taking_last[ prev_index_s(i) ][j];
          num_opt_free[i][j] += num_opt_free[ prev_index_s(i) ][j];
        }               
        if( max_len[i][ prev_index_t(j) ] == max_len[i][j] ) {
          if( !max_num_sol_in_t )               
            num_opt_taking_last[i][j] = num_opt_taking_last[i][ prev_index_t(j) ];
          num_opt_free[i][j] += num_opt_free[i][ prev_index_t(j) ];
        }               
        if( max_len[ prev_index_s(i) ][ prev_index_t(j) ] == max_len[i][j] ) {
          num_opt_free[i][j] -= num_opt_free[ prev_index_s(i) ][ prev_index_t(j) ];
        }               
      }
    }
}

void display_matrix() {
  cout << "s\\t ";
  for(int j = (suffixes) ? 1 : 0; j<=len_t + (suffixes) ? 1 : 0; j++)
    cout << "  " << t[j];
  cout << endl;

  for(int i = (suffixes) ? 1 : 0; i<=len_s + (suffixes) ? 1 : 0; i++) {
    cout << s[i] << "   ";
    for(int j = (suffixes) ? 1 : 0; j<=len_t; j++)
      cout << setw(3) << max_len[i][j];
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
  for(int j = (suffixes) ? 1 : 0; j<=len_t + (suffixes) ? 1 : 0; j++) 
    fout << "& " << t[j] << " ";
  fout << "\\\\" << endl << endl;
  fout << "\\hline" << endl;

  for(int i = (suffixes) ? 1 : 0; i<=len_s + (suffixes) ? 1 : 0; i++) {
    fout << "\\hline" << endl;
    fout << s[i] << " ";
    for(int j = (suffixes) ? 1 : 0; j<=len_t + (suffixes) ? 1 : 0; j++)
      fout << "& $" << max_len[i][j] << "_{" << num_opt_free[i][j] << "}^{" << num_opt_taking_last[i][j] << "}$ ";
    fout << " \\\\" << endl;
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl << endl;
  fout << "\\end{center}" << endl << endl;
  fout << "\\end{table}" << endl << endl;
  fout.close();
}


int main() {
  cin >> suffixes >> max_num_sol_in_t;
  cin >> (s+1);   len_s = strlen(s+1);
  cin >> (t+1);   len_t = strlen(t+1);

  cout << "affronto l'istanza:\n   s = " << (s+1) << "\n   t = " << (t+1) << endl << endl;

  s[0] = t[0] = s[len_s +1] = t[len_t +1] = '-';
  s[len_s +2] = t[len_t +2] = '\n';

  cout << "strighe estese:\n   s = " << s << "\n   t = " << t << endl << endl;

  DP();

  display_matrix();
  latex_matrix();

  return 0;
}
