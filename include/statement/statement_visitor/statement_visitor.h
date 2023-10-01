#pragma once



namespace lox {
class ExpressionStatement;
class PrintStatement;
class AnyStatement;
class VariableStatement;
class BlockStatement;
class IfStatement;
class WhileStatement;
class FunctionStatement;
class ReturnStatement;

template<typename T>
class StatementVisitor{
    public:

        virtual auto Visit(ExpressionStatement &exp) const->T=0;
        virtual auto Visit(PrintStatement &exp)const->T=0;
        virtual auto Visit(VariableStatement &exp)const->T=0;
        virtual auto Visit(BlockStatement &exp)const->T=0;
        virtual auto Visit(IfStatement &exp)const->T=0;
        virtual auto Visit(WhileStatement &exp)const->T=0;
        virtual auto Visit(FunctionStatement &exp)const->T=0;
        virtual auto Visit(ReturnStatement &exp)const->T=0;
        

        virtual ~StatementVisitor<T>()=default;
        
};
}  // namespace lox