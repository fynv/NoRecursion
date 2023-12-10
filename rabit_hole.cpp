#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

using namespace std;

int getMaxVisitableWebpages(int N, int M, vector<int> A, vector<int> B) 
{
    vector<unordered_set<int>> edge_map(N);
    vector<unordered_set<int>> edge_map_r(N);

    for (int i = 0; i < M; i++)
    {
        edge_map[A[i] - 1].insert(B[i] - 1);
        edge_map_r[B[i] - 1].insert(A[i] - 1);
    }

    // strongly connected component  
    struct State
    {
        int idx;
        unordered_set<int>::iterator it;
    };


    vector<bool> reached(N);
    list<int> order;

    for (int i = 0; i < N; i++)
    {
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map_r[i].begin() };
        reached[i] = true;

        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map_r[j];
            if (cur.it != edges.end())
            {
                int k = *cur.it;
                cur.it++;
                if (!reached[k])
                {
                    st.push(cur);
                    cur = { k, edge_map_r[k].begin() };
                    reached[k] = true;
                }
            }
            else
            {
                order.push_front(j);
                if (st.size() < 1) break;
                cur = st.top();
                st.pop();
            }
        }
    }

    fill(reached.begin(), reached.end(), false);

    int comp_id = 0;
    vector<int> comps(N, -1);
    vector<int> comp_count;

    auto iter_order = order.begin();
    while (iter_order != order.end())
    {
        int i = *iter_order;
        iter_order++;
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map[i].begin() };
        reached[i] = true;

        int count = 0;
        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map[j];
            if (cur.it == edges.begin())
            {
                comps[j] = comp_id;
                count++;
            }
            if (cur.it != edges.end())
            {
                int k = *cur.it;
                cur.it++;
                if (!reached[k])
                {
                    st.push(cur);
                    cur = { k, edge_map[k].begin() };
                    reached[k] = true;
                }
            }
            else
            {
                if (st.size() < 1) break;
                cur = st.top();
                st.pop();
            }
        }

        comp_id++;
        comp_count.push_back(count);
    }


    // condensed graph
    vector<unordered_set<int>> edge_map_comp(comp_id);
    vector<unordered_set<int>> edge_map_comp_r(comp_id);

    for (int i = 0; i < N; i++)
    {
        int comp_i = comps[i];
        unordered_set<int>& edges = edge_map[i];
        auto iter = edges.begin();
        while (iter != edges.end())
        {
            int j = *iter;
            int comp_j = comps[j];
            if (comp_j != comp_i)
            {
                edge_map_comp[comp_i].insert(comp_j);
                edge_map_comp_r[comp_j].insert(comp_i);
            }
            iter++;
        }
    }

    // topological sorting + maximize sum  
    vector<int> indegree_count(comp_id);
    vector<int> count_sum = comp_count;

    list<int> q;
    for (int i = 0; i < comp_id; i++)
    {
        if (edge_map_comp_r[i].size() < 1)
        {
            q.push_back(i);
        }
    }

    int max_count = 0;
    while (q.size() > 0)
    {
        int i = q.front();
        q.pop_front();

        {
            unordered_set<int>& edges = edge_map_comp_r[i];
            int max_in = 0;
            auto iter = edges.begin();
            while (iter != edges.end())
            {
                int j = *iter;
                if (count_sum[j] > max_in)
                {
                    max_in = count_sum[j];
                }
                iter++;
            }
            count_sum[i] += max_in;
        }

        unordered_set<int>& edges = edge_map_comp[i];
        if (edges.size() < 1)
        {
            if (count_sum[i] > max_count)
            {
                max_count = count_sum[i];
            }
        }
        auto iter = edges.begin();
        while (iter != edges.end())
        {
            int j = *iter;
            indegree_count[j]++;
            if (indegree_count[j] == edge_map_comp_r[j].size())
            {
                q.push_back(j);
            }
            iter++;
        }
    }



    return max_count;
}


int main()
{
    int N = 10;
    int M = 9;
    vector<int> A = { 3, 2, 5, 9, 10, 3, 3, 9, 4 };
    vector<int> B = { 9, 5, 7, 8, 6, 4, 5, 3, 9 };

    int count = getMaxVisitableWebpages(N, M, A, B);
    printf("%d\n", count);

    return 0;
}
