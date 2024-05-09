library(ggplot2)
#setwd("Desktop/programs/random_projects/zipfc++")

df <- read.table("output.txt", skipNul = TRUE, heade=T)
df["rank"] <- 1:nrow(df)

p <- ggplot(data=df, aes(x=rank, y=count)) + geom_line() + geom_smooth(method="lm") +
  scale_x_continuous(trans="log10") + scale_y_continuous(trans="log10") +
  labs(title="Zipf's Law", subtitle="Alice in Wonderland", y="log(count)", x="log(rank)") + theme_gray()

ggsave("results.pdf", p) 


