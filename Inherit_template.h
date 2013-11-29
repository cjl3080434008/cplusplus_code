#ifndef BASE_CLASS_H
#define BASE_CLASS_H


template<class Type>
class Base_class{
public:
	virtual ~Base_class(){}

	virtual void interface_method1() = 0;
	virtual void interface_method2() = 0;
	virtual void interface_method3() = 0;

	virtual Base_class *clone()		 = 0;
};

template<class Type>
classs Sub_class: public Base_class<Type>
{
public:
	explicit Sub_class(const Type *current):current_(current){}
	virtual ~Sub_class(){delete current;}

	virtual void interface_method1(){}

	virtual void interface_method2(){}

	virtual void interface_method3(){}

	virtual Base_class *clone(){return new Sub_class(*this);}
public:
	void sub_class_method();
	void sub_class_method_1();
	void sub_class_method_1();

private:
	void sub_class_private_method();
	void sub_class_private_method_1();
	void sub_class_private_method_2();

private:
	Type *current_;
};



#endif
