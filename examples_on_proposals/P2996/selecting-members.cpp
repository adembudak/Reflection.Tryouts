
struct S {
  unsigned i : 2, j : 6;
};

consteval auto member_number(int n) {
  if(n == 0) return ^^S::i;
  else if(n == 1) return ^^S::j;
}

int main() {
  S s{0, 0};
  s.[:member_number(1):] = 42;
  // s.[:member_number(5):] = 0;
}
