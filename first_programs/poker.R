library(dplyr)


seeds <- c("Heart", "Diamonds", "Clubs", "Spades")
values <- c(2:10, "jack", "queen", "king", "ace")



##### General utility functions #####


# function that generates a deck of 52 cards, 13 for each suit
generateDeck <- function(){
  
  deck <- list()
  i <- 1
  
  for(seed in seeds){
    for(val in values){
      
      deck[[i]] <- paste0(val, "_" ,seed)
      
      i <- i + 1 
    }
  }
  
  return( deck )
}



# function that shuffles the deck
shuffleDeck <- function(deck){
  
  deck <- sample(deck)
  return( deck )
}



# function that takes the first cards of the deck, then returns 
# the card and the rest of the deck in a list of two elements
giveCard <- function(deck){
  
  card <- deck[[1]]
  deck <- deck[-1]
  
  return( list("card"=card, 
               "deck"=deck) )
}



# function that uses giveCard() to draw any nuber of cards from the deck
turnCards <- function(deck, numToTurn){
  
  cards <- list()
  
  for(i in 1:numToTurn){
    card_deck <- giveCard(deck)
    cards[[i]] <- card_deck$card
    deck <- card_deck$deck
  }
  
  return( list("cards"=cards, 
               "deck"=deck) )
}



# specific case of turnCards(), with numToTurn = 2, a standards Texas Holdem hand
giveHand <- function(deck){
  
  hand_deck <- turnCards(deck, 2)
  names(hand_deck) <- c("hand", "deck")
  
  return( hand_deck )
}


# function that takes a list of cards, returns a numeric 
# vector with the value of each card
getValues <- function(cards){
  
  values <- sapply(cards, function(x){
    
    val <- unlist(strsplit(x, "_"))[1]
    numVal <- which(values == val) + 1
    return( numVal )
  })
  
  return( values )
}


# function that takes a list of cards, returns a vector 
# of strings with the suit for each card
getSuits <- function(cards){
  
  suits <- sapply(cards, function(x){
    
    suit <- unlist(strsplit(x, "_"))[2]
    return( suit )
  })
  
  return( suits )
}



##### Functions to detect meaningful combination of cards #####

# cards is a list of cards, represented as strings in the form "Value_Seed"
# values = getValues(cards)
# countValues = table(getValues(cards))
# suitCounts = table(getSuits(cards))
# diffValues = diff(sort(getValues(cards)), decreasing = TRUE)


# function that returns one if there is a pair in the cards, 0 otherwise
hasPair <- function(countValues){
  
  if(any(countValues == 2)) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a double pair and 0 otherwise
hasDoublePair <- function(countValues){
  
  if(sum(countValues == 2) >= 2) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a three of a kind and 0 otherwise
hasThreeKind <- function(countValues){
  
  if(any(countValues == 3)) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a full house and 0 otherwise
hasFullHouse <- function(countValues){
  
  if( any(countValues == 3) && any(countValues == 2) ) return( 1 )
  return( 0 )
}


# function that returns one if there is a four of a kind and 0 otherwise
hasFourKind <- function(countValues){
  
  if( any(countValues == 4) ) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a flush and 0 otherwise
hasFlush <- function(suitsCount){
  
  if( any(suitsCount >= 5) ) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a straight and 0 otherwise
hasStraight <- function(vals){
  
  vals <- sort(unique(vals))
  n <- length(vals)

  seqLen <- 0
  for(i in 2:n){
    
    if( vals[i] == (vals[i-1] + 1) ){
      seqLen <- seqLen + 1
    }
    
    else{
      seqLen <- 0
    }
    
    if( seqLen == 4 ) return( 1 )
  }
  
  # check if there is a straight with ace, two, three, four, five
  if( all(c(2:5, 14) %in% vals) ) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a straight flush and 0 otherwise
hasStraightFlush <- function(suitCounts, vals){
  
  if( hasStraight(vals) && hasFlush(suitCounts) ) return( 1 )
  
  return( 0 )
}


# function that returns one if there is a royal flush and 0 otherwise
hasRoyalFlush <- function(suitCounts, vals){
  
  if( hasStraightFlush(suitCounts, vals) && 
      all(c(10, 11, 12, 13, 14) %in% vals) ) return( 1 )
  
  return( 0 )
}



# function that evaluates a hand of poker, given the community cards as well.
# it returns the rank of the hand, which is a proxy of how
evaluateHand <- function(cards){
  
  countValues <- table(getValues(cards))
  suitsCount <- table(getSuits(cards))
  vals <- sort(getValues(cards))
  
  rank <- 1
  
  if( hasPair(countValues) ) rank <- 2
  if( hasDoublePair(countValues) ) rank <- 3
  if( hasThreeKind(countValues) ) rank <- 4
  if( hasStraight(vals) ) rank <- 5
  if( hasFlush(suitsCount) ) rank <- 6
  if( hasFullHouse(countValues) ) rank <- 7
  if( hasFourKind(countValues) ) rank <- 8
  if( hasStraightFlush(suitsCount, vals) ) rank <- 9
  if( hasRoyalFlush(suitsCount, vals) ) rank <- 10
  
  return( rank )
}





##### Tie Breaker functions #####

# players = list("Playeri" = list(cards player i))
# scores = list("Playeri" = list(score player i))
# communityCards = list(community cards)


# function that decides the winner between more players that hold a pair
TieBreakerPair <- function(players, scores, communityCards){
  
  
}





##### simulate a hand of Texas Holdem Poker #####

pokerHand <- function(cards, communityCards=NULL, numPlayers){
  
  # generate the deck and remove cards held by player
  deck <- generateDeck()
  deck <- setdiff(deck, cards)
  deck <- shuffleDeck(deck)
  
  # generate a list of players, to each one is assigned a hand of two cards
  players <- list("Player1" = cards)
  for(i in (2:numPlayers)){
    
    newHand <- giveHand(deck)
    deck <- newHand$deck
    players[[paste0("Player", i)]] <- newHand$hand
  }
  
  # generate community cards
  numToTurn <- 5 - length(communityCards)
  addCommunityCards <- turnCards(deck, numToTurn)
  deck <- addCommunityCards$deck
  for(i in 1:numToTurn){
    
    communityCards <- append(communityCards, addCommunityCards$cards[i])
  }
  
  scores <- list()
  for(i in 1:numPlayers){
    
    scores[[paste0("Player", i)]] <- evaluateHand(c(players[[paste0("Player", i)]], communityCards))
  }
  
  return( list("scores"=scores,
               "players"=players,
               "commCards"=communityCards) )
}





pokerHand(list("ace_Heart", "2_Clubs"), 
          communityCards = NULL, 
          numPlayers = 8)


