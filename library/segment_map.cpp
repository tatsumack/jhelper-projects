class SegmentMap : public std::map<int, int> {
private:
    bool flagToMergeAdjacentSegment;
public:
    // if merge [l, c] and [c+1, r], set flagToMergeAdjacentSegment to true
    SegmentMap(bool flagToMergeAdjacentSegment = true) :
            flagToMergeAdjacentSegment(flagToMergeAdjacentSegment) {}

    // __exist -> iterator pair(l, r) (contain p)
    // noexist -> map.end()
    auto get(int p) const {
        auto it = upper_bound(p);
        if (it == begin() || (--it)->second < p) return end();
        return it;
    }

    // insert segment [l, r]
    void insert(int l, int r) {
        auto itl = upper_bound(l), itr = upper_bound(r + flagToMergeAdjacentSegment);
        if (itl != begin()) {
            if ((--itl)->second < l - flagToMergeAdjacentSegment) ++itl;
        }
        if (itl != itr) {
            l = std::min(l, itl->first);
            r = std::max(r, std::prev(itr)->second);
            erase(itl, itr);
        }
        (*this)[l] = r;
    }

    // remove segment [l, r]
    void remove(int l, int r) {
        auto itl = upper_bound(l), itr = upper_bound(r);
        if (itl != begin()) {
            if ((--itl)->second < l) ++itl;
        }
        if (itl == itr) return;
        int tl = std::min(l, itl->first), tr = std::max(r, std::prev(itr)->second);
        erase(itl, itr);
        if (tl < l) (*this)[tl] = l - 1;
        if (r < tr) (*this)[r + 1] = tr;
    }

    // Is p and q in same segment?
    bool same(int p, int q) const {
        const auto&& it = get(p);
        return it != end() && it->first <= q && q <= it->second;
    }
};
