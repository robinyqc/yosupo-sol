#include <algorithm>
/**
 * @brief 给定一个凸函数 f 的一些信息，求出一个特定点值。
 * @param g g(k) 返回 max_x {f(x) - kx}。
 * @param a 需要求 f(a)。
 * @param lb 和 ub 一起描述 f 的微分/差分的值域
 * @param ub 和 lb 一起描述 f 的微分/差分的值域。
 * @param eps 精度
 * @param comp 若 f 是凹函数那么 comp = less()，反之为 greater().
 * 
 * @example 对于整数上的二分，调用 wqs_invoke<int>(g, 1, 0, 20)，
 *          可以对凹函数 f 求出 f(1).
 * @example 对于实数上的二分，调用 
 *          wqs_invoke<double>(g, 2.5, 0.0, 1.0, 1e-5, greater())，
 *          可以对凸函数 f 求出 f(2.5).
 */
template<typename T, typename F, typename C = std::less<T>>
T wqs_invoke(F &&g, T a, T lb, T ub, T eps = 0, const C &comp = C{})
{
    if (eps == T(0)) eps = 1;
    else eps /= ub - lb;
    while (lb + eps <= ub) {
        T ml = lb + (ub - lb) / 2;
        T mr = ml + eps;
        T gl = g(T(ml)) + T(ml) * T(a);
        T gr = g(T(mr)) + T(mr) * T(a);
        if (comp(gl, gr)) lb = mr;
        else ub = ml;
    }
    return g(lb) + T(lb) * T(a);
}