#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

#define MX 10

int main() {
  int i, j, n, m;
  time_t t;
  srand((unsigned) time(&t));

  scanf("%d %d", &n, &m);
  printf("%d %d\n", n, m);
  for(i = 0; i < n; ++i) {
    for(j = 0; j < m; ++j) {
      printf("%d ", (rand() % MX) + 1);
    }
    printf("\n");
  }

  return 0;
}
