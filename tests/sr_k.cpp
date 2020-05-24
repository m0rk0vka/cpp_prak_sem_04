#include <vector>
#include <iostream>
#include <cstdio>
#include <memory>

enum lex_type_t { PLUS, MINUS, MULT, NUMBER, OPEN, CLOSE, END, IDENT};
int x_value;

namespace lexer {
    enum lex_type_t cur_lex_type;
    enum lex_type_t prev_lex_type;
    std::string cur_lex_text;
    int c;

    void init() {
        c = std::cin.get();
        prev_lex_type = END;
    }

    void next() {
        cur_lex_text.clear();
        enum state_t { H, N, P, V, M, O, C, I, OK } state = H;//+ V = MINUS
        while (state != OK) {
            switch (state) {
            case H:
                if (std::isspace(c) && (c != '\n')) {
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
                } else if (std::isdigit(c)) {
                    cur_lex_text += c;
                    state = N;
                } else if (c == '\n') {
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

    class ItemVisitor
    {
        public:
            virtual ~ItemVisitor() {}
            virtual void visitIdentifier(const IdentifierItem &) = 0;
            virtual void visitAddition(const AdditionItem &) = 0;
            virtual void visitSubtraction(const SubtractionItem &) = 0;
            virtual void visitMultiplication(const MultiplicationItem &) = 0;
            virtual void visitNumber(const NumberItem &) = 0;
    };

    void IdentifierItem::accept(ItemVisitor &v) const { v.visitIdentifier(*this); }
    void NumberItem::accept(ItemVisitor &v) const { v.visitNumber(*this); }
    void AdditionItem::accept(ItemVisitor &v) const { v.visitAddition(*this); }
    void SubtractionItem::accept(ItemVisitor &v) const { v.visitSubtraction(*this); }
    void MultiplicationItem::accept(ItemVisitor &v) const { v.visitMultiplication(*this); }
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
            throw std::logic_error("Excess text after the expression");
        }
        return std::move(_poliz);
    }

    oid _addition()
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
                        std::cout << "-, " << arity_m << std::endl;
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
                        std::cout << "+, " << arity_p << std::endl;
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
            std::cout << "-, t = " << arity_m << std::endl;
            _poliz.push_back(std::make_shared<poliz::SubtractionItem>(arity_m));
        } else if (arity_p > 1) {
            std::cout << "+, t = " << arity_p << std::endl;
            _poliz.push_back(std::make_shared<poliz::AdditionItem>(arity_p));
        }
    }

    void _multiplication()
    {
        int arity = 1;
        _factor();
        while (lexer::cur_lex_type == MULT) {
            ++ arity;
            lexer::next();
            _factor();
        }
        if (arity > 1) {
            _poliz.push_back(std::make_shared<poliz::MultiplicationItem>(arity));
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
                throw std::logic_error("Unexpected token; closing parenthesis "
                        "is expected");
            }
            lexer::next();
        } else {
            throw std::logic_error("Unexpected token; identifier or number or "
                    "open parenthesis are expected");
        }
    }
}

template <typename C>
void add_back(C &poliz, const C &other) {
    for (auto i: other) {
        poliz.push_back(i);
    }
}

template <typename C>
void add_back(C &poliz, C &&other) {
    for (auto i: other) {
        poliz.push_back(i);
    }
}

// computes the expression derivative
class Deriver: poliz::ItemVisitor
{
    private:
        std::vector<poliz::Poliz> derivatives; // each item owns its poliz items
        std::vector<poliz::Poliz> operands; // each item owns its poliz items
        std::string _var;
    public:
        poliz::Poliz derive(const poliz::Poliz &e, std::string var) {
            derivatives.clear();
            operands.clear();
            std::swap(_var, var); // this statement is faster replacement for "_var = var;"
                // "var" is passed by copy for this swapping
            for (auto i: e) {
                i->accept(*this);
            }
            return std::move(derivatives.back());
        }
    private:

        // specification of each "visit" function:
        // expects the operands at the top of the stack this.operands
        // expects its derivatives at the top of the stack this.derivatives
        // reads the current operation from the "visit" function argument
        // pops its operands and its derivatives from the stacks
        // pushs the whole expression onto the top of this.operands
        // pushs its derivative onto the top of this.derivations

        void visitIdentifier(const poliz::IdentifierItem &item) {
            operands.push_back({std::make_shared<poliz::IdentifierItem>(item)});
            derivatives.push_back({std::make_shared<poliz::NumberItem>(item.name() == _var)});
        }

        void visitNumber(const poliz::NumberItem &item) {
            operands.push_back({std::make_shared<poliz::NumberItem>(item)});
            derivatives.push_back({std::make_shared<poliz::NumberItem>(0)});
        }

        void visitAddition(const poliz::AdditionItem &item) {
            // d(u + v) = du + dv

            if (item.arity() == 1) {
                return;
            }

            {
                poliz::Poliz addition_expr;
                for (int i = item.arity(); i > 0; --i) {
                    add_back(addition_expr, std::move(*(operands.end() - i)));
                }
                operands.resize(operands.size() - item.arity());
                addition_expr.push_back(std::make_shared<poliz::AdditionItem>(item));
                operands.push_back(std::move(addition_expr));
            }

            {
                poliz::Poliz addition_deriv;
                for (int i = item.arity(); i > 0; --i) {
                    add_back(addition_deriv, std::move(*(derivatives.end() - i)));
                }
                derivatives.resize(derivatives.size() - item.arity());
                addition_deriv.push_back(std::make_shared<poliz::AdditionItem>(item));
                derivatives.push_back(std::move(addition_deriv));
            }
        }

        void visitSubtraction(const poliz::SubtractionItem &item) {
            // d(u - v) = du - dv

            if (item.arity() == 1) {
                return;
            }

            {
                poliz::Poliz subtraction_expr;
                for (int i = item.arity(); i > 0; --i) {
                    add_back(subtraction_expr, std::move(*(operands.end() - i)));
                }
                operands.resize(operands.size() - item.arity());
                subtraction_expr.push_back(std::make_shared<poliz::SubtractionItem>(item));
                operands.push_back(std::move(subtraction_expr));
            }

            {
                poliz::Poliz subtraction_deriv;
                for (int i = item.arity(); i > 0; --i) {
                    add_back(subtraction_deriv, std::move(*(derivatives.end() - i)));
                }
                derivatives.resize(derivatives.size() - item.arity());
                subtraction_deriv.push_back(std::make_shared<poliz::SubtractionItem>(item));
                derivatives.push_back(std::move(subtraction_deriv));
            }
        }

        void visitMultiplication(const poliz::MultiplicationItem &item) {
            // d(u * v * t) = du * v * t + u * dv * t + u * v * dt

            if (item.arity() == 1) {
                return;
            }

            std::vector<poliz::Poliz> item_operands;
            {
                for (int i = item.arity(); i > 0; --i) {
                    item_operands.push_back(std::move(*(operands.end() - i)));
                }
                operands.resize(operands.size() - item.arity());
            }

            std::vector<poliz::Poliz> item_d_operands;
            {
                for (int i = item.arity(); i > 0; --i) {
                    item_d_operands.push_back(std::move(*(derivatives.end() - i)));
                }
                derivatives.resize(derivatives.size() - item.arity());
            }

            {
                poliz::Poliz multipl_expr;
                for (auto &operand: item_operands) {
                    add_back(multipl_expr, operand);
                }
                multipl_expr.push_back(std::make_shared<poliz::MultiplicationItem>(item));
                operands.push_back(std::move(multipl_expr));
            }

            {
                poliz::Poliz multipl_deriv;
                for (int i = 0; i < item.arity(); ++i) {
                    for (int j = 0; j < item.arity(); ++j) {
                        poliz::Poliz &t = (j == i ? item_d_operands[j] : item_operands[j]);
                        add_back(multipl_deriv, t);
                    }
                    multipl_deriv.push_back(std::make_shared<poliz::MultiplicationItem>(item));
                }
                multipl_deriv.push_back(std::make_shared<poliz::AdditionItem>(item.arity()));
                derivatives.push_back(std::move(multipl_deriv));
            }
        }
};

// prints the infix representation of the expression
class InfixPrinter: poliz::ItemVisitor
{
    private:
        std::vector<int> output;
    public:
        int calc(const poliz::Poliz &p) {
            output.clear();
            priorities.clear();
            for(auto i: p) {
                i->accept(*this);
            }
            return output.back();
        }
    private:
        void visitIdentifier(const poliz::IdentifierItem &item) {
            output.push_back(x_value);
        }
        void visitNumber(const poliz::NumberItem &item) {
            output.push_back(item.value());
        }
    private:
        void printNAryItem1(int arity) {
            if (arity == 1) {
                return;
            }

            std::vector<int> out;
            for(int i = 0; i < arity; i++) {
                out.push_back(output.back());
                output.pop_back();
            }
            for (int i = 0; i < arity - 1; ++i) {
                int arg = out.back();
                out.pop_back();
                int arg1 = out.back();
                out.pop_back();
                out.push_back(arg + arg1);
            }
            output.push_back(out.back());
        }

        void printNAryItem2(int arity) {
            if (arity == 1) {
                return;
            }

            std::vector<int> out;
            for(int i = 0; i < arity; i++) {
                out.push_back(output.back());
                output.pop_back();
            }
            for (int i = 0; i < arity - 1; ++i) {
                int arg = out.back();
                out.pop_back();
                int arg1 = out.back();
                out.pop_back();
                out.push_back(arg - arg1);
            }
            output.push_back(out.back());
        }

        void printNAryItem3(int arity) {
            if (arity == 1) {
                return;
            }

            for (int i = 0; i < arity - 1; ++i) {
                int arg = output.back();
                output.pop_back();
                int arg1 = output.back();
                output.pop_back();
                output.push_back(arg * arg1);
            }
        }
    private:
        void visitAddition(const poliz::AdditionItem &item) {
            printNAryItem1(item.arity());
        }
        void visitSubtraction(const poliz::SubtractionItem &item) {
            printNAryItem2(item.arity());
        }
        void visitMultiplication(const poliz::MultiplicationItem &item) {
            printNAryItem3(item.arity());
        }
};

int main(int argc, char **argv)
{
    try {
        auto poliz = parser::parse_full_stdin(); // it owns poliz items
        auto derived = Deriver{}.derive(poliz, "x"); // it owns derived items
        std::cin >> x_value;
        std::cout << InfixPrinter{}.calc(derived) << std::endl;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
