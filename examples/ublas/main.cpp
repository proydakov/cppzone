/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
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
