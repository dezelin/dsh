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

#ifndef PARSER_H
#define PARSER_H

#include "lex.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char.hpp>

namespace dsh {
namespace parser {

using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace dsh::lex;

//
// Dsh grammar.
// See http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10_02
//
template<typename Iterator>
struct dsh_grammar : qi::grammar<Iterator> {
    
    template<typename TokenDef>
    dsh_grammar(TokenDef const& tok) 
        : dsh_grammar::base_type(_complete_command) {
        
        _complete_command %= (_list > _separator)
            | _list;

        _list %= (_list > _separator_op > _and_or)
            | _and_or;

        _and_or %= _pipeline
            | (_and_or > qi::token(TOK_AND_IF) > _linebreak > _pipeline)
            | (_and_or > qi::token(TOK_OR_IF) > _linebreak > _pipeline);

        _pipeline %= _pipe_sequence
            | (qi::token(TOK_BANG) > _pipe_sequence);

        _pipe_sequence %= _command
            | (_pipe_sequence > '|' > _linebreak > _command);

        _command %= _simple_command
            | _compound_command
            | (_compound_command > _redirect_list)
            | _function_definition;

        _compound_command %= _brace_group
            | _subshell
            | _for_clause
            | _case_clause
            | _if_clause
            | _while_clause
            | _until_clause;

        _subshell %= '(' > _compound_list > ')';

        _compound_list %= _term
            | (_newline_list > _term)
            | (_term > _separator)
            | (_newline_list > _term > _separator);

        _term %= (_term > _separator > _and_or)
            | _and_or;

        _for_clause %= (qi::token(TOK_FOR) > _name > _linebreak > _do_group)
            | (qi::token(TOK_FOR) > _name > _linebreak > _in > _sequential_sep 
                    > _do_group)
            | (qi::token(TOK_FOR) > _name > _linebreak > _in > _wordlist 
                    > _sequential_sep > _do_group);

        _name %= qi::token(TOK_NAME);

        _in %= qi::token(TOK_IN);

        _wordlist %= (_wordlist > qi::token(TOK_WORD))
            | qi::token(TOK_WORD);

        _case_clause %= (qi::token(TOK_CASE) > qi::token(TOK_WORD) > _linebreak 
                > _in > _linebreak > _case_list > qi::token(TOK_ESAC))
            | (qi::token(TOK_CASE) > qi::token(TOK_WORD) > _linebreak > _in
                    > _linebreak > _case_list_ns > qi::token(TOK_ESAC))
            | (qi::token(TOK_CASE) > qi::token(TOK_WORD) > _linebreak > _in
                    > _linebreak > qi::token(TOK_ESAC));

        _case_list_ns %= (_case_list > _case_item_ns)
            | _case_item_ns;

        _case_list %= (_case_list > _case_item)
            | _case_item;

        _case_item_ns %= (_pattern > ')' > _linebreak)
            | (_pattern > ')' > _compound_list > _linebreak)
            | ('(' > _pattern > ')' > _linebreak)
            | ('(' > _pattern > ')' > _compound_list > _linebreak);

        _case_item %= (_pattern > ')' > _linebreak > qi::token(TOK_DSEMI) > _linebreak)
            | (_pattern > ')' > _compound_list > qi::token(TOK_DSEMI) > _linebreak)
            | ('(' > _pattern > ')' > _linebreak > qi::token(TOK_DSEMI) > _linebreak)
            | ('(' > _pattern > ')' > _compound_list > qi::token(TOK_DSEMI) > _linebreak);

        _pattern %= qi::token(TOK_WORD)
            | (_pattern > '|' > qi::token(TOK_WORD));

        _if_clause %= (qi::token(TOK_IF) > _compound_list > qi::token(TOK_THEN) 
                > _compound_list > _else_part > qi::token(TOK_FI))
            | (qi::token(TOK_IF) > _compound_list > qi::token(TOK_THEN) 
                    > _compound_list > qi::token(TOK_FI));

        _else_part %= (qi::token(TOK_ELIF) > _compound_list > qi::token(TOK_THEN)
                > _compound_list)
            | (qi::token(TOK_ELIF) > _compound_list > qi::token(TOK_THEN)
                    > _compound_list > _else_part)
            | (qi::token(TOK_ELSE) > _compound_list);

        _while_clause %= qi::token(TOK_WHILE) > _compound_list > _do_group;

        _until_clause %= qi::token(TOK_UNTIL) > _compound_list > _do_group;

        _function_definition %= _fname > '(' > ')' > _linebreak > _function_body;

        _function_body %= _compound_command
            | (_compound_command > _redirect_list);

        _fname %= qi::token(TOK_NAME);

        _brace_group %= qi::token(TOK_LBRACE) > _compound_list 
            > qi::token(TOK_RBRACE);

        _do_group %= qi::token(TOK_DO) > _compound_list > qi::token(TOK_DONE);

        _simple_command %= (_cmd_prefix > _cmd_word > _cmd_suffix)
            | (_cmd_prefix > _cmd_word)
            | _cmd_prefix
            | (_cmd_name > _cmd_suffix)
            | _cmd_name;

        _cmd_name %= qi::token(TOK_WORD);

        _cmd_word %= qi::token(TOK_WORD);

        _cmd_prefix %= _io_redirect
            | (_cmd_prefix > _io_redirect)
            | qi::token(TOK_ASSIGNMENT_WORD)
            | (_cmd_prefix > qi::token(TOK_ASSIGNMENT_WORD));

        _cmd_suffix %= _io_redirect
            | (_cmd_suffix > _io_redirect)
            | qi::token(TOK_WORD)
            | (_cmd_suffix > qi::token(TOK_WORD));

        _redirect_list %= _io_redirect
            | (_redirect_list > _io_redirect);

        _io_redirect %= _io_file
            | (qi::token(TOK_IO_NUMBER) > _io_file)
            | _io_here
            | (qi::token(TOK_IO_NUMBER) > _io_here);

        _io_file %= ('<' > _filename)
            | (qi::token(TOK_LESSAND) > _filename)
            | ('>' > _filename)
            | (qi::token(TOK_GREATAND) > _filename)
            | (qi::token(TOK_DGREAT) > _filename)
            | (qi::token(TOK_LESSGREAT) > _filename)
            | (qi::token(TOK_CLOBBER) > _filename);

        _filename %= qi::token(TOK_WORD);

        _io_here %= (qi::token(TOK_DLESS) > _here_end)
            | (qi::token(TOK_DLESSDASH) > _here_end);

        _here_end %= qi::token(TOK_WORD);

        _newline_list %= qi::token(TOK_NEWLINE)
            | (_newline_list > qi::token(TOK_NEWLINE));

        _linebreak %= _newline_list | eps;

        _separator_op %= '&' | char_(';');

        _separator %= (_separator_op > _linebreak)
            | _newline_list;

        _sequential_sep %= (';' > _linebreak)
            | _newline_list;
    }

    qi::rule<Iterator> _complete_command, _list, _and_or, _pipeline, 
        _pipe_sequence, _command, _compound_command, _subshell, _compound_list,
        _term, _for_clause, _name, _in, _wordlist, _case_clause, _case_list_ns,
        _case_list, _case_item_ns, _case_item, _pattern, _if_clause, _else_part,
        _while_clause, _until_clause, _function_definition, _function_body, 
        _fname, _brace_group, _do_group, _simple_command, _cmd_name, _cmd_word, 
        _cmd_prefix, _cmd_suffix, _redirect_list, _io_redirect, _io_file, 
        _filename, _io_here, _here_end, _newline_list, _linebreak, _separator_op,
        _separator, _sequential_sep;
};

} // namespace parser
} // namespace dsh


#endif // PARSER_H

