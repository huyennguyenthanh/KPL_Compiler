/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
    Token* tmp = currentToken;
    currentToken = lookAhead;
    lookAhead = getValidToken();
    free(tmp);
}

void eat(TokenType tokenType) {
    if (lookAhead->tokenType == tokenType) {
        printToken(lookAhead);
        scan();
    } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
    assert("Parsing a Program ....");
    eat(KW_PROGRAM);
    eat(TK_IDENT);
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_PERIOD);
    assert("Program parsed!");
}

void compileBlock(void) {
    assert("Parsing a Block ....");
    if (lookAhead->tokenType == KW_CONST) {
        eat(KW_CONST);
        compileConstDecl();
        compileConstDecls();
        compileBlock2();
    } else compileBlock2();
    assert("Block parsed!");
}

void compileBlock2(void) {
    if (lookAhead->tokenType == KW_TYPE) {
        eat(KW_TYPE);
        compileTypeDecl();
        compileTypeDecls();
        compileBlock3();
    } else compileBlock3();
}

void compileBlock3(void) {
    if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);
        compileVarDecl();
        compileVarDecls();
        compileBlock4();
    } else compileBlock4();
}

void compileBlock4(void) {
    compileSubDecls();
    compileBlock5();
}

void compileBlock5(void) {
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
}

void compileConstDecls(void) {
    // TODO
    if (lookAhead->tokenType == KW_CONST) {
        eat(KW_CONST);
        compileConstDecl();
    }
}

void compileConstDecl(void) {
    // TODO
    eat(TK_IDENT);
    eat(SB_EQ);
    compileConstant();
    eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
    // TODO
    if (lookAhead->tokenType == KW_TYPE) {
        eat(KW_TYPE);
        compileTypeDecl();
    }
}

void compileTypeDecl(void) {
    // TODO
    while (lookAhead->tokenType == TK_IDENT) {
        eat(TK_IDENT);
        eat(SB_EQ);
        compileType();
        eat(SB_SEMICOLON);
    }
}

void compileVarDecls(void) {
    // TODO
    if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);
        compileVarDecl();
    }
}

void compileVarDecl(void) {
    // TODO
    while (lookAhead->tokenType == TK_IDENT) {
        eat(TK_IDENT);
        eat(SB_COLON);
        compileType();
        eat(SB_SEMICOLON);
    }
}

void compileSubDecls(void) {
    assert("Parsing subtoutines ....");
    // TODO
    if (lookAhead->tokenType == KW_FUNCTION) {
        compileFuncDecl();
        compileSubDecls();
    } else if ((lookAhead->tokenType) == KW_PROCEDURE) {
        compileProcDecl();
        compileSubDecls();
    }
    assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
    assert("Parsing a function ....");
    // TODO
    eat(KW_FUNCTION);
    eat(TK_IDENT);
    compileParams();
    eat(SB_COLON);
    compileBasicType();
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    assert("Function parsed ....");
}

void compileProcDecl(void) {
    assert("Parsing a procedure ....");
    // TODO
    eat(KW_PROCEDURE);
    eat(TK_IDENT);
    compileParams();
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
    // TODO
    if (lookAhead->tokenType == TK_NUMBER)
        eat(TK_NUMBER);
    else if (lookAhead->tokenType == TK_IDENT) {
        eat(TK_IDENT);
        compileConstDecl();
    } else if (lookAhead->tokenType == TK_CHAR) {
        eat(TK_CHAR);
        compileConstant();
    } else
        error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
}

void compileConstant(void) {
    // TODO
    if (lookAhead->tokenType == SB_PLUS) {
        eat(SB_PLUS);
        compileConstant2();
    } else if (lookAhead->tokenType == SB_MINUS) {
        eat(SB_MINUS);
        compileConstant2();
    } else if (lookAhead->tokenType == TK_CHAR) {
        eat(TK_CHAR);
    } else
        compileConstant2();
}

void compileConstant2(void) {
    // TODO
    if (lookAhead->tokenType == TK_IDENT) {
        eat(TK_IDENT);
        compileConstDecls();
    } else if (lookAhead->tokenType == TK_NUMBER)
        eat(TK_NUMBER);
    else
        error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
}

void compileType(void) {
    // TODO
    if (lookAhead->tokenType == KW_INTEGER)
        eat(KW_INTEGER);
    else if (lookAhead->tokenType == KW_CHAR)
        eat(KW_CHAR);
    else if (lookAhead->tokenType == KW_FLOAT)
        eat(KW_FLOAT);
    else if (lookAhead->tokenType == KW_ARRAY) {
        eat(KW_ARRAY);
        eat(SB_LSEL);
        eat(TK_NUMBER);
        eat(SB_RSEL);
        eat(KW_OF);
        compileType();

    } 
    else if (lookAhead->tokenType == TK_IDENT)
        eat(TK_IDENT);
    else
        error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
}

void compileBasicType(void) {
    // TODO
    if (lookAhead->tokenType == KW_INTEGER)
        eat(KW_INTEGER);
    else if (lookAhead->tokenType == KW_CHAR)
        eat(KW_CHAR);
    else
        error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
}

void compileParams(void) {
    // TODO
    if (lookAhead->tokenType == SB_LPAR) {
        eat(SB_LPAR);
        compileParam();
        compileParams2();
        eat(SB_RPAR);
    }
}

void compileParams2(void) {
    // TODO
    while (lookAhead->tokenType == SB_SEMICOLON) {
        eat(SB_SEMICOLON);
        compileParam();
    }
}

void compileParam(void) {
    // TODO
    if (lookAhead->tokenType == TK_IDENT) {
        eat(TK_IDENT);
        eat(SB_COLON);
        compileBasicType();
    } else if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);
        eat(TK_IDENT);
        eat(SB_COLON);
        compileBasicType();
    } else
        error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
}

void compileStatements(void) {
    // TODO
    compileStatement();
    compileStatements2();
}

void compileStatements2(void) {
    // TODO
    if (lookAhead->tokenType == SB_SEMICOLON) {
        eat(SB_SEMICOLON);
        compileStatement();
        compileStatements2();
    } else if (lookAhead->tokenType != KW_END)
        eat(SB_SEMICOLON);
}

void compileStatement(void) {
    switch (lookAhead->tokenType) {
    case TK_IDENT:
        
        compileAssignSt();
        break;
    case KW_CALL:
        compileCallSt();
        break;
    case KW_BEGIN:
        compileGroupSt();
        break;
    case KW_IF:
        compileIfSt();
        break;
    case KW_WHILE:
        compileWhileSt();
        break;
    case KW_FOR:
        compileForSt();
        break;
    // EmptySt needs to check FOLLOW tokens
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
        break;
    // Error occurs
    default:
        error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
        break;
    }
}

void compileAssignSt(void) {
    assert("Parsing an assign statement ....");
    // TODO
    eat(TK_IDENT);
    // compileIndexes();
    switch (lookAhead->tokenType) {
    case SB_ASSIGN:
        eat(SB_ASSIGN);
        break;
    case SB_ASSIGN_PLUS:
        eat(SB_ASSIGN_PLUS);
        break;
    case SB_ASSIGN_SUBTRACT:
        eat(SB_ASSIGN_SUBTRACT);
        break;
    case SB_ASSIGN_TIMES:
        eat(SB_ASSIGN_TIMES);
        break;
    case SB_ASSIGN_DIVIDE:
        eat(SB_ASSIGN_DIVIDE);
        break;
    default:
        break;
    }
    // eat(SB_ASSIGN);
    compileExpression();
    assert("Assign statement parsed ....");
}
void compileAssignPlusSt(void) {
    assert("Parsing an assign plus statement ....");
    // TODO

    eat(TK_IDENT);

    compileIndexes();
    eat(SB_ASSIGN_PLUS);
    compileExpression();
    assert("Assign plus statement parsed ....");
}

void compileCallSt(void) {
    assert("Parsing a call statement ....");
    // TODO
    eat(KW_CALL);
    eat(TK_IDENT);
    compileArguments();
    assert("Call statement parsed ....");
}

void compileGroupSt(void) {
    assert("Parsing a group statement ....");
    // TODO
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
    assert("Group statement parsed ....");
}

void compileIfSt(void) {
    assert("Parsing an if statement ....");
    eat(KW_IF);
    compileCondition();
    eat(KW_THEN);
    compileStatement();
    if (lookAhead->tokenType == KW_ELSE)
        compileElseSt();
    assert("If statement parsed ....");
}

void compileElseSt(void) {
    eat(KW_ELSE);
    compileStatement();
}

void compileWhileSt(void) {
    assert("Parsing a while statement ....");
    // TODO
    eat(KW_WHILE);
    compileCondition();
    eat(KW_DO);
    compileStatement();
    assert("While statement parsed ....");
}

void compileForSt(void) {
    assert("Parsing a for statement ....");
    // TODO
    eat(KW_FOR);
    eat(TK_IDENT);
    eat(SB_ASSIGN);
    compileExpression();
    eat(KW_TO);
    compileExpression();
    eat(KW_DO);
    compileStatement();
    assert("For statement parsed ....");
}

void compileArguments(void) {
    // TODO
    switch (lookAhead->tokenType) {
    case SB_LPAR:
        eat(SB_LPAR);
        compileExpression();
        compileArguments2();
        eat(SB_RPAR);
        break;
    // Check FOLLOW set
    case SB_TIMES:
    case SB_SLASH:
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
        break;
    default:
        error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileArguments2(void) {
    // TODO
    while (lookAhead->tokenType == SB_COMMA) {
        eat(SB_COMMA);
        compileExpression();
    }
}

void compileCondition(void) {
    // TODO
    compileExpression();
    compileCondition2();
}

void compileCondition2(void) {
    // TODO
    switch (lookAhead->tokenType) {
    case SB_EQ:
        eat(SB_EQ);
        break;
    case SB_NEQ:
        eat(SB_NEQ);
        break;
    case SB_LE:
        eat(SB_LE);
        break;
    case SB_LT:
        eat(SB_LT);
        break;
    case SB_GE:
        eat(SB_GE);
        break;
    case SB_GT:
        eat(SB_GT);
        break;
    default:
        error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    }
    compileExpression();
}

void compileExpression(void) {
    assert("Parsing an expression");
    // TODO
    switch (lookAhead->tokenType) {
    case SB_PLUS:
        eat(SB_PLUS);
        compileExpression2();
        break;
    case SB_MINUS:
        eat(SB_MINUS);
        compileExpression2();
        break;
    default:
        compileExpression2();
    }
    assert("Expression parsed");
}

void compileExpression2(void) {
    // TODO
    compileTerm();
    compileExpression3();
}


void compileExpression3(void) {
    // TODO
    switch (lookAhead->tokenType) {
    case SB_PLUS:
        eat(SB_PLUS);
        compileTerm();
        compileExpression3();
        break;
    case SB_MINUS:
        eat(SB_MINUS);
        compileTerm();
        compileExpression3();
        break;
    // check the FOLLOW set
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
        break;
    default:
        error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileTerm(void) {
    // TODO
    compileFactor();
    compileTerm2();
}

void compileTerm2(void) {
    // TODO
    switch (lookAhead->tokenType) {
    case SB_TIMES:
        eat(SB_TIMES);
        compileFactor();
        compileTerm2();
        break;
    case SB_SLASH:
        eat(SB_SLASH);
        compileFactor();
        compileTerm2();
        break;
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
        break;
    default:
        error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileFactor(void) {
    // TODO
    switch (lookAhead->tokenType) {
    case TK_NUMBER:
        eat(TK_NUMBER);
        break;
    case TK_FLOAT:
        eat(TK_FLOAT);
        break;
    case TK_IDENT:
        eat(TK_IDENT);
        switch (lookAhead->tokenType) {
        case SB_LPAR:
            compileArguments();
            break;
        case SB_LSEL:
            compileIndexes();
            break;
        default:
            break;
        }
        break;
    case TK_CHAR:
        eat(TK_CHAR);
        break;
    case SB_LPAR:
        eat(SB_LPAR);
        compileExpression();
        eat(SB_RPAR);
        break;
    default:
        error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileIndexes(void) {
    // TODO
    while (lookAhead->tokenType == SB_LSEL) {
        eat(SB_LSEL);
        compileExpression();
        eat(SB_RSEL);
    }
}

int compile(char *fileName) {
    if (openInputStream(fileName) == IO_ERROR)
        return IO_ERROR;

    currentToken = NULL;
    lookAhead = getValidToken();

    compileProgram();

    free(currentToken);
    free(lookAhead);
    closeInputStream();
    return IO_SUCCESS;

}
