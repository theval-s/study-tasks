#ifndef CPP_SOLUTION_EXPRESSION_H
#define CPP_SOLUTION_EXPRESSION_H

#include <string>
#include <iostream>

class expression {
public:
    virtual std::string prefix_form() = 0;
    virtual bool solve() = 0;
    virtual ~expression() {};
};

class variable : public expression {
private:
    std::string _name;
    bool _value;
public:
    variable(std::string &name) : _name(name) {
    }

    virtual std::string prefix_form() {
        return _name;
    }
    void set_value(bool value){
        _value = value;
    }
    bool solve() override {return _value;}
};

class implication : public expression {
private:
    expression* _left;
    expression* _right;

public:
    implication(expression* left, expression* right) :
            _left(left),
            _right(right) {
    }

    virtual std::string prefix_form() {
        return "(->," + _left->prefix_form() + "," + _right->prefix_form() + ")";
    }
    bool solve() override {
    	return _left->solve() <= _right->solve();
    }
};

class disjunction : public expression {
private:
    expression* _left;
    expression* _right;

public:
    disjunction(expression* left, expression* right) :
            _left(left),
            _right(right) {
    }

    virtual std::string prefix_form() {
        return "(|," + _left->prefix_form() + "," + _right->prefix_form() + ")";
    }
    bool solve() override  {
    	return _left->solve() || _right->solve();
    }
};

class conjunction : public expression {
private:
    expression* _left;
    expression* _right;

public:
    conjunction(expression* left, expression* right) :
            _left(left),
            _right(right) {
    }

    virtual std::string prefix_form() {
        return "(&," + _left->prefix_form() + "," + _right->prefix_form() + ")";
    }
    bool solve() override  {
    	return _left->solve() && _right->solve();
    }
};

class negation : public expression {
private:
    expression* _expr;

public:
    negation(expression* expr) :
            _expr(expr) {
    }

    virtual std::string prefix_form() {
        return "(!" + _expr->prefix_form() + ")";
    }
    bool solve() override {
    	return !_expr->solve();
    }
};

#endif //CPP_SOLUTION_EXPRESSION_H
