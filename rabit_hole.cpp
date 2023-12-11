#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

using namespace std;

// kosaraju
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

// tarjan
int getMaxVisitableWebpages2(int N, int M, vector<int> A, vector<int> B)
{
    vector<unordered_set<int>> edge_map(N);
    for (int i = 0; i < M; i++)
    {
        edge_map[A[i] - 1].insert(B[i] - 1);
    }

    // strongly connected component  
    struct State
    {
        int idx;
        unordered_set<int>::iterator it;
    };

    struct Mark
    {
        unsigned a = 0xFFFFFFFF;
        unsigned b = 0xFFFFFFFF;
    };

    unsigned imark = 0;
    vector<Mark> marks(N);
    int comp_id = 0;
    vector<int> comps(N, -1);
    stack<int> st_scc;

    for (int i = 0; i < N; i++)
    {
        if (marks[i].a != 0xFFFFFFFF) continue;

        stack<State> st;
        State cur = { i, edge_map[i].begin() };
        marks[i] = { imark, imark };
        imark++;
        st_scc.push(i);

        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map[j];
            if (cur.it != edges.end())
            {
                int k = *cur.it;
                cur.it++;
                if (marks[k].a == 0xFFFFFFFF)
                {
                    st.push(cur);
                    cur = { k, edge_map[k].begin() };
                    marks[k] = { imark, imark };
                    imark++;
                    st_scc.push(k);
                }
                else if (comps[k] == -1)
                {
                    if (marks[k].b < marks[j].b)
                    {
                        marks[j].b = marks[k].b;
                    }
                }
            }
            else
            {
                if (marks[cur.idx].b == marks[cur.idx].a)
                {
                    int pop_to = marks[cur.idx].a;
                    while (true)
                    {
                        int k = st_scc.top();
                        st_scc.pop();
                        comps[k] = comp_id;
                        if (marks[k].a == pop_to) break;
                    }
                    comp_id++;
                }

                if (st.size() < 1) break;
                State top = st.top();
                st.pop();
                if (marks[cur.idx].b < marks[top.idx].b)
                {
                    marks[top.idx].b = marks[cur.idx].b;
                }
                cur = top;
            }
        }
    }

    vector<int> comp_count(comp_id, 0);
    for (int i = 0; i < N; i++)
    {
        comp_count[comps[i]]++;
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

    //int count = getMaxVisitableWebpages(N, M, A, B);
    int count = getMaxVisitableWebpages2(N, M, A, B);
    printf("%d\n", count);

    return 0;
}
