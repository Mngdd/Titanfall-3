#ifndef PARSER_H
#define PARSER_H
#include <bits/stdc++.h>
#define PI 3.14159265358979323846

using namespace std;

enum types
{
    DELIMITER = 1,
    VARIABLE,
    NUMBER,
    FUNCTION
};

const int NUMVARS = 26;

class parser
{
    char* exp_ptr;         // points to the expression
    char token[256];       // holds current token
    char tok_type;         // holds token's type
    double vars[NUMVARS];  // holds variable's values
    void eval_exp1 (double& result);
    void eval_exp2 (double& result);
    void eval_exp3 (double& result);
    void eval_exp4 (double& result);
    void eval_exp5 (double& result);
    void eval_exp6 (double& result);
    void get_token ();

  public:
    parser();
    double eval_exp (char* exp);
    char errormsg[64];
};

#endif