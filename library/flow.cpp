struct MAX_FLOW {
    struct edge {
        int to, cap, rev;

        edge(int to, int cap, int rev) : to(to), cap(cap), rev(rev) {}
    };

    vector<vector<edge>> G;
    vector<bool> used;
    int sz;

    MAX_FLOW(int size) {
        sz = size;
        G.resize(sz);
        used.resize(sz);
    }

    void add_edge(int from, int to, int cap) {
        G[from].emplace_back(to, cap, G[to].size());
        G[to].emplace_back(from, 0, G[from].size() - 1);
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        used[v] = true;
        for (auto& e : G[v]) {
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
        while (1) {
            used.clear();
            used.resize(sz);
            int f = dfs(s, t, INF);
            if (f == 0) return flow;
            flow += f;
        }
    }
};

