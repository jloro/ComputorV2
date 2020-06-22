#ifndef MATRIX_HPP
# define MATRIX_HPP

# include <iostream>
# include <string>
# include "Type.hpp"

class Matrix : public Type
{
	public:
		Matrix(int m, int n);
		Matrix();
		Matrix(std::string str);
		virtual	~Matrix();

	//	Matrix & operator=(const Matrix & rhs);

		double **mat;

		int	GetLines(void) const;
		int	GetCol(void) const;

		static Matrix HadamardProduct(Matrix lhs, Matrix & rhs);
		static Matrix EvalExpr(std::string str);
		std::string ToString() const;

		std::string ToPrint() const;
		static void Check(std::string & str);

	private:
		int	_m;//lignes
		int	_n;//colonnes

		static void Calc(std::string & str, int firstSub, int endSub, int posSubcalc, int firstEra, int endEra);
		static void CheckMatrix(std::string str);

};

std::ostream &operator<<(std::ostream & o, Matrix & mat);
Matrix operator+(Matrix lhs, Matrix & rhs);
Matrix operator-(Matrix lhs, Matrix & rhs);
Matrix operator*(Matrix lhs, Matrix & rhs);
Matrix operator*(Matrix lhs, double & rhs);

#endif
