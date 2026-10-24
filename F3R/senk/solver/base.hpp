#ifndef SENK_SOLVER_BASE_HPP
#define SENK_SOLVER_BASE_HPP

#include <cstdint>
#include <cstdio>
#include <cstdlib> // atoi を使用するために追加
#include <utility> // std::forward を使うため

namespace senk {

namespace param {

// 反復回数や一括設定パラメータを一元管理する構造体
struct SolverConfig {
  int32_t m1 = 100;           // 最外郭 (Level 1) FGMRES 反復回数
  int32_t m2 = 8;             // 第2段 (Level 2) FGMRES 反復回数
  int32_t m3 = 4;             // 第3段 (Level 3) FGMRES 反復回数
  int32_t m4 = 2;             // 第4段 (Level 4) Adaptive Richardson 反復回数
  int32_t c  = 64;            // Adaptive Richardson のサイクル長
  int32_t max_outer_iter = 300; // リスタート上限の最大反復数

  // コマンドライン引数 (argv) から数値を一括パースして更新するヘルパー関数
  void parse_args(int argc, char *argv[]) {
    if (argc >= 8) {
      m2 = std::atoi(argv[4]);
      m3 = std::atoi(argv[5]);
      m4 = std::atoi(argv[6]);
      c  = std::atoi(argv[7]);
    }
  }
};

struct kls {
  int32_t max_iter;
};

struct kls_normalize {
  int32_t max_iter;
  bool normalized;
};

struct kls_aug_normalize {
  int32_t max_iter, aug;
  bool normalized;
};

struct ss {
  int32_t max_iter;
  double weight;
};

struct ss_cycle {
  int32_t max_iter;
  double weight;
  int32_t cycle;
};

struct r {
  int32_t max_iter;
};

} // namespace param

/*template <template <typename, typename, typename> class S, typename T, class Op,
    class Pre>
auto Solver(
    const Op &op, const Pre &pre, const typename S<T, Op, Pre>::Params &prm) {
  return S<T, Op, Pre>(op, pre, prm);
}

template <template <typename, typename, typename, typename> class S, typename T,
    typename T2, class Op, class Pre>
auto Solver(const Op &op, const Pre &pre,
    const typename S<T, T2, Op, Pre>::Params &prm) {
  return S<T, T2, Op, Pre>(op, pre, prm);
}

template <template <typename, typename, typename, typename> class S, typename T,
    class Op, class Pre>
auto Solver(const Op &op, const Pre &pre,
    const typename S<T, T, Op, Pre>::Params &prm) {
  return S<T, T, Op, Pre>(op, pre, prm);
} */

template <template <typename, typename, typename> class S, typename T, class Op,
    class Pre, typename... Args>
auto Solver(
    const Op &op, const Pre &pre, const typename S<T, Op, Pre>::Params &prm, Args&&... args) {
  return S<T, Op, Pre>(op, pre, prm, std::forward<Args>(args)...);
}

template <template <typename, typename, typename, typename> class S, typename T,
    typename T2, class Op, class Pre, typename... Args>
auto Solver(const Op &op, const Pre &pre,
    const typename S<T, T2, Op, Pre>::Params &prm, Args&&... args) {
  return S<T, T2, Op, Pre>(op, pre, prm, std::forward<Args>(args)...);
}

template <template <typename, typename, typename, typename> class S, typename T,
    class Op, class Pre, typename... Args>
auto Solver(const Op &op, const Pre &pre,
    const typename S<T, T, Op, Pre>::Params &prm, Args&&... args) {
  return S<T, T, Op, Pre>(op, pre, prm, std::forward<Args>(args)...);
}

namespace converg {

inline auto rrn(double norm_b, double tol) {
  return [=]([[maybe_unused]] int i, double e) { return (e < norm_b * tol); };
}

inline auto rrn_log(double norm_b, double tol) {
  return [=]([[maybe_unused]] int i, double e) {
    printf("%d, %e\n", i, e / norm_b);
    return (e < norm_b * tol);
  };
}

} // namespace converg

} // namespace senk

#endif