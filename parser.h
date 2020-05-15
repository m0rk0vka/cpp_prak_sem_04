#include "structs.h"

enum lex_type_t {SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FROM, INTO, SET, TABLE,
    TEXT, LONG, EQUALLY, OPEN, CLOSE, END, IDENT};

bool isal_num(int c){
    return isalnum(c) || c == '_';
}

namespace lexer {
    lex_type_t cur_lex_type;
    std::string cur_lex_text;
    int cur_lex_pos, c;

    void init() {
        c = std::cin.get();
        if (c == EOF || c == '\n') {
            throw std::logic_error("Empty request");
        }
        cur_lex_pos = 1;
    }

    void next() {
        cur_lex_text.clear();
        enum state_t { H, S, SE, SEL, SELE, SELEC, SELECT, I, IN, INS, INSE, INSER, INSERT,
            U, UP, UPD, UPDA, UPDAT, UPDATE, D, DE, DEL, DELE, DELET, DELETE, C, CR, CRE,
            CREA, CREAT, CREATE, DR, DRO, DROP, F, FR, FRO, FROM, INT, INTO, SET, T, TA, TAB,
            TABL, TABLE, TE, TEX, TEXT, L, LO, LON, LONG, EQUALLY, OPEN, CLOSE, IDENT, OK } state = H;
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
                } else if (isalpha(c) || c == '_') {
                    state = IDENT;
                } else if (c == '(') {
                    state = OPEN;
                } else if (c == ')') {
                    state = CLOSE;
                } else if (c == '=') {
                    state = EQUALLY;
                } else if (c == '\n') {
                    cur_lex_type = lex_type_t::END;
                    state = OK;
                } else {
                    throw std::logic_error("Unexpected caracter with code " + std::to_string(c) + " in position " + std::to_string(cur_lex_pos));
                }
                break;
//          SELECT
            case S:
                if (c == 'E') {
                    state = SE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SEL:
                if (c == 'E') {
                    state = SELE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELE:
                if (c == 'C') {
                    state = SELEC;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELEC:
                if (c == 'T') {
                    state = SELECT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case SELECT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::SELECT;
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
                    cur_lex_type = lex_type_t::IDENT;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INS:
                if (c == 'E') {
                    state = INSE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSE:
                if (c == 'R') {
                    state = INSER;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSER:
                if (c == 'T') {
                    state = INSERT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INSERT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::INSERT;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UP:
                if (c == 'D') {
                    state = UPD;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPD:
                if (c == 'A') {
                    state = UPDA;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDA:
                if (c == 'T') {
                    state = UPDAT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDAT:
                if (c == 'E') {
                    state = UPDATE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case UPDATE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::UPDATE;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DE:
                if (c == 'L') {
                    state = DEL;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DEL:
                if (c == 'E') {
                    state = DELE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELE:
                if (c == 'T') {
                    state = DELET;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELET:
                if (c == 'E') {
                    state = DELETE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DELETE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::DELETE;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CR:
                if (c == 'E') {
                    state = CRE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CRE:
                if (c == 'A') {
                    state = CREA;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREA:
                if (c == 'T') {
                    state = CREAT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREAT:
                if (c == 'E') {
                    state = CREATE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case CREATE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::CREATE;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DRO:
                if (c == 'P') {
                    state = DROP;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case DROP:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::DROP;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FR:
                if (c == 'O') {
                    state = FRO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FRO:
                if (c == 'M') {
                    state = FROM;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case FROM:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::FROM;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case INTO:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::INTO;
                    state = OK;
                }
                break;
//          SET
            case SET:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::SET;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TA:
                if (c == 'B') {
                    state = TAB;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TAB:
                if (c == 'L') {
                    state = TABL;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TABL:
                if (c == 'E') {
                    state = TABLE;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TABLE:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::TABLE;
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
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TEX:
                if (c == 'T') {
                    state = TEXT;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case TEXT:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::TEXT;
                    state = OK;
                }
                break;
//          LONG
            case L:
                if (c == 'O') {
                    state = LO;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LO:
                if (c == 'N') {
                    state = LON;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LON:
                if (c == 'G') {
                    state = LONG;
                } else if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;

            case LONG:
                if (isal_num(c)) {
                    state = IDENT;
                } else {
                    cur_lex_type = lex_type_t::LONG;
                    state = OK;
                }
                break;
//          IDENT
            case IDENT:
                if (isal_num(c)) {
                    // stay in IDENT
                } else {
                    cur_lex_type = lex_type_t::IDENT;
                    state = OK;
                }
                break;
//          OPEN&CLOSE&EQUALLY
            case OPEN:
                cur_lex_type = lex_type_t::OPEN;
                state = OK;
                break;
            case CLOSE:
                cur_lex_type = lex_type_t::CLOSE;
                state = OK;
                break;
            case EQUALLY:
                cur_lex_type = lex_type_t::EQUALLY;
                break;
//          OK
            case OK:
                break;
            }

            if (state != OK){
                if (!isspace(c))
                {
                    cur_lex_text.push_back(c);
                }
                c = std::cin.get();
                ++cur_lex_pos;
            }
        }
    }
}

namespace parser{
    std::string request_type;
    struct_select request_select;
    struct_insert request_insert;
    struct_update request_update;
    struct_delete request_delete;
    struct_create request_create;
    struct_drop request_drop;

    void init() {
        lexer::init();
        lexer::next();
        request_select.clear();
        request_insert.clear();
        request_update.clear();
        request_delete.clear();
        request_create.clear();
        request_drop.clear();
    }

    void check_end() {
        if (lexer::cur_lex_type != lex_type_t::END){
            throw std::logic_error("No end");
        }
    }

    void S();
    void I();
    void U();
    void DE();
    void C();
    void DR();

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
            request_type = "UPDETE";
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

    void S(){
        //obrabotka field list
        if (lexer::cur_lex_type != lex_type_t::FROM) {
            throw std::logic_error("No word \"FROM\" after field list");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_select.name = lexer::cur_lex_text;
        lexer::next();
        //WHERE-clause
    }

    void I(){
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
        //obrabotka field value
        if (lexer::cur_lex_type != lex_type_t::CLOSE) {
            throw std::logic_error("No \")\" after field value");
        }
        lexer::next();
    }

    void U(){
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
        //zapisat' field name
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::EQUALLY) {
            throw std::logic_error("No \"=\" after field name");
        }
        lexer::next();
        //vyrashenie
        //WHERE-clause
    }

    void DE(){
        if (lexer::cur_lex_type != lex_type_t::FROM) {
            throw std::logic_error("No word \"FROM\" after \"DELETE\"");
        }
        lexer::next();
        if (lexer::cur_lex_type != lex_type_t::IDENT) {
            throw std::logic_error("Bad table name");
        }
        request_delete.name = lexer::cur_lex_text;
        lexer::next();
        //WHERE-clause
    }

    void C(){
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
        //obrabotka field description list
        if (lexer::cur_lex_type != lex_type_t::CLOSE) {
            throw std::logic_error("No \")\" after field description list");
        }
        lexer::next();
    }

    void DR(){
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
}