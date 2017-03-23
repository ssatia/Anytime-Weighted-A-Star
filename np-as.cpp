#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <queue>
#include <vector>
using namespace std;

struct timespec start, stop;

struct state {
  double g, h;
  pair<int, int> slide;
  vector<short> steps;
  vector<vector<int> > config;

  state() {
    g = h = slide.first = slide.second = 0;
  }

  state(vector<vector<int> > state_rep) {
    bool found = false;
    g = 0;
    update_heuristic();
    config = state_rep;

    for(int i = 0; i < state_rep.size(); ++i) {
      for(int j = 0; j < state_rep[i].size(); ++j) {
        if(state_rep[i][j] == 0) {
          slide.first = i;
          slide.second = j;
          found = true;
        }
      }
      if(found) break;
    }
  }

  void update_heuristic() {
    int i, j, n, k = config.size();
    pair<int, int> should;

    h = 0;
    for(i = 0; i < k; ++i) {
      for(j = 0; j < k; ++j) {
        n = config[i][j];
        if(!n) continue;
        should.first = n / k;
        should.second = n % k;
        h += abs(i - should.first) + abs(j - should.second);
      }
    }
  }

  bool is_goal() {
    int i, j, count = 1;
    bool goal = 1;

    for(i = 0; i < config.size(); ++i) {
      for(j = 0; j < config[i].size(); ++j) {
        if(config[i][j] != count) {
          goal = 0;
          break;
        }
        count = (count + 1) % 9;
      }
      if(!goal) break;
    }

    return goal;
  }

  void print() {
    int i, j;
    /*
    printf("state rep:\n");
    for(i = 0; i < config.size(); ++i) {
      for(j = 0; j < config[i].size(); ++j) {
        printf("%d ", config[i][j]);
      }
      printf("\n");
    }

    printf("slide: %d %d\ng cost: %lf\nh cost: %lf\n", slide.first, slide.second, g, h);

    printf("steps taken\n");
    */
    printf("%lu\n", steps.size());
    /*
    for(i = 0; i < steps.size(); ++i) {
      switch(steps[i]) {
        case 0: printf("UP\n"); break;
        case 1: printf("RIGHT\n"); break;
        case 2: printf("DOWN\n"); break;
        case 3: printf("LEFT\n");
      }
    }
    */
    // printf("\n");
  }

  bool operator<(const state& other) const {
    return g + h > other.g + other.h;
  }
};

int main() {
  int i, j, k, t, iter;
  clock_t time_start, time_stop;
  double duration;
  state best, tmp;
  pair<int, int> new_slide;
  vector<vector<int> > start_state;
  map<vector<vector<int> >, bool> vis;
  bool found = false;

  freopen("test/puzzle3x3-30.txt", "r", stdin);
  freopen("test/puzzle3x3-30_as_out.txt", "w", stdout);

  // 0 - UP; 1 - RIGHT; 2 - DOWN; 3 - LEFT
  int x_i[] = {0, 1, 0, -1};
  int y_i[] = {-1, 0, 1, 0};

  scanf("%d", &k);

  for(i = 0; i < k; ++i) {
    vector<int> tmp;
    for(j = 0; j < k; ++j) {
      scanf("%d", &t);
      tmp.push_back(t);
    }
    start_state.push_back(tmp);
  }

  time_start = clock();

  state start(start_state);
  vis[start_state] = 1;
  priority_queue<state> pq;
  pq.push(start);

  best.g = INT_MAX;
  iter = 0;
  while(!pq.empty() && !found) {
    state cur = pq.top();
    pq.pop();
    vis[cur.config] = 1;
    // cur.print();

    for(i = 0; i < 4; ++i) {
      tmp = cur;
      new_slide = tmp.slide;
      new_slide.first += y_i[i];
      new_slide.second += x_i[i];

      if(new_slide.first < 0 ||
        new_slide.second < 0 ||
        new_slide.first >= k ||
        new_slide.second >= k) {
        continue;
      }

      swap(tmp.config[tmp.slide.first][tmp.slide.second], tmp.config[new_slide.first][new_slide.second]);

      if(vis[tmp.config]) {
        continue;
      }

      tmp.slide = new_slide;
      ++tmp.g;
      tmp.update_heuristic();
      tmp.steps.push_back(i);

      if(tmp.is_goal()) {
        best = tmp;
        found = true;
        break;
      }

      pq.push(tmp);
    }

    ++iter;
  }

  time_stop = clock();
  duration = double(time_stop - time_start) / CLOCKS_PER_SEC;

  best.print();

  printf("Time taken: %f\n", duration);

  return 0;
}
