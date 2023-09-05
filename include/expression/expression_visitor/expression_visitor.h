#pragma once



namespace lox {
class AbstractExpression;
class BinaryExpression;
class GroupingExpression;
class LiteralExpression;
class UnaryExpression;

template<typename T>
class ExpressionVisitor{
    public:

        virtual auto Visit(BinaryExpression &exp) const->T=0;
        virtual auto Visit(GroupingExpression &exp)const->T=0;
        virtual auto Visit(LiteralExpression &exp)const->T=0;
        virtual auto Visit(UnaryExpression &exp)const->T=0;

        virtual ~ExpressionVisitor<T>()=default;
        
};
}  // namespace lox