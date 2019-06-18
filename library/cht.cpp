template<typename T>
class ConvecHullTrick {
private:
    // 直線群(配列)
    std::vector<std::pair<T, T>> lines;
    // 最小値(最大値)を求めるxが単調であるか
    bool isMonotonicX;
    // 最小/最大を判断する関数
    std::function<bool(T l, T r)> comp;

public:
    // コンストラクタ ( クエリが単調であった場合はflag = trueとする )
    ConvexHullTrick(bool flagX = false, std::function<bool(T l, T r)> compFunc = [](T l, T r) {return l >= r; })
            :isMonotonicX(flagX), comp(compFunc)  {
        //lines.emplace_back(0, 0);
    };

    // 直線l1, l2, l3のうちl2が不必要であるかどうか
    bool check(std::pair<T, T> l1, std::pair<T, T> l2, std::pair<T, T> l3) {
        if (l1 < l3) std::swap(l1, l3);
        return (l3.second - l2.second) * (l2.first - l1.first) >= (l2.second - l1.second) * (l3.first - l2.first);
    }

    // 直線y=ax+bを追加する
    void add(T a, T b) {
        std::pair<T, T> line(a, b);
        while (lines.size() >= 2 && check(*(lines.end() - 2), lines.back(), line))
            lines.pop_back();
        lines.emplace_back(line);
    }

    // i番目の直線f_i(x)に対するxの時の値を返す
    T f(int i, T x) {
        return lines[i].first * x + lines[i].second;
    }

    // i番目の直線f_i(x)に対するxの時の値を返す
    T f(std::pair<T, T> line, T x) {
        return line.first * x + line.second;
    }

    // 直線群の中でxの時に最小(最大)となる値を返す
    T get(T x) {
        // 最小値(最大値)クエリにおけるxが単調
        if (isMonotonicX) {
            static int head = 0;
            while (lines.size() - head >= 2 && comp(f(head, x), f(head + 1, x)))
                ++head;
            return f(head, x);
        }
        else {
            int low = -1, high = lines.size() - 1;
            while (high - low > 1) {
                int mid = (high + low) / 2;
                (comp(f(mid, x), f(mid + 1, x)) ? low : high) = mid;
            }
            return f(high, x);
        }
    }
};