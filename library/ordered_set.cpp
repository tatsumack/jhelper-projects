using key_type = int;
const key_type INF = 1e9;
const int MAX_N = 500010;  // 挿入の回数 + EPS

struct node {
    key_type key;
    int priority;
    node *left, *right;
    int size;
    key_type min;

    static node *const nil;

    node() : node(key_type()) {}

    node(const key_type &x) : node(x, xor128(), nil, nil, 1, x) {}

    node(const key_type &key_, int priority_, node *left_, node *right_, int size_,
         const key_type &min_)
            : key(key_), priority(priority_), left(left_), right(right_), size(size_), min(min_) {}

    void *operator new(std::size_t) {
        static node pool[MAX_N];
        static int p = 0;
        return pool + p++;
    }

    static int32_t xor128() {
        static uint32_t x = 123456789, y = 362436069, z = 521288629, w = time(0);
        uint32_t t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w & 0x3FFFFFFF;
    }
};

using np = node * ;
using cnp = const node *;
using npair = std::pair<np, np>;

node *const node::nil = new node(key_type(), -1, nullptr, nullptr, 0, INF);

class treap {
protected:
    np root;

public:
    treap() : root(node::nil) {}

    treap(np root_) : root(root_) {}

    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2099535#1
    treap(key_type *left, key_type *right) : root(__make_tree(left, right)) {
        int n = right - left;
        std::vector<int> ps(n);
        std::generate(ps.begin(), ps.end(), node::xor128);
        std::sort(ps.begin(), ps.end());
        std::queue<np> que;
        que.push(root);
        while (que.size()) {
            np n = que.front();
            que.pop();
            if (n == node::nil) continue;
            ps.pop_back();
            n->priority = ps.back();
            que.push(n->left);
            que.push(n->right);
        }
    }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2099535#1
    np __make_tree(key_type *left, key_type *right) {
        int sz = right - left;
        if (sz == 0) return node::nil;
        key_type *mid = left + sz / 2;
        np lc = __make_tree(left, mid);
        np rc = __make_tree(mid + 1, right);
        return new node(*mid, -1, lc, rc, sz, std::min({ lc->min, *mid, rc->min }));
    }

public:
    int size() const { return root->size; }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void set(int k, const key_type &x) { __set(root, k, x); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void __set(np n, int k, const key_type &x) {
        if (k < n->left->size)
            __set(n->left, k, x);
        else if (n->left->size == k)
            n->key = x;
        else
            __set(n->right, k - n->left->size - 1, x);
        __update(n);
    }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    key_type get(int k) const { return __get(root, k); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    key_type __get(np n, int k) const {
        if (k < n->left->size)
            return __get(n->left, k);
        else if (n->left->size == k)
            return n->key;
        else
            return __get(n->right, k - n->left->size - 1);
    }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    key_type range_min(int l, int r) { return __range_min(root, l, r); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    key_type __range_min(cnp n, int l, int r) const {
        l = std::max<int>(l, 0);
        r = std::min(r, n->size);
        if (l >= r) return INF;
        if (l == 0 && r == n->size) return n->min;
        key_type res = INF;
        int sl = n->left->size;
        res = std::min(res, __range_min(n->left, l, r));
        res = std::min(res, __range_min(n->right, l - sl - 1, r - sl - 1));
        if (l <= sl && sl < r) res = std::min(res, n->key);
        return res;
    }

public:
    void merge_from_left(treap &t) { root = __merge(t.root, root); }

    void merge_from_right(treap &t) { root = __merge(root, t.root); }

protected:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    np __merge(np l, np r) const {
        if (l == node::nil) return r;
        if (r == node::nil) return l;
        if (l->priority > r->priority) {
            l->right = __merge(l->right, r);
            return __update(l);
        }
        else {
            r->left = __merge(l, r->left);
            return __update(r);
        }
    }

public:
    // http://codeforces.com/contest/830/submission/28560572
    std::pair<treap, treap> split_min() {
        np l, r;
        std::tie(l, r) = __split_min(root, root->min);
        return std::make_pair(treap(l), treap(r));
    }

protected:
    // http://codeforces.com/contest/830/submission/28560572
    npair __split_min(np n, const key_type &min) const {
        if (n == node::nil) return { node::nil, node::nil };
        if (n->left->min == min || n->key == min) {
            npair p = __split_min(n->left, min);
            n->left = p.second;
            return npair(p.first, __update(n));
        }
        else {
            npair p = __split_min(n->right, min);
            n->right = p.first;
            return npair(__update(n), p.second);
        }
    }

public:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    std::pair<treap, treap> split_at(int k) {
        np l, r;
        std::tie(l, r) = __split_at(root, k);
        return std::make_pair(treap(l), treap(r));
    }

protected:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    npair __split_at(np n, int k) const {
        if (n == node::nil) return { node::nil, node::nil };
        if (k <= n->left->size) {
            npair p = __split_at(n->left, k);
            n->left = p.second;
            return npair(p.first, __update(n));
        }
        else {
            npair p = __split_at(n->right, k - n->left->size - 1);
            n->right = p.first;
            return npair(__update(n), p.second);
        }
    }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void insert_at(const key_type &x, int k) { root = __insert_at(root, x, k); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    np __insert_at(np n, const key_type &x, int k) const {
        np l, r;
        std::tie(l, r) = __split_at(n, k);
        return __merge(__merge(l, new node(x)), r);
    }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void erase_at(int k) { root = __erase_at(root, k); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    np __erase_at(np n, int k) const {
        np l, r, m;
        std::tie(l, r) = __split_at(n, k);
        std::tie(m, r) = __split_at(r, 1);
        return __merge(l, r);
    }

public:
    const key_type &front() const {
        assert(root != node::nil);
        return __front(root)->key;
    }

protected:
    cnp __front(cnp n) const { return n->left != node::nil ? __front(n->left) : n; }

public:
    const key_type &back() const {
        assert(root != node::nil);
        return __back(root)->key;
    }

protected:
    cnp __back(cnp n) const { return n->right != node::nil ? __back(n->right) : n; }

public:
    key_type front() {
        assert(root != node::nil);
        return __front(root)->key;
    }

protected:
    np __front(np n) const { return n->left != node::nil ? __front(n->left) : n; }

public:
    key_type back() {
        assert(root != node::nil);
        return __back(root)->key;
    }

protected:
    np __back(np n) const { return n->right != node::nil ? __back(n->right) : n; }

public:
    void push_front(const key_type &x) {
        root = __merge(new node(x, node::xor128(), node::nil, node::nil, 1, x), root);
    }

    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void push_back(const key_type &x) {
        root = __merge(root, new node(x, node::xor128(), node::nil, node::nil, 1, x));
    }

    void pop_front() {
        assert(root->size != 0);
        root = __split_at(root, 1).second;
    }

    void pop_back() {
        assert(root->size != 0);
        root = __split_at(root, root->size - 1).first;
    }

protected:
    np __update(np n) const {
        n->size = 1 + n->left->size + n->right->size;
        n->min = std::min({ n->left->min, n->key, n->right->min });
        return n;
    }

public:
    std::string to_string() const {
        std::string res;
        __to_string(root, res);
        return res;
    }

protected:
    void __to_string(cnp n, std::string &res) const {
        if (n != node::nil) {
            res += "(";
            __to_string(n->left, res);
            res += ")" + std::to_string(n->key) + "(";
            __to_string(n->right, res);
            res += ")";
        }
    }
};

class ordered_set : public treap {
public:
    ordered_set() : treap() {}
    ordered_set(node *n) : treap(n) {}

public:
    std::pair<ordered_set, ordered_set> split_at(int k) {
        np l, r;
        std::tie(l, r) = __split_at(root, k);
        return std::make_pair(ordered_set(l), ordered_set(r));
    }

public:
    std::pair<ordered_set, ordered_set> split_less(const key_type &x) {
        np l, r;
        std::tie(l, r) = __split_less(root, x);
        return std::make_pair(ordered_set(l), ordered_set(r));
    }

protected:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    npair __split_less(np n, const key_type &x) const {
        if (n == node::nil) return npair(node::nil, node::nil);
        if (n->key < x) {
            npair p = __split_less(n->right, x);
            n->right = p.first;
            return npair(__update(n), p.second);
        }
        else {
            npair p = __split_less(n->left, x);
            n->left = p.second;
            return npair(p.first, __update(n));
        }
    }

public:
    std::pair<ordered_set, ordered_set> split_leq(const key_type &x) {
        np l, r;
        std::tie(l, r) = __split_leq(root, x);
        return std::make_pair(ordered_set(l), ordered_set(r));
    }

protected:
    npair __split_leq(np n, const key_type &x) const {
        if (n == node::nil) return npair(node::nil, node::nil);
        if (n->key <= x) {
            npair p = __split_leq(n->right, x);
            n->right = p.first;
            return npair(__update(n), p.second);
        }
        else {
            npair p = __split_leq(n->left, x);
            n->left = p.second;
            return npair(p.first, __update(n));
        }
    }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    void insert(const key_type &x) { root = __insert(root, x); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    np __insert(np n, const key_type &x) const {
        np l, r;
        std::tie(l, r) = __split_less(n, x);
        return __merge(__merge(l, new node(x)), r);
    }

public:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    void erase_one(const key_type &x) {
        assert(includes(x));
        root = __erase_one(root, x);
    }

protected:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    np __erase_one(np n, const key_type &x) const {
        np l, m, r;
        std::tie(l, r) = __split_less(n, x);
        std::tie(m, r) = __split_at(r, 1);
        return __merge(l, r);
    }

public:
    void erase_all(const key_type &x) { root = __erase_one(root, x); }

protected:
    np __erase_all(np n, const key_type &x) const {
        np l, m, r;
        std::tie(l, r) = __split_less(n, x);
        std::tie(m, r) = __split_leq(r, x);
        return __merge(l, r);
    }

public:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    int count_less(const key_type &x) const { return __count_less(root, x); }

protected:
    // yukicoder 449 http://yukicoder.me/submissions/131855
    int __count_less(cnp n, const key_type &x) const {
        if (n == node::nil)
            return 0;
        else if (n->key < x)
            return n->left->size + 1 + __count_less(n->right, x);
        else
            return __count_less(n->left, x);
    }

public:
    int count_leq(const key_type &x) const { return __count_leq(root, x); }

protected:
    int __count_leq(cnp n, const key_type &x) const {
        if (n == node::nil)
            return 0;
        else if (n->key <= x)
            return n->left->size + 1 + __count_leq(n->right, x);
        else
            return __count_leq(n->left, x);
    }

public:
    int count(const key_type &x) const { return count_leq(x) - count_less(x); }

public:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    bool includes(const key_type &x) const { return __includes(root, x); }

protected:
    // AOJ 1508 http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2083288
    bool __includes(np n, const key_type &x) const {
        if (n == node::nil)
            return false;
        else if (n->key < x)
            return __includes(n->right, x);
        else if (n->key == x)
            return true;
        else
            return __includes(n->left, x);
    }

public:
    key_type lower_bound(const key_type &x) const { return __lower_bound(root, x)->key; }

protected:
    np __lower_bound(np n, const key_type &x) const {
        if (n == node::nil)
            return node::nil;
        else if (n->key <= x) {
            np res = __lower_bound(n->left, x);
            return res != node::nil ? res : n;
        }
        else
            return __lower_bound(n->right, x);
    }

public:
    key_type upper_bound(const key_type &x) const { return __upper_bound(root, x)->key; }

protected:
    np __upper_bound(np n, const key_type &x) const {
        if (n == node::nil)
            return node::nil;
        else if (n->key < x) {
            np res = __upper_bound(n->left, x);
            return res != node::nil ? res : n;
        }
        else
            return __upper_bound(n->right, x);
    }
};
// http://tubo28.me/algorithm/treap/

