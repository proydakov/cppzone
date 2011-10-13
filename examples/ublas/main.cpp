/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    boost::numeric::ublas::matrix<int> matrix(2, 2);
    matrix(0, 0) = 1;
    matrix(0, 1) = 1;
    matrix(1, 0) = 1;
    matrix(1, 1) = 0;
    std::cout << matrix << std::endl;
    
    boost::numeric::ublas::vector<int> vector(2);
    vector[0] = 0;
    vector[1] = 1;
    std::cout << vector << std::endl;
    
    std::cout << prod(matrix, vector) << std::endl;
    
    boost::numeric::ublas::scalar_matrix<int> scalar_matrix(3, 3);
    std::cout << scalar_matrix << std::endl;
    
    boost::numeric::ublas::identity_matrix<int> identity_matrix(3);
    std::cout << identity_matrix << std::endl;
    
    return 0;
}
