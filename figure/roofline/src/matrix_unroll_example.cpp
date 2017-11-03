#include <cassert>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std::chrono;
using TP= time_point<steady_clock>;

template <typename Value>
class matrix
{
public:
    using value_type= Value;

    matrix() : nrows(0), ncols(0), data(0) {}

    matrix(int nrows, int ncols)
        : nrows(nrows), ncols(ncols), data( new value_type[nrows * ncols] ) {}

    matrix(const matrix& that)
        : nrows(that.nrows), ncols(that.ncols), data(new value_type[nrows * ncols])
        {
            for (int i= 0, size= nrows*ncols; i < size; ++i)
                data[i]= that.data[i];
        }

    ~matrix() { if(data) delete [] data; }

    void operator=(const matrix& that)
        {
            #ifndef NO_CHECKS
            assert(nrows == that.nrows && ncols == that.ncols);
            #endif
            for (int i= 0, size= nrows*ncols; i < size; ++i)
                data[i]= that.data[i];
        }

    int num_rows() const { return nrows; }
    int num_cols() const { return ncols; }

private:
    void check(int r, int c) const { assert(0 <= r && r < nrows && 0 <= c && c <= ncols); }
public:
    value_type& operator()(int r, int c)
        {
#ifndef NO_CHECKS
            check(r, c);
#endif
            return data[r*ncols + c];
        }
    const value_type& operator()(int r, int c) const
        {
#ifndef NO_CHECKS
            check(r, c);
#endif
            return data[r*ncols + c];
        }
private:
    int     nrows, ncols;
    value_type* data;
};

template <typename Value>
std::ostream& operator<<(std::ostream& os, const matrix<Value>& A)
{
    for (int r= 0; r < A.num_rows(); r++) {
    os << '[';
    for (int i= 0; i < A.num_cols(); ++i)
        os << A(r, i) << (i+1 < A.num_cols() ? "  " : "");
    os << "]\n";
    }
    return os;
}


template <unsigned BSize, typename Value>
struct multi_tmp
{
    static const unsigned bs= BSize;
    using sub_type= multi_tmp<BSize-1, Value>;

    multi_tmp(const Value& v) : value(v), sub(v) {}
    Value     value;
    sub_type  sub;
};

template <typename Value>
struct multi_tmp<0, Value>
{
    multi_tmp(const Value& v) {}
};


template <unsigned Index0, unsigned Max0, unsigned Index1, unsigned Max1>
struct mult_block
{
    using next= mult_block<Index0, Max0, Index1+1, Max1>;

    template <typename Tmp, typename Matrix>
    void operator()(Tmp& tmp, const Matrix& A, const Matrix& B, unsigned i, unsigned j, unsigned k)
    {
    // std::cout << "tmp." << tmp.bs << "+= A[" << i + Index0 << "][" << j << "] * B[" << j << "][" << k + Index1 << "]\n";
    tmp.value+= A(i + Index0, j) * B(j, k + Index1);
    next()(tmp.sub, A, B, i, j, k);
    }

    template <typename Tmp, typename Matrix>
    void update(const Tmp& tmp, Matrix& C, unsigned i, unsigned k)
    {
    // std::cout << "C[" << i + Index0 << "][" << k + Index1 << "]= tmp." << tmp.bs << "\n";
    C(i + Index0, k + Index1)= tmp.value;
    next().update(tmp.sub, C, i, k);
    }
};

template <unsigned Index0, unsigned Max0, unsigned Max1>
struct mult_block<Index0, Max0, Max1, Max1>
{
    using next= mult_block<Index0+1, Max0, 0, Max1>;

    template <typename Tmp, typename Matrix>
    void operator()(Tmp& tmp, const Matrix& A, const Matrix& B, unsigned i, unsigned j, unsigned k)
    {
    // std::cout << "tmp." << tmp.bs << "+= A[" << i + Index0 << "][" << j << "] * B[" << j << "][" << k + Max1 << "]\n";
    tmp.value+= A(i + Index0, j) * B(j, k + Max1);
    next()(tmp.sub, A, B, i, j, k);
    }

    template <typename Tmp, typename Matrix>
    void update(const Tmp& tmp, Matrix& C, unsigned i, unsigned k)
    {
    // std::cout << "C[" << i + Index0 << "][" << k + Max1 << "]= tmp." << tmp.bs << "\n";
    C(i + Index0, k + Max1)= tmp.value;
    next().update(tmp.sub, C, i, k);
    }
};

#if 0
template <unsigned Index0, unsigned Max0, unsigned Index1, unsigned Max1>
struct loop2
{
    static const unsigned next_index0= Index0, next_index1= Index1 + 1;
};

template <unsigned Index0, unsigned Max0, unsigned Max1>
struct loop2<Index0, Max0, Max1, Max1>
{
    static const unsigned next_index0= Index0 + 1, next_index1= 0;
};


template <unsigned Index0, unsigned Max0, unsigned Index1, unsigned Max1>
struct mult_block
{
    using l= loop2<Index0, Max0, Index1, Max1>;
    using next= mult_block<l::next_index0, Max0, l::next_index1, Max1>;

    template <typename Tmp, typename Matrix>
    void operator()(Tmp& tmp, const Matrix& A, const Matrix& B, unsigned i, unsigned j, unsigned k)
    {
    // std::cout << "tmp." << tmp.bs << "+= A[" << i + Index0 << "][" << j << "] * B[" << j << "][" << k + Index1 << "]\n";
    tmp.value+= A(i + Index0, j) * B(j, k + Index1);
    next()(tmp.sub, A, B, i, j, k);
    }

    template <typename Tmp, typename Matrix>
    void update(const Tmp& tmp, Matrix& C, unsigned i, unsigned k)
    {
    std::cout << "C[" << i + Index0 << "][" << k + Index1 << "]= tmp." << tmp.bs << "\n";
    C(i + Index0, k + Index1)= tmp.value;
    next().update(tmp.sub, C, i, k);
    }
};
#endif

template <unsigned Max0, unsigned Max1>
struct mult_block<Max0, Max0, Max1, Max1>
{
    template <typename Tmp, typename Matrix>
    void operator()(Tmp& tmp, const Matrix& A, const Matrix& B, unsigned i, unsigned j, unsigned k)
    {
    // std::cout << "tmp." << tmp.bs << "+= A[" << i + Max0 << "][" << j << "] * B[" << j << "][" << k + Max1 << "]\n";
    tmp.value+= A(i + Max0, j) * B(j, k + Max1);
    }

    template <typename Tmp, typename Matrix>
    void update(const Tmp& tmp, Matrix& C, unsigned i, unsigned k)
    {
    // std::cout << "C[" << i + Max0 << "][" << k + Max1 << "]= tmp." << tmp.bs << "\n";
    C(i + Max0, k + Max1)= tmp.value;
    }
};


template <typename Matrix>
inline void mult(const Matrix& A, const Matrix& B, Matrix& C)
{
    #ifndef NO_CHECKS
    assert(A.num_cols() == B.num_rows()); // ...
    #endif

    using value_type= typename Matrix::value_type;
    unsigned s= A.num_rows();

    for (unsigned i= 0; i < s; i++)
    for (unsigned k= 0; k < s; k++) {
        value_type tmp(0);
        for (unsigned j= 0; j < s; j++)
        tmp+= A(i, j) * B(j, k);
        C(i, k)= tmp;
    }
}

template <unsigned Size0, unsigned Size1, typename Matrix>
inline void mult(const Matrix& A, const Matrix& B, Matrix& C)
{
#ifndef NO_CHECKS
    assert(A.num_cols() == B.num_rows()); // ...
    assert(A.num_rows() % Size0 == 0);       // we omit cleanup here
    assert(A.num_cols() % Size1 == 0);       // we omit cleanup here
#endif
    using value_type= typename Matrix::value_type;
    unsigned s= A.num_rows();
    mult_block<0, Size0-1, 0, Size1-1> block;

    for (unsigned i= 0; i < s; i+= Size0)
        for (unsigned k= 0; k < s; k+= Size1) {
            multi_tmp<Size0 * Size1, value_type> tmp(value_type(0));
            for (unsigned j= 0; j < s; j++)
                block(tmp, A, B, i, j, k);
            block.update(tmp, C, i, k);
        }
}

inline double duration_s(const TP& from, const TP& to, unsigned rep)
{
    return duration_cast<nanoseconds>((to - from) / rep).count() / 1e9;
}


template <typename Matrix>
void bench(const Matrix& A, const Matrix& B, Matrix& C, const unsigned rep)
{
    TP t1= steady_clock::now();
    asm("#bench loop start");
    for (unsigned j= 0; j < rep; j++)
      mult(A, B, C);
    asm("#bench loop end");
    TP t2= steady_clock::now();
    double t= duration_s(t1, t2, rep);
    unsigned s= A.num_rows();

    std::cout << "Time for "<< std::setw(12) << "mult" << " is "
              << std::setw(2) << 1000000.0 * t << " mmicros.\n";
    if (s < 10)
        std::cout << "C is " << C << '\n';
}

template <unsigned Size0, unsigned Size1, typename Matrix>
void bench_unroll(const Matrix& A, const Matrix& B, Matrix& C, const unsigned rep)
{
    TP t1= steady_clock::now();
    asm("#bench_unroll loop start");
    for (unsigned j= 0; j < rep; j++)
      mult<Size0, Size1>(A, B, C);
    asm("#bench_unroll loop stop");
    TP t2= steady_clock::now();
    double t= duration_s(t1, t2, rep);
    unsigned s= A.num_rows();


    std::stringstream myconfig;
    myconfig << "mult<" << std::setw(2) << Size0 << "," << std::setw(2) << Size1 << ">";

    std::cout << "Time for " << std::setw(12) << myconfig.str().c_str() << " is "
              << std::setw(8) << 1000000.0 * t << " mmicros. \n";

    if (s < 10)
        std::cout << "C is " << C << '\n';
}


int main(int argc, char** argv)
{



    const unsigned s= 256;  // s= 4 for testing and 128 for timing
    matrix<float> A(s, s), B(s, s), C(s, s);
    // matrix<double> A(s, s), B(s, s), C(s, s);

    for (unsigned i= 0; i < s; i++)
        for (unsigned j= 0; j < s; j++) {
            A(i, j)= 100.0 * i + j;
            B(i, j)= 200.0 * i + j;
        }

    const unsigned rep= 100;

    if(argc == 1){
        mult(A, B, C);
        if (s < 10)
            std::cout << "C is " << C << '\n';

        mult<2, 4>(A, B, C);
        if (s < 10)
            std::cout << "C is " << C << '\n';


        bench(A, B, C, rep);

#if 1
        bench_unroll<1, 1>(A, B, C, rep);
        bench_unroll<1, 2>(A, B, C, rep);
        bench_unroll<1, 4>(A, B, C, rep);
        bench_unroll<1, 8>(A, B, C, rep);
        bench_unroll<1, 16>(A, B, C, rep);
        bench_unroll<2, 1>(A, B, C, rep);
        bench_unroll<2, 2>(A, B, C, rep);
        bench_unroll<2, 4>(A, B, C, rep);
        bench_unroll<2, 8>(A, B, C, rep);
        bench_unroll<4, 1>(A, B, C, rep);
        bench_unroll<4, 4>(A, B, C, rep);
        bench_unroll<8, 1>(A, B, C, rep);
        bench_unroll<16, 1>(A, B, C, rep);

#endif
    }


    if(argc > 1){
        int first = 0, second = 0;
        first = std::stoi(argv[1]);
        if(argc > 2)
            second = std::stoi(argv[2]);

        if(first == 0 || second == 0){
            bench(A, B, C, rep);
            return 0;
        }

        int encoded = 100*first + second;

        switch(encoded) {
        case 101:
            bench_unroll<1, 1>(A, B, C, rep);
            break;
        case 102:
            bench_unroll<1, 2>(A, B, C, rep);
            break;
        case 104:
            bench_unroll<1, 4>(A, B, C, rep);
            break;
        case 108:
            bench_unroll<1, 8>(A, B, C, rep);
            break;
        case 116:
            bench_unroll<1, 16>(A, B, C, rep);
            break;
        case 201:
            bench_unroll<2, 1>(A, B, C, rep);
            break;
        case 202:
            bench_unroll<2, 2>(A, B, C, rep);
            break;
        case 204:
            bench_unroll<2, 4>(A, B, C, rep);
            break;
        case 208:
            bench_unroll<2, 8>(A, B, C, rep);
            break;
        case 401:
            bench_unroll<4, 1>(A, B, C, rep);
            break;
        case 404:
            bench_unroll<4, 4>(A, B, C, rep);
            break;
        case 801:
            bench_unroll<8, 1>(A, B, C, rep);
            break;
        case 804:
            bench_unroll<8, 4>(A, B, C, rep);
            break;
        case 808:
            bench_unroll<8, 8>(A, B, C, rep);
            break;
        case 1601:
            bench_unroll<16, 1>(A, B, C, rep);
            break;
        default:
            std::cerr << "unknown args received: " << first << " " << second << "\n";
            break;
        };

    }

    return 0;
}
