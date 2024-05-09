import pandas as pd
import numpy as np
import seaborn as sns

def mode(x):

    counts = {}
    for value in x:
        counts[value] = 1 if value not in counts else counts[value] + 1

    maxCount = 0
    maxValue = None

    for key, value in counts.items():
        if value > maxCount:
            maxCount = value
            maxValue = key
    
    return maxValue


def midPoint(x):

    x = np.sort(x)
    return [ (x[i] + x[i+1]) / 2 for i in range(len(x) - 1)]



class Node():

    def __init__(self, var=None, value=None, depth=1,
                 left=None, right=None, leaf=False):
        
        self.var = var
        self.value = value
        self.depth = depth
        self.left = left
        self.right = right
        self.leaf = leaf



class tree():

    def __init__(self, maxDepth=10, nMin=10):
        self.maxDepth = maxDepth
        self.nMin = nMin
        self.root = Node()

    
    def fit(self, data, Y, error, node=None):

        if node == None:
            node = self.root

        if node.depth == self.maxDepth or data.shape[0] <= self.nMin or self.error(data, Y, error) == 0:
            node.leaf = mode(data[Y])
            return

        X = [col for col in data.columns if col != Y]

        var, value = self.findBestBranch(data, Y, error)
        node.var = var
        node.value = value

        left = data[data[var] < value]
        right = data[data[var] >= value]

        if left.shape[0] > 0:
            node.left = Node(depth=node.depth + 1)
            self.fit(left, Y, error, node.left)

        if right.shape[0] > 0:
            node.right = Node(depth=node.depth + 1)
            self.fit(right, Y, error, node.right)


    def findBestBranch(self, data, Y, error):

        bestVar = None
        bestVal = None
        bestError = float('inf')

        X = [col for col in data.columns if col != Y]

        for col in X:
            for value in midPoint(data[col]):

                left = data[data[col] < value]
                right = data[data[col] >= value]

                if left.shape[0] > 0 and right.shape[0] > 0:

                    err = self.error(left, Y, error) + self.error(right, Y, error)

                    if err < bestError:
                        bestError = err
                        bestVar = col
                        bestVal = value

        return (bestVar, bestVal)
    

    def error(self, data, Y, fun):

        if fun.lower() == 'error':
            if (len(data[Y]) == 0):
                return 0
            
            guess = mode(data[Y])
            return sum(data[Y] != guess)
        
        elif fun.lower() == 'gini':
            gini = 0
            for value in data[Y].unique():
                gini += sum(data[Y] == value) * (1 - sum(data[Y] == value)) / data.shape[0]**2
            return gini

        elif fun.lower() == 'cross_entropy':
            ce = 0
            for value in data[Y].unique():
                ce += sum(data[Y] == value) * np.log(sum(data[Y] == value) / data.shape[0]) / data.shape[0]
            return -ce
        
        else:
            raise ValueError("Invalid method. Expected one of: ['error', 'gini', 'cross_entropy']")
    

    def predictSingle(self, row, node=None):

        if node == None:
            node = self.root

        if node.leaf != False:
            return node.leaf
        
        if row[1][node.var] < node.value:
            return self.predictSingle(row, node.left)
        
        else:
            return self.predictSingle(row, node.right)
        

    def predict(self, data, node=None):
            
        if node == None:
                node = self.root
    
        return [self.predictSingle(row, node) for row in data.iterrows()]
    

    def printTree(self, node=None, indent=0):

        if node == None:
            node = self.root

        if(node.var is not None):
            print("  "*indent + f"{node.var} - {node.value}\n")
            print("  "*indent + f"Depth: {node.depth}\n")

        if(node.left is not None):
            self.printTree(node.left, indent+1)

        if(node.right is not None):
            self.printTree(node.right, indent+1)


    def accuracy(self, Y, data, pred):

        return sum(data[Y] == pred) / data.shape[0]
    

    def confusionMatrix(self, Y, data, pred):

        matrix = pd.DataFrame(np.zeros((len(data[Y].unique()), len(data[Y].unique()))), 
                              index=data[Y].unique(), 
                              columns=data[Y].unique())
        
        for i in range(data.shape[0]):
            matrix.loc[data[Y].iloc[i], pred[i]] += 1

        return matrix
    

    def cv(self, data, Y, fun, folds=10):

        data = data.sample(frac=1).reset_index(drop=True)
        split = data.shape[0] // folds
        accuracy = []

        for i in range(folds):
            test = data.iloc[i*split:(i+1)*split]
            train = data.drop(index=test.index)
            classifier = tree(maxDepth=self.maxDepth, nMin=self.nMin)
            classifier.fit(train, Y, fun)
            accuracy.append(classifier.accuracy(Y, 
                                                test,
                                                classifier.predict(test)))
            
        return (np.mean(accuracy), np.std(accuracy))

    





if __name__ == '__main__':
    
    data = sns.load_dataset('iris')
    data = pd.DataFrame(data)
    data = data.sample(frac=1).reset_index(drop=True)

    ttsplit = 0.8
    split = int(ttsplit * data.shape[0])

    traindata = data.iloc[:split, :]
    testdata = data.iloc[split:, :]

    for error in ['error', 'gini', 'cross_entropy']:

        classifier = tree(nMin=8)
        classifier.fit(traindata, 'species', error)
        classifier.printTree()

        print(f"\nError: {error}.\n")
        print("\nAccuracy on the test set: ")
        print(classifier.accuracy('species', 
                                  testdata, 
                                  classifier.predict(testdata)))
        print("\n")
    
        print("Confusion Matrix: ")
        cm = classifier.confusionMatrix('species', 
                                        testdata, 
                                        classifier.predict(testdata))
        print(cm, "\n")

        nfold = 10
        nfoldacc = classifier.cv(data, 'species', error, nfold)
        print(f"Cross Fold validation with {nfold} folds: \n")
        print(f"Mean accuracy: {nfoldacc[0]}; SE accuracy:{nfoldacc[1]}")

        print("\n##################################################### \n")
    


