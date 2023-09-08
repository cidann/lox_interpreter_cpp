#pragma once


namespace lox {
class ExpressionStatement;
class PrintStatement;
class AnyStatement;
class VariableStatement;
class BlockStatement;

template<typename T>
class StatementVisitor{
    public:

        virtual auto Visit(ExpressionStatement &exp) const->T=0;
        virtual auto Visit(PrintStatement &exp)const->T=0;
        virtual auto Visit(VariableStatement &exp)const->T=0;
        virtual auto Visit(BlockStatement &exp)const->T=0;
        

        virtual ~StatementVisitor<T>()=default;
        
};
}  // namespace lox