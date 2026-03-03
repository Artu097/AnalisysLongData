#include "dbSilhouette.hpp"

// Función auxiliar para el índice
inline unsigned int idx(unsigned int k, unsigned int t, unsigned int r, unsigned int c, unsigned int K, unsigned int T, unsigned int R) {
  return k + t*K + r*K*T + c*R*K*T;
}

// [[Rcpp::export]]
Rcpp::NumericVector dbSilhouetteCpp(Rcpp::NumericMatrix data, Rcpp::List postPartitionMCMC) {
  _matrix<double> y(data.rows(), data.cols());
  _vector<_vector<_matrix<double>>> theta;
  _vector<unsigned int> pi(data.rows());

  // Llenar matriz y
  for (unsigned int i=0; i<y.rows(); i++) {
    for (unsigned int j=0; j<y.cols(); j++) {
      y(i,j) = data(i,j);
    }
  }

  Rcpp::NumericVector th = postPartitionMCMC["th"];
  Rcpp::NumericVector tmpP = postPartitionMCMC["p"];
  unsigned int T = postPartitionMCMC["f"];
  unsigned int K = ((Rcpp::NumericVector)postPartitionMCMC["d"]).size();
  unsigned int R_size = data.cols();
  unsigned int C = th.size() / (T * K * R_size);

  theta.resize(T);
  for (unsigned int t=0; t<T; t++) {
    theta[t].resize(K);
    for (unsigned int j=0; j<K; j++) {
      theta[t][j].resize(R_size, C);
      for (unsigned int r=0; r<R_size; r++) {
        for (unsigned int c=0; c<C; c++) {
          theta[t][j](r,c) = th[idx(j, t, r, c, K, T, R_size)];
        }
      }
    }
  }

  for (unsigned int i=0; i<pi.size(); i++) pi[i] = tmpP[i];

  // Llamada final al template usando el modelo datamodel
  return dbSilhouette<datamodel>(y, theta, pi);
}
