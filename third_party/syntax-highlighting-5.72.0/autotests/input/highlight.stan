/* Stan Highlighting Example

  This file contains a syntatically correct but nonsensical Stan program that
  includes almost every feature of the language needed to validate syntax
  highlighters. It will compile (as of Stan 2.17.1), but it does nothing
  useful.

  Author: Jeffrey Arnold <jeffrey.anold@gmail.com>
  Copyright: Jeffrey Arnold (2018)
  License: MIT

*/
// line comment
# deprecated line comment
functions {
  #include stuff.stan
  #include "morestuff.stan"
  #include 'moststuff.stan'
  #include <evenmorestuff.stan>

  // declarations
  void oof(real x);

  // definitions
  // return types
  void oof(real x) {
    print("print ", x);
  }
  /*
    @param x A number
    @return x + 1
  */
  real foo(real x) {
    return x;
  }
  int bar(int x) {
    return x;
  }
  vector baz(vector x) {
    return x;
  }
  row_vector qux(row_vector x) {
    return x;
  }
  matrix quux(matrix x) {
    return x;
  }
  // numbers of arguments
  void corge() {
    print("no parameters");
  }
  void grault(int a, real b, vector c, row_vector d, matrix f) {
    print("many parameters");
  }
  void garply(real a, real[] b, real[,] c, real[,,] d) {
    print("array arguments");
  }
  // array return types
  int[] waldo(int[] x) {
    return x;
  }
  int[,] fred(int[,] x) {
    return x;
  }
  int[,,] plough(int[,,] x) {
    return x;
  }
  // data only function argument
  real plugh(data real x) {
    return x;
  }
  // ode function
  real[] ode_func(real a, real[] b, real[] c, real[] d, int[] e) {
    return b;
  }
}
data {
  // non-int variable types
  int x_int;
  real x_real;
  real y_real;
  vector[1] x_vector;
  ordered[1] x_ordered;
  positive_ordered[1] x_positive_ordered;
  simplex[1] x_simplex;
  unit_vector[1] x_unit_vector;
  row_vector[1] x_row_vector;
  matrix[1, 1] x_matrix;
  cholesky_factor_corr[2] x_cholesky_factor_corr;
  cholesky_factor_cov[2] x_cholesky_factor_cov;
  cholesky_factor_cov[2, 3] x_cholesky_factor_cov_2;
  corr_matrix[2] x_corr_matrix;
  cov_matrix[2] x_cov_matrix;

  // range constraints
  real<lower = 0., upper = 1.> alpha;
  real<lower = 0.> bravo;
  real<upper = 1.> charlie;

  // arrays
  int echo[1];
  int foxtrot[1, 1];
  int golf[1, 1, 1];

  // identifier with all valid letters
  real abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789;

  // hard pattern
  real<lower = (bravo < charlie), upper = (bravo > charlie)> ranger;

  // identifier patterns
  real a;
  real a3;
  real a_3;
  real Sigma;
  real my_cpp_style_variable;
  real myCamelCaseVariable;
  real abcdefghijklmnojk;
  // names beginning with keywords
  real iffffff;
  real whilest;
  // name ending with truncation
  real fooT;
}
transformed data {
  // declaration and assignment
  int india = 1;
  real romeo = 1.0;
  row_vector[2] victor = [1, 2];
  matrix[2, 2] mike = [[1, 2], [3, 4]];
  real sierra[2] = {1., 2.};
}
parameters {
  real hotel;
}
transformed parameters {
  real juliette;
  juliette = hotel * 2.;
}
model {
  real x;
  int k;
  vector[2] y = [1., 1.]';
  matrix[2, 2] A = [[1., 1.], [1., 1.]];
  real odeout[2, 2];
  real algout[2, 2];

  // if else statements
  if (x_real < 0) x = 0.;

  if (x_real < 0) {
    x = 0.;
  }

  if (x_real < 0) x = 0.;
  else x = 1.;

  if (x_real < 0) {
    x = 0.;
  } else {
    x = 1.;
  }

  if (x_real < 0) x = 0.;
  else if (x_real > 1) x = 1.;
  else x = 0.5;

  if (x_real < 0) {
    x = 0.;
  } else if (x_real > 1) {
    x = 1.;
  } else {
    x = 0.5;
  }

  // for loops
  for (i in 1:5) {
    print("i = ", i);
  }
  // for (j in echo) {
  //   print("j = ", j);
  // }
  // while loop
  while (1) {
    break;
    continue;
  }

  // reject statement
  reject("reject statment ", x_real);

  // print statement
  print("print statement ", x_real);
  print("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_~@#$%^&*`'-+={}[].,;: ");

  // increment log probability statements;
  target += 1.;

  // valid integer literals
  k = 0;
  k = 1;
  k = -1;
  k = 256;
  k = -127098;
  k = 007;

  // valid real literals
  x = 0.0;
  x = 1.0;
  x = 3.14;
  x = 003.14;
  x = -217.9387;
  x = 0.123;
  x = .123;
  x = 1.;
  x = -0.123;
  x = -.123;
  x = -1.;
  x = 12e34;
  x = 12E34;
  x = 12.e34;
  x = 12.E34;
  x = 12.0e34;
  x = 12.0E34;
  x = .1e34;
  x = .1E34;
  x = -12e34;
  x = -12E34;
  x = -12.e34;
  x = -12.E34;
  x = -12.0e34;
  x = -12.0E34;
  x = -.1e34;
  x = -.1E34;
  x = 12e-34;
  x = 12E-34;
  x = 12.e-34;
  x = 12.E-34;
  x = 12.0e-34;
  x = 12.0E-34;
  x = .1e-34;
  x = .1E-34;
  x = -12e-34;
  x = -12E-34;
  x = -12.e-34;
  x = -12.E-34;
  x = -12.0e-34;
  x = -12.0E-34;
  x = -.1e-34;
  x = -.1E-34;
  x = 12e+34;
  x = 12E+34;
  x = 12.e+34;
  x = 12.E+34;
  x = 12.0e+34;
  x = 12.0E+34;
  x = .1e+34;
  x = .1E+34;
  x = -12e+34;
  x = -12E+34;
  x = -12.e+34;
  x = -12.E+34;
  x = -12.0e+34;
  x = -12.0E+34;
  x = -.1e+34;
  x = -.1E+34;

  // assignment statements
  x = 1;
  x += 1.;
  x -= 1.;
  x *= 1.;
  x /= 1.;
  y .*= x_vector;
  y ./= x_vector;

  // operators
  x = x_real && 1;
  x = x_real || 1;
  x = x_real < 1.;
  x = x_real <= 1.;
  x = x_real > 1.;
  x = x_real >= 1.;
  x = x_real + 1.;
  x = x_real - 1.;
  x = x_real * 1.;
  x = x_real / 1.;
  x = x_real ^ 2.;
  x = x_real % 2;
  x = !x_real;
  x = +x_real;
  x = -x_real;
  x = x_int ? x_real : 0.;

  y = x_row_vector';
  y = x_matrix \ x_vector;
  y = x_vector .* x_vector;
  y = x_vector ./ x_vector;

  // parenthized expression
  x = (x_real + x_real);

  // block statement
  {
    real z;
    z = 1.;
  }

  // built-in functions
  x = log(1.);
  x = exp(1.);

  // non-built-in function
  x = foo(1.);

  // constants and nullary functions
  x = machine_precision();
  x = pi();
  x = e();
  x = sqrt2();
  x = log2();
  x = log10();
  // special values
  x = not_a_number();
  x = positive_infinity();
  x = negative_infinity();
  x = machine_precision();
  // log probability
  x = target();

  // sampling statement
  x_real ~ normal(0., 1.);

  // truncation
  x_real ~ normal(0., 1.) T[-1., 1.];
  x_real ~ normal(0., 1.) T[, 1.];
  x_real ~ normal(0., 1.) T[-1., ];
  x_real ~ normal(0., 1.) T[ , ];

  // transformation on lhs of sampling
  log(x_real) ~ normal(0., 1.);

  // lhs indexes
  y[1] = 1.;
  A[1, 2] = 1.;
  A[1][2] = 1.;

  // special functions
  odeout = integrate_ode(ode_func, {1.}, x_real, {1.}, {1.}, {1.}, {0});
  odeout = integrate_ode_bdf(ode_func, {1.}, x_real, {1.}, {1.}, {1.}, {0},
                             x_real, x_real, x_int);
  odeout = integrate_ode_rk45(ode_func, {1.}, x_real, {1.}, {1.}, {1.}, {0},
                              x_real, x_real, x_int);
  // algout = algebra_solver(algebra_func, x_vector, x_vector, {1.}, {0});

  // distribution functions
  x = normal_lpdf(0.5 | 0., 1.);
  x = normal_cdf(0.5, 0., 1.);
  x = normal_lcdf(0.5 | 0., 1.);
  x = normal_lccdf(0.5 | 0., 1.);
  x = binomial_lpmf(1 | 2, 0.5);

  // deprecated features
  foo <- 1;
  increment_log_prob(0.0);
  y_hat = integrate_ode(sho, y0, t0, ts, theta, x_r, x_i);
  x = get_lp();
  x = multiply_log(1.0, 1.0);
  x = binomial_coefficient_log(1.0, 1.0);
  // deprecated distribution functions versions
  x = normal_log(0.5, 0.0, 1.0);
  x = normal_cdf_log(0.5, 0.0, 1.0);
  x = normal_ccdf_log(0.5, 0.0, 1.0);

}
generated quantities {
  real Y;
  // rng function
  Y = normal_rng(0., 1.);
}
