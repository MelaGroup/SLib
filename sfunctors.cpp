#include "sfunctors.h"

SMainComponent::SMainComponent(const QImage &src)
{
    SCovarienceMatrix cov_mat(src);
    eigenvalues=cov_mat.eigenvalues();
    eigenvectors=cov_mat.eigenvectors(eigenvalues);
    auto v=eigenvectors.col(component_number);
    r=v.x;  g=v.y;  b=v.z;
}

void SMainComponent::useComponent(int component_1_2or3)
{
    int& num=component_1_2or3;
    if (num>0 && num<4)
    {
        component_number=num;
        auto v=eigenvectors.col(component_number);
        r=v.x;  g=v.y;  b=v.z;
    }
    else throw std::invalid_argument("SMaincomponent::use() - Use only 1,2 or 3");
}
