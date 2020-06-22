#include "Matrix.hpp"
#include <algorithm>
#include <regex>
#include <curses.h>
#include "Core.hpp"

Matrix::Matrix(int m, int n) : Type(eType::Matrix), _m(m), _n(n)
{
	mat = new double*[m];
	for (int i = 0; i < m; i++)
	{
		mat[i] = new double[n];
		for (int j = 0; j < n; j++)
			mat[i][j] = 0;
	}
}

Matrix::Matrix() : Type(eType::Matrix), _m(0), _n(0)
{
	mat = new double*[0];
}

Matrix::Matrix(std::string str) : Type(eType::Matrix)
{
	str.erase(0, 1);
	int i = 0, j = 0;
	_m = std::count(str.begin(), str.end(), ';') + 1;
	mat = new double*[_m];
	std::smatch match;
	while (std::regex_search(str, match, std::regex("(?:(?:\\[|,)(?:-?\\d+(?:\\.\\d+)?)+)+\\]")))
	{
		std::string curr = match.str();
		j = 0;
		curr.erase(0, 1);
		_n = std::count(curr.begin(), curr.end(), ',') + 1;
		mat[i] = new double[_n];
		while (curr.compare("") != 0)
		{
			std::string currCut = curr.substr(0, curr.find(','));
			mat[i][j] = std::stod(currCut);
			j++;
			curr.erase(0, currCut.length() + 1);
		}
		str.erase(0, match.length() + 1);
		i++;
	}
}

Matrix::~Matrix()
{
//	for (int i = 0; i < _m; i++)
//		delete [] mat[i];
//	delete [] mat;
}

int Matrix::GetLines(void) const { return _m; }
int Matrix::GetCol(void) const { return _n; }

std::string Matrix::ToString() const
{
	std::string ret = "[";
	for (int i = 0; i < _m; i++)
	{
		ret += "[";
		for (int j = 0; j < _n; j++)
		{
			ret += Core::Dtoa(mat[i][j]);
			if (j != _n - 1)
				ret += ",";
		}
		ret += "]";
		if (i != _m - 1)
			ret += ";";
	}
	ret += "]";
	return ret;
}

std::string Matrix::ToPrint() const
{
	std::string ret = "";
	for (int i = 0; i < _m; i++)
	{
		ret += "[ ";
		for (int j = 0; j < _n; j++)
		{
			ret += Core::Dtoa(mat[i][j]);
			if (j != _n - 1)
				ret += " , ";
		}
		ret += " ]";
		if (i != _m - 1)
			ret += "\n";
	}
	return ret;
}
/*
pb copy aussi rhs jai limpression
Matrix & Matrix::operator=(const Matrix & rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < _m; i++)
			delete [] mat[i];
		delete [] mat;
		mat = nullptr;
		_m = rhs.GetLines();
		_n = rhs.GetCol();
		mat = new double*[_m];
		for (int i = 0; i < _m; i++)
		{
			mat[i] = new double[_n];
			std::copy(rhs.mat[i], rhs.mat[i] + _n, mat[i]);
		}
	}
	return *this;
}
*/
Matrix operator+(Matrix lhs, Matrix & rhs)
{
	int m = lhs.GetLines();
	int n = lhs.GetCol();

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			lhs.mat[i][j] += rhs.mat[i][j];
		}
	}
	return lhs;
}

Matrix operator-(Matrix lhs, Matrix & rhs)
{
	int m = lhs.GetLines();
	int n = lhs.GetCol();

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			lhs.mat[i][j] -= rhs.mat[i][j];
		}
	}
	return lhs;
}

Matrix operator*(Matrix lhs, Matrix & rhs)
{
	int m = lhs.GetLines();
	int n = rhs.GetCol();
	Matrix ret(m, n);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < m; k++)
				ret.mat[i][j] += lhs.mat[i][k] * rhs.mat[k][j];
		}
	}
	return ret;
}

std::ostream &operator<<(std::ostream & o, Matrix & mat)
{
	int m = mat.GetLines();
	int n = mat.GetCol();

	o <<  "m: " << m << ", n: " << n << std::endl;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			o << "|"<< mat.mat[i][j];
		}
		o << "|";
		if (i != m - 1)
			o << std::endl;
	}
	return o;
}

Matrix operator*(Matrix lhs, double & rhs)
{
	int m = lhs.GetLines();
	int n = lhs.GetCol();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			lhs.mat[i][j] *= rhs;
		}
	}
	return lhs;
}

Matrix Matrix::HadamardProduct(Matrix lhs, Matrix & rhs)
{
	int m = lhs.GetLines();
	int n = lhs.GetCol();
	Matrix ret(m, n);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ret.mat[i][j] = lhs.mat[i][j] * rhs.mat[i][j];
		}
	}
	return ret;
}

void Matrix::Calc(std::string & str, int firstSub, int endSub, int posSubcalc, int firstEra, int endEra)
{
	Matrix ret, left, right;
	std::string subcalc = str.substr(firstSub, endSub);
	str.erase(firstEra, endEra);
	std::string::size_type sz;
	std::string::size_type tmp = subcalc.find("]]") + 2;
	left = Matrix(subcalc.substr(0, tmp));
	if (subcalc[tmp] == '^')
	{
		int power = std::stod(subcalc.substr(tmp + 1), &sz);
		Matrix matTmp = left;
		for (int i = 1; i < power; i++)
			left = left * matTmp;
		sz += tmp + 1;
	}
	else
		sz = tmp;
	if (str[sz + 1] == '*')
		sz++;
	right = Matrix(subcalc.substr(sz + 1));
	if (subcalc.substr(subcalc.rfind(right.ToString()) + right.ToString().length()).find('^') != std::string::npos)
	{
		int power = std::stod(subcalc.substr(subcalc.rfind('^') + 1));
		Matrix matTmp = right;
		for (int i = 1; i < power; i++)
			right = right * matTmp;
	}
	if (subcalc[sz] == '*' && subcalc[sz - 1] != '*')
		ret = left * right;
	else if (subcalc[sz] == '-')
		ret = left - right;
	else if (subcalc[sz] == '+')
		ret = left + right;
	else if (subcalc[sz] == '*' && subcalc[sz - 1] == '*')
		ret = Matrix::HadamardProduct(left, right);
	str.insert(posSubcalc, ret.ToString());
}

Matrix Matrix::EvalExpr(std::string str)
{
	Matrix ret;
	std::smatch m;

	while (1)
	{
		if (str.find_last_of("(") != std::string::npos)
		{
			int posFirstP = str.find_last_of("(");
			int posLastP = str.find(")", str.find_last_of("("));
			std::string subcalc = str.substr(posFirstP + 1, posLastP - posFirstP - 1);
			int posSubcalc = posFirstP;
			str.erase(posFirstP, posLastP - posFirstP + 1);
			ret = EvalExpr(subcalc);
			str.insert(posSubcalc, ret.ToString());
		}
		else if (std::regex_search(str, m, std::regex("\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?(?:\\*|\\*\\*)\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?")))
		{
				for (auto x:m)
					Calc(str, str.find(x.str()), x.str().length(), str.find(x.str()), str.find(x.str()), x.str().length());
		}
		else if (std::regex_search(str, m, std::regex("\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?(?:\\+|\\-)\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?")))
		{
				for (auto x:m)
					Calc(str, str.find(x.str()), x.str().length(), str.find(x.str()), str.find(x.str()), x.str().length());

		}
		else
		{
			if (str.find("^") != std::string::npos)
			{
				int tmp = str.find("]]") + 2;
				Matrix nb = Matrix(str.substr(0, tmp));
				double power = std::stod(str.substr(tmp + 1));
				Matrix matTmp = nb;
				for (int i = 1; i < power; i++)
					nb = nb * matTmp;
				str = nb.ToString();
			}
			break;
		}
	}
	return Matrix(str);
}

void Matrix::CheckMatrix(std::string str)
{
	std::smatch m;
	std::string tmp = str;
	int col = -1;

	if (!std::regex_search(tmp, m, std::regex("\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+")))
		throw std::runtime_error("Syntax error.");
	while (std::regex_search(tmp, m, std::regex("\\[[^\\]\\[]*\\]")))
	{
		printw("%s\n", str.c_str());
		tmp = m[0].str().c_str();
		if (col == -1)
			col = std::count(tmp.begin(), tmp.end(), ',');
		else if (col != std::count(tmp.begin(), tmp.end(), ','))
			throw std::runtime_error("Syntax error: Matrix \""+str+"\" don't have same number of columns.");
		tmp = m.suffix();
		str.erase(str.find(m.str()), m.length());
	}
}

void Matrix::Check(std::string & str)
{
	std::smatch m;
	std::string tmp;
	Matrix leftMat, rightMat;

	while (std::regex_search(str, m, std::regex("(\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?)(?:\\*)(\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?)")))
	{
		std::string left = m[1].str(), right = m[2].str();
		CheckMatrix(left);
		CheckMatrix(right);
		leftMat = Matrix(left);
		rightMat = Matrix(right);
		if (leftMat.GetCol() != rightMat.GetLines())
			throw std::runtime_error("Syntax error: Number of column of left matrix ust be equal to number of right's line.");
		str.erase(m.position(), m.length());
		str.insert(m.position(), Matrix(leftMat.GetLines(), rightMat.GetCol()).ToString());
	}

	while (std::regex_search(str, m, std::regex("(\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?)(?:\\*\\*|\\+|-)(\\[(?:(?:\\[[^\\]]*\\])(?:;|\\]))+(?:\\^\\d+)?)")))
	{
		std::string left = m[1].str(), right = m[2].str();
		CheckMatrix(left);
		CheckMatrix(right);
		leftMat = Matrix(left);
		rightMat = Matrix(right);
		if (leftMat.GetLines() != rightMat.GetLines() || leftMat.GetCol() != rightMat.GetCol())
			throw std::runtime_error("Syntax error: matrices \""+m[0].str()+"\" not the same size.");
		str.erase(m.position(), m.length());
		str.insert(m.position(), Matrix(leftMat.GetLines(), rightMat.GetCol()).ToString());
	}
	CheckMatrix(str);
}
