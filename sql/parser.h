#include "structs.h"
#include <sstream>

int c1, c2;
std::stringstream str_cin, str_like;

enum lex_type_t {SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FROM, INTO, SET, TABLE,
    TEXT, LONG, PLUS, MINUS, MULT, DIV, MOD, EQUALLY, GREAT, LESS, GREAT_EQUAL, LESS_EQUAL,
    NOT_EQUAL, OPEN, CLOSE, WHERE, NOT, LIKE, IN, ALL, AND, OR, END, STR_VAL, COMMA, LONGNUM, IDENT,
    CHAR, PERCENT, UNDER, XOR, DASH};

bool isal_num(int c) {
    return isalnum(c) || c == '_';
}

struct lex {
    lex_type_t type;
    std::string text;
};

namespace lexer {
    lex_type_t cur_lex_type;
    std::string cur_lex_text;
    int cur_lex_pos, c;

    void init() {
        c = str_cin.get();
        if (c == EOF || c == '\n') {
            throw std::logic_error("Empty request");
        }
        lexer::cur_lex_pos = 1;
    }

    void next() {
        lexer::cur_lex_text.clear();
        enum state_t { H, S, SE, SEL, SELE, SELEC, SELECT, I, IN, INS, INSE, INSER, INSERT,
            U, UP, UPD, UPDA, UPDAT, UPDATE, D, DE, DEL, DELE, DELET, DELETE, C, CR, CRE,
            CREA, CREAT, CREATE, DR, DRO, DROP, F, FR, FRO, FROM, INT, INTO, SET, T, TA, TAB,
            TABL, TABLE, TE, TEX, TEXT, L, LO, LON, LONG, EQ, OP, CL, PL, MIN, MULT, DIV, MOD,
            GR, GR_EQ, LS, LS_EQ, NT, NT_EQ, W, WH, WHE, WHER, WHERE, N, NO, NOT, LI, LIK, LIKE,
            A, AL, ALL, AN, AND, O, OR, START_S, MIDDLE_S, END_S, COM, LONGNUM, IDENT, OK } state = H;
        while (state != OK) {
            switch (state) {
            case H:
                if (isspace(c)) {
                    //stay in H
                } else if (c == 'S') {
                    state = S;
                } else if (c == 'I') {
                    state = I;
                } else if (c == 'U') {
                    state = U;
                } else if (c == 'D') {
                    state = D;
                } else if (c == 'C') {
                    state = C;
                } else if (c == 'F') {
                    state = F;
                } else if (c == 'T') {
                    state = T;
                } else if (c == 'L') {
                    state = L;
                } else if (c == 'W') {
                    state = W;
                } else if (c == 'N') {
                    state = N;
                } else if (c == 'A') {
                    state = A;
                } else if (c == 'O') {
                    state = O;
                } else if (c == '(') {
                    state = OP;
                } else if (c == ')') {
                    state = CL;
                } else if (c == '=') {
                    state = EQ;
                } else if (c == '+') {
                    state = PL;
                } else if (c == '-') {
                    state = MIN;
                } else if (c == '*') {
                    state = MULT;
                } else if (c == '/') {
                    state = DIV;
                } else if (c == '%') {
                    state = MOD;
                } else if (c == '>') {
                    state = GR;
                } else if (c == '<') {
                    state = LS;
                } else if (c == '!') {
                    state = NT;
                } else if (c == '\'') {
                    state = START_S;
                } else if (c == ',') {
                    state = COM;
                } else if (isalpha(c) || c == '_') {
                    state = IDENT;
                } else if (isdigit(c)) {
                    state = LONGNUM;
                } else if (c == '\n' || c == EOF) {
                    lexer::cur_lex_type = lex_type_t::END;
                    state = OK;
                } else {
                    throw std::logic_error("Unexpected caracter with code " + std::to_string(c) + " in position " + std::to_string(lexer::cur_lex_pos));
                }
                break;
//          SELECT
            case S:
                if (c == 'E') {
                    state = SE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SE:
                if (c == 'L') {
                    state = SEL;
                } else if (c == 'T') {
                    state = SET;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SEL:
                if (c == 'E') {
                    state = SELE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELE:
                if (c == 'C') {
                    state = SELEC;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELEC:
                if (c == 'T') {
                    state = SELECT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELECT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::SELECT;
                    state = OK;
                }
                break;
//          INSERT
            case I:
                if (c == 'N') {
                    state = IN;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case IN:
                if (c == 'S') {
                    state = INS;
                } else if (c == 'T') {
                    state = INT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IN; // word IN - not IDENT identifier
                    state = OK;
                }
                break;

            case INS:
                if (c == 'E') {
                    state = INSE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSE:
                if (c == 'R') {
                    state = INSER;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSER:
                if (c == 'T') {
                    state = INSERT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSERT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::INSERT;
                    state = OK;
                }
                break;
//          UPDATE
            case U:
                if (c == 'P') {
                    state = UP;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UP:
                if (c == 'D') {
                    state = UPD;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPD:
                if (c == 'A') {
                    state = UPDA;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDA:
                if (c == 'T') {
                    state = UPDAT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDAT:
                if (c == 'E') {
                    state = UPDATE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDATE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::UPDATE;
                    state = OK;
                }
                break;
//          DELETE
            case D:
                if (c == 'E') {
                    state = DE;
                } else if (c == 'R') {
                    state = DR;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DE:
                if (c == 'L') {
                    state = DEL;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DEL:
                if (c == 'E') {
                    state = DELE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELE:
                if (c == 'T') {
                    state = DELET;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELET:
                if (c == 'E') {
                    state = DELETE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELETE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::DELETE;
                    state = OK;
                }
                break;
//          CREATE
            case C:
                if (c == 'R') {
                    state = CR;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CR:
                if (c == 'E') {
                    state = CRE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CRE:
                if (c == 'A') {
                    state = CREA;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREA:
                if (c == 'T') {
                    state = CREAT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREAT:
                if (c == 'E') {
                    state = CREATE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREATE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::CREATE;
                    state = OK;
                }
                break;
//          DROP
            case DR:
                if (c == 'O') {
                    state = DRO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DRO:
                if (c == 'P') {
                    state = DROP;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DROP:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::DROP;
                    state = OK;
                }
                break;
//          FROM
            case F:
                if (c == 'R') {
                    state = FR;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FR:
                if (c == 'O') {
                    state = FRO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FRO:
                if (c == 'M') {
                    state = FROM;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FROM:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::FROM;
                    state = OK;
                }
                break;
//          INTO
            case INT:
                if (c == 'O') {
                    state = INTO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INTO:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::INTO;
                    state = OK;
                }
                break;
//          SET
            case SET:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::SET;
                    state = OK;
                }
                break;
//          TABLE
            case T:
                if (c == 'A') {
                    state = TA;
                } else if (c == 'E') {
                    state = TE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TA:
                if (c == 'B') {
                    state = TAB;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TAB:
                if (c == 'L') {
                    state = TABL;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TABL:
                if (c == 'E') {
                    state = TABLE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TABLE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::TABLE;
                    state = OK;
                }
                break;
//          TEXT
            case TE:
                if (c == 'X') {
                    state = TEX;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TEX:
                if (c == 'T') {
                    state = TEXT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TEXT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::TEXT;
                    state = OK;
                }
                break;
//          LONG
            case L:
                if (c == 'O') {
                    state = LO;
                } else if (c == 'I') {
                    state = LI;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LO:
                if (c == 'N') {
                    state = LON;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LON:
                if (c == 'G') {
                    state = LONG;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LONG:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::LONG;
                    state = OK;
                }
                break;
//          LONGNUM
            case LONGNUM:
                if (isdigit(c)) {
                    //stai in LONGNUM
                } else {
                    lexer::cur_lex_type = lex_type_t::LONGNUM;
                    state = OK;
                }
                break;
//          IDENT
            case IDENT:
                if (isal_num(c)) {
                    // stay in IDENT
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;
//          WHERE
            case W:
                if (c == 'H') {
                    state = WH;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case WH:
                if (c == 'E') {
                    state = WHE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case WHE:
                if (c == 'R') {
                    state = WHER;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case WHER:
                if (c == 'E') {
                    state = WHERE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case WHERE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::WHERE;
                    state = OK;
                }
                break;
//          NOT
            case N:
                if (c == 'O') {
                    state = NO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case NO:
                if (c == 'T') {
                    state = NOT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case NOT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::NOT;
                    state = OK;
                }
                break;
//          LIKE
            case LI:
                if (c == 'K') {
                    state = LIK;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LIK:
                if (c == 'E') {
                    state = LIKE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LIKE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::LIKE;
                    state = OK;
                }
                break;
//          ALL
            case A:
                if (c == 'L') {
                    state = AL;
                } else if (c == 'N') {
                    state = AN;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case AL:
                if (c == 'L') {
                    state = ALL;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case ALL:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::ALL;
                    state = OK;
                }
                break;
//          AND
            case AN:
            if (c == 'D') {
                state = AND;
            } else if (isal_num(c)) {
                state = IDENT;
            } else {
                lexer::cur_lex_type = lex_type_t::IDENT;
                state = OK;
            }
            break;

        case AND:
            if (isal_num(c)) {
                state = IDENT;
            } else {
                lexer::cur_lex_type = lex_type_t::AND;
                state = OK;
            }
            break;
//          OR
            case O:
                if (c == 'R') {
                    state = OR;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case OR:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    lexer::cur_lex_type = lex_type_t::OR;
                    state = OK;
                }
                break;
//          COMMA
            case COM:
                lexer::cur_lex_type = lex_type_t::COMMA;
                state = OK;
                break;
//          STR_VAL
            case START_S:
                if (c == '\n') {
                    throw std::logic_error("Unexpected caracter with code " + std::to_string(c) + " in position " + std::to_string(lexer::cur_lex_pos));
                } else if (c == '\'') {
                    throw std::logic_error("You input empty string between \'\' in position " + std::to_string(lexer::cur_lex_pos - 1));
                } else {
                    state = MIDDLE_S;
                }
                break;
            case MIDDLE_S:
                if (c == '\n') {
                    throw std::logic_error("Unexpected caracter with code " + std::to_string(c) + " in position " + std::to_string(lexer::cur_lex_pos));
                } else if (c == '\'') {
                    state = END_S;
                }
                break;
            case END_S:
                lexer::cur_lex_type = lex_type_t::STR_VAL;
                state = OK;
                break;
//          WHERE-clause
            case OP:
                lexer::cur_lex_type = lex_type_t::OPEN;
                state = OK;
                break;
            case CL:
                lexer::cur_lex_type = lex_type_t::CLOSE;
                state = OK;
                break;
            case EQ:
                lexer::cur_lex_type = lex_type_t::EQUALLY;
                state = OK;
                break;
            case PL:
                lexer::cur_lex_type = lex_type_t::PLUS;
                state = OK;
                break;
            case MIN:
                lexer::cur_lex_type = lex_type_t::MINUS;
                state = OK;
                break;
            case MULT:
                lexer::cur_lex_type = lex_type_t::MULT;
                state = OK;
                break;
            case DIV:
                lexer::cur_lex_type = lex_type_t::DIV;
                state = OK;
                break;
            case MOD:
                lexer::cur_lex_type = lex_type_t::MOD;
                state = OK;
                break;
            case GR:
                if (c == '=') {
                    state = GR_EQ;
                } else {
                    lexer::cur_lex_type = lex_type_t::GREAT;
                    state = OK;
                }
                break;
            case GR_EQ:
                lexer::cur_lex_type = lex_type_t::GREAT_EQUAL;
                state = OK;
                break;
            case LS:
                if (c == '=') {
                    state = LS_EQ;
                } else {
                    lexer::cur_lex_type = lex_type_t::LESS;
                    state = OK;
                }
                break;
            case LS_EQ:
                lexer::cur_lex_type = lex_type_t::LESS_EQUAL;
                state = OK;
                break;
            case NT:
                if (c == '=') {
                    state = NT_EQ;
                } else {
                    throw std::logic_error("Unexpected caracter with code " + std::to_string(c) + " in position " + std::to_string(lexer::cur_lex_pos));
                }
                break;
            case NT_EQ:
                lexer::cur_lex_type = lex_type_t::NOT_EQUAL;
                state = OK;
                break;
//          OK
            case OK:
                break;
            }

            if (state != OK) {
                if (!isspace(c) || state == START_S) {
                    lexer::cur_lex_text.push_back(c);
                }
                c = str_cin.get();
                ++cur_lex_pos;
            }
        }
    }
}

namespace like{
    lex_type_t cur_lex_type;
    int cur_lex_text, c;

    void init() {
        str_like.get();
        c = str_like.get();
    }

    void next() {
        enum state_t { H, CH, P, U, O, CL, X, D, OK } state = H;
        while (state != OK) {
            switch (state) {
            case H:
                if (c == '%') {
                    state = P;
                } else if (c == '_') {
                    state = U;
                } else if (c == '[') {
                    state = O;
                } else if (c == ']') {
                    state = CL;
                } else if (c == '^') {
                    state = X;
                } else if (c == '-') {
                    state = D;
                } else if (c == '\'') {
                    cur_lex_type = lex_type_t::END;
                    state = OK;
                } else {
                    state = CH;
                }
                break;

            case CH:
                cur_lex_type = lex_type_t::CHAR;
                state = OK;
                break;

            case P:
                cur_lex_type = lex_type_t::PERCENT;
                state = OK;
                break;

            case U:
                cur_lex_type = lex_type_t::UNDER;
                state = OK;
                break;

            case O:
                cur_lex_type = lex_type_t::OPEN;
                state = OK;
                break;

            case CL:
                cur_lex_type = lex_type_t::CLOSE;
                state = OK;
                break;

            case X:
                cur_lex_type = lex_type_t::XOR;
                state = OK;
                break;

            case D:
                cur_lex_type = lex_type_t::DASH;
                state = OK;
                break;

            case OK:
                break;
            }

            if (state != OK) {
                cur_lex_text = c;
                c = str_like.get();
            }
        }
    }
}

bool comparison_op() {
    bool strictly = lexer::cur_lex_type == lex_type_t::EQUALLY || lexer::cur_lex_type == lex_type_t::GREAT || lexer::cur_lex_type == lex_type_t::LESS;
    bool no_strictly = lexer::cur_lex_type == lex_type_t::GREAT_EQUAL || lexer::cur_lex_type == lex_type_t::LESS_EQUAL || lexer::cur_lex_type == lex_type_t::NOT_EQUAL;
    return strictly || no_strictly;
}

namespace parser{
    std::string request_type;
    struct_select request_select;
    struct_insert request_insert;
    struct_update request_update;
    struct_delete request_delete;
    struct_create request_create;
    struct_drop request_drop;
    struct_field_description des_tmp;
//  to WHERE-clause
    std::string where_clause_type;
    struct_like_where_clause like_where_clause;
    struct_in_where_clause in_where_clause;
    struct_bool_where_clause bool_where_clause;
    int index_vec_lex;
    bool where_clause_like_flag, where_clause_bool_flag;
    std::vector<lex> vec_lex;
    std::vector<std::string> vec_str_tmp;
    std::string str_tmp;

    void init() {
        lexer::init();
        lexer::next();
        request_select.clear();
        request_insert.clear();
        request_update.clear();
        request_delete.clear();
        request_create.clear();
        request_drop.clear();
//      WHERE-clause
        where_clause_type.clear();
        like_where_clause.clear();
        in_where_clause.clear();
        bool_where_clause.clear();
        index_vec_lex = 0;
        where_clause_like_flag =  where_clause_bool_flag = false;
        vec_lex.clear();
        vec_str_tmp.clear();
        str_tmp.clear();
    }

    void check_end() {
        if (lexer::cur_lex_type != lex_type_t::END) {
            throw std::logic_error("No end");
        }
    }

    void S();
    void I();
    void I_Field_Value();
    void I_Long_Int();
    void U();
    void U_Expression();
    void U_Long_Expression();
    void U_Long_Term();
    void U_Long_Factor();
    void U_Long_Value();
    void U_Long_Int();
    void DE();
    void C();
    void C_Field_Description_List();
    void C_Field_Type();
    void DR();
//  WHERE-clause
    void W();
    void W_Like();
    void W_Like_Sample_String();
    void W_Like_Enum();
    void W_Like_Enum_Next_If_Not_Dash();
    void W_Like_Enum_Start();
    void W_Like_Enum_If_Dash();
    void W_Like_Enum_Next();
    void W_Like_Enum_If_Not_Dash();
    void W_In();
    void W_In_Expression();
    void W_In_Long_Expression();
    void W_In_Long_Term();
    void W_In_Long_Factor();
    void W_In_Long_Value();
    void W_In_Long_Int();
    void W_In_Const_List();
    void W_In_Const_String();
    void W_In_Const_Long();
    void W_In_Const_Long_Int();
    void W_Bool();
    void W_Bool_Term();
    void W_Bool_Factor();
    void W_Bool_Expression();
    void W_Bool_Ratio();
    void W_Bool_Text_Ratio();
    void W_Bool_Long_Ratio();
    void W_Bool_Long_Term();
    void W_Bool_Long_Factor();
    void W_Bool_Long_Value();
    void W_Bool_Long_Int();
    void W_Bool_Comparison_Operation();

    void H() {
        if (lexer::cur_lex_type == lex_type_t::SELECT) {
            request_type = "SELECT";
            lexer::next();
            S();
        } else if (lexer::cur_lex_type == lex_type_t::INSERT) {
            request_type = "INSERT";
            lexer::next();
            I();
        } else if (lexer::cur_lex_type == lex_type_t::UPDATE) {
            request_type = "UPDATE";
            lexer::next();
            U();
        } else if (lexer::cur_lex_type == lex_type_t::DELETE) {
            request_type = "DELETE";
            lexer::next();
            DE();
        } else if (lexer::cur_lex_type == lex_type_t::CREATE) {
            request_type = "CREATE";
            lexer::next();
            C();
        } else if (lexer::cur_lex_type == lex_type_t::DROP) {
            request_type = "DROP";
            lexer::next();
            DR();
        } else {
            throw std::logic_error("No words \"SELECT|INSERT|UPDATE|DELETE|CREATE|DROP\" at the start");
        }
    }

    void S() {
        if (lexer::cur_lex_type == lex_type_t::IDENT) {
            request_select.fields.push_back(lexer::cur_lex_text);
            lexer::next();
            while (lexer::cur_lex_type == lex_type_t::COMMA) {
                lexer::next();
                if (lexer::cur_lex_type != lex_type_t::IDENT) {
                    throw std::logic_error("Bad field name");
                }
                request_select.fields.push_back(lexer::cur_lex_text);
                lexer::next();
            }
        } else if (lexer::cur_lex_type == lex_type_t::MULT) {
            lexer::next();
        } else {
            throw std::logic_error("No fields after word \'SELECT\'");
        }
        if (lexer::cur_lex_type != lex_type_t::FROM) {
            throw std::logic_error("No word \"FROM\" after field list");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_select.name = lexer::cur_lex_text;
        lexer::next();
        W();
    }

    void I() {
        if (lexer::cur_lex_type != lex_type_t::INTO) {
            throw std::logic_error("No word \"INTO\" after \"INSERT\"");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_insert.name = lexer::cur_lex_text;
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::OPEN) {
            throw std::logic_error("No \"(\" before field value");
        }
        lexer::next();
        I_Field_Value();
        while (lexer::cur_lex_type == lex_type_t::COMMA) {
            lexer::next();
            I_Field_Value();
        }
        if (lexer::cur_lex_type != lex_type_t::CLOSE) {
            throw std::logic_error("No \")\" after field value");
        }
        lexer::next();
    }

    void I_Field_Value() {
        if (lexer::cur_lex_type == lex_type_t::STR_VAL) {
            request_insert.fields_str.push_back(lexer::cur_lex_text);
            request_insert.flags.push_back(0);
            lexer::next();
        } else {
            I_Long_Int();
        }
    }

    void I_Long_Int() {
        if (lexer::cur_lex_type == lex_type_t::LONGNUM) {
            request_insert.fields_num.push_back(std::strtol(lexer::cur_lex_text.data(), nullptr, 10));
            if (errno == ERANGE)
            {
                errno = 0;
                throw std::logic_error("Too big number " + lexer::cur_lex_text);
            }
            request_insert.flags.push_back(1);
            lexer::next();
        } else if (lexer::cur_lex_type == lex_type_t::PLUS) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'+\'");
            }
            request_insert.fields_num.push_back(std::strtol(lexer::cur_lex_text.data(), nullptr, 10));
            if (errno == ERANGE)
            {
                errno = 0;
                throw std::logic_error("Too big number " + lexer::cur_lex_text);
            }
            request_insert.flags.push_back(1);
            lexer::next();
        } else if (lexer::cur_lex_type == lex_type_t::MINUS) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'-\'");
            }
            request_insert.fields_num.push_back(-1 * std::strtol(lexer::cur_lex_text.data(), nullptr, 10));
            if (errno == ERANGE)
            {
                errno = 0;
                throw std::logic_error("Too big number -" + lexer::cur_lex_text);
            }
            request_insert.flags.push_back(1);
            lexer::next();
        } else {
            throw std::logic_error("No string or long int in field value");
        }
    }

    void U() {
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_update.name = lexer::cur_lex_text;
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::SET) {
            throw std::logic_error("No word \"SET\" after table name");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad field name");
        }
        request_update.field = lexer::cur_lex_text;
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::EQUALLY) {
            throw std::logic_error("No \"=\" after field name");
        }
        lexer::next();
        U_Expression();
        W();
    }

    void U_Expression() {
        if (lexer::cur_lex_type == lex_type_t::STR_VAL) {
            request_update.expression.push_back(lexer::cur_lex_text);
            lexer::next();
        } else {
            U_Long_Expression();
        }
    }

    void U_Long_Expression() {
        U_Long_Term();
        while (lexer::cur_lex_type == lex_type_t::PLUS || lexer::cur_lex_type == lex_type_t::MINUS) {
            bool sign = lexer::cur_lex_type == lex_type_t::MINUS;
            lexer::next();
            U_Long_Term();
            if (!sign) {
                request_update.expression.push_back(std::string("+"));
            } else {
                request_update.expression.push_back(std::string("-"));
            }
        }
    }

    void U_Long_Term() {
        U_Long_Factor();
        while (lexer::cur_lex_type == lex_type_t::MULT || lexer::cur_lex_type == lex_type_t::DIV || lexer::cur_lex_type == lex_type_t::MOD)
        {
            int op = -1;
            if (lexer::cur_lex_type == lex_type_t::MULT) {
                op = 0;
            } else if (lexer::cur_lex_type == lex_type_t::DIV) {
                op = 1;
            } else if (lexer::cur_lex_type == lex_type_t::MOD) {
                op = 2;
            }
            lexer::next();
            U_Long_Factor();
            if (op == 0) {
                request_update.expression.push_back(std::string("*"));
            } else if (op == 1) {
                request_update.expression.push_back(std::string("/"));
            } else if (op == 2) {
                request_update.expression.push_back(std::string("%"));
            }
        }
    }

    void U_Long_Factor() {
        if (lexer::cur_lex_type == lex_type_t::OPEN) {
            lexer::next();
            U_Long_Expression();
            if (lexer::cur_lex_type != lex_type_t::CLOSE) {
                throw std::logic_error("No \')\' after \'(\'");
            }
            lexer::next();
        } else {
            U_Long_Value();
        }
    }

    void U_Long_Value() {
        if (lexer::cur_lex_type == lex_type_t::IDENT) {
            request_update.expression.push_back(lexer::cur_lex_text);
            lexer::next();
        } else {
            U_Long_Int();
        }
    }

    void U_Long_Int() {
        if (lexer::cur_lex_type == lex_type_t::LONGNUM) {
            request_update.expression.push_back(lexer::cur_lex_text);
            lexer::next();
        } else if (lexer::cur_lex_type == lex_type_t::PLUS) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'+\'");
            }
            request_update.expression.push_back(lexer::cur_lex_text);
            lexer::next();
        } else if (lexer::cur_lex_type == lex_type_t::MINUS) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'-\'");
            }
            request_update.expression.push_back(std::string("-") + lexer::cur_lex_text);
            lexer::next();
        } else {
            throw std::logic_error("No string or long int in expression");
        }
    }

    void DE() {
        if (lexer::cur_lex_type != lex_type_t::FROM) {
            throw std::logic_error("No word \"FROM\" after \"DELETE\"");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_delete.name = lexer::cur_lex_text;
        lexer::next();
        W();
    }

    void C() {
        if (lexer::cur_lex_type != lex_type_t::TABLE) {
            throw std::logic_error("No word \"TABLE\" after \"CREATE\"");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_create.name = lexer::cur_lex_text;
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::OPEN) {
            throw std::logic_error("No \"(\" before field description list");
        }
        lexer::next();
        C_Field_Description_List();
        if (lexer::cur_lex_type != lex_type_t::CLOSE) {
            throw std::logic_error("No \")\" after field description list");
        }
        lexer::next();
    }

    void C_Field_Description_List() {
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("");
        }
        des_tmp.field = lexer::cur_lex_text;
        lexer::next();
        C_Field_Type();
        while (lexer::cur_lex_type == lex_type_t::COMMA) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::IDENT) {
                throw std::logic_error("");
            }
            des_tmp.field = lexer::cur_lex_text;
            lexer::next();
            C_Field_Type();
        }
    }

    void C_Field_Type() {
        if (lexer::cur_lex_type == lex_type_t::LONG) {
            des_tmp.size = -1;
            request_create.fields_description.push_back(std::move(des_tmp));
            lexer::next();
        } else if (lexer::cur_lex_type == lex_type_t::TEXT) {
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::OPEN) {
                throw std::logic_error("No \'(\' after \'TEXT\'");
            }
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'(\'");
            }
            des_tmp.size = std::strtol(lexer::cur_lex_text.data(), nullptr, 10);
            if (errno == ERANGE) {
                errno = 0;
                throw std::logic_error("Too big number " + lexer::cur_lex_text);
            }
            request_create.fields_description.push_back(std::move(des_tmp));
            lexer::next();
            if (lexer::cur_lex_type != lex_type_t::CLOSE) {
                throw std::logic_error("No \')\' after \'(\'");
            }
            lexer::next();
        } else {
            throw std::logic_error("No string or number in field description list");
        }
    }

    void DR() {
        if (lexer::cur_lex_type != lex_type_t::TABLE) {
            throw std::logic_error("No word \"TABLE\" after \"DROP\"");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_drop.name = lexer::cur_lex_text;
        lexer::next();
    }

    void W() {
        if (lexer::cur_lex_type != lex_type_t::WHERE) {
            throw std::logic_error("Forgot or incorrect entry \'WHERE\'");
        }
        lexer::next();
        //go on WHERE-clause
        while (lexer::cur_lex_type != lex_type_t::END) {
            if (!where_clause_like_flag && lexer::cur_lex_type == lex_type_t::LIKE) {
                where_clause_like_flag = true;
            } else if (!where_clause_bool_flag && comparison_op()) {
                where_clause_bool_flag = true;
            }
            vec_lex.push_back(lex{ lexer::cur_lex_type, lexer::cur_lex_text });
            lexer::next();
        }
        vec_lex.push_back(lex{ lex_type_t::END, std::string("END") });
        if (where_clause_like_flag) {
            where_clause_type = "LIKE";
            W_Like();
        } else if (where_clause_bool_flag) {
            where_clause_type = "BOOL";
            W_Bool();
        } else if (vec_lex[index_vec_lex].type == lex_type_t::ALL) {
            where_clause_type = "ALL";
            ++index_vec_lex;
        } else {
            where_clause_type = "IN";
            W_In();
        }
    }

    void W_Like() {
        if (vec_lex[index_vec_lex].type != lex_type_t::IDENT) {
            throw std::logic_error("No field name before \'LIKE\'");
        }
        like_where_clause.field_name = vec_lex[index_vec_lex].text;
        ++index_vec_lex;
        if (vec_lex[index_vec_lex].type == lex_type_t::NOT) {
            like_where_clause.use_not = true;
            ++index_vec_lex;
        }
        if (vec_lex[index_vec_lex].type != lex_type_t::LIKE) {
            throw std::logic_error("You can entry only \'NOT\' before \'LIKE\'");
        }
        ++index_vec_lex;
        if (vec_lex[index_vec_lex].type != lex_type_t::STR_VAL) {
            throw std::logic_error("You must entry sample string after \'LIKE\'");
        }
        str_like.clear();
        str_like << vec_lex[index_vec_lex].text;
        like::init();
        like::next();
        if (like::cur_lex_type == lex_type_t::CHAR) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::PERCENT) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::UNDER) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::OPEN) {
            like::next();
            W_Like_Enum();
            if (like::cur_lex_type != lex_type_t::CLOSE) {
                throw std::logic_error("Incorrect syntax of string in LIKE clause");
            }
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::XOR) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::DASH) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::CLOSE) {
            like::next();
            W_Like_Sample_String();
        } else {
            throw std::logic_error("Incorrect syntax of string in LIKE clause");
        }
        like_where_clause.sample_string = vec_lex[index_vec_lex].text;
        ++index_vec_lex;
    }

    void W_Like_Sample_String() {
        if (like::cur_lex_type == lex_type_t::CHAR) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::PERCENT) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::UNDER) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::OPEN) {
            like::next();
            W_Like_Enum();
            if (like::cur_lex_type != lex_type_t::CLOSE) {
                throw std::logic_error("Incorrect syntax of string in LIKE clause");
            }
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::XOR) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::DASH) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::CLOSE) {
            like::next();
            W_Like_Sample_String();
        } else if (like::cur_lex_type == lex_type_t::END) {
            // nothing to do
        } else {
            throw std::logic_error("Incorrect syntax of string in LIKE clause");
        }
    }

    void W_Like_Enum() {
        if (like::cur_lex_type == lex_type_t::XOR) {
            like::next();
            W_Like_Enum_Start();
            W_Like_Enum_Next();
        } else {
            W_Like_Enum_Start();
            W_Like_Enum_Next();
        }
    }

    void W_Like_Enum_Start() {
        if (like::cur_lex_type == lex_type_t::CHAR) {
            c1 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::PERCENT) {
            c1 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::UNDER) {
            c1 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::OPEN) {
            c1 = like::cur_lex_text;
            like::next();
        } else {
            throw std::logic_error("Incorrect syntax of string in LIKE clause");
        }
    }

    void W_Like_Enum_Next() {
        if (like::cur_lex_type == lex_type_t::DASH) {
            like::next();
            W_Like_Enum_If_Dash();
            if (c1 > c2) {
                throw std::logic_error("Incorrect syntax of string in LIKE clause.\nFirst character in range should be less or equal than second one");
            }
        } else {
            W_Like_Enum_If_Not_Dash();
        }
    }

    void W_Like_Enum_If_Dash() {
        if (like::cur_lex_type == lex_type_t::CHAR) {
            c2 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::PERCENT) {
            c2 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::UNDER) {
            c2 = like::cur_lex_text;
            like::next();
        } else if (like::cur_lex_type == lex_type_t::OPEN) {
            c2 = like::cur_lex_text;
            like::next();
        } else {
            throw std::logic_error("Incorrect syntax of string in LIKE clause");
        }
    }

    void W_Like_Enum_If_Not_Dash() {
        if (like::cur_lex_type == lex_type_t::CLOSE) {
            // nothing to do
        } else {
            W_Like_Enum_Next_If_Not_Dash();
            W_Like_Enum_If_Not_Dash();
        }
    }

    void W_Like_Enum_Next_If_Not_Dash() {
        if (like::cur_lex_type == lex_type_t::CHAR) {
            like::next();
        } else if (like::cur_lex_type == lex_type_t::PERCENT) {
            like::next();
        } else if (like::cur_lex_type == lex_type_t::UNDER) {
            like::next();
        } else if (like::cur_lex_type == lex_type_t::OPEN) {
            like::next();
        } else {
            throw std::logic_error("Incorrect syntax of string in LIKE clause");
        }
    }

    void W_In() {
        W_In_Expression();
        if (vec_lex[index_vec_lex].type == lex_type_t::NOT) {
            in_where_clause.use_not = true;
            ++index_vec_lex;
        }
        if (vec_lex[index_vec_lex].type != lex_type_t::IN) {
            throw std::logic_error("No word \'In\' in expression");
        }
        ++index_vec_lex;
        if (vec_lex[index_vec_lex].type != lex_type_t::OPEN) {
            throw std::logic_error("No \'(\' after \'IN\'");
        }
        ++index_vec_lex;
        W_In_Const_List();
        if (vec_lex[index_vec_lex].type != lex_type_t::CLOSE)
        {
            throw std::logic_error("No \')\' after \'(\'");
        }
        ++index_vec_lex;
    }

    void W_In_Expression() {
        if (vec_lex[index_vec_lex].type == lex_type_t::STR_VAL) {
            in_where_clause.expression.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            W_In_Long_Expression();
        }
    }

    void W_In_Long_Expression() {
        W_In_Long_Term();
        while (vec_lex[index_vec_lex].type == lex_type_t::PLUS || vec_lex[index_vec_lex].type == lex_type_t::MINUS) {
            int op;
            if (vec_lex[index_vec_lex].type == lex_type_t::PLUS) {
                op = 0;
            } else {
                op = 1;
            }
            ++index_vec_lex;
            W_In_Long_Term();
            if (op == 0) {
                in_where_clause.expression.push_back(std::string("+"));
            } else {
                in_where_clause.expression.push_back(std::string("-"));
            }
        }
    }

    void W_In_Long_Term() {
        W_In_Long_Factor();
        while (vec_lex[index_vec_lex].type == lex_type_t::MULT || vec_lex[index_vec_lex].type == lex_type_t::DIV|| vec_lex[index_vec_lex].type == lex_type_t::MOD) {
            int op;
            if (vec_lex[index_vec_lex].type == lex_type_t::MULT) {
                op = 0;
            } else if (vec_lex[index_vec_lex].type == lex_type_t::DIV) {
                op = 1;
            } else {
                op = 2;
            }
            ++index_vec_lex;
            W_In_Long_Factor();
            if (op == 0) {
                in_where_clause.expression.push_back(std::string("*"));
            } else if (op == 1) {
                in_where_clause.expression.push_back(std::string("/"));
            } else {
                in_where_clause.expression.push_back(std::string("%"));
            }
        }
    }

    void W_In_Long_Factor()
    {
        if (vec_lex[index_vec_lex].type == lex_type_t::OPEN) {
            ++index_vec_lex;
            W_In_Long_Expression();
            if (vec_lex[index_vec_lex].type != lex_type_t::CLOSE) {
                throw std::logic_error("No \')\' after \'(\'");
            }
            ++index_vec_lex;
        } else {
            W_In_Long_Value();
        }
    }

    void W_In_Long_Value() {
        if (vec_lex[index_vec_lex].type == lex_type_t::IDENT) {
            in_where_clause.expression.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            W_In_Long_Int();
        }
    }

    void W_In_Long_Int() {
        if (vec_lex[index_vec_lex].type == lex_type_t::LONGNUM) {
            in_where_clause.expression.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::PLUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'+\'");
            }
            in_where_clause.expression.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::MINUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'-\'");
            }
            in_where_clause.expression.push_back(std::string("-") + vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            throw std::logic_error("No Text or Long expression");
        }
    }

    void W_In_Const_List() {
        if (vec_lex[index_vec_lex].type == lex_type_t::STR_VAL) {
            W_In_Const_String();
        } else {
            W_In_Const_Long();
        }
    }

    void W_In_Const_String() {
        if (vec_lex[index_vec_lex].type != lex_type_t::STR_VAL) {
            throw std::logic_error("No text expression");
        }
        in_where_clause.list_consts_str.push_back(vec_lex[index_vec_lex].text);
        ++index_vec_lex;
        while (vec_lex[index_vec_lex].type == lex_type_t::COMMA) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::STR_VAL) {
                throw std::logic_error("No Text expression after\',\'");
            }
            in_where_clause.list_consts_str.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        }
    }

    void W_In_Const_Long() {
        W_In_Const_Long_Int();
        while (vec_lex[index_vec_lex].type == lex_type_t::COMMA) {
            ++index_vec_lex;
            W_In_Const_Long_Int();
        }
    }

    void W_In_Const_Long_Int() {
        if (vec_lex[index_vec_lex].type == lex_type_t::LONGNUM) {
            in_where_clause.list_consts_num.push_back(std::strtol(vec_lex[index_vec_lex].text.data(), nullptr, 10));
            if (errno == ERANGE) {
                errno = 0;
                throw std::logic_error("Too big number " + vec_lex[index_vec_lex].text);
            }
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::PLUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'+\'");
            }
            in_where_clause.list_consts_num.push_back(std::strtol(vec_lex[index_vec_lex].text.data(), nullptr, 10));
            if (errno == ERANGE) {
                errno = 0;
                throw std::logic_error("Too big number " + vec_lex[index_vec_lex].text);
            }
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::MINUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'-\'");
            }
            in_where_clause.list_consts_num.push_back(std::strtol(vec_lex[index_vec_lex].text.data(), nullptr, 10) * -1);
            if (errno == ERANGE) {
                errno = 0;
                throw std::logic_error("Too big number -" + vec_lex[index_vec_lex].text);
            }
            ++index_vec_lex;
        } else {
            throw std::logic_error("No Text or Long expression");
        }
    }

    void W_Bool() {
        W_Bool_Term();
        while (vec_lex[index_vec_lex].type == lex_type_t::OR) {
            ++index_vec_lex;
            W_Bool_Term();
            bool_where_clause.expression.push_back(std::string("OR"));
        }
    }

    void W_Bool_Term() {
        W_Bool_Factor();
        while (vec_lex[index_vec_lex].type == lex_type_t::AND) {
            ++index_vec_lex;
            W_Bool_Factor();
            bool_where_clause.expression.push_back(std::string("AND"));
        }
    }

    void W_Bool_Factor() {
        if (vec_lex[index_vec_lex].type == lex_type_t::NOT) {
            ++index_vec_lex;
            W_Bool_Factor();
            bool_where_clause.expression.push_back(std::string("NOT"));
        } else if (vec_lex[index_vec_lex].type == lex_type_t::OPEN) {
            ++index_vec_lex;
            W_Bool_Expression();
            if (vec_lex[index_vec_lex].type != lex_type_t::CLOSE) {
                throw std::logic_error("No \')\' after \'(\'");
            }
            ++index_vec_lex;
        }
        else {
            throw std::logic_error("No logic expression or ratio");
        }
    }

    void W_Bool_Expression() {
        int tmp_index = index_vec_lex;
        try {
            W_Bool_Ratio();
            for (std::string& item : vec_str_tmp) {
                bool_where_clause.expression.push_back(std::move(item));
            }
            vec_str_tmp.clear();
        } catch (std::logic_error) {
            index_vec_lex = tmp_index;
            vec_str_tmp.clear();
            W_Bool();
        }
    }

    void W_Bool_Ratio() {
        W_Bool_Text_Ratio();
        W_Bool_Comparison_Operation();
        W_Bool_Text_Ratio();
        vec_str_tmp.push_back(str_tmp);
    }

    void W_Bool_Text_Ratio() {
        if (vec_lex[index_vec_lex].type == lex_type_t::STR_VAL) {
            vec_str_tmp.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            W_Bool_Long_Ratio();
        }
    }

    void W_Bool_Long_Ratio() {
        W_Bool_Long_Term();
        while (vec_lex[index_vec_lex].type == lex_type_t::PLUS || vec_lex[index_vec_lex].type == lex_type_t::MINUS) {
            int op;
            if (vec_lex[index_vec_lex].type == lex_type_t::PLUS) {
                op = 0;
            } else {
                op = 1;
            }
            ++index_vec_lex;
            W_Bool_Long_Term();
            if (op == 0) {
                vec_str_tmp.push_back(std::string("+"));
            } else {
                vec_str_tmp.push_back(std::string("-"));
            }
        }
    }

    void W_Bool_Long_Term() {
        W_Bool_Long_Factor();
        while (vec_lex[index_vec_lex].type == lex_type_t::MULT || vec_lex[index_vec_lex].type == lex_type_t::DIV|| vec_lex[index_vec_lex].type == lex_type_t::MOD) {
            int op;
            if (vec_lex[index_vec_lex].type == lex_type_t::MULT) {
                op = 0;
            } else if (vec_lex[index_vec_lex].type == lex_type_t::DIV) {
                op = 1;
            } else {
                op = 2;
            }
            ++index_vec_lex;
            W_Bool_Long_Factor();
            if (op == 0) {
                vec_str_tmp.push_back(std::string("*"));
            } else if (op == 1) {
                vec_str_tmp.push_back(std::string("/"));
            } else {
                vec_str_tmp.push_back(std::string("%"));
            }
        }
    }

    void W_Bool_Long_Factor() {
        if (vec_lex[index_vec_lex].type == lex_type_t::OPEN) {
            ++index_vec_lex;
            W_Bool_Long_Ratio();
            if (vec_lex[index_vec_lex].type != lex_type_t::CLOSE) {
                throw std::logic_error("No \')\' after \'(\'");
            }
            ++index_vec_lex;
        } else {
            W_Bool_Long_Value();
        }
    }

    void W_Bool_Long_Value() {
        if (vec_lex[index_vec_lex].type == lex_type_t::IDENT) {
            vec_str_tmp.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            W_Bool_Long_Int();
        }
    }

    void W_Bool_Long_Int() {
        if (vec_lex[index_vec_lex].type == lex_type_t::LONGNUM) {
            vec_str_tmp.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::PLUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'+\'");
            }
            vec_str_tmp.push_back(vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::MINUS) {
            ++index_vec_lex;
            if (vec_lex[index_vec_lex].type != lex_type_t::LONGNUM) {
                throw std::logic_error("No number after \'-\'");
            }
            vec_str_tmp.push_back(std::string("-") + vec_lex[index_vec_lex].text);
            ++index_vec_lex;
        } else {
            throw std::logic_error("No Text or Long expression");
        }
    }

    void W_Bool_Comparison_Operation() {
        if (vec_lex[index_vec_lex].type == lex_type_t::EQUALLY) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::GREAT) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::LESS) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::GREAT_EQUAL) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::LESS_EQUAL) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else if (vec_lex[index_vec_lex].type == lex_type_t::NOT_EQUAL) {
            str_tmp = vec_lex[index_vec_lex].text;
            ++index_vec_lex;
        } else {
            throw std::logic_error("No comparison opation");
        }
    }
}