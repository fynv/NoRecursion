#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

using namespace std;

// kosaraju
void scc(int N, int M, vector<int> A, vector<int> B)
{
    vector<unordered_set<int>> edge_map(N);
    vector<unordered_set<int>> edge_map_r(N);

    for (int i = 0; i < M; i++)
    {
        edge_map[A[i] - 1].insert(B[i] - 1);
        edge_map_r[B[i] - 1].insert(A[i] - 1);
    }
    
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

    vector<vector<int>> lst_scc;

    auto iter_order = order.begin();
    while (iter_order != order.end())
    {
        int i = *iter_order;
        iter_order++;
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map[i].begin() };
        reached[i] = true;

        vector<int> scc;
        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map[j];
            if (cur.it == edges.begin())
            {
                scc.push_back(j);
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

        lst_scc.push_back(scc);
    }

    for (int i = 0; i < lst_scc.size(); i++)
    {
        printf("scc %d: [ ", i + 1);
        auto& scc = lst_scc[i];
        for (int j = 0; j < scc.size(); j++)
        {
            printf("%d ", scc[j] + 1);            
        }
        printf("]\n");
    }
}

// tarjan
void scc2(int N, int M, vector<int> A, vector<int> B)
{
    vector<unordered_set<int>> edge_map(N);
    for (int i = 0; i < M; i++)
    {
        edge_map[A[i] - 1].insert(B[i] - 1);    
    }

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
                else if(comps[k] == -1)
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

    vector<vector<int>> lst_scc(comp_id);
    for (int i = 0; i < N; i++)
    {
        lst_scc[comps[i]].push_back(i);
    }

    for (int i = 0; i < lst_scc.size(); i++)
    {
        printf("scc %d: [ ", i + 1);
        auto& scc = lst_scc[i];
        for (int j = 0; j < scc.size(); j++)
        {
            printf("%d ", scc[j] + 1);
        }
        printf("]\n");
    }

}

int main()
{
	int N = 10;
	int M = 9;
	vector<int> A = { 3, 2, 5, 9, 10, 3, 3, 9, 4 };
	vector<int> B = { 9, 5, 7, 8, 6, 4, 5, 3, 9 };

	//scc(N, M, A, B);
    scc2(N, M, A, B);

	return 0;
}
