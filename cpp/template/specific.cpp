#include <iostream>
#include <vector>

// 通用矩阵类模板
template <typename T, size_t Rows, size_t Cols>
class Matrix {
private:
    std::vector<T> data;
public:
    Matrix() : data(Rows * Cols) {}

    // 访问矩阵元素
    T& operator()(size_t i, size_t j) {
        return data[i * Cols + j];
    }

    const T& operator()(size_t i, size_t j) const {
        return data[i * Cols + j];
    }

    // 矩阵加法
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    // 打印矩阵
    void print() const {
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 全特化：布尔矩阵（逻辑或代替加法）
template <size_t Rows, size_t Cols>
class Matrix<bool, Rows, Cols> {
private:
    std::vector<bool> data;
public:
    Matrix() : data(Rows * Cols) {}

    bool& operator()(size_t i, size_t j) {
        return data[i * Cols + j];
    }

    const bool& operator()(size_t i, size_t j) const {
        return data[i * Cols + j];
    }

    // 布尔矩阵逻辑或
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                result(i, j) = (*this)(i, j) || other(i, j);
            }
        }
        return result;
    }

    void print() const {
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                std::cout << ((*this)(i, j) ? "1 " : "0 ");
            }
            std::cout << std::endl;
        }
    }
};

// 偏特化：方阵（添加求行列式功能）
template <typename T, size_t N>
class Matrix<T, N, N> {
private:
    std::vector<T> data;
public:
    Matrix() : data(N * N) {}

    T& operator()(size_t i, size_t j) {
        return data[i * N + j];
    }

    const T& operator()(size_t i, size_t j) const {
        return data[i * N + j];
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    // 简单示例：2x2 方阵行列式计算
    T determinant() const {
        if (N == 2) {
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
        }
        return T();
    }

    void print() const {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 同时满足布尔类型和方阵的特化版本
template <size_t N>
class Matrix<bool, N, N> {
private:
    std::vector<bool> data;
public:
    Matrix() : data(N * N) {}

    bool& operator()(size_t i, size_t j) {
        return data[i * N + j];
    }

    const bool& operator()(size_t i, size_t j) const {
        return data[i * N + j];
    }

    // 布尔矩阵逻辑或
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                result(i, j) = (*this)(i, j) || other(i, j);
            }
        }
        return result;
    }

    // 布尔方阵行列式（简单示例，这里可以根据需求实现）
    bool determinant() const {
        if (N == 2) {
            return ((*this)(0, 0) && (*this)(1, 1)) || (!(*this)(0, 0) && !(*this)(1, 1));
        }
        return false;
    }

    void print() const {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                std::cout << ((*this)(i, j) ? "1 " : "0 ");
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // 通用矩阵加法
    Matrix<int, 2, 3> m1;
    Matrix<int, 2, 3> m2;
    m1(0, 0) = 1; m1(0, 1) = 2; m1(0, 2) = 3;
    m2(0, 0) = 4; m2(0, 1) = 5; m2(0, 2) = 6;
    auto m3 = m1 + m2;
    std::cout << "General Matrix Addition:" << std::endl;
    m3.print();

    // 布尔矩阵逻辑或
    Matrix<bool, 2, 2> b1;
    Matrix<bool, 2, 2> b2;
    b1(0, 0) = true; b1(0, 1) = false;
    b2(0, 0) = false; b2(0, 1) = true;
    auto b3 = b1 + b2;
    std::cout << "\nBoolean Matrix OR:" << std::endl;
    b3.print();

    // 布尔方阵行列式计算
    std::cout << "\nBoolean Square Matrix Determinant: " << b3.determinant() << std::endl;

    return 0;
}