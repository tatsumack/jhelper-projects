//
// Created by Tatsuhiko Makino on 2019/01/31.
//

struct MaximumFlow {
    struct edge {
        int to;
        int cap;
        int rev;
    };

    int size;
    vector<vector<edge>> G;
    vector<bool> used;

    MaximumFlow(int n) {
        size = n;
        G.resize(n);
        used.resize(n);
    }

    void add_edge(int from, int to, int cap) {
        G[from].push_back((edge){to, cap, static_cast<int>(G[to].size())});
        G[to].push_back((edge) {from, cap, static_cast<int>(G[from].size()) - 1});
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        used[v] = true;
        for (int i = 0; i < G[v].size(); i++) {
            edge& e = G[v][i];
            if (!used[e.to] && e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (true) {
            used.clear();
            used.resize(size);
            int f = dfs(s, t, INF);

            if (f == 0) return flow;
            flow += f;
        }
    }
};

