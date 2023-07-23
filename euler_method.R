library(ggplot2)


euler_method <- function(diff_eq, 
                         params, 
                         x_0, y_0, 
                         dx = 0.01, 
                         right.bound,
                         method = "fourth_order"){
  
  x <- seq(x_0, right.bound, by = dx)
  
  y <- vector(mode="numeric", length=length(x))
  y[1] <- y_0
  
  
  if (method == "standard"){
    
    for (j in 2:length(x)){
      
      y[j] <- y[j-1] + diff_eq(y[j-1], params) * dx
    }
  }
  
  
  if (method == "fourth_order"){
    
    for (j in 2:length(x)){
      
      k1 <- diff_eq(y[j-1], params) * dx
      k2 <- diff_eq(y[j-1] + k1/2, params) * dx
      k3 <- diff_eq(y[j-1] + k2/2, params) * dx
      k4 <- diff_eq(y[j-1] + k3, params) * dx
      
      y[j] <- y[j-1] + (k1 + 2*k2 + 2*k3 + k4) / 6
    }
  }

  
  df <- data.frame("x"=x,
                   "y"=y)
  
  return(df)
}




phase_space <- function(diff_eq, 
                        params, 
                        x_0, y_0, 
                        dx = 0.01, 
                        right.bound,
                        method = "fourth_order"){
  
  x <- seq(x_0, right.bound, by = dx)
  
  p <- ggplot()
  
  
  for (j in 1:length(y_0)){
    
    res <- euler_method(diff_eq,
                        params,
                        x_0, y_0[j],
                        dx = dx,
                        right.bound = right.bound,
                        method = method)
  
    
    p <- p + geom_line(data = res, aes(x = x, y = y))
    
  }
  
  p <- p + theme_bw()
  return( p )
}



# logistic curve
de <- function(x, params){
  
  return( params[1] * x * (1 - x/params[2]) )
}

# intractable de
de <- function(x, params){
  
  return( x + exp(-x) )
}



# parameters of the differential equation
params <- c(0.2, 100)

# initial x
x_0 = 0

# initial y
y_0 = 1


# single trajectory
res <- euler_method(de, params, x_0, y_0, dx = 0.01, right.bound = 100)

p <- ggplot() + geom_line(data = res, mapping = aes(x=x, y=y))
p


# vector of initial y, needed for the phase space
y_0 <- seq(0, 120, 2)

# phase space plot
res <- phase_space(de, params, x_0, y_0, dx = 0.1, right.bound = 50)
res

