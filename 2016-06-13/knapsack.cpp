// Romeo Rizzi, 2016-06-06

#include<cassert>
#include<iostream>
#include<fstream>
#include <iomanip>  // for info on use see : http://faculty.cs.niu.edu/~mcmahon/CS241/c241man/node83.html 

using namespace std;

const char *LATEX_FILE = "knapsack.tex";
const int MAX_N = 50; int n;   // numero di oggetti tra cui scegliere
const int MAX_B = 50; int b;   // capacita' dello zaino

int val[MAX_N +1];
int peso[MAX_N +1];

int prob[MAX_N +1][MAX_B +1];

int my_max(int a, int b) { return (a>b) ? a : b; }

void display_matrix() {
  for(int i = 0; i<=n; i++) {
    for(int j = 0; j<=b; j++)
      cout << prob[i][j] << " ";
    cout << endl;
  }
}

void latex_matrix() {
  ofstream fout(LATEX_FILE); assert( fout );
  fout << "\\begin{sidewaystable}[h!]" << endl;
  fout << "  \\begin{center}" << endl;
  fout << "    \\sc \\Large  Tabella di Programmazione Dinamica per il problema dello Zaino" << endl;
  fout << "  \\end{center}" << endl << endl;

  fout << "{\\tiny" << endl;
  fout << "\\ \\hspace{-1.2cm}" << endl;
  fout << "\\begin{tabular}{r||";
  for(int j = 0; j<=b; j++) 
    fout << "c|";
  fout << "}" << endl;
  fout << "\\hline" << endl << endl;

  fout << "            ";
  for(int j = 0; j<=b; j++) 
    fout << "&\\begin{sideways}$" << j << "$\\end{sideways}";
  fout << "\\\\" << endl << endl;

  for(int i = 0; i<=n; i++) {
    fout << "\\hline" << endl;
    fout << '$' << char('A'+i-1) << '(' << peso[i] << ',' << val[i] << ")$   ";
    for(int j = 0; j<=b; j++) {
      if(prob[i][j] >= 0)
        fout << "& $" << prob[i][j] << "$ ";
      else
        fout << "& $" <<  "\\times" << "$ ";
    }
    fout << " \\\\" << endl;
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl << endl;
  fout << "}" << endl << endl;


  fout << "\\begin{center}" << endl;
  fout << "   (come stilata in riferimento ai seguenti oggetti)" << endl;
  fout << "\\begin{tabular}{|r||";
  for(int i = 1; i<=n; i++) 
    fout << "c|";
  fout << "}" << endl;
  fout << "\\hline" << endl;
  fout << "   nome   ";
  for(int i = 1; i<=n; i++) 
    fout << " & " <<  char('A'+i-1);
  fout << "\\\\" << endl;
  fout << "\\hline" << endl;
  fout << "   peso   ";
  for(int i = 1; i<=n; i++) 
    fout << "& $" << peso[i] << "$ ";
  fout << "\\\\" << endl;
  fout << "   valore ";
  for(int i = 1; i<=n; i++) 
    fout << "& $" << val[i] << "$ ";
  fout << "\\\\" << endl;
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  fout << "\\end{center}" << endl << endl;
  fout << "\\vfill" << endl;
  fout << "\\begin{center} \\Large" << endl;
  fout << "{\\bf \\sc  Tabella delle Risposte}" << endl;
  fout << "\\input{knapsack.risp.full}" << endl;
  fout << "\\end{center}" << endl << endl;
  fout << "\\end{sidewaystable}" << endl << endl;
  fout << "\\clearpage" << endl;
  fout.close();
}


int main() {
  cin >> n >> b;
  for(int i = 1; i<=n; i++)
    cin >> val[i];
  for(int i = 1; i<=n; i++)
    cin >> peso[i];

  cout << "affronto l'istanza:\n   n = " << n << ", B = " << b << endl << endl << " val: ";
  for(int i = 1; i<=n; i++)
    cout << val[i] << " ";
  cout << endl << "peso: ";
  for(int i = 1; i<=n; i++)
    cout << peso[i] << " ";
  cout << endl << endl;

  for(int j = 1; j<=b; j++)
    prob[0][j] = -1;
  prob[0][0] = 0;
  for(int i = 1; i<=n; i++)
    for(int j = 0; j<=b; j++) {
      prob[i][j] = prob[i-1][j];
      if( (j >= peso[i]) && (prob[i-1][ j-peso[i] ] >= 0) )
        prob[i][j] = my_max( prob[i][j], val[i] + prob[i-1][ j-peso[i] ] );
    }
  display_matrix();
  latex_matrix();

  return 0;
}
