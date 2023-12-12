//
// Created by khris on 22.11.2023.
//
#include "environment.h"
#include "settings.h"
#include <Graph_lib/Point.h>


using namespace std;

Player::Player(const string &namePlayer, int xcord, int ycord) {
    name = namePlayer;
    x = xcord;
    y = ycord;
    alive = true;
    awaits_respawn = true;
    //body = new  = new Graph_lib::Circle{Point(x, y), PlayerRad};
}
string Player::GetName() {
    return name;
}

void Player::Revive() {
    alive = true;
    awaits_respawn = false;
}


pair<int, int> Player::GetCords() {
    return pair<int, int>{x, y};
}

////////////// мат выражения
//
//It supports operators: + - * / ^ ( )

//It supports math functions : SIN, COS, TAN, ASIN, ACOS, ATAN, SINH,
//COSH, TANH, ASINH, ACOSH, ATANH, LN, LOG, EXP, SQRT, SQR, ROUND, INT.

//It supports variables A to Z.


#define PI 3.14159265358979323846

using namespace std;

enum types { DELIMITER = 1,
             VARIABLE,
             NUMBER,
             FUNCTION };
const int NUMVARS = 26;
class parser {
    char *exp_ptr;       // points to the expression
    char token[256];     // holds current token
    char tok_type;       // holds token's type
    double vars[NUMVARS];// holds variable's values
    void eval_exp1(double &result);
    void eval_exp2(double &result);
    void eval_exp3(double &result);
    void eval_exp4(double &result);
    void eval_exp5(double &result);
    void eval_exp6(double &result);
    void get_token();

public:
    parser();
    double eval_exp(char *exp);
    char errormsg[64];
};
// Parser constructor.
parser::parser() {
    int i;
    exp_ptr = NULL;
    for (i = 0; i < NUMVARS; i++)
        vars[i] = 0.0;
    errormsg[0] = '\0';
}
// Parser entry point.
double parser::eval_exp(char *exp) {
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token) {
        strcpy(errormsg, "No Expression Present");// no expression present
        return (double) 0;
    }
    eval_exp1(result);
    if (*token)// last token must be null
        strcpy(errormsg, "Syntax Error");
    return result;
}
// Process an assignment.
void parser::eval_exp1(double &result) {
    int slot;
    char temp_token[80];
    if (tok_type == VARIABLE) {
        // save old token
        char *t_ptr = exp_ptr;
        strcpy(temp_token, token);
        // compute the index of the variable
        slot = *token - 'A';
        get_token();
        if (*token != '=') {
            exp_ptr = t_ptr;          // return current token
            strcpy(token, temp_token);// restore old token
            tok_type = VARIABLE;
        } else {
            get_token();// get next part of exp
            eval_exp2(result);
            vars[slot] = result;
            return;
        }
    }
    eval_exp2(result);
}
// Add or subtract two terms.
void parser::eval_exp2(double &result) {
    char op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-') {
        get_token();
        eval_exp3(temp);
        switch (op) {
            case '-':
                result = result - temp;
                break;
            case '+':
                result = result + temp;
                break;
        }
    }
}
// Multiply or divide two factors.
void parser::eval_exp3(double &result) {
    char op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/') {
        get_token();
        eval_exp4(temp);
        switch (op) {
            case '*':
                result = result * temp;
                break;
            case '/':
                result = result / temp;
                break;
        }
    }
}
// Process an exponent.
void parser::eval_exp4(double &result) {
    double temp;
    eval_exp5(result);
    while (*token == '^') {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}
// Evaluate a unary + or -.
void parser::eval_exp5(double &result) {
    char op;
    op = 0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-') {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-')
        result = -result;
}
// Process a function, a parenthesized expression, a value or a variable
void parser::eval_exp6(double &result) {
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc) {
        strcpy(temp_token, token);
        get_token();
    }
    if ((*token == '(')) {
        get_token();
        eval_exp2(result);
        if (*token != ')')
            strcpy(errormsg, "Unbalanced Parentheses");
        if (isfunc) {
            if (!strcmp(temp_token, "SIN"))
                result = sin(PI / 180 * result);
            else if (!strcmp(temp_token, "COS"))
                result = cos(PI / 180 * result);
            else if (!strcmp(temp_token, "TAN"))
                result = tan(PI / 180 * result);
            else if (!strcmp(temp_token, "ASIN"))
                result = 180 / PI * asin(result);
            else if (!strcmp(temp_token, "ACOS"))
                result = 180 / PI * acos(result);
            else if (!strcmp(temp_token, "ATAN"))
                result = 180 / PI * atan(result);
            else if (!strcmp(temp_token, "SINH"))
                result = sinh(result);
            else if (!strcmp(temp_token, "COSH"))
                result = cosh(result);
            else if (!strcmp(temp_token, "TANH"))
                result = tanh(result);
            else if (!strcmp(temp_token, "ASINH"))
                result = asinh(result);
            else if (!strcmp(temp_token, "ACOSH"))
                result = acosh(result);
            else if (!strcmp(temp_token, "ATANH"))
                result = atanh(result);
            else if (!strcmp(temp_token, "LN"))
                result = log(result);
            else if (!strcmp(temp_token, "LOG"))
                result = log10(result);
            else if (!strcmp(temp_token, "EXP"))
                result = exp(result);
            else if (!strcmp(temp_token, "SQRT"))
                result = sqrt(result);
            else if (!strcmp(temp_token, "SQR"))
                result = result * result;
            else if (!strcmp(temp_token, "ROUND"))
                result = round(result);
            else if (!strcmp(temp_token, "INT"))
                result = floor(result);
            else
                strcpy(errormsg, "Unknown Function");
        }
        get_token();
    } else
        switch (tok_type) {
            case VARIABLE:
                result = vars[*token - 'A'];
                get_token();
                return;
            case NUMBER:
                result = atof(token);
                get_token();
                return;
            default:
                strcpy(errormsg, "Syntax Error");
        }
}
// Obtain the next token.
void parser::get_token() {
    char *temp;
    tok_type = 0;
    temp = token;
    *temp = '\0';
    if (!*exp_ptr)// at end of expression
        return;
    while (isspace(*exp_ptr))// skip over white space
        ++exp_ptr;
    if (strchr("+-*/%^=()", *exp_ptr)) {
        tok_type = DELIMITER;
        *temp++ = *exp_ptr++;// advance to next char
    } else if (isalpha(*exp_ptr)) {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        while (isspace(*exp_ptr))// skip over white space
            ++exp_ptr;
        tok_type = (*exp_ptr == '(') ? FUNCTION : VARIABLE;
    } else if (isdigit(*exp_ptr) || *exp_ptr == '.') {
        while (!strchr(" +-/*%^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    if ((tok_type == VARIABLE) && (token[1]))
        strcpy(errormsg, "Only first letter of variables is considered");
}

std::vector<std::pair<int, int>> cut_x_y(vector<Obstacle> &findxyobst) {
    std::vector<std::pair<int, int>> points;
    for (const auto &i: findxyobst) {
        if (i.hole == true) {
            int a = i.center.x;
            int b = i.center.y;
            int r = i.radius;
            for (int x = a - r; x <= a + r; x += 1) {
                for (int y = b - r; y <= b + r; y += 1) {
                    if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2)) {
                        points.push_back(std::make_pair(x, y));
                    }
                }
            }
        }
    }
    return points;
}


std::vector<std::pair<int, int>> obstac_x_y(vector<Obstacle> &findxyobst) {
    std::vector<std::pair<int, int>> points;

    for (size_t i = 0; i < findxyobst.size(); ++i) {
        if (findxyobst[i].hole == false) {
            int a = findxyobst[i].center.x;
            int b = findxyobst[i].center.y;
            int r = findxyobst[i].radius;
            for (int x = a - r; x <= a + r; x += 1) {
                for (int y = b - r; y <= b + r; y += 1) {
                    if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2)) {
                        points.push_back(std::make_pair(x, y));
                    }
                }
            }
        }
    }

    return points;
}

vector<pair<int, int>> player_x_y(vector<Graph_lib::Point> &findxyobst) {
    vector<pair<int, int>> points;
    for (const auto &i: findxyobst) {

        int a = i.x;
        int b = i.y;
        const int r = PlayerRad;
        for (int x = a - r; x <= a + r; x += 1) {
            for (int y = b - r; y <= b + r; y += 1) {
                if (std::pow(x - a, 2) + std::pow(y - b, 2) <= std::pow(r, 2)) {
                    points.push_back(std::make_pair(x, y));
                }
            }
        }
    }

    return points;
}

Graph_lib::Point check_player(int x, int y, vector<Graph_lib::Point> &players) {//тут ищем кому приндлежит данная точка
    int min_dist = 1000;
    Graph_lib::Point closest;
    for (const auto &i: players) {
        int x_player = i.x;
        int y_player = i.y;
        double distance = sqrt(pow(x - x_player, 2) + sqrt(pow(y - y_player, 2)));
        if (distance < min_dist) {
            min_dist = distance;
            closest = Graph_lib::Point(x_player, y_player);
        }
    }
    return closest;
}


vector<pair<int, int>> Func_trace(string &func_enter, pair<int, int> &cords,
                                  vector<Obstacle> &obstacle_mini,
                                  vector<Graph_lib::Point> &players_cords) {
    //const int size_map_y = FieldHeight; // длина поля для пробега y
    const int size_map_x = FieldWidth;// длина поля для пробега х
    //string func_enter = "2 ^ x"; // потом передастся

    //pair cords{100,100}; // координаты игрока не забыть потом за комментить
    bool right_true = true;
    size_t pos = func_enter.find('x');
    vector<pair<int, int>> cord_vector;
    vector<pair<int, int>> white_cords = cut_x_y(obstacle_mini);
    vector<pair<int, int>> black_cords = obstac_x_y(obstacle_mini); // нашли все координаты препятствия
    vector<pair<int, int>> gamers_cords = player_x_y(players_cords);// нашли все координаты игроков

    if (pos != string::npos)// проверка есть ли аргумент Х в функции
    {
        vector<int> index;
        for (size_t i = 0; i <= func_enter.size() - 1; ++i) {
            if (func_enter[i] == 'x') {
                index.push_back(i);
            }
        }
        for (int x = 0; x <= size_map_x; ++x)// перебираем х
        {
            string str_imagin = func_enter;
            string new_value = to_string(x);// из инта в стринг
            for (size_t i = 0; i <= index.size() - 1; ++i) {
                size_t pos1 = str_imagin.find('x');
                str_imagin.replace(pos1, 1, new_value);
            }
            char expstr[256];
            strcpy(expstr, str_imagin.c_str());//
            parser ob;
            double ans = ob.eval_exp(expstr);// вычисляем мат выражение
            if (*ob.errormsg) {
                throw ob.errormsg;
            }

            else {
                int x_i;
                if (right_true)// если стреляем вправо
                {
                    x_i = cords.first + x;
                } else {
                    x_i = cords.first - x;
                }
                if (not(isnan(ans)))// значение non
                {
                    //int y_i = cords.second + static_cast<int>(round(ans));
                    int y_i = cords.second - static_cast<int>(round(ans));
                    pair<int, int> p = make_pair(x_i, y_i);
                    auto obst_for = find(black_cords.begin(), black_cords.end(), p);//для проверки на попадание в препятсвие
                    auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);

                    if (x <= PlayerRad + SpawnObsMinDist) {
                        cord_vector.push_back(make_pair(x_i, y_i));
                        continue;
                    }
                    if (gamer_for != gamers_cords.end())//true если попали в игрока
                    {                                   //удалям координаты из вектора
                        cord_vector.push_back(make_pair(x_i, y_i));
                        players_cords.erase(remove(players_cords.begin(), players_cords.end(), check_player(x_i, y_i, players_cords)), players_cords.end());
                        continue;
                    }

                    auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                    if (cut_for != white_cords.end()) {
                        cord_vector.push_back(make_pair(x_i, y_i));
                        continue;
                    }

                    if (obst_for != black_cords.end()) {// попали в препятсвия
                        int rad = WhiteObsRad;
                        Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                        obstacle_mini.push_back(with_out);// попали в препятствие x_i добавляем вырез
                        break;
                    }
                    cord_vector.push_back(make_pair(x_i, y_i));
                }
            }
        }

    } else {
        for (int x = 0; size_map_x >= x; ++x) {
            char expstr[256];
            strcpy(expstr, func_enter.c_str());
            parser ob;
            cout << expstr << endl;
            double ans = ob.eval_exp(expstr);

            if (right_true) {
                int x_i = cords.first + x;
                int y_i = ans;
                pair<int, int> p = make_pair(x_i, y_i);
                auto obst_for = find(black_cords.begin(), black_cords.end(), p);//для проверки на попадание в препятсвие
                auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);
                if (x <= PlayerRad + SpawnObsMinDist) {
                    continue;
                }
                if (gamer_for != gamers_cords.end())//true если попали в игрока
                {
                    //удалям координаты из вектора
                    cord_vector.push_back(make_pair(x_i, y_i));
                    players_cords.erase(remove(players_cords.begin(), players_cords.end(), check_player(x_i, y_i, players_cords)), players_cords.end());
                    continue;
                }

                auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                if (cut_for != white_cords.end()) {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }

                if (obst_for != black_cords.end()) {// попали в препятсвия
                    int rad = WhiteObsRad;
                    Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                    obstacle_mini.push_back(with_out);// попали в препятствие x_i добавляем вырез
                    break;
                }
                cord_vector.push_back(make_pair(x_i, y_i));

            } else {
                int x_i = cords.first - x;
                int y_i = ans;
                pair<int, int> p = make_pair(x_i, y_i);
                auto obst_for = find(black_cords.begin(), black_cords.end(), p);//для проверки на попадание в препятсвие
                auto gamer_for = find(gamers_cords.begin(), gamers_cords.end(), p);
                if (x <= PlayerRad + SpawnObsMinDist) {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }
                if (gamer_for != gamers_cords.end())//true если попали в игрока
                {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    //удалям координаты из вектора
                    players_cords.erase(remove(players_cords.begin(), players_cords.end(), check_player(x_i, y_i, players_cords)), players_cords.end());
                    continue;
                }

                auto cut_for = find(white_cords.begin(), white_cords.end(), p);
                if (cut_for != white_cords.end()) {
                    cord_vector.push_back(make_pair(x_i, y_i));
                    continue;
                }


                if (obst_for != black_cords.end()) {// попали в препятсвия
                    int rad = WhiteObsRad;
                    Obstacle with_out = Obstacle{Graph_lib::Point(x_i, y_i), rad, true};
                    obstacle_mini.push_back(with_out);// попали в препятствие x_i добавляем вырез
                    break;
                }
                cord_vector.push_back(make_pair(x_i, y_i));
            }
        }
    }
    return cord_vector;
}
//for (const auto &p: cord_vector)
//{
//	cout << p.first << ' ' << p.second<< endl;
//}


///} //возможно скобку где то не закрыл....
