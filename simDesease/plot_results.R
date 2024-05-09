library(ggplot2)

# setwd("Desktop/programs/random_projects/simDesease/")
data <- read.csv("results.csv")


df <- data.frame(
                 "time" = rep(data$i, ncol(data)-1),
                 "count" = c(data$vulnerable, data$exposed,
                             data$infected, data$recovered),
                 "status" = c(rep("vulnerable", nrow(data)),
                              rep("exposed", nrow(data)),
                              rep("infected", nrow(data)),
                              rep("recovered", nrow(data)))
                 )

p <- ggplot(data=df, aes(x = time, y = count, col=status)) + geom_line(linewidth=1.1) + 
       labs(title="Number of subjects, conditioned on illness status.",
            y="", x="time") + theme_gray()

ggsave("results.pdf", p)
