#include <iostream>
#include <tuple>
#include <iomanip>

#include "ArithExpr.hpp"
#include "DescriptorFunctions.hpp"

void checkTypeCompatibility(std::string scope, TypeDescriptor *t1, TypeDescriptor *t2) {
    if ( !Descriptor::validTypeOp(t1, t2) ) {
            std::cout << scope << "-Fatal Error - Operands / Operators not compatible" << std::endl;
            std::cout << "Operator Enum { INTEGER = 0, DOUBLE = 1, BOOL = 2, STRING = 3 }" << std::endl;
            std::cout << "LHS Operator: " << t1->type() << "\t RHS Operator: " << t2->type() << std::endl;
            exit(1);
        }
}

// ExprNode START
ExprNode::ExprNode(std::shared_ptr<Token> token): _token{token} {}

ExprNode::~ExprNode(){
    if (destructor)
        std::cout << "~ExprNode()" << std::endl;
}

const std::shared_ptr<Token>& ExprNode::token() { return _token; }
//ExprNode END


// InfixExprNode START
InfixExprNode::InfixExprNode(std::shared_ptr<Token> tk) :
    ExprNode{tk},
    _left(nullptr),
    _right(nullptr)
{}

InfixExprNode::~InfixExprNode() {
    if (destructor)
        std::cout << "InfixExprNode Destructor Called" << std::endl;
}

void InfixExprNode::dumpAST(std::string space) {

    std::cout << space << std::setw(15) << std::left << "InfixExprNode " << this << "\tToken ";
    token()->print();
    std::cout << std::endl;
    _left->dumpAST(space + '\t');
    if ( _right != nullptr )
        _right->dumpAST(space + '\t');
}

std::unique_ptr<TypeDescriptor> InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.

    // add later/
    if ( token()->isSubtractionOperator() && _right == nullptr ) {

        auto lValue = _left->evaluate(symTab);

        if (lValue->type() == TypeDescriptor::INTEGER) {

            Descriptor::Int::flipSignBit(lValue.get());
            return std::move(lValue);

        } else if (lValue->type() == TypeDescriptor::DOUBLE) {

            Descriptor::Double::flipSignBit(lValue.get());
            return std::move(lValue);

        } else {
            std::cout << "InfixExprNode::evaluate - Error - Invalid Subtraction operator on type " << lValue->type() << std::endl;
            exit(1);
        }
    }

    auto lValue = _left->evaluate(symTab);
    auto rValue = _right->evaluate(symTab);

    if (debug)
        std::cout << "InfixExprNode::evaluate: " << lValue->type() << " " << token()->symbol() << " " << rValue->type() << std::endl;

    checkTypeCompatibility("InfixExprNode::evaluate()", lValue.get(), rValue.get());

   return Descriptor::relOperatorDescriptor(lValue.get(), rValue.get(), token());
}

void InfixExprNode::print() {
    _left->print();
    token()->print();
    _right->print();
}
// InfixExprNode END

// ComparisonExprNode START
 ComparisonExprNode::ComparisonExprNode(std::shared_ptr<Token> tk):
    ExprNode{tk},
    _left{nullptr},
    _right{nullptr}
{}

ComparisonExprNode::~ComparisonExprNode() {
    if (destructor)
        std::cout << "~ComparisonExprNode()" << std::endl;
}

void ComparisonExprNode::print() {
    // std::cout << "ComparisonExprNode::print()" << std::endl;
}

std::unique_ptr<TypeDescriptor> ComparisonExprNode::evaluate(SymTab &symTab) {

    auto lValue = _left->evaluate(symTab);
    auto rValue = _right->evaluate(symTab);

    if (debug)
        std::cout << "ComparisonExprNode::evaluate:" << std::endl;

    checkTypeCompatibility("ComparisonExprNode::evaluate()", lValue.get(), rValue.get());

    return Descriptor::comparisonDescriptor(lValue.get(), rValue.get(), token());

}

void ComparisonExprNode::dumpAST(std::string space) {

    std::cout << space << std::setw(15) << std::left << "ComparisonExprNode " << this << "\tToken";
    token()->print();
    std::cout << std::endl;
    _left->dumpAST(space + '\t');
    _right->dumpAST(space + '\t');
}
// ComparisonExprNode END

// BooleanExprNode START
BooleanExprNode::BooleanExprNode(std::shared_ptr<Token> tk):
    ExprNode{tk},
    _left{nullptr},
    _right{nullptr}
{}

BooleanExprNode::~BooleanExprNode() {
    if (destructor)
        std::cout << "~BooleanExprNode()" << std::endl;
}

void BooleanExprNode::print() {
    std::cout << "BooleanExprNode::print" << std::endl;
}

std::unique_ptr<TypeDescriptor> BooleanExprNode::evaluate(SymTab &symTab) {
    // std::cout << "BooleanExprNode::evaluate" << std::endl;

    // NOT always take left route -> _right == nullptr

    if ( token()->isNot() ) {

        auto result = _left->evaluate(symTab);
        return Descriptor::negateDescriptor(result.get());

    } else if ( token()->isAnd() ) {

        if (!_left || !_right) {
            std::cout << "BooleanExprNode::evaluate() nullptr AND" << std::endl;
            exit(1);
        }

        auto lValue = _left->evaluate(symTab);
        auto rValue = _right->evaluate(symTab);

        checkTypeCompatibility("BooleanExprNode::evaluate()", lValue.get(), rValue.get());

        return Descriptor::andDescriptor(lValue.get(), rValue.get());

    } else if ( token()->isOr() ) {

        if (!_left || !_right) {
            std::cout << "BooleanExprNode::evaluate() nullptr OR" << std::endl;
            exit(1);
        }

        auto lValue = _left->evaluate(symTab);
        auto rValue = _right->evaluate(symTab);

        checkTypeCompatibility("BooleanExprNode::evaluate()", lValue.get(), rValue.get());

        return Descriptor::orDescriptor(lValue.get(), rValue.get());
    }

    std::cout << "BooleanExprNode::evaluate BAD TOKEN" << std::endl;
    exit(1);

    return nullptr;

}

void BooleanExprNode::dumpAST(std::string space) {

    std::cout << space << std::setw(15) << std::left << "BooleanExprNode " << this << "\tToken: ";
    token()->print();
    std::cout << std::endl;
}
//BooleanExprNode END

// WholeNumber START
WholeNumber::WholeNumber(std::shared_ptr<Token> token):
    ExprNode{token}
{}

WholeNumber::~WholeNumber() {
    if (destructor)
        std::cout << "~WholeNumber" << std::endl;
}

void WholeNumber::print() {
    token()->print();
}

std::unique_ptr<TypeDescriptor> WholeNumber::evaluate(SymTab &symTab) {

    if (debug)
        std::cout << "WholeNumber::evaluate: returning " << token()->getWholeNumber() << std::endl;

    return Descriptor::Int::createIntDescriptor( token()->getWholeNumber() );
}

void WholeNumber::dumpAST(std::string space) {

    std::cout << space;
    std::cout << std::setw(15) << std::left << "WholeNumber " << this;
    std::cout << "\tToken: ";
    token()->print();
    std::cout << std::endl;
}
// WholeNumber END

// Double START
Double::Double(std::shared_ptr<Token> token):
    ExprNode{token}
{}

Double::~Double() {
    if (destructor)
        std::cout << "~Double" << std::endl;
}

void Double::print() {
    token()->print();
}

std::unique_ptr<TypeDescriptor> Double::evaluate(SymTab &symTab [[maybe_unused]]) {

    if (debug)
        std::cout << "Double::evaluate: returning " << token()->getFloat() << std::endl;

    return Descriptor::Double::createDoubleDescriptor( (double) token()->getFloat() );
}

void Double::dumpAST(std::string space) {
    std::cout << space << std::setw(15) << std::left << "Double " << this << "\tToken: ";
    token()->print();
    std::cout << std::endl;
}
// Double END

// Variable START
Variable::Variable(std::shared_ptr<Token> token):
    ExprNode{token}
{}

Variable::~Variable() {
    if (destructor) {
        std::cout << "~Variable()" << std::endl;
    }
}

void Variable::print() {
    token()->print();
}

std::unique_ptr<TypeDescriptor> Variable::evaluate(SymTab &symTab) {
    if ( !symTab.isDefined(token()->getName()) ) {
        std::cout << "Variable::evaluate - Fatal Error - Bypassing Debug\n";
        std::cout << "Use of undefined variable, " << token()->getName() << std::endl;
        exit(1);
    }

    return Descriptor::copyReferencePtr(symTab.getValueFor( token()->getName() ));
}

void Variable::dumpAST(std::string space) {

    std::cout << space << std::setw(15) << std::left << "Variable " << this << "\tToken: " ;
    token()->print();
    std::cout << std::endl;
}

// Variable END

// StringExp START
StringExp::StringExp(std::shared_ptr<Token> token):
    ExprNode{token}
{}

StringExp::~StringExp() {
    if (destructor)
        std::cout << "~StringExp()" << std::endl;
}

void StringExp::print() {
    std::cout << "PRINT" << std::endl;
}

std::unique_ptr<TypeDescriptor> StringExp::evaluate(SymTab &symTab) {
    return Descriptor::String::createStringDescriptor(token()->getString());
}

void StringExp::dumpAST(std::string space) {

    std::cout << space << std::setw(15) << std::left << "StringExp " << this << "\tToken: " ;
    token()->print();
    std::cout << std::endl;
}
// StringExp END


//Start FunctionCall

FunctionCall::FunctionCall(std::shared_ptr<Token> functionName, std::unique_ptr<std::vector<std::unique_ptr<ExprNode>>> testList):
    ExprNode{functionName},
   _testList{std::move(testList)},
   _functionName{functionName->getName()}
    {}

std::unique_ptr<TypeDescriptor> FunctionCall::evaluate(SymTab &symTab) {

    // std::cout << "Evaluate Function Call " << std::endl;
    
    //I prefer this variable to be typed
    std::shared_ptr<FunctionDefinition> functionPointer = symTab.getFunction(_functionName);
    
    auto vect = functionPointer->getParamList();
    int numParams = vect.size();

    if ( numParams != _testList->size()  ) {
        std::cout << "Error FunctionCall::evaluate -> Caller Args != Calling Args" << std::endl;
        exit(1);
    }


    int i = 0;
    for_each(_testList->begin(), _testList->end(), [&](auto &item) {

        symTab.setValueFor(vect[i], item->evaluate(symTab));
        i++;
    });

    symTab.activateScope();


    functionPointer->evaluate(symTab);
    // return retVal;
    return Descriptor::Int::createIntDescriptor(1);
}

void FunctionCall::dumpAST(std::string indent) {

    std::cout << indent << "FunctionCall: " << _functionName << " " << this << "\n";

    for_each(_testList->begin(), _testList->end(), [&] (auto &args) {
        args->dumpAST(indent + "\t");
    });
}

void FunctionCall::print() {}
// End FunctionCall


//START ArrayOperation
ArrayOperation::ArrayOperation(
    std::shared_ptr<Token> tk,
    std::string id, 
    std::string keyword, 
    std::unique_ptr<ExprNode> test):
    ExprNode{tk},
    _id{id},
    _keyword{keyword},
    _test{std::move(test)}
{}

void ArrayOperation::dumpAST(std::string spaces) {
    std::cout << spaces << "ArrayOperation ( " << _keyword << " ): " << _id << " "  << this << std::endl;
    _test->dumpAST(spaces + "\t");
}

void ArrayOperation::print() {}

std::unique_ptr<TypeDescriptor> ArrayOperation::evaluate(SymTab &symTab){ return nullptr;}
//END ArrayOperation


//START ArrayInit
ArrayInit::ArrayInit(
    std::shared_ptr<Token> tk,
    std::unique_ptr<std::vector<std::unique_ptr<ExprNode>>> testList):
    ExprNode{tk},
    _testList{std::move(testList)}
{}

void ArrayInit::dumpAST(std::string spaces) {
    std::cout << spaces << "ArrayInit: " << this << std::endl;
    for_each(_testList->begin(), _testList->end(), [spaces](auto &test){
        test->dumpAST(spaces + "\t");
    });
}

void ArrayInit::print() {}

std::unique_ptr<TypeDescriptor> ArrayInit::evaluate(SymTab &symTab){
    return nullptr;
}
//END ArrayInit

// START ArraySubscription
ArraySubscription::ArraySubscription(
    std::shared_ptr<Token> tk,
    std::string id, 
    std::unique_ptr<ExprNode> test):
    ExprNode{tk},
    _id{id},
    _test{std::move(test)}
{}

void ArraySubscription::dumpAST(std::string spaces) {
    std::cout << spaces << "ArraySubscription: " << _id << " " << this << std::endl;
    _test->dumpAST(spaces + "\t");
}

void ArraySubscription::print() {}

std::unique_ptr<TypeDescriptor> ArraySubscription::evaluate(SymTab &symTab){
    return nullptr;
}
// END ArraySubscription


