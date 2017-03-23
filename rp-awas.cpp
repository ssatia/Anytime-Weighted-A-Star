#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <queue>
#include <vector>
using namespace std;

const double H_WEIGHT = 3.5;

vector<vector<int> > grid;
pair<int, int> goal;

struct state {
  double g, h, w_h;
  pair<int, int> pos;
  vector<pair<int, int> > steps;

  state() {
    g = h = pos.first = pos.second = 0;
  }

  state(pair<int, int> loc) {
    pos = loc;
    g = 0;
    steps.push_back(pos);
    update_heuristic();
  }

  void update_heuristic() {
    h = abs(goal.first - pos.first) + abs(goal.second - pos.second);
    w_h = h * H_WEIGHT;
  }

  bool is_goal() {
    return pos == goal;
  }

  void print() {
    int i, j;
    /*
    printf("pos: %d %d\ng cost: %lf\nh cost: %lf\n", pos.first, pos.second, g, h);

    printf("steps taken\n");
    */

    printf("%lu\n", steps.size() - 1);
    /*
    for(i = 0; i < steps.size(); ++i) {
      printf("%d %d\n", steps[i].first, steps[i].second);
    }
    */
    // printf("\n");
  }

  bool operator<(const state& other) const {
    return g + h > other.g + other.h;
  }
};

int main() {
  int i, j, n, m, iter, t;
  clock_t time_start, time_stop;
  double duration;
  const int ITER_MAX = 1e8;
  state best, tmp;
  pair<int, int> start_pos(0, 0), new_pos;

  // freopen("pm1.in", "r", stdin);
  // freopen("pm1.out", "w", stdout);

  // 0 - UP; 1 - RIGHT; 2 - DOWN; 3 - LEFT
  int x_i[] = {0, 1, 0, -1};
  int y_i[] = {-1, 0, 1, 0};

  scanf("%d %d", &n, &m);

  vector<vector<int> > g_cost(n, vector<int> (m, -1));

  for(i = 0; i < n; ++i) {
    vector<int> tmp;
    for(j = 0; j < m; ++j) {
      scanf("%d", &t);
      tmp.push_back(t);
    }
    grid.push_back(tmp);
  }

  goal = make_pair(n - 1, m - 1);

  time_start = clock();

  priority_queue<state> pq;
  state start(start_pos);
  g_cost[start_pos.first][start_pos.second] = 0;
  pq.push(start);

  best.g = INT_MAX;
  iter = 0;
  while(!pq.empty() && iter < ITER_MAX) {
    state cur = pq.top();
    pq.pop();
    // cur.print();

    // Skip sub-optimal nodes
    if(tmp.g + tmp.h >= best.g) {
      continue;
    }

    for(i = 0; i < 4; ++i) {
      tmp = cur;
      new_pos = tmp.pos;
      new_pos.first += y_i[i];
      new_pos.second += x_i[i];

      if(new_pos.first < 0 ||
        new_pos.second < 0 ||
        new_pos.first >= n ||
        new_pos.second >= m) {
          continue;
      }

      tmp.pos = new_pos;
      tmp.g += grid[tmp.pos.first][tmp.pos.second];
      tmp.update_heuristic();
      tmp.steps.push_back(tmp.pos);

      if(tmp.is_goal()) {
        best = tmp;
        continue;
      }

      if(
        (
          g_cost[tmp.pos.first][tmp.pos.second] == -1 ||
          g_cost[tmp.pos.first][tmp.pos.second] > tmp.g
        ) &&
        tmp.g + tmp.h < best.g) {
          g_cost[tmp.pos.first][tmp.pos.second] = tmp.g;
          pq.push(tmp);
      }
    }

    ++iter;
  }

  time_stop = clock();
  duration = double(time_stop - time_start) / CLOCKS_PER_SEC;

  best.print();

  printf("Time taken: %f\n", duration);

  return 0;
}
