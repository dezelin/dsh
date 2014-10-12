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

#include <boost/spirit/lex.h>

namespace dsh {

    namespace lex {

        //
        // Lexer token ids as defined in IEEE Std 1003.2
        //
        enum tokens {
            TOK_WORD,
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
            TOK_DLESSDASH
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
            TOK_IN
        };


    }

}


#endif // LEX_H
