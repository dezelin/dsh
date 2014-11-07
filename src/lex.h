//
//  Dsh - POSIX command line shell
//  Copyright (C) 2014 Aleksandar Dezelin <dezelin@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LEX_H
#define LEX_H

#include <string>
#include <boost/spirit/include/lex.hpp>

namespace dsh {
namespace lex {

namespace _lex = boost::spirit::lex;

//
// Lexer token ids as defined in IEEE Std 1003.2
//
enum tokens {
    // POSIX tokens
    TOK_WORD = _lex::min_token_id + 1, 
    TOK_WHITESPACE,
    TOK_ASSIGNMENT_WORD,
    TOK_NAME,
    TOK_NEWLINE,
    TOK_IO_NUMBER,
    TOK_AND_IF,
    TOK_OR_IF,
    TOK_DSEMI,
    TOK_DLESS,
    TOK_DGREAT,
    TOK_LESSAND,
    TOK_GREATAND,
    TOK_LESSGREAT,
    TOK_DLESSDASH,
    TOK_CLOBBER,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_ELIF,
    TOK_FI,
    TOK_DO,
    TOK_DONE,
    TOK_CASE,
    TOK_ESAC,
    TOK_WHILE,
    TOK_UNTIL,
    TOK_FOR,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_BANG,
    TOK_IN,

    // Dsh tokens
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_LESS,
    TOK_GREAT,
    TOK_EQ,
    TOK_NEQ,
    TOK_AND,
    TOK_SEMI,
    TOK_QUOTES_SINGLE,
    TOK_QUOTES_DOUBLE,
    TOK_COMMENT,
    TOK_NUMBER,
    TOK_EXPANSION
};

template <typename lexer>
struct dsh_lexer : public _lex::lexer<lexer> {
    dsh_lexer() :
        //
        // POSIX token definitions
        // 
        _word("(^\\s|.)+?", TOK_WORD),
        _whitespace("[\\x20\\t\\r\\f\\v]", TOK_WHITESPACE),
        _assignment_word("[_a-zA-Z]\\w*=", TOK_ASSIGNMENT_WORD),
        _name("[_a-zA-Z]\\w*", TOK_NAME),
        _newline("\n", TOK_NEWLINE),
        _io_number("[<>]\\d+", TOK_IO_NUMBER),
        _and_if("&&", TOK_AND_IF),
        _or_if("\\|\\|", TOK_OR_IF), // ||
        _dsemi(";;", TOK_DSEMI),
        _dless("<<", TOK_DLESS),
        _dgreat(">>", TOK_DGREAT),
        _lessand("<&", TOK_LESSAND),
        _greatand(">&", TOK_GREATAND),
        _lessgreat("<>", TOK_LESSGREAT),
        _dlessdash("<<-", TOK_DLESSDASH),
        _clobber(">\\|", TOK_CLOBBER), // >|
        _if("if", TOK_IF),
        _then("then", TOK_THEN),
        _else("else", TOK_ELSE),
        _elif("elif", TOK_ELIF),
        _fi("fi", TOK_FI),
        _do("do", TOK_DO),
        _done("done", TOK_DONE),
        _case("case", TOK_CASE),
        _esac("esac", TOK_ESAC),
        _while("while", TOK_WHILE),
        _until("until", TOK_UNTIL),
        _for("for", TOK_FOR),
        _lbrace("\\{", TOK_LBRACE),
        _rbrace("\\}", TOK_RBRACE),
        _bang("!", TOK_BANG),
        _in("in", TOK_IN),
        //
        // Dsh token definitions
        //
        _lparen("\\(", TOK_LPAREN),
        _rparen("\\)", TOK_RPAREN),
        _lbracket("\\[", TOK_LBRACKET),
        _rbracket("\\]", TOK_RBRACKET),
        _less("<", TOK_LESS),
        _great(">", TOK_GREAT),
        _eq("=", TOK_EQ),
        _neq("!=", TOK_NEQ),
        _and("&", TOK_AND),
        _semi(";", TOK_SEMI),
        _single_quotes("\\\'[^\']*\\\'", TOK_QUOTES_SINGLE),
        _double_quotes("\\\"([^\"]|\\\\\\\")*\\\"", TOK_QUOTES_DOUBLE),
        _comment("#.*$", TOK_COMMENT),
        _number("\\d+(\\.\\d+)?", TOK_NUMBER),
        //_expansion("\\$([0-9]|[@*#?-!0]|[_a-zA-Z]\\w*|\\{.*\\}|\\(.*\\))?", TOK_EXPANSION) 
        _expansion("\\$([#])?", TOK_EXPANSION)
        //_test("-([aefdcbpSLhgurwxstznovR]|nt|ot|ef|eq|ne|le|ge|lt|gt)", TOK_TEST)
    {
        this->self = 
            // POSIX tokens
            _whitespace | _newline | _assignment_word | _io_number | 
            _and_if | _or_if | _dsemi | _dless | _dgreat | _lessand | _greatand |
            _lessgreat | _dlessdash | _clobber | _if | _then | _else | _elif | 
            _fi | _do | _done | _case | _esac | _while | _until | _for | 
            _lbrace | _rbrace | _bang | _in | _name |
            // Dsh tokens
            _lparen | _rparen | _lbracket | _rbracket | _less | 
            _great | _eq | _neq | _and | _semi | _single_quotes | 
            _double_quotes | _comment | _number | _expansion | 
            
            _word;
    }

    private:
        // POSIX tokens
        _lex::token_def<> _word, _whitespace, _assignment_word, _name, _newline, _io_number, 
            _and_if, _or_if, _dsemi, _dless, _dgreat, _lessand, _greatand, 
            _lessgreat, _dlessdash, _clobber, _if, _then, _else, _elif, _fi, 
            _do, _done, _case, _esac, _while, _until, _for, _lbrace, _rbrace, 
            _bang, _in;

        // Dsh tokens
        _lex::token_def<> _lparen, _rparen, _lbracket, _rbracket, _less, _great,
            _eq, _neq, _and, _semi, _single_quotes, _double_quotes, _comment, _number, 
            _expansion;
    };

} // namespace lex
} // namespace dsh

#endif // LEX_H

