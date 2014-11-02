#include "lex.h"

#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace lex = boost::spirit::lex;
namespace dsh_lex = dsh::lex;


typedef lex::lexertl::token<char const *> token_type;
typedef lex::lexertl::actor_lexer<token_type> lexer_type;
typedef dsh_lex::dsh_lexer<lexer_type> dsh_lexer_type;


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

    switch (tok.id()) {
        case dsh_lex::TOK_NEWLINE:
            val = "\\n";
            break;
        default:
            break;
    }

    cout << "ID: " << tok.id() << ", VALUE: " << val << "\n";
}

int main(int argc, char **argv) {


    dsh_lexer_type lexer;
    string buff(read_from_file(argc == 1 ? "/tmp/test.sh" : argv[1]));

    
    char const *buff_first = buff.c_str();
    char const *buff_last = &buff_first[buff.size()];
    /*
    lexer_type::iterator_type it = lexer.begin(buff_first, buff_last);
    lexer_type::iterator_type end = lexer.end();

    while(it != end && token_is_valid(*it)) {
        print_token(*it);
        ++it;
    }
    */

    
    bool r = lex::tokenize(buff_first, buff_last, lexer);
    
    if (!r) {
        string rest(buff_first, buff_last);
        std::cerr << "Lexical analysis failed\n" << "stopped at: \""
            << rest << "\"\n";
    }
    
    return 0;
}
