#include "Parse/Parser.h"
#include "AST/AST.h"
#include "Basic/TokenKinds.h"

namespace cawk {
void Parser::Advance(bool Regex) { Lex.Next(Tok, Regex); }

Token Parser::Peek(std::size_t N, bool Regex) const {
  Token T;
  auto BufferPtr = Lex.GetBufferPtr();

  for (; N != 0; --N)
    Lex.Next(T, Regex);

  Lex.SetBufferPtr(BufferPtr);
  return T;
}

TranslationUnitDecl *Parser::ParseProgram() { return nullptr; }

Decl *Parser::ParseItem() {
  switch (Tok.GetKind()) {
  default: {
    auto Pattern = ParsePattern();
    auto Action = Tok.Is(tok::l_brace) ? ParseAction() : nullptr;
    return RuleDecl::Create(Pattern, Action);
  }
  case tok::kw_BEGIN:
  case tok::kw_END: {
    auto Pattern = Tok;
    Lex.Next(Tok);
    return RuleDecl::Create(DeclRefExpr::Create(Pattern), ParseAction());
  }
  case tok::kw_function: {
    Expect(tok::kw_function);
    auto Identifier = Tok;
    Expect(tok::identifier, tok::l_paren);
    auto Params = ParseParamList();
    Expect(tok::r_paren);
    Skip(tok::newline);
    auto Body = ParseAction();
    return FunctionDecl::Create(Identifier, Params, Body);
  }
  }
}

std::vector<ParamVarDecl *> Parser::ParseParamList() {
  std::vector<ParamVarDecl *> Params;

  auto Param = Tok;
  if (!Consume(tok::identifier))
    return Params;

  Params.push_back(ParamVarDecl::Create(Param));

  for (; Consume(tok::comma);) {
    Params.push_back(ParamVarDecl::Create(Tok));
    Expect(tok::identifier);
  }

  return Params;
}

Expr *Parser::ParsePattern() {
  switch (Tok.GetKind()) {
  default:
    return ParseNormalPattern();
  case tok::kw_BEGIN:
  case tok::kw_END:
    return ParseSpecialPattern();
  }
}

Expr *Parser::ParseNormalPattern() { return ParseExpr(); }

DeclRefExpr *Parser::ParseSpecialPattern() {
  auto Pattern = Tok;
 ExpectOneOf(tok::kw_BEGIN, tok::kw_END);
  return DeclRefExpr::Create(Pattern);
}

} // namespace cawk

#if 0
program          : item_list
                 | item_list item
                 ;


item_list        : /* empty */
                 | item_list item terminator
                 ;


item             : action
                 | pattern action
                 | normal_pattern
                 | Function NAME      '(' param_list_opt ')'
                       newline_opt action
                 | Function FUNC_NAME '(' param_list_opt ')'
                       newline_opt action
                 ;


param_list_opt   : /* empty */
                 | param_list
                 ;


param_list       : NAME
                 | param_list ',' NAME
                 ;


pattern          : normal_pattern
                 | special_pattern
                 ;


normal_pattern   : expr
                 | expr ',' newline_opt expr
                 ;


special_pattern  : Begin
                 | End
                 ;


action           : '{' newline_opt                             '}'
                 | '{' newline_opt terminated_statement_list   '}'
                 | '{' newline_opt unterminated_statement_list '}'
                 ;


terminator       : terminator NEWLINE
                 |            ';'
                 |            NEWLINE
                 ;


terminated_statement_list : terminated_statement
                 | terminated_statement_list terminated_statement
                 ;


unterminated_statement_list : unterminated_statement
                 | terminated_statement_list unterminated_statement
                 ;


terminated_statement : action newline_opt
                 | If '(' expr ')' newline_opt terminated_statement
                 | If '(' expr ')' newline_opt terminated_statement
                       Else newline_opt terminated_statement
                 | While '(' expr ')' newline_opt terminated_statement
                 | For '(' simple_statement_opt ';'
                      expr_opt ';' simple_statement_opt ')' newline_opt
                      terminated_statement
                 | For '(' NAME In NAME ')' newline_opt
                      terminated_statement
                 | ';' newline_opt
                 | terminatable_statement NEWLINE newline_opt
                 | terminatable_statement ';'     newline_opt
                 ;


unterminated_statement : terminatable_statement
                 | If '(' expr ')' newline_opt unterminated_statement
                 | If '(' expr ')' newline_opt terminated_statement
                      Else newline_opt unterminated_statement
                 | While '(' expr ')' newline_opt unterminated_statement
                 | For '(' simple_statement_opt ';'
                  expr_opt ';' simple_statement_opt ')' newline_opt
                      unterminated_statement
                 | For '(' NAME In NAME ')' newline_opt
                      unterminated_statement
                 ;


terminatable_statement : simple_statement
                 | Break
                 | Continue
                 | Next
                 | Exit expr_opt
                 | Return expr_opt
                 | Do newline_opt terminated_statement While '(' expr ')'
                 ;


simple_statement_opt : /* empty */
                 | simple_statement
                 ;


simple_statement : Delete NAME '[' expr_list ']'
                 | expr
                 | print_statement
                 ;


print_statement  : simple_print_statement
                 | simple_print_statement output_redirection
                 ;


simple_print_statement : Print  print_expr_list_opt
                 | Print  '(' multiple_expr_list ')'
                 | Printf print_expr_list
                 | Printf '(' multiple_expr_list ')'
                 ;


output_redirection : '>'    expr
                 | APPEND expr
                 | '|'    expr
                 ;


expr_list_opt    : /* empty */
                 | expr_list
                 ;


expr_list        : expr
                 | multiple_expr_list
                 ;


multiple_expr_list : expr ',' newline_opt expr
                 | multiple_expr_list ',' newline_opt expr
                 ;


expr_opt         : /* empty */
                 | expr
                 ;


expr             : unary_expr
                 | non_unary_expr
                 ;


unary_expr       : '+' expr
                 | '-' expr
                 | unary_expr '^'      expr
                 | unary_expr '*'      expr
                 | unary_expr '/'      expr
                 | unary_expr '%'      expr
                 | unary_expr '+'      expr
                 | unary_expr '-'      expr
                 | unary_expr          non_unary_expr
                 | unary_expr '<'      expr
                 | unary_expr LE       expr
                 | unary_expr NE       expr
                 | unary_expr EQ       expr
                 | unary_expr '>'      expr
                 | unary_expr GE       expr
                 | unary_expr '˜'      expr
                 | unary_expr NO_MATCH expr
                 | unary_expr In NAME
                 | unary_expr AND newline_opt expr
                 | unary_expr OR  newline_opt expr
                 | unary_expr '?' expr ':' expr
                 | unary_input_function
                 ;


non_unary_expr   : '(' expr ')'
                 | '!' expr
                 | non_unary_expr '^'      expr
                 | non_unary_expr '*'      expr
                 | non_unary_expr '/'      expr
                 | non_unary_expr '%'      expr
                 | non_unary_expr '+'      expr
                 | non_unary_expr '-'      expr
                 | non_unary_expr          non_unary_expr
                 | non_unary_expr '<'      expr
                 | non_unary_expr LE       expr
                 | non_unary_expr NE       expr
                 | non_unary_expr EQ       expr
                 | non_unary_expr '>'      expr
                 | non_unary_expr GE       expr
                 | non_unary_expr '˜'      expr
                 | non_unary_expr NO_MATCH expr
                 | non_unary_expr In NAME
                 | '(' multiple_expr_list ')' In NAME
                 | non_unary_expr AND newline_opt expr
                 | non_unary_expr OR  newline_opt expr
                 | non_unary_expr '?' expr ':' expr
                 | NUMBER
                 | STRING
                 | lvalue
                 | ERE
                 | lvalue INCR
                 | lvalue DECR
                 | INCR lvalue
                 | DECR lvalue
                 | lvalue POW_ASSIGN expr
                 | lvalue MOD_ASSIGN expr
                 | lvalue MUL_ASSIGN expr
                 | lvalue DIV_ASSIGN expr
                 | lvalue ADD_ASSIGN expr
                 | lvalue SUB_ASSIGN expr
                 | lvalue '=' expr
                 | FUNC_NAME '(' expr_list_opt ')'
                      /* no white space allowed before '(' */
                 | BUILTIN_FUNC_NAME '(' expr_list_opt ')'
                 | BUILTIN_FUNC_NAME
                 | non_unary_input_function
                 ;


print_expr_list_opt : /* empty */
                 | print_expr_list
                 ;


print_expr_list  : print_expr
                 | print_expr_list ',' newline_opt print_expr
                 ;


print_expr       : unary_print_expr
                 | non_unary_print_expr
                 ;


unary_print_expr : '+' print_expr
                 | '-' print_expr
                 | unary_print_expr '^'      print_expr
                 | unary_print_expr '*'      print_expr
                 | unary_print_expr '/'      print_expr
                 | unary_print_expr '%'      print_expr
                 | unary_print_expr '+'      print_expr
                 | unary_print_expr '-'      print_expr
                 | unary_print_expr          non_unary_print_expr
                 | unary_print_expr '˜'      print_expr
                 | unary_print_expr NO_MATCH print_expr
                 | unary_print_expr In NAME
                 | unary_print_expr AND newline_opt print_expr
                 | unary_print_expr OR  newline_opt print_expr
                 | unary_print_expr '?' print_expr ':' print_expr
                 ;


non_unary_print_expr : '(' expr ')'
                 | '!' print_expr
                 | non_unary_print_expr '^'      print_expr
                 | non_unary_print_expr '*'      print_expr
                 | non_unary_print_expr '/'      print_expr
                 | non_unary_print_expr '%'      print_expr
                 | non_unary_print_expr '+'      print_expr
                 | non_unary_print_expr '-'      print_expr
                 | non_unary_print_expr          non_unary_print_expr
                 | non_unary_print_expr '˜'      print_expr
                 | non_unary_print_expr NO_MATCH print_expr
                 | non_unary_print_expr In NAME
                 | '(' multiple_expr_list ')' In NAME
                 | non_unary_print_expr AND newline_opt print_expr
                 | non_unary_print_expr OR  newline_opt print_expr
                 | non_unary_print_expr '?' print_expr ':' print_expr
                 | NUMBER
                 | STRING
                 | lvalue
                 | ERE
                 | lvalue INCR
                 | lvalue DECR
                 | INCR lvalue
                 | DECR lvalue
                 | lvalue POW_ASSIGN print_expr
                 | lvalue MOD_ASSIGN print_expr
                 | lvalue MUL_ASSIGN print_expr
                 | lvalue DIV_ASSIGN print_expr
                 | lvalue ADD_ASSIGN print_expr
                 | lvalue SUB_ASSIGN print_expr
                 | lvalue '=' print_expr
                 | FUNC_NAME '(' expr_list_opt ')'
                     /* no white space allowed before '(' */
                 | BUILTIN_FUNC_NAME '(' expr_list_opt ')'
                 | BUILTIN_FUNC_NAME
                 ;


lvalue           : NAME
                 | NAME '[' expr_list ']'
                 | '$' expr
                 ;


non_unary_input_function : simple_get
                 | simple_get '<' expr
                 | non_unary_expr '|' simple_get
                 ;


unary_input_function : unary_expr '|' simple_get
                 ;


simple_get       : GETLINE
                 | GETLINE lvalue
                 ;


newline_opt      : /* empty */
                 | newline_opt NEWLINE
                 ;
#endif
