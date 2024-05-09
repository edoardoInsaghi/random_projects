
compute_derivative <- function(fun, dir, point, params, grad = FALSE, h = 10^-7){
  
  if(grad){
    return( dot(grad, dir) )
  }
  
  return( (fun(point + h * dir, params) - fun(point, params)) / h )
}




compute_gradient <- function(fun, params, point){
  
  n <- length(point)
  
  grad <- vector(mode="numeric", length = n)
  
  for(i in 1:n){
    
    ei <- vector(mode="numeric", length = n)
    ei[i] <- 1
    
    grad[i] <- compute_derivative(fun, dir = ei, point, params)
  }
  
  return( grad )
}




gradient_descent <- function(fun,
                             params,
                             x0,
                             lr = 0.001,
                             max_iters = 10000,
                             epsilon = 10^-4){
  
  df <- Inf
  iters <- 0
  
  while(df > epsilon & iters < max_iters){
    
    grad <- compute_gradient(fun, params, x0)
    
    df <- abs( fun(x0, params) - fun(x0 + lr * grad, params) )
    if(df < epsilon){print(paste0("convergence reached in ", iters, " iterations. "))}
    
    iters <- iters + 1
    if(iters == max_iters){print("maximum number of iterations reached.")}
    
    x0 <- x0 + lr * grad
  }

  return( x0 )
}





fun <- function(x, params = NULL){
  
  return( -x[1]^2 -x[2]^2 )
}



SSR <- function(betas, params){

  yhat <- betas[1] + betas[2] * params$x
  
  ssr <- -sum((params$y - yhat)^2)
  
  return( ssr )
}

  
n <- 100
beta0 <- 10
beta1 <- -15

x <- runif(n, 0, 100)
y <- beta0 + beta1 * x + rnorm(n, 0, 10)

df <- data.frame("y" = y,
                 "x" = x)


model <- lm(data = df, y ~ x)
summary(model)

x0 <- c(0, 0)
lse <- gradient_descent(fun = SSR, 
                        params = df, 
                        x0 = c(0, 0), 
                        lr = 0.000001, 
                        max_iters = 10^6)
lse







