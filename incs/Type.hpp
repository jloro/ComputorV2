#ifndef TYPE_HPP
# define TYPE_HPP

# include <string>

enum class eType
{
	Real = 0,
	Complex,
	Matrix,
	Function
};

class Type
{
	public:
		Type(eType type/*, std::string name*/);
		virtual	~Type();

		//std::string name;

		eType GetType() const;
		virtual std::string	ToPrint() const = 0;
		virtual std::string	ToString() const = 0;
	protected:
		eType		_type;
};

#endif
