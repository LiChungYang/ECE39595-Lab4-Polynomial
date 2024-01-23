#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>
#include <list>
#include "poly.h"
#include "parser.h"

#define ANSI_GREEN "\x1b[1;32m"
#define ANSI_RESET "\x1b[0m"
#define log_end() std::cout << ANSI_GREEN <<"End of " <<  __func__ << ANSI_RESET << "\n"

void example() {
	polynomial polyFromFile = build_poly_from_file("result.txt");
	polyFromFile.print();
	log_end();
}

void test_zero_polynomial() {
	polynomial zero_poly;
	zero_poly.print();
	log_end();
}

void test_init_linear_polynomial() {
	std::vector<std::pair<power,coeff>> param_vec = {{1,2}, {0,1}};
	//expect to see 2*x^1 + 1*x^0
	polynomial linear_poly(param_vec.begin(), param_vec.end());

	assert(linear_poly.find_degree_of() == 1);
	linear_poly.print();
	log_end();
}

void test_copy_linear_polynomial() {
	std::vector<std::pair<power,coeff>> param_vec = {{1,2}, {0,1}};
	//expect to see 2*x^1 + 1*x^0
	polynomial linear_poly(param_vec.begin(), param_vec.end());
	polynomial linear_copy_poly(linear_poly);
	assert(linear_poly.find_degree_of() == 1);
	assert(linear_copy_poly.find_degree_of() == 1);
	std::cout << "linear_poly.print() : ";
	linear_poly.print();
	std::cout << "linear_copy_poly.print() : ";
	linear_copy_poly.print();
	log_end();
}

void test_init_quadratic_polynomial() {
	using namespace std;
	list<pair<power,coeff>> param_list = {{2,1}, {1,-1}, {0,1}};
	polynomial quad_poly(param_list.begin(), param_list.end());
	polynomial quad_copy(quad_poly);
	assert(quad_poly.find_degree_of() == 2);
	assert(quad_copy.find_degree_of() == 2);
	std::cout<<"quad_poly.print() : ";
	quad_poly.print();
	std::cout << "quad_copy.print() : ";
	quad_copy.print();
	log_end();
}

void test_equal_op() {
	using namespace std;
	list<pair<power,coeff>> param_list = {{2,1}, {1,-1}, {0,1}};
	polynomial poly1;
	polynomial linear_poly(param_list.begin(), param_list.end());

	poly1 = linear_poly;

	assert(poly1.find_degree_of() == linear_poly.find_degree_of());
	cout << "poly1.print(): ";
	poly1.print();
	cout << "linear_poly.print():";
	linear_poly.print();
	log_end();
}

void test_sparse_quad_poly() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{2,1},{1,0}, {0,3}};
	polynomial sparse_poly(param_vec.begin(),param_vec.end());
	assert(sparse_poly.find_degree_of() == 2);
	cout << "sparse_poly.print(): ";
	sparse_poly.print();
	log_end();
}

void test_poly_add_poly() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{1,4}, {0,2}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());	//expect to see 4*x^1 + 2;
	vector<pair<power,coeff>>constPoly_param = {{0,2}};
	polynomial constPoly(constPoly_param.begin(), constPoly_param.end());
	cout << "before addition: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "constPoly.print(): ";
	constPoly.print();

	polynomial sumPoly = linearPoly + constPoly;
	cout<< "after addition\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "sumPoly.print(): ";
	sumPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = sumPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";

	assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_poly_add_poly2() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{5,1}, {3,2}, {0,1}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());	//expect to see 4*x^1 + 2;
	vector<pair<power,coeff>>secondPoly_parm = {{0,1}, {0,3}};
	polynomial secondPoly(secondPoly_parm.begin(), secondPoly_parm.end());
	cout << "before addition: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "constPoly.print(): ";
	secondPoly.print();

	polynomial sumPoly = linearPoly + secondPoly;
	cout<< "after addition\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "sumPoly.print(): ";
	sumPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = sumPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";

	assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_poly_int_add() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{5,1}, {3,2}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());	//expect to see 4*x^1 + 2;
	int secondint_parm = 5;
	cout << "before addition: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "secondint_parm.print(): ";
	printf("intval %d\n", secondint_parm);

	polynomial sumPoly =  linearPoly + secondint_parm;
	cout<< "after addition\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "sumPoly.print(): ";
	sumPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = sumPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";

	assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_poly_poly_time() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{5,1}, {3,2}, {0,1}};
	//vector<pair<power,coeff>> param_vec = {{5,0}, {3,0}, {0,0}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());
	vector<pair<power,coeff>>secondPoly_parm = {{0,1}, {0,3}};
	//vector<pair<power,coeff>>secondPoly_parm = {{4,0}, {0,0}};
	polynomial secondPoly(secondPoly_parm.begin(), secondPoly_parm.end());
	cout << "before multi: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "secondPoly.print(): ";
	secondPoly.print();

	polynomial sumPoly = linearPoly * secondPoly;
	cout<< "after multi\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "mulPoly.print(): "; //expect 1x^9+3x^5+2x^7+6x^3+x^4+3
	sumPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = sumPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";

	//assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_poly_int_time() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{3,2}, {5,1}, {0,1}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());
	int secondint_parm = 3;
	cout << "before multi: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "secondint_parm.print(): ";
	printf("int: %d\n", secondint_parm);

	polynomial mulPoly = linearPoly * secondint_parm;
	cout<< "after multi\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "mulPoly.print(): "; //expect 1x^9+3x^5+2x^7+6x^3+x^4+3
	mulPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = mulPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";

	//assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_poly_int_moldulus() {
	using namespace std;
	//vector<pair<power,coeff>> param_vec = {{2,1}, {1,2}, {0,1}};
	vector<pair<power,coeff>> param_vec = {{1,1}, {0,2}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());
	//vector<pair<power,coeff>>secondPoly_parm = {{1,1}, {0,1}};
	vector<pair<power,coeff>>secondPoly_parm = {{1,1}, {0,1}};
	polynomial secondPoly(secondPoly_parm.begin(), secondPoly_parm.end());
	cout << "before multi: \n";
	cout << "linearPoly.print(): ";
	linearPoly.print();
	cout<< "secondPoly.print(): ";
	secondPoly.print();

	polynomial moldulusPoly = linearPoly % secondPoly;
	cout<< "after moldu\n";
	cout << "linear_poly.print(): ";
	linearPoly.print();
	cout<< "moldulusPoly.print(): "; //expect 1x^9+3x^5+2x^7+6x^3+x^4+3
	moldulusPoly.print();

	//canonical_form
	vector<pair<power,coeff>> canonical_form_vec = moldulusPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";
	
	//assert(sumPoly.find_degree_of() == linearPoly.find_degree_of());
	log_end();
}

void test_canonical_form() {
	using namespace std;
	vector<pair<power,coeff>> param_vec = {{0,0}};
	polynomial linearPoly(param_vec.begin(), param_vec.end());
	vector<pair<power,coeff>> canonical_form_vec = linearPoly.canonical_form();
	cout << "after canonical form ";
	for(auto vec_iter: canonical_form_vec) {
		cout << "{first = " << vec_iter.first << " second = " << vec_iter.second << "} ";
	}
	cout << "\n";
	
	log_end();
}








void test_linear_mod_linear() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{1,1}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1}, {0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "remainder = p1 \% p2: \n";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_linear_mod_linear_diff_coeff() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{1,2}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1}, {0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "remainder = p1 \% p2: \n";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_quad_mod_linear() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{2,1}, {1,1}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1}, {0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: 3 + 1\n";
	cout << "computed remainder: \n";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_quad_skip_linear_mod_linear() {
	using namespace std;
	vector<pair<power, coeff>> param1 = {{2,4}, {0, 1}};
	vector<pair<power, coeff>> param2 = {{1, 1}, {0,-3}};
	polynomial p1(param1.begin(), param1.end());
	polynomial p2(param2.begin(), param2.end());

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: 37 \n";
	cout << "computed remainder: ";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_quad_mod_linear_diff_coeff() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{2,3}, {1,1}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1}, {0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: 11\n";
	cout << "computed remainder: ";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_third_power_mod_linear() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{3,1}, {2,1}, {1,1}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1}, {0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: -5\n";
	cout << "computed remainder: ";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

void test_mod_constant_poly() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{3,1}, {2,1}, {1,1}, {0,1}};
	vector<pair<power, coeff>> params2 = {{0,2}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: 0\n";
	cout << "computed remainder: ";
	remainder.print();
	
	assert(remainder.find_degree_of() == 0);
	log_end();
}

static bool compare_vec(std::vector<std::pair<power,coeff>> const &v1, std::vector<std::pair<power,coeff>> const &v2) {
	//invoke this function to check if 2 polynomial's canonical forms are identical
	if(v1.size() != v2.size()) {
		return false;
	}
	else {
		bool result = true;
		for(size_t idx = 0; idx < v1.size() && result == true; idx++) {
			result = v1.at(idx) == v2.at(idx);
		}
		return result;
	}
}

void test_remainder_zero() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{2,1}, {1,2}, {0,1}};
	vector<pair<power, coeff>> params2 = {{1,1},{0,1}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: 0\n";
	cout << "computed remainder: ";
	remainder.print();
	assert(remainder.find_degree_of() == 0);
	assert(compare_vec(remainder.canonical_form(), vector<pair<power,coeff>>(1, {0,0})) == true);
	log_end();
}

void test_mod_same_power() {
	using namespace std;
	vector<pair<power, coeff>> params1 = {{2,5}, {1,2}, {0,1}};
	vector<pair<power, coeff>> params2 = {{2,1}, {1,2},{0,1}};
	polynomial p1(params1.begin(), params1.end()); 
	polynomial p2(params2.begin(), params2.end()); 

	cout << "before modulus: \n";
	cout << "p1: ";
	p1.print();
	cout << "p2: ";
	p2.print();

	polynomial remainder = p1 % p2;

	cout << "after modulus: \n";
	cout << "expected: -8*x^1 - 4\n";
	cout << "computed remainder: ";
	remainder.print();
	assert(remainder.find_degree_of() == 1);
	log_end();
}

int main(int argc, char* argv[]) {
	test_zero_polynomial();
	test_init_linear_polynomial();
	test_init_quadratic_polynomial();
	test_copy_linear_polynomial();
	test_equal_op();
	test_sparse_quad_poly();
	test_poly_add_poly();
	test_poly_add_poly2();
	test_poly_int_add();
	test_poly_poly_time();
	test_poly_int_time();
	test_poly_int_moldulus();
	test_canonical_form();


	/*
	test_linear_mod_linear();
	test_linear_mod_linear_diff_coeff();
	test_quad_mod_linear();
	test_quad_skip_linear_mod_linear();
	test_quad_mod_linear_diff_coeff();
	test_third_power_mod_linear();
	test_mod_constant_poly();
	test_remainder_zero();
	test_mod_same_power();
	*/

	//big data
	std::ifstream file("simple_poly.txt");
	std::string monomial;
    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

	/*
	while(input >> monomial)
	{
		//monomial.push_back(monomial);
	}

    file.close();

	*/
	//example();
	return EXIT_SUCCESS;
}
