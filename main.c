#include "Scanner.h"
#include "parser.h"

int main(){
    int parsingResult;
    parsingResult = parse();
    fprintf(stderr, "Parsing output: %d\n", parsingResult);
    return parsingResult;
}