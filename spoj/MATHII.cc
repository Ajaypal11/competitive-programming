#include <cassert>
#include <cmath>
#include <cstdio>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>

using uint64 = unsigned long long;
using uint32 = unsigned int;

uint64 sum_sigma(uint64 n) {
  auto out = [n] (uint64 x, uint32 y) {
    return x * y > n;
  };
  auto cut = [n] (uint64 x, uint32 dx, uint32 dy) {
    return x * x * dy >= n * dx;
  };
  const uint64 sn = sqrtl(n);
  const uint64 cn = pow(n, 0.36);//cbrtl(n);
  uint64 x = n / sn;
  uint32 y = n / x + 1;
  std::stack<std::pair<uint32, uint32>> stk;
  stk.emplace(1, 0);
  stk.emplace(1, 1);
  uint64 ret = 0;
  while (true) {
    uint32 lx, ly;
    std::tie(lx, ly) = stk.top();
    stk.pop();
    while (out(x + lx, y - ly)) {
      ret += x * ly + uint64(ly + 1) * (lx - 1) / 2;
      x += lx, y -= ly;
    }
    if (y <= cn) break;
    uint32 rx = lx, ry = ly;
    while (true) {
      std::tie(lx, ly) = stk.top();
      if (out(x + lx, y - ly)) break;
      rx = lx, ry = ly;
      stk.pop();
    }
    while (true) {
      uint32 mx = lx + rx, my = ly + ry;
      if (out(x + mx, y - my)) {
        stk.emplace(lx = mx, ly = my);
      } else {
        if (cut(x + mx, lx, ly)) break;
        rx = mx, ry = my;
      }
    }
  }
  for (--y; y > 0; --y) ret += n / y;
  return ret;
}

uint64 sum_tau3(uint64 n) {
  uint64 ret = 0, cn = cbrtl(n);
  for (uint64 i = 1; i <= cn; ++i) {
    ++ret;
    uint64 ni = n / i;
    uint64 uj = sqrt(ni);
    if (uj <= 1e4) {
      for (uint64 j = i + 1; j <= uj; ++j) {
        ret += (ni / j - j) * 6;
      }
    } else {
      ret += 6 * sum_sigma(ni);
      for (uint64 j = 1; j <= i; ++j) ret -= ni / j * 6;
      ret -= (uj - i) * (uj + i + 1) * 3;
    }
    ret += (uj - i) * 3;
    ret += (n / (i * i) - i) * 3;
  }
  return ret;
}

int main() {
  std::vector<uint64> tests;
  for (uint64 n; scanf("%llu", &n) == 1; ) {
    tests.emplace_back(n);
  }
  int cas = 0;
  for (auto &&n: tests) {
    uint64 ret = 0;
    ret = sum_tau3(n);
    printf("Case %d: %llu\n", ++cas, ret);
  }
  return 0;
}
