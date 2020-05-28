#include <iostream>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>
#include <sstream>
#include <algorithm>
#include <limits>


/*
Lexem:
P -> +

V -> -

M -> *

D -> /

O -> (

C -> )

N(number) -> 0 | ... | 9 | N1 | ... | N9

I(iden) -> A_ | B_
A -> [A-z] | A [A-z]
B -> A[0-9] | B [0-9]


GRAMMAR:
    E -> M {+M |-M}
    M -> F {*F | /F}
    F -> (E) | iden | number

preobrazyem grammaticy:
E -> MA
A -> + MA | -MA | eps
M -> FB
B -> * FB | / FB | eps
F -> (E) | iden | number

first(+MA) && first(-MA) = empty
first(*FB) && first(/FB) = empty
first(A) && follow(A) = empty
first(B) && follow(B) = empty

recursivnyi spysk primenim
 */

enum lex_type_t { PLUS, MINUS, MULT, DIV, NUMBER, OPEN, CLOSE, END, IDENT};

namespace lexer {
    enum lex_type_t cur_lex_type;
    std::string cur_lex_text;
    int c;

    void init() {
        c = std::cin.get();
    }

    void next() {
        cur_lex_text.clear();
        enum state_t { H, N, P, V, M, O, D, C, I, OK } state = H;//+ V = MINUS +D = DIV
        while (state != OK) {
            switch (state) {
            case H:
                if (std::isspace(c) || (c == '\n')) {
                    // stay in H
                } else if (c == '+') {
                    state = P;
                } else if (c == '*') {
                    state = M;
                } else if (c == '(') {
                    state = O;
                } else if (c == '-') {
                    state = V;
                } else if (c == ')') {
                    state = C;
                } else if (c == 'd') {
                    state = D;
                } else if (std::isdigit(c)) {
                    cur_lex_text += c;
                    state = N;
                } else if (c == EOF) {
                    cur_lex_type = END;
                    state = OK;
                } else if (std::isalpha(c) || (c == '_')){
                    cur_lex_text += c;
                    state = I;
                } else {
                    throw std::logic_error("");
                }
                break;

            case N:
                if (std::isdigit(c)) {
                    // stay in N
                    cur_lex_text += c;
                } else {
                    cur_lex_type = NUMBER;
                    state = OK;
                }
                break;

            case P:
                cur_lex_type = PLUS;
                state = OK;
                break;

            case M:
                cur_lex_type = MULT;
                state = OK;
                break;

            case V:
                cur_lex_type = MINUS;
                state = OK;
                break;

            case O:
                cur_lex_type = OPEN;
                state = OK;
                break;

            case C:
                cur_lex_type = CLOSE;
                state = OK;
                break;

            case D:
                cur_lex_type = DIV;
                state = OK;
                break;

            case I:
                if (std::isalnum(c) || (c == '_')){
                    //stay in I
                    cur_lex_text += c;
                } else {
                    cur_lex_type = IDENT;
                    state = OK;
                }
                break;

            case OK:
                break;
            }

            if (state != OK){
                c = std::cin.get();
            }
        }
    }
}

namespace poliz
{
    class ItemVisitor;

    class ExpressionItem
    {
        public:
            virtual void accept(ItemVisitor &) const = 0;
            virtual ~ExpressionItem() {}
    };

    typedef std::vector<std::shared_ptr<ExpressionItem>> Poliz;

    class IdentifierItem: public ExpressionItem
    {
        private:
            std::string _name;
        public:
            IdentifierItem(std::string name) noexcept: _name(std::move(name)) {}

            const std::string &name() const noexcept { return _name; }

            void accept(ItemVisitor &) const override;
    };

    class NumberItem: public ExpressionItem
    {
        private:
            int _value;
        public:
            NumberItem(int value) noexcept: _value(value) {}

            int value() const noexcept { return _value; }

            void accept(ItemVisitor &) const override;
    };

    class AdditionItem: public ExpressionItem
    {
        private:
            int _arity;
        public:
            AdditionItem(int arity) noexcept: _arity(arity) {}
            int arity() const noexcept { return _arity; }
            void accept(ItemVisitor &) const override;
    };

    class SubtractionItem: public ExpressionItem
    {
        private:
            int _arity;
        public:
            SubtractionItem(int arity) noexcept: _arity(arity) {}
            int arity() const noexcept { return _arity; }
            void accept(ItemVisitor &) const override;
    };

    class MultiplicationItem: public ExpressionItem
    {
        private:
            int _arity;
        public:
            MultiplicationItem(int arity) noexcept: _arity(arity) {}
            int arity() const noexcept { return _arity; }
            void accept(ItemVisitor &) const override;
    };

    class DivisionItem: public ExpressionItem
    {
        private:
            int _arity;
        public:
            DivisionItem(int arity) noexcept: _arity(arity) {}
            int arity() const noexcept { return _arity; }
            void accept(ItemVisitor &) const override;
    };

    class ItemVisitor
    {
        public:
            virtual ~ItemVisitor() {}
            virtual void visitIdentifier(const IdentifierItem &) = 0;
            virtual void visitAddition(const AdditionItem &) = 0;
            virtual void visitSubtraction(const SubtractionItem &) = 0;
            virtual void visitMultiplication(const MultiplicationItem &) = 0;
            virtual void visitDivision(const DivisionItem &) = 0;
            virtual void visitNumber(const NumberItem &) = 0;
    };

    void IdentifierItem::accept(ItemVisitor &v) const { v.visitIdentifier(*this); }
    void NumberItem::accept(ItemVisitor &v) const { v.visitNumber(*this); }
    void AdditionItem::accept(ItemVisitor &v) const { v.visitAddition(*this); }
    void SubtractionItem::accept(ItemVisitor &v) const { v.visitSubtraction(*this); }
    void MultiplicationItem::accept(ItemVisitor &v) const { v.visitMultiplication(*this); }
    void DivisionItem::accept(ItemVisitor &v) const { v.visitDivision(*this); }
}

namespace parser
{
    /*****
GRAMMAR:
addition -> multiplication { PLUS | MINUS multiplication }
multiplication -> factor { MULT factor }
factor -> IDENT | NUMBER | OPEN addition CLOSE
    *******/

    // poliz owns its items objects
    poliz::Poliz _poliz;

    // each function push_backs its subexpression poliz onto the parser::poliz
    void _addition();
    void _multiplication();
    void _factor();

    // caller owns the poliz items
    poliz::Poliz parse_full_stdin()
    {
        _poliz.clear();
        lexer::init();
        lexer::next();
        _addition();
        if (lexer::cur_lex_type != END) {
            throw std::logic_error("");
        }
        return std::move(_poliz);
    }

    void _addition()
    {
        int arity_p = 0, arity_m = 0;
        char prev = '=';
        _multiplication();
        while (lexer::cur_lex_type == PLUS || lexer::cur_lex_type == MINUS) {
            if (lexer::cur_lex_type == PLUS) {
                if (prev == '=') {
                    ++arity_p;
                }
                if (prev == '-') {
                    ++ arity_p;
                    if (arity_m > 1) {
                        _poliz.push_back(std::make_shared<poliz::SubtractionItem>(arity_m));
                    }
                }
                arity_m = 0;
                prev = '+';
                ++ arity_p;
            } else {
                if (prev == '=') {
                    ++arity_m;
                }
                if (prev == '+') {
                    ++ arity_m;
                    if (arity_p > 1) {
                        _poliz.push_back(std::make_shared<poliz::AdditionItem>(arity_p));
                    }
                }
                arity_p = 0;
                prev = '-';
                ++ arity_m;
            }
            lexer::next();
            _multiplication();
        }
        if (arity_m > 1) {
            _poliz.push_back(std::make_shared<poliz::SubtractionItem>(arity_m));
        } else if (arity_p > 1) {
            _poliz.push_back(std::make_shared<poliz::AdditionItem>(arity_p));
        }
    }

    void _multiplication()
    {
        int arity_mult = 0, arity_div = 0;
        char prev = '=';
        _factor();
        while (lexer::cur_lex_type == MULT || lexer::cur_lex_type == DIV) {
            if (lexer::cur_lex_type == MULT) {
                if (prev == '=') {
                    ++arity_mult;
                }
                if (prev == '/') {
                    ++ arity_mult;
                    if (arity_div > 1) {
                        _poliz.push_back(std::make_shared<poliz::DivisionItem>(arity_div));
                    }
                }
                arity_div = 0;
                prev = '*';
                ++ arity_mult;
            } else {
                if (prev == '=') {
                    ++arity_div;
                }
                if (prev == '*') {
                    ++ arity_div;
                    if (arity_mult > 1) {
                        _poliz.push_back(std::make_shared<poliz::MultiplicationItem>(arity_mult));
                    }
                }
                arity_mult = 0;
                prev = '/';
                ++ arity_div;
            }
            lexer::next();
            _factor();
        }
        if (arity_mult > 1) {
            _poliz.push_back(std::make_shared<poliz::MultiplicationItem>(arity_mult));
        } else if (arity_div > 1) {
            _poliz.push_back(std::make_shared<poliz::DivisionItem>(arity_div));
        }
    }

    void _factor()
    {
        if (lexer::cur_lex_type == IDENT) {
            std::string id { lexer::cur_lex_text };
            lexer::next();
            _poliz.push_back(std::make_shared<poliz::IdentifierItem>(std::move(id)));
        } else if (lexer::cur_lex_type == NUMBER) {
            int value { std::atoi((lexer::cur_lex_text).c_str()) };
            lexer::next();
            _poliz.push_back(std::make_shared<poliz::NumberItem>( value ));
        } else if (lexer::cur_lex_type == OPEN) {
            lexer::next();
            _addition();
            if (lexer::cur_lex_type != CLOSE) {
                throw std::logic_error("");
            }
            lexer::next();
        } else {
            throw std::logic_error("");
        }
    }
}

// prints the infix representation of the expression
class InfixPrinter: poliz::ItemVisitor
{
    private:
        std::vector<std::string> output;
        std::vector<int> priorities;
        // priorities: 3 = id, num ; 2 = mult ; 1 = add
    public:
        std::ostream &print(const poliz::Poliz &p, std::ostream &s) {
            output.clear();
            priorities.clear();
            for (auto i: p) {
                i->accept(*this);
            }
            return s << output.back();
        }
    private:
        void visitIdentifier(const poliz::IdentifierItem &item) {
            output.push_back(item.name());
            priorities.push_back(5);
        }
        void visitNumber(const poliz::NumberItem &item) {
            output.push_back(std::to_string(item.value()));
            priorities.push_back(5);
        }
    private:
        void printNAryItem(int arity, int priority, char sign) {
            if (arity == 1) {
                return;
            }

            std::vector<std::string> outVector;
            for (int i = 0; i < arity; ++i) {
                std::string arg_text = std::move(output.back());
                output.pop_back();
                int arg_priority = priorities.back();
                priorities.pop_back();

                if (i > 0) {
                    outVector.push_back(std::string{" "} + sign + " ");
                }
                if (arg_priority < priority || (arg_priority == 2 && priority == 2) || (arg_priority == 4 && priority == 4)) {
                    outVector.push_back("(" + arg_text + ")");
                } else {
                    outVector.push_back(arg_text);
                }
            }
            std::reverse(outVector.begin(), outVector.end());
            std::string out;
            for (auto i: outVector) {
                out += i;
            }
            output.push_back(std::move(out));
            priorities.push_back(priority);
        }
    private:
        void visitAddition(const poliz::AdditionItem &item) {
            printNAryItem(item.arity(), 1, '+');
        }
        void visitMultiplication(const poliz::MultiplicationItem &item) {
            printNAryItem(item.arity(), 3, '*');
        }
        void visitDivision(const poliz::DivisionItem &item) {
            printNAryItem(item.arity(), 4, '/');
        }
        void visitSubtraction(const poliz::SubtractionItem &item) {
            printNAryItem(item.arity(), 2, '-');
        }
};

/* Simplification of poliz:
 *    expr + 0 ==> expr; -> expr3 expr2 expr1 0 + + +
 *    0 + expr ==> expr; -> expr3 expr2 0 expr1 + + +
 *    expr - 0 ==> expr; -> expr3 expr2 expr1 0 - - -
 *    expr * 0 ==> 0;    -> expr3 expr2 expr1 0 * (arity)
 *    0 * expr ==> 0.    -> expr3 expr2 0 expr1 *
 */
class SimplificationPoliz: poliz::ItemVisitor
{
    private:
        poliz::Poliz temp;
        bool read, zero, one;
        int l_arity;
    public:
        poliz::Poliz &print(const poliz::Poliz &p) {
            read = zero = one = false;
            l_arity = 0;
            for (auto i : p) {
                read = false;
                i -> accept(*this);
            }
            return temp;
        }
    private:
        void printNAryItem(int arity, char sign) {
            poliz::Poliz temp_1;
            int new_arity = arity;
            read = true;
            if (sign == '+') {
                int i = 0;
                while (i < arity) {
                    l_arity = 0;
                    temp.back() -> accept(*this);
                    if (l_arity > 0) {
                        while (l_arity > 0) {
                            temp_1.push_back(temp.back());
                            temp.pop_back();
                            --l_arity;
                        }
                    } else {
                        if (!zero){
                            temp_1.push_back(temp.back());
                        } else {
                            --new_arity;
                        }
                        temp.pop_back();
                    }
                    i++;
                }
                for (int i = 0; i < new_arity; ++i) {
                    temp.push_back(temp_1.back());
                    temp_1.pop_back();
                }
                if (new_arity > 1) {
                    temp.push_back(std::make_shared<poliz::AdditionItem>(new_arity));
                }
            }
            if (sign == '-') {
                for (int i = 0; i < arity; ++i) {
                    l_arity = 0;
                    temp.back() -> accept(*this);
                    while (l_arity > 0) {
                        temp_1.push_back(temp.back());
                        temp.pop_back();
                        --l_arity;
                    }
                    if (!zero || (i == arity - 1)){
                        temp_1.push_back(temp.back());
                    } else {
                        --new_arity;
                    }
                    temp.pop_back();
                }
                for (int i = 0; i < new_arity; ++i) {
                    temp.push_back(temp_1.back());
                    temp_1.pop_back();
                }
                if (new_arity > 1) {
                    temp.push_back(std::make_shared<poliz::SubtractionItem>(new_arity));
                }
            }
            if (sign == '*') {
                bool flag = false;
                for (int i = 0; i < arity; ++i) {
                    l_arity = 0;
                    temp.back() -> accept(*this);
                    while (l_arity > 0) {
                        temp_1.push_back(temp.back());
                        temp.pop_back();
                        --l_arity;
                    }
                    if (zero) {
                        flag = true;
                    }
                    if (!one){
                        temp_1.push_back(temp.back());
                    } else {
                        --new_arity;
                    }
                    temp.pop_back();
                }
                if (!flag) {
                    for (int i = 0; i < new_arity; ++i) {
                        temp.push_back(temp_1.back());
                        temp_1.pop_back();
                    }
                    if (new_arity > 1) {
                        temp.push_back(std::make_shared<poliz::MultiplicationItem>(new_arity));
                    }
                } else {
                    temp.push_back(std::make_shared<poliz::NumberItem>(0));
                }
            }
            if (sign == '/') {
                for (int i = 0; i < arity; ++i) {
                    l_arity = 0;
                    temp.back() -> accept(*this);
                    if (l_arity > 0){
                        --i;
                    }
                    while (l_arity > 0) {
                        temp_1.push_back(temp.back());
                        temp.pop_back();
                        --l_arity;
                    }
                    if (!one || (i == arity - 1)){
                        temp_1.push_back(temp.back());
                    } else {
                        --new_arity;
                    }
                    temp.pop_back();
                }
                for (int i = 0; i < new_arity; ++i) {
                    temp.push_back(temp_1.back());
                    temp_1.pop_back();
                }
                if (new_arity > 1) {
                    temp.push_back(std::make_shared<poliz::DivisionItem>(new_arity));
                }
            }
            read = false;
        }
    private:
        void visitIdentifier(const poliz::IdentifierItem &item) {
            if (read) {
                zero = false;
                one = false;
                l_arity = 0;
            } else {
                temp.push_back(std::make_shared<poliz::IdentifierItem>(item.name()));
            }
        }
        void visitNumber(const poliz::NumberItem &item) {
            if (read) {
                zero = item.value() == 0;
                one = item.value() == 1;
                l_arity = 0;
                std::cout << "read num" << std::endl;
            } else {
                temp.push_back(std::make_shared<poliz::NumberItem>(item.value()));
                std::cout << "write num" <<std::endl;
            }
        }
        void visitAddition(const poliz::AdditionItem &item) {
            if (read){
                zero = false;
                l_arity = item.arity() + 1;
            } else {
                printNAryItem(item.arity(), '+');
            }
        }
        void visitMultiplication(const poliz::MultiplicationItem &item) {
            if (read){
                zero = false;
                l_arity = item.arity() + 1;
            } else {
                printNAryItem(item.arity(), '*');
            }
        }
        void visitSubtraction(const poliz::SubtractionItem &item) {
            if (read){
                zero = false;
                l_arity = item.arity() + 1;
                std::cout << "read -" << std::endl;
            } else {
                std::cout << "write -" << std::endl;
                printNAryItem(item.arity(), '-');
            }
        }
        void visitDivision(const poliz::DivisionItem &item) {
            if (read){
                zero = false;
                l_arity = item.arity();
            } else {
                printNAryItem(item.arity(), '/');
            }
        }
};

int main()
{
    //sostav proizvodnyi
    try {
        auto poliz = parser::parse_full_stdin(); // it owns poliz items
        InfixPrinter{}.print(poliz, std::cout) << std::endl;
        auto s_poliz = SimplificationPoliz{}.print(poliz);
        for (auto el: s_poliz) {
            std::cout << "el" << std::endl;
        }
        InfixPrinter{}.print(s_poliz, std::cout) << std::endl;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

