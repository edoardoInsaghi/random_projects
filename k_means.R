library(dplyr)
library(ggplot2)

setwd("Desktop/R_files/ml_algorithms/k_means_clustering")


distance <- function(x1, x2){
  
  # simple euclidean norm
  return( sqrt(sum((x1-x2)^2)) )
}



assign_to_group <- function(x, groups, means){
  
  means <- split(means, row(means))
  
  # calculates, for a single point, all the distances from the means
  distances <- sapply(means, function(m){distance(x, m)})
  
  # point in assigned to the group whose mean is closest to it
  group <- groups[which.min(distances)]
  
  return( group )
}



##### CLUSTERING FUNCTION #####

k_means_clustering <- function(data, k){
  
  # number of data points
  n <- nrow(data)
  
  # if k == 1 the algorithm is trivial
  if (k == 1){

    avg <- colMeans(data)

    data_split <- suppressWarnings(split(data, row(data)))
    
    within_dev <- lapply(data_split, function(x){return(sum((x - avg)^2))})
    
    within_dev <- do.call("sum", unname(within_dev))
    
    data$group <- rep("1", n)
    
    return(list(data = data,
                within_dev = within_dev))
  }
  
  # initial points chosen at random
  init <- sample.int(n, k)
  
  # assigning initial groups
  groups <- as.character(1:k)
  data$group <- rep("0", n)
  data$group[init] <- groups
  old_groups <- data$group
  
  # practical data-frame to use, numeric values only
  use_df <- data %>% select(-c(group)) %>% as.matrix()
  
  # initially the means are the points chosen at random in the beginning
  means <- use_df[init,]
  
  # loop until convergence is reached
  convergence <- FALSE  
  while (!convergence){
    
    # for every point, its distance from each mean is calculated, then the point
    # is assigned to the group whose mean it is closest to
    points <- split(use_df, row(use_df))
    new_groups <- sapply(points, function(p){assign_to_group(p, groups, means)})
    
    # update the groups
    data$group <- new_groups
    
    # update the means
    for (j in 1:k){
      means[j,] <- colMeans(use_df[data$group==groups[j], ])
    }
    
    # checking for convergence
    if (identical(new_groups, old_groups)){
      convergence <- TRUE
    }
    
    # update the old groups
    old_groups <- new_groups
  }
  
  # calculate within-groups variance
  within_dev <- 0
  
  for (j in 1:k){
    
    within_obs <- use_df[data$group==groups[j], ]
    avg <- colMeans(within_obs)
    
    within_obs <- split(within_obs, row(within_obs))
    
    within_dev_group <- lapply(within_obs, function(x){return(sum((x - avg)^2))})
    
    within_dev <- within_dev +  do.call("sum", unname(within_dev_group))
  }
  
  return( list(data = data,
               within_dev = within_dev) )
}



##### GENERATING DATA #####

gen_data <- function(k, num, means, vars){
  
  groups <- as.character(1:k)
  
  df <- data.frame(x=NULL, y=NULL, group=NULL)
  
  for (j in 1:k){
    
    x <- rnorm(num[j], means[j,1], vars[j,1])
    y <- rnorm(num[j], means[j,2], vars[j,2])
    group <- rep(groups[j], num[j])
    new_df <- data.frame(x, y, group)
    
    df <- rbind(df, new_df)
  }
  
  return(df)
  
}



##### CHOICE OF K #####

find_k <- function(data, k_max = 10, iters = 20){
  
  final_res <- list()
  
  for (k in 1:k_max){
    print(k)
    curr_best <- Inf
    
    for (j in 1:iters){

      res <- k_means_clustering(data, k)
      
      if (res$within_dev < curr_best) {
        curr_best <- res$within_dev
        final_res[[k]] <- res
      }
    }
  }
  
  vars <- c()
  for (j in 1:k_max){
    vars[j] <- final_res[[j]]$within_dev
  }
  
  df <- data.frame("deviance" = vars,
                   "k" = 1:k_max)
  
  p <- ggplot(data = df, aes(x = k, y = vars)) + geom_point() + geom_line() + 
    labs(title = "ElbowPlot for the SSWG",
         x = "number of groups",
         y = "Deviance within groups") + theme_bw()
  
  return( list(vars = vars,
               plot_res = p))
}



##### TESTING #####

# generating data
k = 7
num <- rep(100, k)
means <- rbind(c(-10,5), c(-12,-5), c(-5,5), c(0,0), c(-10,-10), c(-7, 12), c(-5,-5))
vars <- rbind(c(1,3), c(1,1), c(0.25,0.5), c(1,1), c(0.25,0.25), c(2, 1), c(1,1))

df <- gen_data(k, num, means, vars)

test_df <- df %>% select(-c(group))

p <- ggplot(data=test_df, aes(x=x, y=y)) + 
  geom_point(size=0.6) + theme_bw()
p


# choosing k
k_choice_res <- find_k(test_df, k_max = 10, iters = 10)
k_choice_res$plot_res


# k_means_clustering
k_found <- 7
res <- k_means_clustering(test_df, k_found)

p <- ggplot(data=res$data, aes(x=x, y=y, colour=group)) + 
  geom_point(size=0.6) + labs(title = "K-means clusters",
                              subtitle = paste("\t k =", k, "\t sample size =", sum(num))) +
  theme_bw()
p









