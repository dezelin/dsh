#include "lex.h"
#include "parser.h"

#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace lex = boost::spirit::lex;
namespace dsh_lex = dsh::lex;
namespace dsh_parser = dsh::parser;

typedef lex::lexertl::token<char const *> token_type;
typedef lex::lexertl::actor_lexer<token_type> lexer_type;
typedef dsh_lex::dsh_lexer<lexer_type> dsh_lexer_type;
typedef dsh_parser::dsh_grammar<dsh_lexer_type::iterator_type> dsh_parser_type;


string read_from_file(char const *file_name) {
    ifstream ifs(file_name);
    if (!ifs.is_open()) {
        cerr << "Can't open file \"" << file_name << "\"\n";
        return string();
    }

    ifs.unsetf(ios::skipws);
    return string(istreambuf_iterator<char>(ifs.rdbuf()),
            istreambuf_iterator<char>());
}

void print_token(token_type const & tok) {
    if (!token_is_valid(tok)) {
        cerr << "Invalid token!\n";
        return;
    }

    string val(tok.value().begin(), tok.value().end());
    string token;

    switch (tok.id()) {
        case dsh_lex::TOK_WORD:
            token = "TOK_WORD";
            break;
        case dsh_lex::TOK_ASSIGNMENT_WORD:
            token = "TOK_ASSIGNMENT_WORD";
            break;
        case dsh_lex::TOK_NAME:
            token = "TOK_NAME";
            break;
        case dsh_lex::TOK_NEWLINE:
            token = "TOK_NEWLINE";
            val = "\\n";
            break;
        case dsh_lex::TOK_IO_NUMBER:
            token = "TOK_IO_NUMBER";
            break;
        case dsh_lex::TOK_AND_IF:
            token = "TOK_AND_IF";
            break;
        case dsh_lex::TOK_OR_IF:
            token = "TOK_OR_IF";
            break;
        case dsh_lex::TOK_DSEMI:
            token = "TOK_DSEMI";
            break;
        case dsh_lex::TOK_DLESS:
            token = "TOK_DLESS";
            break;
        case dsh_lex::TOK_DGREAT:
            token = "TOK_DGREAT";
            break;
        case dsh_lex::TOK_LESSAND:
            token = "TOK_LESSAND";
            break;
        case dsh_lex::TOK_GREATAND:
            token = "TOK_GREATAND";
            break;
        case dsh_lex::TOK_LESSGREAT:
            token = "TOK_LESSGREAT";
            break;
        case dsh_lex::TOK_DLESSDASH:
            token = "TOK_LESSGREAT";
            break;
        case dsh_lex::TOK_CLOBBER:
            token = "TOK_CLOBBER";
            break;
        case dsh_lex::TOK_IF:
            token = "TOK_IF";
            break;
        case dsh_lex::TOK_THEN:
            token = "TOK_THEN";
            break;
        case dsh_lex::TOK_ELSE:
            token = "TOK_ELSE";
            break;
        case dsh_lex::TOK_ELIF:
            token = "TOK_ELIF";
            break;
        case dsh_lex::TOK_FI:
            token = "TOK_FI";
            break;
        case dsh_lex::TOK_DO:
            token = "TOK_DO";
            break;
        case dsh_lex::TOK_DONE:
            token = "TOK_DONE";
            break;
        case dsh_lex::TOK_CASE:
            token = "TOK_CASE";
            break;
        case dsh_lex::TOK_ESAC:
            token = "TOK_ESAC";
            break;
        case dsh_lex::TOK_WHILE:
            token = "TOK_WHILE";
            break;
        case dsh_lex::TOK_UNTIL:
            token = "TOK_UNTIL";
            break;
        case dsh_lex::TOK_FOR:
            token = "TOK_FOR";
            break;
        case dsh_lex::TOK_LBRACE:
            token = "TOK_LBRACE";
            break;
        case dsh_lex::TOK_RBRACE:
            token = "TOK_RBRACE";
            break;
        case dsh_lex::TOK_BANG:
            token = "TOK_BANG";
            break;
        case dsh_lex::TOK_IN:
            token = "TOK_IN";
            break;
        // Bash tokens
        case dsh_lex::TOK_LPAREN:
            token = "TOK_LPAREN:";
            break;
        case dsh_lex::TOK_RPAREN:
            token = "TOK_RPAREN";
            break;
        case dsh_lex::TOK_LBRACKET:
            token = "TOK_LBRACKET";
            break;
        case dsh_lex::TOK_RBRACKET:
            token = "TOK_RBRACKET";
            break;
        case dsh_lex::TOK_LESS:
            token = "TOK_LESS";
            break;
        case dsh_lex::TOK_GREAT:
            token = "TOK_GREAT";
            break;
        case dsh_lex::TOK_EQ:
            token = "TOK_EQ";
            break;
        case dsh_lex::TOK_NEQ:
            token = "TOK_NEQ";
            break;
        case dsh_lex::TOK_AND:
            token = "TOK_AND";
            break;
        case dsh_lex::TOK_SEMI:
            token = "TOK_SEMI";
            break;
        case dsh_lex::TOK_QUOTES_SINGLE:
            token = "TOK_QUOTES_SINGLE";
            break;
        case dsh_lex::TOK_QUOTES_DOUBLE:
            token = "TOK_QUOTES_DOUBLE";
            break;
        case dsh_lex::TOK_WHITESPACE:
            token = "TOK_WHITESPACE";
            break;
        case dsh_lex::TOK_COMMENT:
            token = "TOK_COMMENT";
            break;
        case dsh_lex::TOK_NUMBER:
            token = "TOK_NUMBER";
            break;
        case dsh_lex::TOK_EXPANSION:
            token = "TOK_EXPANSION";
            break;
        default:
            break;
    }

    cout << "ID: " << token << ", VALUE: " << val << "\n";
}

int main(int argc, char **argv) {


    dsh_lexer_type lexer;
    string buff(read_from_file(argc == 1 ? "/tmp/test.sh" : argv[1]));

    char const *buff_first = buff.c_str();
    char const *buff_last = &buff_first[buff.size()];
    
    
    lexer_type::iterator_type it = lexer.begin(buff_first, buff_last);
    lexer_type::iterator_type end = lexer.end();

    while(it != end && token_is_valid(*it)) {
        print_token(*it);
        ++it;
    }
    
    
    
    bool r = lex::tokenize(buff_first, buff_last, lexer);
    
    if (!r) {
        string rest(buff_first, buff_last);
        std::cerr << "Lexical analysis failed\n" << "stopped at: \"" << rest << "\"\n";
        return 1;
    }
    
       
    
    /*
    dsh_parser_type parser(lexer);
    bool r = lex::tokenize_and_parse(buff_first, buff_last, lexer, parser);
    if (!r) {
        std::string rest(buff_first, buff_last);
        std::cerr << "Parsing failed\n" << "stopped at: \"" << rest << "\"\n";
        return 1;
    }

    */

    return 0;
}

