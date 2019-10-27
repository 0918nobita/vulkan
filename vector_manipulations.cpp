#include <iostream>
#include <vector>
#include <algorithm>

// vector はシーケンスコンテナの一種で、各要素は線形に、順序を保ったまま格納される。

// 可変長配列として実装される。各要素が連続して配置されるため、イテレータだけでなく添字によるアクセスも高速である。

// 配列とは違い、ストレージは vector 自体が管理するため、自動的に領域の拡張が行われる。

// 確保されている要素数を管理しているが、それとは別に「確保済みのメモリサイズを管理するキャパシティ用のメンバ変数」を持ち、
// これは常に `size()` と同じか大きい値となる。
// 確保済みの余計な部分は、要素数の増加に備えて確保しているものだ。

// 領域の再確保が発生すると、全ての要素が新しい領域にコピーされるため、非常にコストがかかる。
// 最終的な要素数が大きくなるとわかっている場合は、あらかじめ `reserve()` メンバ関数でキャパシティを増加させておくことが望ましい。

void print(const char* name, const std::vector<int>& v) {
  std::cout << name << " : {";
  bool first = true;
  for (int x : v) {
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }

    std::cout << x;
  }

  std::cout << "}" << std::endl;
}

int main() {
  std::vector<int> v1 = {1, 2, 3, 4, 5};
  // std::vector::begin 先頭要素を指すイテレータを返す
  v1.erase(v1.begin() + 2);
  print("(1)", v1);

  std::vector<int> v2 = {1, 2, 3, 4, 5};
  v2.erase(v2.begin(), v2.begin() + 2);
  print("(2)", v2);

  std::vector<int> v3 = {1, 2, 3, 4, 5};
  // remove_if によって、条件を満たす要素がすべて前方に寄せられ、その末端の要素の次を指すイテレータが返る
  auto iter = std::remove_if(v3.begin(), v3.end(), [](int x) { return x % 2 == 0; });
  // [iter, v3.end()) を erase することで v3 のサイズを変更できる
  v3.erase(iter, v3.end());
  print("(3)", v3);
}
