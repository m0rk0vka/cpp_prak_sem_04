#include <iostream>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>

/****************
GRAMMAR_LEX:

    S -> 0..9 N
    N -> 0..9 N | eps

    S -> A..z I
    I -> A..z I | 0..9 I | eps

    S -> (

    S -> )

    S -> +

    S -> -

    S -> *

    S -> div

*******************/

/****************
GRAMMAR_SAME:

    E -> M { [+ | -] M }
    M -> F { [* | div] F }
    F -> ( E ) | number | id

Evidence:

GRAMMAR_SAME_EQUIVALENT:

    E -> M S
    S -> + E | - E | epsilon
    M -> F P
    P -> * M | div M | epsilon
    F -> ( E ) | number | id


    1) No FIRST conflicts
    2) No more than one eps alternative
    3) First(S) = { +, - }; Follow(S) = { ) } OK
       First(P) = { *, div }; Follow(P) = { +, - , _ } OK

*******************/

enum lex_type_t { PLUS, MINUS, MULT, DEL, NUMBER, OPEN, CLOSE, END, ID };

namespace lexer {
    enum lex_type_t cur_lex_type;
    std::string cur_lex_text;

    int c;
    void init()
    {
        c = std::cin.get();
    }

    void next()
    {
        cur_lex_text.clear();
        enum state_t { H, N, P, M, D, DI, DIV, O, C, I, OK, MIN };
        state_t state = H;
        while (state != OK) {
            switch (state) {
            case H:

                if (c == '+') {
                    state = P;
                }
                else if (c == '-') {
                    state = MIN;
                }
                else if (c == '*') {
                    state = M;
                }
                else if (c == 'd') {
                    state = D;
                }
                else if (c == '(') {
                    state = O;
                }
                else if (c == ')') {
                    state = C;
                }
                else if (std::isdigit(c)) {
                    state = N;
                }
                else if (c == EOF) {
                    cur_lex_type = END;
                    state = OK;
                }
                else if (std::isspace(c)) {
                    // stay in H
                }
                else if (std::isalpha(c)) {
                    state = I;
                }
                else {
                    throw std::logic_error("unexpected character with code " + std::to_string(c));
                }
                break;

            case N:
                if (std::isdigit(c)) {
                    // stay in N
                }
                else {
                    cur_lex_type = NUMBER;
                    state = OK;
                }
                break;

            case I:
                if (std::isalnum(c))
                {
                    // stay in I
                }
                else
                {
                    cur_lex_type = ID;
                    state = OK;
                }
                break;

            case P:
                cur_lex_type = PLUS;
                state = OK;
                break;

            case MIN:
                cur_lex_type = MINUS;
                state = OK;
                break;

            case M:
                cur_lex_type = MULT;
                state = OK;
                break;

            case D:
                if (c == 'i') {
                    state = DI;
                } else {
                    state = I;
                }
                break;

            case DI:
                if (c == 'v') {
                    state = DIV;
                } else {
                    state = I;
                }
                break;

            case DIV:
                if (isalnum(c)) {
                    state = I;
                } else {
                    cur_lex_type = DEL;
                    state = OK;
                }
                break;

            case O:
                cur_lex_type = OPEN;
                state = OK;
                break;

            case C:
                cur_lex_type = CLOSE;
                state = OK;
                break;

            case OK:
                break;
            }

            if (state != OK) {
                if (!std::isspace(c) && cur_lex_type != END) {
                    cur_lex_text.push_back(c);
                }

                c = std::cin.get();
            }
        }

    }
}

namespace loop_parser {

    std::vector<std::string> pol;

    void init()
    {
        lexer::init();
        lexer::next();
        pol.clear();
    }

    void E();
    void M();
    void F();

    void E()
    {
        bool type;
        M();
        while (lexer::cur_lex_type == PLUS || lexer::cur_lex_type == MINUS) {
            if (lexer::cur_lex_type == PLUS)
            {
                type = true;
            }
            else
            {
                type = false;
            }
            lexer::next();
            M();
            if (type)
            {
                pol.push_back(std::string("+"));
            }
            else
            {
                pol.push_back(std::string("-"));
            }
        }
    }

    void M()
    {
        bool type;
        F();
        while (lexer::cur_lex_type == MULT || lexer::cur_lex_type == DEL) {
            if (lexer::cur_lex_type == MULT)
            {
                type = true;
            }
            else
            {
                type = false;
            }
            lexer::next();
            F();
            if (type)
            {
                pol.push_back(std::string("*"));
            }
            else
            {
                pol.push_back(std::string("div"));
            }
        }
    }

    void F()
    {
        if (lexer::cur_lex_type == OPEN) {
            lexer::next();
            E();
            if (lexer::cur_lex_type != CLOSE) {
                throw std::logic_error( "unexpected token; ) is expected; got <<" + lexer::cur_lex_text + ">>");
            }
            lexer::next();
        }
        else if (lexer::cur_lex_type == NUMBER) {
            pol.push_back(lexer::cur_lex_text);
            lexer::next();
        }
        else if (lexer::cur_lex_type == ID) {
            pol.push_back(lexer::cur_lex_text);
            lexer::next();
        }
        else {
            throw std::logic_error("unexpected token; ( or number are expected; got <<" + lexer::cur_lex_text + ">>");
        }
    }
}

class Postfix_Printer
{
    std::stack<std::pair<std::string, int>> st;
    std::string res;
public:
    void make_postfix(const std::vector<std::string>& pol)
    {
        for (const std::string& item : pol)
        {
            if (isdigit(item[0]) || isalpha(item[0]))
            {
                st.push(std::make_pair(item, 3));
            }
            else if (item == "*")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1.second < 2)
                {
                    arg1.first = "(" + arg1.first + ")";
                }
                if (arg2.second < 2)
                {
                    arg2.first = "(" + arg2.first + ")";
                }
                st.push(std::make_pair(arg1.first + arg2.first + " * ", 2));
            }
            else if (item == "div")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1.second < 2)
                {
                    arg1.first = "(" + arg1.first + ")";
                }
                if (arg2.second < 2)
                {
                    arg2.first = "(" + arg2.first + ")";
                }
                st.push(std::make_pair(arg1.first + arg2.first + " div ", 2));
            }
            else if (item == "+")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                st.push(std::make_pair(arg1.first + arg2.first + " + ", 1));
            }
            else
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg2.second == 1)
                {
                    arg2.first = "(" + arg2.first + ")";
                }
                st.push(std::make_pair(arg1.first + arg2.first + " - ", 1));
            }
        }
        res = std::move(st.top().first);
        st.pop();
    }
    void print_res()
    {
        std::cout << res << std::endl;
    }
};

class Transformator_into_POLIZ
{
    std::stack<std::string> st;
    std::string res;
public:
    void make_transform(const std::vector<std::string>& pol)
    {
        for (const std::string& item : pol)
        {
            if (isdigit(item[0]) || isalpha(item[0]))
            {
                st.push(item);
            }
            else if (item == "*")
            {
                std::string arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1 == "0" || arg2 == "0")
                {
                    st.push("0");
                }
                else if (arg1 == "1")
                {
                    st.push(arg2);
                }
                else if (arg2 == "1")
                {
                    st.push(arg1);
                }
                else
                {
                    st.push(arg1 + " " + arg2 + " * ");
                }
            }
            else if (item == "div")
            {
                std::string arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg2 == "1")
                {
                    st.push(arg1);
                }
                else
                {
                    st.push(arg1 + " " + arg2 + " div ");
                }
            }
            else if (item == "+")
            {
                std::string arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1 == "0")
                {
                    st.push(arg2);
                }
                else if (arg2 == "0")
                {
                    st.push(arg1);
                }
                else
                {
                    st.push(arg1 + " " + arg2 + " + ");
                }
            }
            else
            {
                std::string arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg2 == "0")
                {
                    st.push(arg1);
                }
                else
                {
                    st.push(arg1 + " " + arg2 + " - ");
                }
            }
        }
        res = std::move(st.top());
        st.pop();
    }
    void print_res()
    {
        std::cout << res << std::endl;
    }
};

class Transformator
{
    std::stack<std::pair<std::string, int>> st;
    std::string res;
public:
    void make_transform(const std::vector<std::string>& pol)
    {
        for (const std::string& item : pol)
        {
            if (isdigit(item[0]) || isalpha(item[0]))
            {
                st.push(std::make_pair(item, 3));
            }
            else if (item == "*")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1.first == "0" || arg2.first == "0")
                {
                    st.push(std::make_pair("0", 3));
                }
                else if (arg1.first == "1")
                {
                    st.push(arg2);
                }
                else if (arg2.first == "1")
                {
                    st.push(arg1);
                }
                else
                {
                    if (arg1.second < 2)
                    {
                        arg1.first = "(" + arg1.first + ")";
                    }
                    if (arg2.second < 2)
                    {
                        arg2.first = "(" + arg2.first + ")";
                    }
                    st.push(std::make_pair(arg1.first + " * " + arg2.first, 2));
                }
            }
            else if (item == "div")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg2.first == "1")
                {
                    st.push(arg1);
                }
                else
                {
                    if (arg1.second < 2)
                    {
                        arg1.first = "(" + arg1.first + ")";
                    }
                    if (arg2.second < 2)
                    {
                        arg2.first = "(" + arg2.first + ")";
                    }
                    st.push(std::make_pair(arg1.first + " div " + arg2.first, 2));
                }
            }
            else if (item == "+")
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg1.first == "0")
                {
                    st.push(arg2);
                }
                else if (arg2.first == "0")
                {
                    st.push(arg1);
                }
                else
                {
                    if (arg2.second == 1)
                    {
                        arg2.first = "(" + arg2.first + ")";
                    }
                    st.push(std::make_pair(arg1.first + " + " + arg2.first, 1));
                }
            }
            else
            {
                std::pair<std::string, int> arg1, arg2;
                arg2 = st.top();
                st.pop();
                arg1 = st.top();
                st.pop();
                if (arg2.first == "0")
                {
                    st.push(arg1);
                }
                else
                {
                    if (arg2.second == 1)
                    {
                        arg2.first = "(" + arg2.first + ")";
                    }
                    st.push(std::make_pair(arg1.first + " - " + arg2.first, 1));
                }
            }
        }
        res = std::move(st.top().first);
        st.pop();
    }
    void print_res()
    {
        std::cout << res << std::endl;
    }
};

int main()
{
    try
    {
        loop_parser::init();
        loop_parser::E();

        if (lexer::cur_lex_type != END)
        {
            throw std::logic_error("not END");
        }

        /*Postfix_Printer obj1(loop_parser::pol);
        obj1.make_postfix();
        obj1.print_res();*/


        /*
        Transformator_into_POLIZ obj2;
        obj2.make_transform(loop_parser::pol);
        obj2.print_res();

        std::cout << std::endl;
        */

        Transformator obj3;
        obj3.make_transform(loop_parser::pol);
        obj3.print_res();
    }
    catch (std::logic_error)
    {
        std::cerr << "Incorrect expression." << std::endl;
    }
    return 0;
}