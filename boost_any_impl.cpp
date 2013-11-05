class any
{
public:
	//重要的是内部的任意类型的抽象类
	class placeholder
	{
	public:
		virtual ~placeholder(){}

		virtual placeholder &clone() const = 0;
		virtual const std::type_info type() const = 0; 
	}

	template<class RealValue>
	class holder: public placeholder
	{
	public:
		holder(RealValue &val):val_(val){}

		virtual holder &clone() const
		{
			return new holder(val_);
		}

		virtual const std::type_info type() const
		{
			return typeid(RealValue);
		}
	private:
		RealValue val_;
	}

template<class RealValue>
any(const RealValue &value):content(new holer<RealValue>(value)){}

private:
	placeholder *content_;

}
这样就可以接受到所有类型的数据，在以这个单位   容器<any> 中 则容器中的所有的数据都是any类型既任意类型。
