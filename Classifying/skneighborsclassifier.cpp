#include "skneighborsclassifier.h"

SMetric S::euclidean()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                dist+=(f[i]-s[i])*(f[i]-s[i]);
            dist=sqrt(dist);
        }
        return dist;
    };
    return SMetric(ret,"euclidean");
}

SMetric S::manhattan()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                dist+=fabs(f[i]-s[i]);
        }
        return dist;
    };
    return SMetric(ret,"manhattan");
}

SMetric S::chebyshev()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                if (dist<fabs(f[i]-s[i]))
                    dist=fabs(f[i]-s[i]);
        }
        return dist;
    };
    return SMetric(ret,"chebyshev");
}

SKNeighborsClassifier::SKNeighborsClassifier(int n_neighbors, SMetric metric)
    :n_neighbors(n_neighbors),metric(metric)
{
    if (n_neighbors<1)
        throw std::invalid_argument("SKNeighborsClassifier: the number of neighbours must be greater than 0");
}

void SKNeighborsClassifier::fit(const SDataFrame &X, const std::vector<int> &Y)
{
    if (X.rows()!=int(Y.size()))
        throw std::invalid_argument("SKNeighborsClassifier: table objects-attributes and a vector of responses not compatible");
    table=X,answers=Y;
}

std::vector<int> SKNeighborsClassifier::predict(const SDataFrame &X)
{
    using namespace std;
    std::vector<int> predictions(X.rows());
    for (int j=0;j<X.rows();++j)
    {
        multimap<double,int> distancies;
        for (int i=0;i<table.rows();++i)
            distancies.insert(make_pair(metric(X.row(j),table.row(i)),i));

        map<int,int> counters;
        auto It=distancies.begin();
        for (int i=0;i<n_neighbors;++i,++It)
            ++counters[answers[It->second]];

        pair<int,int> max_counter{0,0};
        for (pair<int,int> c:counters)
            if (max_counter.second<c.second) max_counter=c;

        predictions[j]=max_counter.first;
    }
    return predictions;
}

